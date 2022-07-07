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
    explicit ThreadPool(size_t threadNumber):m_stop(false){//防止构造函数隐式转换
        for(size_t i=0;i<threadNumber;++i)
        {
            m_thread.emplace_back(//this:函数体可以使用Lambda所在类的成员变量。
                [this](){
                    for(;;)
                    {
                         function<void()>task;
                        {
                             unique_lock< mutex>lk(m_mutex);//unique_lock   unique_lock也可以加 adopt_lock参数，表示互斥量已经被lock，
							//不需要再重复lock。该互斥量之前必须已经lock，才可以使用该参数。  //通过析构函数来unlock
                            m_cv.wait(lk,[this](){ return m_stop||!tasks.empty();});//阻塞
							//wait()用来等一个东西
			           //如果第二个参数lambda表达式返回值是true，那wait()直接返回；
			           //如果第二个参数lambda表达式返回值是false,那么wait()将解锁互斥量，并堵塞到本行，
			            //那堵塞到什么时候为止呢？堵塞到其他某个线程调用notify_one()成员函数为止；
                            if(m_stop&&tasks.empty()) return;
                            task= move(tasks.front());//move用来表示一个对象t可以被“移动”，即允许资源从t有效地转移到另一个对象，即资源所有权转移
							//取出任务队列头元素
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

    template<typename F,typename... Args>//variadic templates  传入函数指针及参数
    auto submit(F&& f,Args&&... args)-> future<decltype(f(args...))>//尾返回类型推导
		// future，它提供了一个访问异步操作结果的途径。我们可以使用 future的wait()方法来设置屏障，阻塞线程，实现线程同步
		//并最终使用 future的get()方法来获得执行结果。
	{
        auto taskPtr= make_shared< packaged_task<decltype(f(args...))()>>(
             bind( forward<F>(f), forward<Args>(args)...)//参数绑定  forward移动语义 完美转发
        );//返回智能指针 share_ptr
		//packaged_task 包装一个可调用的对象，并且允许异步获取该可调用对象产生的结果，也就是将其的返回值传给future
		// packaged_task<函数返回类型(参数类型)> 变量名{函数名}
        {
             unique_lock< mutex>lk(m_mutex);
            if(m_stop) throw  runtime_error("submit on stopped ThreadPool");
            tasks.emplace([taskPtr](){ (*taskPtr)(); }); //lambda表达式
        }
        m_cv.notify_one();
        return taskPtr->get_future();

    }
};

#endif //THREADPOOL_H
