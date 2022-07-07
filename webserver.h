// encode UTF-8

// @Author        : yjh
// @Date          : 2022-06-10

#ifndef WEBSERVER_H   //防御式编程 防止头文件重复定义
#define WEBSERVER_H
#include <unordered_map>
#include <fcntl.h>       // fcntl()
#include <unistd.h>      // close()
#include <assert.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "epoller.h"
#include "timer.h"
#include "log.h"
#include "sqlconnpool.h"
#include "threadpool.h"
#include "sqlconnRAII.h"
#include "HTTPconnection.h"
using namespace std;

class WebServer {
public:
    WebServer(int port, int trigMode, int timeoutMS, bool OptLinger,
            int sqlPort, const char* sqlUser, const  char* sqlPwd,
            const char* dbName, int connPoolNum, int threadNum,
            bool openLog, int logLevel, int logQueSize);//初始化参数
    ~WebServer();

    void Start(); //一切的开始

private:
    //对服务端的socket进行设置，最后可以得到listenFd
    bool initSocket_(); //初始化socket
    
    void initEventMode_(int trigMode);

    void addClientConnection(int fd, sockaddr_in addr); //添加一个HTTP连接
    void closeConn_(HTTPconnection* client);            //关闭一个HTTP连接

  
    void handleListen_();
    void handleWrite_(HTTPconnection* client);
    void handleRead_(HTTPconnection* client);

    void onRead_(HTTPconnection* client);
    void onWrite_(HTTPconnection* client);
    void onProcess_(HTTPconnection* client);

    void sendError_(int fd, const char* info);
    void extentTime_(HTTPconnection* client);

    static const int MAX_FD = 65536;
    static int setFdNonblock(int fd);

    int port_;
    int timeoutMS_;  /* 毫秒MS,定时器的默认过期时间 */
    bool isClose_;//结束标志 
    int listenFd_;
    bool openLinger_;
    char* srcDir_;//需要获取的路径
    
    uint32_t listenEvent_;
    uint32_t connectionEvent_;
   
     unique_ptr<TimerManager>timer_;//智能指正 让权
     unique_ptr<ThreadPool> threadpool_;
     unique_ptr<Epoller> epoller_; //raii
     unordered_map<int, HTTPconnection> users_;
};


#endif //WEBSERVER_H