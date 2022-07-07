// encode UTF-8

// @Author        : yjh
// @Date          : 2022-06-21

#ifndef TIMER_H
#define TIMER_H

#include<queue>
#include<deque>
#include<unordered_map>
#include<ctime>
#include<chrono>
#include<functional>
#include<memory>
#include "HTTPconnection.h"
using namespace std;

typedef  function<void()> TimeoutCallBack;
typedef  chrono::high_resolution_clock Clock;
typedef  chrono::milliseconds MS;
typedef Clock::time_point TimeStamp;
//typedef  unique_ptr<HTTPconnection> HTTPconnection_Ptr;

class TimerNode{
public:
    int id;             //用来标记定时器
    TimeStamp expire;   //设置过期时间
    TimeoutCallBack cb; //设置一个回调函数用来方便删除定时器时将对应的HTTP连接关闭

    //需要的功能可以自己设定
    bool operator<(const TimerNode& t) //重载<  按超时时间比较
    {
        return expire<t.expire;
    }
};

class TimerManager{
    typedef  shared_ptr<TimerNode> SP_TimerNode;
public:
    TimerManager() {heap_.reserve(64);}//为vector<timenode>分配空间
    ~TimerManager() {clear();}
    //设置定时器 
    void addTimer(int id,int timeout,const TimeoutCallBack& cb);
    //处理过期的定时器
    void handle_expired_event();
    //下一次处理过期定时器的时间
    int getNextHandle();

    void update(int id,int timeout);
    //删除制定id节点，并且用指针触发处理函数
    void work(int id);

    void pop();
    void clear();

private:
    void del_(size_t i);
    void shiftup_(size_t i);
    bool shiftdown_(size_t index,size_t n);
    void swapNode_(size_t i,size_t j);
     vector<TimerNode>heap_;
     unordered_map<int,size_t>ref_;//映射一个fd对应的定时器在heap_中的位置
};

#endif //TIMER_H