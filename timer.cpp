// encode UTF-8

// @Author        : yjh
// @Date          : 2022-06-21

#include "timer.h"
void TimerManager::shiftup_(size_t i) {
    assert(i >= 0 && i < heap_.size());
    size_t j = (i - 1) / 2;//父节点
    while(j >= 0) {
        if(heap_[j] < heap_[i]) { break; }
        swapNode_(i, j);
        i = j;
        j = (i - 1) / 2;
    }
}

void TimerManager::swapNode_(size_t i, size_t j) {
    assert(i >= 0 && i < heap_.size());
    assert(j >= 0 && j < heap_.size());
     swap(heap_[i], heap_[j]);//交换定时器结点
    ref_[heap_[i].id] = i;//更新fd映射
    ref_[heap_[j].id] = j;
} 

bool TimerManager::shiftdown_(size_t index, size_t n) //向下调整
{
    assert(index >= 0 && index < heap_.size());
    assert(n >= 0 && n <= heap_.size());
    size_t i = index;//父节点
    size_t j = i * 2 + 1;//左子节点
    while(j < n) {
        if(j + 1 < n && heap_[j + 1] < heap_[j]) j++;//有右子节点 且右子节点超时时间小于左子结点 切换到右节点
        if(heap_[i] < heap_[j]) break;
        swapNode_(i, j);
        i = j;
        j = i * 2 + 1;
    }
    return i > index;
}

void TimerManager::addTimer(int id, int timeout, const TimeoutCallBack& cb) {
    assert(id >= 0);
    size_t i;
    if(ref_.count(id) == 0) {//没有该结点
        /* 新节点：堆尾插入，调整堆 */
        i = heap_.size();
        ref_[id] = i;
        heap_.push_back({id, Clock::now() + MS(timeout), cb});//入堆
        shiftup_(i);//调整堆 上浮
    } 
    else {
        /* 已有结点：调整堆 */  //替换 id文件描述符
        i = ref_[id];
        heap_[i].expire = Clock::now() + MS(timeout);//chrono::high_resolution_clock
        heap_[i].cb = cb;
        if(!shiftdown_(i, heap_.size())) { //上浮 或 下沉
            shiftup_(i);
        }
    }
}

void TimerManager::work(int id) {
    /* 删除指定id结点，并触发回调函数 */
    if(heap_.empty() || ref_.count(id) == 0) {
        return;
    }
    size_t i = ref_[id];
    TimerNode node = heap_[i];
    node.cb();
    del_(i);
}

void TimerManager::del_(size_t index) {
    /* 删除指定位置的结点 */
    assert(!heap_.empty() && index >= 0 && index < heap_.size());
    /* 将要删除的结点换到队尾，然后调整堆 */
    size_t i = index;
    size_t n = heap_.size() - 1;
    assert(i <= n);
    if(i < n) {
        swapNode_(i, n);
        if(!shiftdown_(i, n)) {
            shiftup_(i);
        }
    }
    /* 队尾元素删除 */
    ref_.erase(heap_.back().id);
    heap_.pop_back();
}

void TimerManager::update(int id, int timeout) {
    /* 调整指定id的结点 */
    assert(!heap_.empty() && ref_.count(id) > 0);
    heap_[ref_[id]].expire = Clock::now() + MS(timeout);;
    shiftdown_(ref_[id], heap_.size());
}

void TimerManager::handle_expired_event() {
    /* 清除超时结点 */
    if(heap_.empty()) {
        return;
    }
    while(!heap_.empty()) {
        TimerNode node = heap_.front();
        if( chrono::duration_cast<MS>(node.expire - Clock::now()).count() > 0) // MS tydefine 毫秒  转换时间
			//未超时
		{ 
            break; 
        }
        node.cb();//调用超时函数
        pop();
    }
}

void TimerManager::pop() {
    assert(!heap_.empty());
    del_(0);
}

void TimerManager::clear() {
    ref_.clear();
    heap_.clear();
}

int TimerManager::getNextHandle() {
    handle_expired_event();//清除超时结点
    size_t res = -1;
    if(!heap_.empty()) {
        res =  chrono::duration_cast<MS>(heap_.front().expire - Clock::now()).count();//最小超时时间
        if(res < 0) { res = 0; }
    }
    return res;
}