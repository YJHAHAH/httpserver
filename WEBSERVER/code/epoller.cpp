// encode UTF-8

// @Author        : yjh
// @Date          : 2022-06-22
#include "epoller.h"
//#include<sys/epoll.h> 
//epoll_create（）创建一个epoll实例。其中nfd为epoll句柄，参数max_size标识这个监听的数目最大有多大
Epoller::Epoller(int maxEvent):epollerFd_(epoll_create(512)), events_(maxEvent){  //用vector存放事件结构体
    assert(epollerFd_ >= 0 && events_.size() > 0); //出错即终止
}

Epoller::~Epoller() {
    close(epollerFd_);
}
bool Epoller::addFd(int fd, uint32_t events) {  //添加到监听树
    if(fd < 0) return false;
    epoll_event ev = {0};
    ev.data.fd = fd;
    ev.events = events;
    return 0 == epoll_ctl(epollerFd_, EPOLL_CTL_ADD, fd, &ev);
}

bool Epoller::modFd(int fd, uint32_t events) {
    if(fd < 0) return false;
    epoll_event ev = {0};
    ev.data.fd = fd;
    ev.events = events;
    return 0 == epoll_ctl(epollerFd_, EPOLL_CTL_MOD, fd, &ev);
}

bool Epoller::delFd(int fd) {
    if(fd < 0) return false;
    epoll_event ev = {0};
    return 0 == epoll_ctl(epollerFd_, EPOLL_CTL_DEL, fd, &ev);
}

int Epoller::wait(int timeoutMs) {
    return epoll_wait(epollerFd_, &events_[0], static_cast<int>(events_.size()), timeoutMs);
}

int Epoller::getEventFd(size_t i) const {
    assert(i < events_.size() && i >= 0);
    return events_[i].data.fd;
}

uint32_t Epoller::getEvents(size_t i) const {
    assert(i < events_.size() && i >= 0);
    return events_[i].events;
}