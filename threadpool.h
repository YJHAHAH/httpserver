// encode UTF-8

// @Author        : yjh
// @Date          : 2022-06-20

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<thread>
#include<condition_variable>
#include<mutex>
#include<vector>
#include<queue>
#include<future>
using namespace std;
class ThreadPool{
private:
    bool m_stop;
     vector< thread>m_thread; //线程池
     queue< function<void()>>tasks; //任务队列
     mutex m_mutex; //互斥锁
     condition_variable m_cv; //条件变量

public:
    explicit ThreadPool(size_t threadNumber):m_stop(false){
        for(size_t i=0;i<threadNumber;++i)
        {
            m_thread.emplace_back(
                [this](){
                    for(;;)//死循环
                    {
                         function<void()>task;
                        {
                             unique_lock<mutex>lk(m_mutex);
                            m_cv.wait(lk,[this](){ return m_stop||!tasks.empty();});//阻塞
                            if(m_stop&&tasks.empty()) return;
                            task= move(tasks.front());
                            tasks.pop();
                        }
                        task();//调用task 执行任务
                    }
                }
            );
        }
    }

    ThreadPool(const ThreadPool &) = delete;//禁止拷贝构造
    ThreadPool(ThreadPool &&) = delete;

    ThreadPool & operator=(const ThreadPool &) = delete;//禁止拷贝赋值
    ThreadPool & operator=(ThreadPool &&) = delete;

    ~ThreadPool()
	{
        {
             unique_lock< mutex>lk(m_mutex);
            m_stop=true;
        }
        m_cv.notify_all();
        for(auto& threads:m_thread)
        {
            threads.join();//结束线程、回收线程资源
        }
    }

    template<typename F,typename... Args>
    auto submit(F&& f,Args&&... args)-> future<decltype(f(args...))>
	{
        auto taskPtr= make_shared< packaged_task<decltype(f(args...))()>>(
             bind( forward<F>(f), forward<Args>(args)...)
        );
        {
             unique_lock< mutex>lk(m_mutex);
            if(m_stop) throw  runtime_error("submit on stopped ThreadPool");
            tasks.emplace([taskPtr](){ (*taskPtr)(); }); 
        }
        m_cv.notify_one();
        return taskPtr->get_future();
    }
};

#endif //THREADPOOL_H
