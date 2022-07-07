/*
 * @Author       : mark
 * @Date         : 2020-06-16
 * @copyleft Apache 2.0
 */ 
#include "log.h"

using namespace std;

Log::Log() {
    lineCount_ = 0;
    isAsync_ = false;
    writeThread_ = nullptr;
    deque_ = nullptr;
    toDay_ = 0;
    fp_ = nullptr;
}

Log::~Log() {
    if(writeThread_ && writeThread_->joinable()) {
        while(!deque_->empty()) {
            deque_->flush();
        };
        deque_->Close();
        writeThread_->join();
    }
    if(fp_) {
        lock_guard<mutex> locker(mtx_);
        flush();
        fclose(fp_);
    }
}

int Log::GetLevel() {
    lock_guard<mutex> locker(mtx_);
    return level_;
}

void Log::SetLevel(int level) {
    lock_guard<mutex> locker(mtx_);
    level_ = level;
}

void Log::init(int level = 1, const char* path, const char* suffix,
    int maxQueueSize) {
    isOpen_ = true;
    level_ = level;
    if(maxQueueSize > 0) {
        isAsync_ = true;
        if(!deque_) {
            unique_ptr<BlockDeque< string>> newDeque(new BlockDeque< string>);//创建日志阻塞队列
            deque_ = move(newDeque);
             unique_ptr< thread> NewThread(new thread(FlushLogThread));//创建日志线程
            writeThread_ = move(NewThread);//移动语义 所有权转让   异步将日志写入文件
        }
    } else {
        isAsync_ = false;
    }

    lineCount_ = 0;

    time_t timer = time(nullptr);
    struct tm *sysTime = localtime(&timer);
    struct tm t = *sysTime;
    path_ = path;
    suffix_ = suffix;
    char fileName[LOG_NAME_LEN] = {0};
    snprintf(fileName, LOG_NAME_LEN - 1, "%s/%04d_%02d_%02d%s", 
            path_, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, suffix_);
    toDay_ = t.tm_mday;

    {
        lock_guard<mutex> locker(mtx_);
        buff_.initPtr();
        if(fp_) { //如果文件已打开 
            flush();
            fclose(fp_); 
        }

        fp_ = fopen(fileName, "a");//打开 追加
        if(fp_ == nullptr) {//未打开
            mkdir(path_, 0777);//<sys/stat.h>//创建目录  用户组、所有者、其它用户 具有读、写、执行权限  
			//S_IRWXU |S_IRWXG|S_IRWXO|
            fp_ = fopen(fileName, "a");
        } 
        assert(fp_ != nullptr);
    }
}

