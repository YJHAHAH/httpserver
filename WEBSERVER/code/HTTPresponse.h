// encode UTF-8

// @Author        : yjh
// @Date          : 2022-06-23


#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <unordered_map>
#include <fcntl.h>  //open
#include <unistd.h> //close
#include <sys/stat.h> //stat
#include <sys/mman.h> //mmap,munmap
#include <assert.h>
#include "buffer.h"
#include "log.h"
using namespace std;
class HTTPresponse
{
public:
    HTTPresponse();
    ~HTTPresponse();

    void init(const  string& srcDir, string& path,bool isKeepAlive=false,int code=-1);
    void makeResponse(Buffer& buffer);
    void unmapFile_();
    char* file();
    size_t fileLen() const;
    void errorContent(Buffer& buffer, string message);
    int code() const {return code_;}


private:
    void addStateLine_(Buffer& buffer);
    void addResponseHeader_(Buffer& buffer);
    void addResponseContent_(Buffer& buffer);

    void errorHTML_();
     string getFileType_();

    int code_;
    bool isKeepAlive_;

     string path_;
     string srcDir_;

    char* mmFile_;
    struct  stat mmFileStat_;

    static const  unordered_map< string,  string> SUFFIX_TYPE;
    static const  unordered_map<int,  string> CODE_STATUS;
    static const  unordered_map<int,  string> CODE_PATH;
    
};



#endif //HTTP_RESPONSE_H