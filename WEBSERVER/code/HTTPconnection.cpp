// encode UTF-8

// @Author        : yjh
// @Date          : 2022-06-23

#include "HTTPconnection.h"

const char* HTTPconnection::srcDir;
atomic<int> HTTPconnection::userCount;//静态变量
bool HTTPconnection::isET;

HTTPconnection::HTTPconnection() { 
    fd_ = -1;
    addr_ = { 0 };
    isClose_ = true;
};

HTTPconnection::~HTTPconnection() { 
    closeHTTPConn(); 
};

void HTTPconnection::initHTTPConn(int fd, const sockaddr_in& addr) {
    assert(fd > 0);
    userCount++;
    addr_ = addr;
    fd_ = fd;
    writeBuffer_.initPtr(); //写缓冲区初始化
    readBuffer_.initPtr();  //读缓冲区初始化
    isClose_ = false;
	LOG_INFO("Client[%d](%s:%d) in, userCount:%d", fd_, getIP(), getPort(), (int)userCount);
}

void HTTPconnection::closeHTTPConn() {
    response_.unmapFile_();
    if(isClose_ == false){
        isClose_ = true; 
        userCount--;
        close(fd_);
		LOG_INFO("Client[%d](%s:%d) quit, UserCount:%d", fd_, getIP(), getPort(), (int)userCount)
    }
}

int HTTPconnection::getFd() const {
    return fd_;
};

struct sockaddr_in HTTPconnection::getAddr() const {
    return addr_;
}

const char* HTTPconnection::getIP() const {
    return inet_ntoa(addr_.sin_addr);
}

int HTTPconnection::getPort() const {
    return addr_.sin_port;
}

ssize_t HTTPconnection::readBuffer(int* saveErrno) {
    ssize_t len = -1;
    do {
        len = readBuffer_.readFd(fd_, saveErrno);
        // cout<<fd_<<" read bytes:"<<len<< endl;
        if (len <= 0) {
            break;
        }
    } while (isET);
    return len;
}

ssize_t HTTPconnection::writeBuffer(int* saveErrno) {
    ssize_t len = -1;
    do {
        len = writev(fd_, iov_, iovCnt_);//写数据 writev以顺序iov[0]、iov至iov[iovcnt-1]从各缓冲区中聚集输出数据到fd
        if(len <= 0) {
            *saveErrno = errno;
            break;
        }
		//writev多次调用，并不会改变参数iovec*的内容，长度也不会改变，所以在while循环写入时可能要调整指针位置；
        if(iov_[0].iov_len + iov_[1].iov_len  == 0) { break; } /* 传输结束 */
        else if(static_cast<size_t>(len) > iov_[0].iov_len) {//iov[0]传输结束
            iov_[1].iov_base = (uint8_t*) iov_[1].iov_base + (len - iov_[0].iov_len);//调整iov[1]指针
            iov_[1].iov_len -= (len - iov_[0].iov_len);//调整iov[1]长度
            if(iov_[0].iov_len) { //写缓冲区写完
                writeBuffer_.initPtr();//初始化写缓冲区
                iov_[0].iov_len = 0;
            }
        }
        else {
            iov_[0].iov_base = (uint8_t*)iov_[0].iov_base + len; //更新写缓冲区
            iov_[0].iov_len -= len; 
            writeBuffer_.updateReadPtr(len);//更新读指针
        }
    } while(isET || writeBytes() > 10240);//边沿触发 或者 写的数据超过缓冲区大小
    return len;
}

bool HTTPconnection::handleHTTPConn() {
    request_.init();//初始化请求行
    if(readBuffer_.readableBytes() <= 0) { //没有可读数据  接收数据		EPOLLIN
        // cout<<"readBuffer is empty!"<< endl;
        return false;
    }
    else if(request_.parse(readBuffer_)) {//解析http请求
		LOG_DEBUG("%s", request_.path().c_str());
        response_.init(srcDir, request_.path(), request_.isKeepAlive(), 200);//初始化http响应
    }else {
         cout<<"400!"<< endl;
        //readBuffer_.printContent();
        response_.init(srcDir, request_.path(), false, 400);//BAD REQUEST
    }

    response_.makeResponse(writeBuffer_);//准备要发送的事件
    /* 响应头 */
    iov_[0].iov_base = const_cast<char*>(writeBuffer_.curReadPtr());
    iov_[0].iov_len = writeBuffer_.readableBytes();
    iovCnt_ = 1;

    /* 文件 */ 
    if(response_.fileLen() > 0  && response_.file()) {
        iov_[1].iov_base = response_.file();//内存映射区
        iov_[1].iov_len = response_.fileLen();
        iovCnt_ = 2;
    }
	LOG_DEBUG("filesize:%d, %d  to %d", response_.fileLen(), iovCnt_, writeBytes());
    return true; //EPOLLOUT
}