void Log::write(int level, const char *format, ...) //写日志
{
    struct timeval now = {0, 0};//time.h  //
	//struct timeval
	//{
	//	__time_t tv_sec;        /* Seconds. */
	//	__suseconds_t tv_usec;  /* Microseconds. */
	//};
    gettimeofday(&now, nullptr);//获取当前时间 不指定时区
    time_t tSec = now.tv_sec;//秒
	//struct tm {
	//	int tm_sec; /* 秒 – 取值区间为[0,59] */
	//	int tm_min; /* 分 - 取值区间为[0,59] */
	//	int tm_hour; /* 时 - 取值区间为[0,23] */
	//	int tm_mday; /* 一个月中的日期 - 取值区间为[1,31] */
	//	int tm_mon; /* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */
	//	int tm_year; /* 年份，其值等于实际年份减去1900 */
	//	int tm_wday; /* 星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 */
	//	int tm_yday; /* 从每年的1月1日开始的天数 – 取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推 */
	//	int tm_isdst; /* 夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的时候，tm_isdst为0；不了解情况时，tm_isdst()为负。
	//	long int tm_gmtoff; /*指定了日期变更线东面时区中UTC东部时区正秒数或UTC西部时区的负秒数*/
	//	const char *tm_zone; /*当前时区的名字(与环境变量TZ有关)*/
	//};
    struct tm *sysTime = localtime(&tSec);//获取系统时间
    struct tm t = *sysTime;
    va_list vaList;//可变参数列表

    /* 日志日期 日志行数 */
    if (toDay_ != t.tm_mday || (lineCount_ && (lineCount_  %  MAX_LINES == 0)))//日志不是当天 或 日志条数超过最大条数
		//创建新的日志文件
    {
        unique_lock<mutex> locker(mtx_);
        locker.unlock();
        
        char newFile[LOG_NAME_LEN];//日志文件名
        char tail[36] = {0};
        snprintf(tail, 36, "%04d_%02d_%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);

        if (toDay_ != t.tm_mday)//不是当天文件 以当前日期创建
        {
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s%s", path_, tail, suffix_);//限定最长文件名
            toDay_ = t.tm_mday;
            lineCount_ = 0;
        }
        else {//日志文件超过最大条数
            snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s-%d%s", path_, tail, (lineCount_  / MAX_LINES), suffix_);
        }
        
        locker.lock();//线程锁
        flush();//刷新文件流
        fclose(fp_);//关闭旧文件
        fp_ = fopen(newFile, "a");//重新创建 以追加的方式打开新文件
        assert(fp_ != nullptr);
    }

    {
        unique_lock<mutex> locker(mtx_);
        lineCount_++;
        int n = snprintf(buff_.curWritePtr(), 128, "%d-%02d-%02d %02d:%02d:%02d.%06ld ",
                    t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
                    t.tm_hour, t.tm_min, t.tm_sec, now.tv_usec);//写入缓存
                    
        buff_.updateWritePtr(n);//更新缓存写入指针
        AppendLogLevelTitle_(level);//添加标题

        va_start(vaList, format);//<stdarg.h>
		//void va_start(va_list ap, last_arg);
		//ap -- 这是一个 va_list 类型的对象，它用来存储通过 va_arg 获取额外参数时所必需的信息。
		//last_arg -- 最后一个传递给函数的已知的固定参数。
        int m = vsnprintf(buff_.curWritePtr(), buff_.writeableBytes(), format, vaList);// 将format 可变参数值 写到缓冲区
        va_end(vaList);

        buff_.updateWritePtr(m);//更新缓冲区指针
        buff_.append("\n\0", 2);//添加 换行 字符串结束符

        if(isAsync_ && deque_ && !deque_->full()) {
            deque_->push_back(buff_.AlltoStr());//将缓冲区内容写到阻塞队列
        } else {
            fputs(buff_.curReadPtr(), fp_);//非异步 直接写入文件
        }
        buff_.initPtr();//初始化缓冲区
    }
}

void Log::AppendLogLevelTitle_(int level) //添加标题
{
    switch(level) {
    case 0:
        buff_.append("[debug]: ", 9);
        break;
    case 1:
        buff_.append("[info] : ", 9);
        break;
    case 2:
        buff_.append("[warn] : ", 9);
        break;
    case 3:
        buff_.append("[error]: ", 9);
        break;
    default:
        buff_.append("[info] : ", 9);
        break;
    }
}

void Log::flush() {
    if(isAsync_) { //异步
        deque_->flush(); //刷新阻塞队列
    }
    fflush(fp_);// 刷新流 stream 的输出缓冲区。
}

void Log::AsyncWrite_() {
    string str = "";
    while(deque_->pop(str)) { //日志线程 阻塞队列 取字符串 写入文件
        lock_guard<mutex> locker(mtx_);
		// lock_guard属于C++11特性，锁管理遵循RAII习语管理资源，
		//锁管理器在构造函数中自动绑定它的互斥体并加锁，在析构函数中解锁，大大减少了死锁的风险
        fputs(str.c_str(), fp_);
    }
}

Log* Log::Instance() {
    static Log inst;
    return &inst;
}

void Log::FlushLogThread() {
    Log::Instance()->AsyncWrite_();
}