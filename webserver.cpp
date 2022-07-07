// encode UTF-8
// @Author      : yjh
// @Date          : 2022-06-11

#include "webserver.h"
//构造函数
WebServer::WebServer(int port, int trigMode, int timeoutMS, bool OptLinger,int sqlPort, 
const char* sqlUser, const  char* sqlPwd,const char* dbName, int connPoolNum, int threadNum,
bool openLog, int logLevel, int logQueSize):
    port_(port),openLinger_(OptLinger),timeoutMS_(timeoutMS),isClose_(false),
    timer_(new TimerManager()),threadpool_(new ThreadPool(threadNum)),epoller_(new Epoller())
{
    //获取当前工作目录的绝对路径  路径拼接
    srcDir_=getcwd(nullptr,256); //当前的工作目录绝对路径复制到参数buf 所指的内存空间，参数size 为buf 的空间大小 //#include <unistd.h>
    assert(srcDir_);
    //拼接字符串
    strncat(srcDir_,"/resources/",16);// char *strncat(char *dest, const char *src, size_t n) 把 src 所指向的字符串追加到 dest 所指向的字符串的结尾，直到 n 字符长度为止。
    HTTPconnection::userCount=0;//静态成员变量初始化
    HTTPconnection::srcDir=srcDir_;//切换目录
	SqlConnPool::Instance()->Init("localhost", sqlPort, sqlUser, sqlPwd, dbName, connPoolNum);//初始化sql连接池
    initEventMode_(trigMode);//边沿触发方式
    if(!initSocket_()) isClose_=true;
	if (openLog) //初始化日志系统
	{
		Log::Instance()->init(logLevel, "./log", ".log", logQueSize);
		if (isClose_) { LOG_ERROR("========== Server init error!=========="); }//宏 变参数调用 ##__VA_ARGS__
		else {
			LOG_INFO("========== Server init ==========");
			LOG_INFO("Port:%d, OpenLinger: %s", port_, OptLinger ? "true" : "false");
			LOG_INFO("Listen Mode: %s, OpenConn Mode: %s",
				(listenEvent_ & EPOLLET ? "ET" : "LT"),
				(connectionEvent_& EPOLLET ? "ET" : "LT"));
			LOG_INFO("LogSys level: %d", logLevel);
			LOG_INFO("srcDir: %s",  HTTPconnection::srcDir);
			LOG_INFO("SqlConnPool num: %d, ThreadPool num: %d", connPoolNum, threadNum);
		}
	}
}

WebServer::~WebServer()
{
    close(listenFd_);//关闭socket
    isClose_=true;//退出循环
    free(srcDir_);//释放空间
	SqlConnPool::Instance()->ClosePool();//单例
}

void WebServer::initEventMode_(int trigMode) {
    listenEvent_ = EPOLLRDHUP;//客户端直接调用close，会触犯EPOLLRDHUP事件 
	//通过EPOLLRDHUP属性，来判断是否对端已经关闭，这样可以减少一次系统调用
    connectionEvent_ = EPOLLONESHOT | EPOLLRDHUP;//EPOLLONESHOT保证同一SOCKET只能被一个线程处理，不会跨越多个线程
    switch (trigMode)
    {
    case 0:
        break;  //默认水平触发
    case 1:
        connectionEvent_ |= EPOLLET;//边沿触发
        break;
    case 2:
        listenEvent_ |= EPOLLET;
        break;
    case 3:
        listenEvent_ |= EPOLLET; //监听
        connectionEvent_ |= EPOLLET;//连接
        break;
    default:
        listenEvent_ |= EPOLLET;
        connectionEvent_ |= EPOLLET;
        break;
    }
    HTTPconnection::isET = (connectionEvent_ & EPOLLET);
}

