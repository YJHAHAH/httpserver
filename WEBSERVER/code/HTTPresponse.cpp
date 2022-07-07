// encode UTF-8

// @Author        : yjh
// @Date          : 2022-06-24

#include "HTTPresponse.h"

const  unordered_map< string,  string> HTTPresponse::SUFFIX_TYPE = {
    { ".html",  "text/html" },
    { ".xml",   "text/xml" },
    { ".xhtml", "application/xhtml+xml" },
    { ".txt",   "text/plain" },
    { ".rtf",   "application/rtf" },
    { ".pdf",   "application/pdf" },
    { ".word",  "application/nsword" },
    { ".png",   "image/png" },
    { ".gif",   "image/gif" },
    { ".jpg",   "image/jpeg" },
    { ".jpeg",  "image/jpeg" },
    { ".au",    "audio/basic" },
    { ".mpeg",  "video/mpeg" },
    { ".mpg",   "video/mpeg" },
    { ".avi",   "video/x-msvideo" },
    { ".gz",    "application/x-gzip" },
    { ".tar",   "application/x-tar" },
    { ".css",   "text/css "},
    { ".js",    "text/javascript "},
};

const  unordered_map<int,  string> HTTPresponse::CODE_STATUS = {
    { 200, "OK" },
    { 400, "Bad Request" },
    { 403, "Forbidden" },
    { 404, "Not Found" },
};

const  unordered_map<int,  string> HTTPresponse::CODE_PATH = {
    { 400, "/400.html" },
    { 403, "/403.html" },
    { 404, "/404.html" },
};

HTTPresponse::HTTPresponse() {
    code_ = -1;
    path_ = srcDir_ = "";
    isKeepAlive_ = false;
    mmFile_ = nullptr; 
    mmFileStat_ = { 0 };
};

HTTPresponse::~HTTPresponse() {
    unmapFile_();//释放内存映射区
}

void HTTPresponse::init(const  string& srcDir,  string& path, bool isKeepAlive, int code){
    assert(srcDir != "");
    if(mmFile_) { unmapFile_(); }
    code_ = code;
    isKeepAlive_ = isKeepAlive;
    path_ = path;
    srcDir_ = srcDir;
    mmFile_ = nullptr; 
    mmFileStat_ = { 0 };
}


//struct stat
//{
//	dev_t     st_dev;     /* ID of device containing file */文件使用的设备号
//		ino_t     st_ino;     /* inode number */    索引节点号
//		mode_t    st_mode;    /* protection */  文件对应的模式，文件，目录等
//		nlink_t   st_nlink;   /* number of hard links */    文件的硬连接数
//		uid_t     st_uid;     /* user ID of owner */    所有者用户识别号
//		gid_t     st_gid;     /* group ID of owner */   组识别号
//		dev_t     st_rdev;    /* device ID (if special file) */ 设备文件的设备号
//		off_t     st_size;    /* total size, in bytes */ 以字节为单位的文件容量
//		blksize_t st_blksize; /* blocksize for file system I/O */ 包含该文件的磁盘块的大小
//		blkcnt_t  st_blocks;  /* number of 512B blocks allocated */ 该文件所占的磁盘块
//		time_t    st_atime;   /* time of last access */ 最后一次访问该文件的时间
//		time_t    st_mtime;   /* time of last modification */ / 最后一次修改该文件的时间
//		time_t    st_ctime;   /* time of last status change */ 最后一次改变该文件状态的时间
//}
void HTTPresponse::makeResponse(Buffer& buff) {
    /* 判断请求的资源文件 */
	// #include <sys/stat.h> 
	//int stat(const char *path, struct stat *buf) //获取文件信息 -1失败  0 成功
	//string::data() 返回一个指向path的指针
	//文件不存在 或 请求的文件为目录
    if(stat((srcDir_ + path_).data(), &mmFileStat_) < 0 || S_ISDIR(mmFileStat_.st_mode)) {
        code_ = 404;
    }
    else if(!(mmFileStat_.st_mode & S_IROTH)) //其它用户没有读权限
	{
        code_ = 403;
    }
    else if(code_ == -1) { //默认值 -1
        code_ = 200; 
    }
    errorHTML_();
    addStateLine_(buff);//状态行
    addResponseHeader_(buff);//响应头
    addResponseContent_(buff);//文件
}

