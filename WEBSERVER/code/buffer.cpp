// encode UTF-8

// @Author        : yjh
// @Date          : 2022-6-22

#include "buffer.h"

Buffer::Buffer(int initBuffersize):buffer_(initBuffersize),readPos_(0),writePos_(0){}

size_t Buffer::readableBytes() const
{
    return writePos_-readPos_;
}

size_t Buffer::writeableBytes() const
{
    return buffer_.size()-writePos_;
}

size_t Buffer::readBytes() const
{
    return readPos_;
}

const char* Buffer::curReadPtr() const
{
    return BeginPtr_()+readPos_;
}

const char* Buffer::curWritePtrConst() const
{
    return BeginPtr_()+writePos_;
}

char* Buffer::curWritePtr()
{
    return BeginPtr_()+writePos_;
}

void Buffer::updateReadPtr(size_t len)
{
    assert(len<=readableBytes());
    readPos_+=len;
}

void Buffer::updateReadPtrUntilEnd(const char* end)
{
    assert(end>=curReadPtr());
    updateReadPtr(end-curReadPtr());
}

void Buffer::updateWritePtr(size_t len)
{
    assert(len<=writeableBytes());
    writePos_+=len;
}

void Buffer::initPtr()
{
    bzero(&buffer_[0],buffer_.size());//将指定内存块的前n个字节全部设置为零 //cstring
    readPos_=0;
    writePos_=0;
}

void Buffer::allocateSpace(size_t len)
{
    //如果buffer_里面剩余的空间有len就进行调整，否则需要申请空间。
    //剩余空间包括write指针之前的空间和可写的空间
    if(writeableBytes()+readBytes()<len)//空间不足  已读+可写 为可用空间
    {
        buffer_.resize(writePos_+len+1);//申请空间
    }
    else{
        //将读指针置为0,调整一下
        size_t readable=readableBytes();
        std::copy(BeginPtr_()+readPos_,BeginPtr_()+writePos_,BeginPtr_());//将已读指针前移 复制未读数据
        readPos_=0;
        writePos_=readable; //更新读写指针
        assert(readable==readableBytes());
    }
}

void Buffer::ensureWriteable(size_t len)//确保全部读出
{
    if(writeableBytes()<len)//读不完 分配空间
    {
        allocateSpace(len);
    }
    assert(writeableBytes()>=len);
}

void Buffer::append(const char* str,size_t len)
{
    assert(str);
    ensureWriteable(len);//分配空间 扩展缓冲区
    std::copy(str,str+len,curWritePtr());//将临时缓冲区数据复制到缓冲区
    updateWritePtr(len);//更新写的位置
}

void Buffer::append(const std::string& str)
{
    append(str.data(),str.length());
}

void Buffer::append(const void* data,size_t len)
{
    assert(data);
    append(static_cast<const char*>(data),len);
}

void Buffer::append(const Buffer& buffer)
{
    append(buffer.curReadPtr(),buffer.readableBytes());
}

ssize_t Buffer::readFd(int fd,int* Errno)//读取文件内容
{
    char buff[65535];//暂时的缓冲区
    struct iovec iov[2];
	//struct iovec {
	//	void *iov_base; /*指向一个缓冲区，这个缓冲区是存放readv()所接收的数据或 //writev()将要发送的数据*/
	//	size_t iov_len; /*接收的最大长度以及实际写入的长度*/
	//};
    const size_t writable=writeableBytes();

    iov[0].iov_base=BeginPtr_()+writePos_;//自带缓冲区
    iov[0].iov_len=writable;
    iov[1].iov_base=buff;//临时缓冲区  扩展 分散接收来的数据
    iov[1].iov_len=sizeof(buff);

    const ssize_t len=readv(fd,iov,2);//<sys/uio.h> //分散读   即将数据从文件描述符读到分散的内存块中；
    if(len<0)
    {
        //std::cout<<"从fd读取数据失败！"<<std::endl;
        *Errno=errno;
    }
    else if(static_cast<size_t>(len)<=writable)//未写满自带缓冲区
    {
        writePos_+=len;//更新已写数据的指针 writepos
    }
    else{
        writePos_=buffer_.size();//缓冲区写满
        append(buff,len-writable); //扩展缓冲区
    }
    return len; 
}

ssize_t Buffer::writeFd(int fd,int* Errno)
{
    size_t readSize=readableBytes();
    ssize_t len=write(fd,curReadPtr(),readSize);
    if(len<0)
    {
        //std::cout<<"往fd写入数据失败！"<<std::endl;
        *Errno=errno;
        return len;
    }
    readPos_+=len;
    return len;
}

std::string Buffer::AlltoStr()
{
    std::string str(curReadPtr(),readableBytes());
    initPtr();
    return str;
}

char* Buffer::BeginPtr_()
{
    return &*buffer_.begin();
}

const char* Buffer::BeginPtr_() const
{
    return &*buffer_.begin();
}