void WebServer::Start()
{
    int timeMS=-1;//epoll wait timeout==-1就是无事件一直阻塞
    if(!isClose_) 
    {
        cout<<"============================";
        cout<<"Server Start!";
        cout<<"============================";
        cout<<endl;
		LOG_INFO("========== Server start ==========");
    }
    while(!isClose_)//服务器未关闭时 循环
    {
        if(timeoutMS_>0)
        {
            timeMS=timer_->getNextHandle();//找超时最小的结点 处理
        }
        int eventCnt=epoller_->wait(timeMS);
		//int epoll_wait（int epfd，struct epoll_event * events， int maxevents，int timeout）;
		//epoll_wait() 函数如果检测到有事件发生，或者超时
		//就将所有就绪的事件从内核事件表（由epfd参数指定）中复制到第2参数events指针指向的数组中。
        for(int i=0;i<eventCnt;++i)
        {
            int fd=epoller_->getEventFd(i);
            uint32_t events=epoller_->getEvents(i);

            if(fd==listenFd_)//收到新的连接请求
            {
                handleListen_();    
            }
            else if(events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {//对端断开连接(读关闭)  读写都关闭 对方异常关闭 
                assert(users_.count(fd) > 0);
                closeConn_(&users_[fd]);//关闭
            }
            else if(events & EPOLLIN) { //有数据要读
                assert(users_.count(fd) > 0);
                handleRead_(&users_[fd]);
            }
            else if(events & EPOLLOUT) { //有数据要写
                assert(users_.count(fd) > 0);
                handleWrite_(&users_[fd]);
            } 
            else {
                cout<<"Unexpected event"<<endl;
				LOG_ERROR("Unexpected event");
            }
        }
    }
}

void WebServer::sendError_(int fd, const char* info)
{
    assert(fd>0);
    int ret=send(fd,info,strlen(info),0);
    if(ret<0)
    {
        cout<<"send error to client"<<fd<<" error!"<<endl;
		LOG_WARN("send error to client[%d] error!", fd);
    }
    close(fd);
}

void WebServer::closeConn_(HTTPconnection* client)
{
    assert(client);
    cout<<"client"<<client->getFd()<<" quit!"<<endl;
	LOG_INFO("Client[%d] quit!", client->getFd());
    epoller_->delFd(client->getFd());//删除文件描述符
    client->closeHTTPConn();//关闭连接
}

void WebServer::addClientConnection(int fd, sockaddr_in addr)
{
    assert(fd>0);
    users_[fd].initHTTPConn(fd,addr);//初始化连接  添加节点  读写缓冲区初始化
    if(timeoutMS_>0)
    {
        timer_->addTimer(fd,timeoutMS_,bind(&WebServer::closeConn_,this,&users_[fd]));//timeout cb  关闭连接
    }
    epoller_->addFd(fd,EPOLLIN | connectionEvent_);
    setFdNonblock(fd);
	LOG_INFO("Client[%d] in!", users_[fd].getFd());
}

void WebServer::handleListen_() {
    struct sockaddr_in addr;//sockaddr_in在头文件#include<netinet/in.h>或#include <arpa/inet.h>中定义，
	//该结构体解决了sockaddr的缺陷，把port和addr 分开储存在两个变量中
    socklen_t len = sizeof(addr);
    do {
        int fd = accept(listenFd_, (struct sockaddr *)&addr, &len);
		// #include <sys/socket.h>
		//int accept(int sock, struct sockaddr *addr, socklen_t *addrlen);
		//accept() 返回一个新的套接字来和客户端通信，addr 保存了客户端的IP地址和端口号，而 sock 是服务器端的套接字
        if(fd <= 0) { return;}
        else if(HTTPconnection::userCount >= MAX_FD) {
            sendError_(fd, "Server busy!");
			LOG_WARN("Clients is full!");
            return;
        }
        addClientConnection(fd, addr);//将成功连接的客户端保存
    } while(listenEvent_ & EPOLLET);
}

void WebServer::handleRead_(HTTPconnection* client) {
    assert(client);
    extentTime_(client);
    threadpool_->submit(bind(&WebServer::onRead_, this, client));
}

void WebServer::handleWrite_(HTTPconnection* client)
{
    assert(client);
    extentTime_(client);
    threadpool_->submit(bind(&WebServer::onWrite_, this, client)); //线程池   //非静态成员函数则需要传递this指针作为第一个参数
}

void WebServer::extentTime_(HTTPconnection* client)//有活动 重新计时
{
    assert(client);
    if(timeoutMS_>0)
    {
        timer_->update(client->getFd(),timeoutMS_);//调整超时事件 now()+timeoutms
    }
}

void WebServer::onRead_(HTTPconnection* client) 
{
    assert(client);
    int ret = -1;
    int readErrno = 0;
    ret = client->readBuffer(&readErrno);//读缓冲区数据
	//触发一次后需要不断读取所有数据直到读到EAGAIN 没有数据可读 为止。否则剩下的数据只有在下次对端有写入时才能一起取出来了。
    if(ret <= 0 && readErrno != EAGAIN) { //没有读到数据 且 未读完
        closeConn_(client);
        return;
    }
    onProcess_(client);//处理请求 准备好响应头 文件 
}
//1. 对于读操作 epollin
//（1）当缓冲区由不可读变为可读的时候，即缓冲区由空变为不空的时候。
//（2）当有新数据到达时，即缓冲区中的待读数据变多的时候。
//（3）当缓冲区有数据可读，且应用进程对相应的描述符进行EPOLL_CTL_MOD 修改EPOLLIN事件时。
//2. 对于写操作 epollout
//（1）当缓冲区由不可写变为可写时。
//（2）当有旧数据被发送走，即缓冲区中的内容变少的时候。
//（3）当缓冲区有空间可写，且应用进程对相应的描述符进行EPOLL_CTL_MOD 修改EPOLLOUT事件时

void WebServer::onProcess_(HTTPconnection* client) //传输文件
{
	//true 数据准备好 传输 epollout   //false 数据读写完毕 监听读事件 epollin
    if(client->handleHTTPConn()) {  //可写
        epoller_->modFd(client->getFd(), connectionEvent_ | EPOLLOUT);//修改EPOLL监听事件 write
    } 
    else {//有数据可读  tcp数据到来 
        epoller_->modFd(client->getFd(), connectionEvent_ | EPOLLIN);  //修改EPOLL监听事件 read  
    }
}

void WebServer::onWrite_(HTTPconnection* client) {
    assert(client);
    int ret = -1;
    int writeErrno = 0;
    ret = client->writeBuffer(&writeErrno);//在写缓冲区写数据
    if(client->writeBytes() == 0) {
        /* 传输完成 */
        if(client->isKeepAlive()) {//如果客户端是keepalive  
            onProcess_(client);//EPOLLIN
            return;
        }
    }
    else if(ret < 0) {
        if(writeErrno == EAGAIN) //发送缓存被占满
		{
            /* 继续传输 */
            epoller_->modFd(client->getFd(), connectionEvent_ | EPOLLOUT);//监听写事件
            return;
        }
    }
    closeConn_(client);//关闭连接
}
bool WebServer::initSocket_() {//socket初始化
    int ret;
    struct sockaddr_in addr;//地址结构体
	/*端口检查*/
    if(port_ > 65535 || port_ < 1024) { 
        return false;
    }
    addr.sin_family = AF_INET;//地址族 IPV4  AF_INET6 ipv6
    addr.sin_addr.s_addr = htonl(INADDR_ANY); //自动获取IP地址    //网络字节序（long）
    addr.sin_port = htons(port_); //端口号  网络字节序（short）
    struct linger optLinger = { 0 };//#include <arpa/inet.h>
    if(openLinger_) {
        /* 优雅关闭: 直到所剩数据发送完毕或超时 */
        optLinger.l_onoff = 1;
        optLinger.l_linger = 1;
    }
	//三种断开方式：
	//1. l_onoff = 0; l_linger忽略
	//	close()立刻返回，底层会将未发送完的数据发送完成后再释放资源，即优雅退出。
	//2. l_onoff != 0; l_linger = 0;
	//close()立刻返回，但不会发送未发送完成的数据，而是通过一个REST包强制的关闭socket描述符，即强制退出。
	//3. l_onoff != 0; l_linger > 0;
	//close()不会立刻返回，内核会延迟一段时间，这个时间就由l_linger的值来决定。如果超时时间到达之前，
	//发送完未发送的数据(包括FIN包)并得到另一端的确认，close()会返回正确，socket描述符优雅性退出。
	//否则，close()会直接返回错误值，未发送数据丢失，socket描述符被强制性退出。需要注意的时，
	//如果socket描述符被设置为非堵塞型，则close()会直接返回值
    listenFd_ = socket(AF_INET, SOCK_STREAM, 0);//设置socket  IPV4 TCP
    if(listenFd_ < 0) {
		LOG_ERROR("Create socket error!", port_);
        cout<<"Create socket error!"<<endl;
        return false;
    }
	//第二个参数level是被设置的选项的级别，如果想要在套接字级别上设置选项，就必须把level设置为 SOL_SOCKET
    ret = setsockopt(listenFd_, SOL_SOCKET, SO_LINGER, &optLinger, sizeof(optLinger));//设置优雅断开
    if(ret < 0) {
        close(listenFd_);
		LOG_ERROR("Init linger error!", port_);
        cout<<"Init linger error!"<<endl;
        return false;
    }

    int optval = 1;
    /* 端口复用 */
    /* 只有最后一个套接字会正常接收数据。 */
	//第二个参数level是被设置的选项的级别，如果想要在套接字级别上设置选项，就必须把level设置为 SOL_SOCKET
	//SO_REUSEADDR SO_REUSEADDR，打开或关闭地址复用功能
    ret = setsockopt(listenFd_, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int));
    if(ret == -1) {
		LOG_ERROR("set socket setsockopt error !");
         cout<<"set socket setsockopt error !"<< endl;
        close(listenFd_);
        return false;
    }

    ret = bind(listenFd_, (struct sockaddr *)&addr, sizeof(addr)); //地址绑定
    if(ret < 0) {
		LOG_ERROR("Bind Port:%d error!", port_);
         cout<<"Bind Port"<<port_<<" error!"<< endl;
        close(listenFd_);
        return false;
    }

    ret = listen(listenFd_, 6);
    if(ret < 0) {
		LOG_ERROR("Listen port:%d error!", port_);
        printf("Listen port:%d error!\n", port_);
        close(listenFd_);
        return false;
    }
    ret = epoller_->addFd(listenFd_,  listenEvent_ | EPOLLIN); //将描述符fd加入epoll监控
    if(ret == 0) {
		LOG_ERROR("Add listen error!");
        printf("Add listen error!\n");
        close(listenFd_);
        return false;
    }
    setFdNonblock(listenFd_);//将socket 设置为非阻塞
	LOG_INFO("Server port:%d", port_);
    printf("Server port:%d\n", port_);
    return true;
}

int WebServer::setFdNonblock(int fd) {
    assert(fd > 0);
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);//将socket 设置为非阻塞
}