char* HTTPresponse::file() {
    return mmFile_;
}

size_t HTTPresponse::fileLen() const {
    return mmFileStat_.st_size;
}

void HTTPresponse::errorHTML_() {
    if(CODE_PATH.count(code_) == 1) {//有错误请求
        path_ = CODE_PATH.find(code_)->second;
        stat((srcDir_ + path_).data(), &mmFileStat_);
    }
}

void HTTPresponse::addStateLine_(Buffer& buff) {
     string status;
    if(CODE_STATUS.count(code_) == 1) {
        status = CODE_STATUS.find(code_)->second;
    }
    else {
        code_ = 400;
        status = CODE_STATUS.find(400)->second;
    }
    buff.append("HTTP/1.1 " +  to_string(code_) + " " + status + "\r\n");
}

void HTTPresponse::addResponseHeader_(Buffer& buff) {
    buff.append("Connection: ");
    if(isKeepAlive_) {
        buff.append("keep-alive\r\n");
        buff.append("keep-alive: max=6, timeout=120\r\n");
    } else{
        buff.append("close\r\n");
    }
    buff.append("Content-type: " + getFileType_() + "\r\n");
}

void HTTPresponse::addResponseContent_(Buffer& buff) {
    int srcFd = open((srcDir_ + path_).data(), O_RDONLY);
    if(srcFd < 0) { 
        errorContent(buff, "File NotFound!");
        return; 
    }

    // 将文件映射到内存提高文件的访问速度 
    // MAP_PRIVATE 建立一个写入时拷贝的私有映射
	//mmap从函数名就可以看出来这是memory map, 即地址的映射, 是一种内存映射文件的方法
	//参数   addr 指定映射的超始地址，通常为NULL,由系统指定
	//  length 映射到内存的文件长度
	//	prot  映射区的保护方式   读 PROT_READ     写 PROT_WRITE     读写 PROT_READ | PROT_WRITE
	//	flags 映射区的特性   MAP_SHARED  写入映射区的数据会复制回文件 且允许其它映射该文件的进程共享
	//	MAP_PRIVATE  对映射区的写入操作会产生一个映射区的复制(copy - on - write), 对此区域所做的修改不会写回原文件
	//	fd     由open 返回的文件描述符，代表要映射的文件
	//	offset 以文件开始处的偏移量，必须是4K整数倍，通常为0，表示从文件头开始映射
	//建立私有映射 不影响原文件
	//成功返回地址指针  失败返回-1
    int* mmRet = (int*)mmap(0, mmFileStat_.st_size, PROT_READ, MAP_PRIVATE, srcFd, 0);
    if(*mmRet == -1) {
        errorContent(buff, "File NotFound!");
        return; 
    }
    mmFile_ = (char*)mmRet;//建立文件-内存映射
    close(srcFd);//关闭文件
    buff.append("Content-length: " +  to_string(mmFileStat_.st_size) + "\r\n\r\n");//生成内容信息
}

void HTTPresponse::unmapFile_() {
    if(mmFile_) {
        munmap(mmFile_, mmFileStat_.st_size);//释放内存映射区
        mmFile_ = nullptr;
    }
}

 string HTTPresponse::getFileType_() {
    /* 判断文件类型 */
     string::size_type idx = path_.find_last_of('.');
    if(idx ==  string::npos) {
        return "text/plain";
    }
     string suffix = path_.substr(idx);
    if(SUFFIX_TYPE.count(suffix) == 1) {
        return SUFFIX_TYPE.find(suffix)->second;
    }
    return "text/plain";
}

void HTTPresponse::errorContent(Buffer& buff,  string message) 
{
     string body;
     string status;
    body += "<html><title>Error</title>";
    body += "<body bgcolor=\"ffffff\">";
    if(CODE_STATUS.count(code_) == 1) {
        status = CODE_STATUS.find(code_)->second;
    } else {
        status = "Bad Request";
    }
    body +=  to_string(code_) + " : " + status  + "\n";
    body += "<p>" + message + "</p>";
    body += "<hr><em>WebServer_test</em></body></html>";

    buff.append("Content-length: " +  to_string(body.size()) + "\r\n\r\n");
    buff.append(body);
}
