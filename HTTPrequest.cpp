// encode UTF-8

// @Author        : yjh
// @Date          : 2022-06-24

#include "HTTPrequest.h"
using namespace std;
const unordered_set<string> HTTPrequest::DEFAULT_HTML{
            "/index", "/register", "/login","/welcome", "/video", "/picture",};
const unordered_map<string, int> HTTPrequest::DEFAULT_HTML_TAG{
			{"/register.html", 0}, {"/login.html", 1},};

void HTTPrequest::init() {
    method_ = path_ = version_ = body_ = "";
    state_ = REQUEST_LINE;//从请求行开始解析
    header_.clear();
    post_.clear();
}

bool HTTPrequest::isKeepAlive() const {
    if(header_.count("Connection") == 1) {
        return header_.find("Connection")->second == "keep-alive" && version_ == "1.1";//保持http连接  http1.1默认开启
    }
    return false;
}

bool HTTPrequest::parse(Buffer& buff) {//读缓冲区 解析
    const char CRLF[] = "\r\n";//行结束符
    if(buff.readableBytes() <= 0) { //无可读数据
        return false;
    }
    // cout<<"parse buff start:"<< endl;
    //buff.printContent();
    // cout<<"parse buff finish:"<< endl;
    while(buff.readableBytes() && state_ != FINISH)//状态不为FINISH
	{
		//请求消息  请求行  请求头  空行   //主体数据
        const char* lineEnd = search(buff.curReadPtr(), buff.curWritePtrConst(), CRLF, CRLF + 2);//<algorithm> //读取一行  
        string line(buff.curReadPtr(), lineEnd);
        switch(state_)
        {
        case REQUEST_LINE://请求行
            // cout<<"REQUEST: "<<line<< endl;
            if(!parseRequestLine_(line)) {
                return false;
            }
            parsePath_();
            break;    
        case HEADERS://请求头
            parseRequestHeader_(line);
            if(buff.readableBytes() <= 2) {
                state_ = FINISH;
            }
            break;
        case BODY://数据本体
			parseBody_(line);
            break;
        default:
            break;
        }
        if(lineEnd == buff.curWritePtr())//可读数据读完
		{ break; }
        buff.updateReadPtrUntilEnd(lineEnd + 2);//更新读指针 换行
    }
	LOG_DEBUG("[%s], [%s], [%s]", method_.c_str(), path_.c_str(), version_.c_str());
    return true;
}

void HTTPrequest::parsePath_() {
    if(path_ == "/") {
        path_ = "/index.html"; 
    }
    else {
        for(auto &item: DEFAULT_HTML) {
            if(item == path_) {
                path_ += ".html";
                break;
            }
        }
    }
}

bool HTTPrequest::parseRequestLine_(const  string& line) { //解析请求行
	//1.备选字符集：规定某*一位字符*可用的备选字符的集合
	//语法：[可选字符列表] [a - z]-- > 1位小写字母  [A - Z]-- > 1位大写字母  必须匹配其中1个
	//[^ ]匹配除空格外任意字符

	// 2.预定义字符集：为常用的字符集专门提供的简化写法！
	/*  “\d”-->[0-9]-->1位数字

	   “\w”-- > [0 - 9a - zA - Z_]-- > 1位字母, 数字或_
		“\s”-- > 1位空字符：匹配任何空白字符，包括空格、制表符、换页符等等。等价于[\f\n\r\t\v]。
		“.“: 除换行回车外的任何一个字符  如"a.[0-9]"：表示一个字符串有一个"a"后面跟着一个任意字符和一个数字
		\t--------匹配一个制表符
		预定义字符的反义：预定义字符的大写形式都是小写的反义
		\D---- > 1位非数字字符
		\S------匹配任何非空白字符。等价于[^\f\n\r\t\v]。
		\v------匹配一个垂直制表符
		\n------匹配一个换行符
		\r------ - 匹配一个回车符
		*/

// 3. 数量词：规定相邻的字符集可出现的次数
	//{n}-- > 必须反复出现n位   {n,m}--> 最少出现n次，最多出现m次  {n,}-->至少出现n次，多了不限！

	// 不确定数量：3种：
	//*: 有没有都行，次数不限，相当于{0,}    +: 至少1次，重复次数不限，相当于{1,}  ?: 有没有都行，最多1次,相当于{0,1}
	// ():分组    |: 左右两正则表达式选其一

	//4. 指定匹配位置：
	// ^表达式: 必须以表达式的规则为开头  表达式$: 必须以表达式的规则为结尾
	//只要在程序中执行验证：都要前加^后加$*  表示从头到尾完整匹配。
	//5.预判：在正式匹配正则表达式之前，先预读整个字符串，进行初步匹配，如果预判都未通过，则不再验证！
	//(?=表达式): 先浏览字符串是否满足表达式的要求   (?!表达式):先检查字符串是否不满足表达式要求

	//6.所谓特殊字符，就是一些有特殊含义的字符，如"*.txt"中的*，简单的说就是表示任何字符串的意思。
	//如果要查找文件名中有*的文件，则需要对*进行转义，即在其前加一个\




    regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");//正则表达式
    smatch subMatch;
    if(regex_match(line, subMatch, patten))//正则表达式匹配 HTML解析
	{   
		//验证——从头到尾完整匹配！ 查找——只要部分匹配即可！
		// string whole=subMatch[0] 完整匹配
		//method： GET POST HEAD  PUT  DELETE CONNECT OPTIONS  TRACE
        method_ = subMatch[1];//请求方法 
        path_ = subMatch[2];//请求路径
        version_ = subMatch[3];//http版本
        state_ = HEADERS;//更新解析状态
        return true;
    }
    return false;
}

void HTTPrequest::parseRequestHeader_(const  string& line) {
    regex patten("^([^:]*): ?(.*)$");//正则表达式  regex  smatch  regex_match
    smatch subMatch;
    if(regex_match(line, subMatch, patten)) {
        header_[subMatch[1]] = subMatch[2]; //头 哈希映射
		// host: 只能有1个空格   (.*)任意字符
    }
    else { //空行  解析
        state_ = BODY;
    }
}

void HTTPrequest::parseBody_(const  string& line) {
    body_ = line;
    parsePost_();//解析post报文
    state_ = FINISH;
	LOG_DEBUG("Body:%s, len:%d", line.c_str(), line.size());
}

// int HTTPrequest::convertHex(char ch) {
//     if(ch >= 'A' && ch <= 'F') return ch -'A' + 10;
//     if(ch >= 'a' && ch <= 'f') return ch -'a' + 10;
//     return ch;
// }
unsigned char HTTPrequest::FromHex(unsigned char ch)
{
    unsigned char y;  
    if (ch >= 'A' && ch <= 'Z') y = ch - 'A' + 10;  
    else if (ch >= 'a' && ch <= 'z') y = ch - 'a' + 10;  
    else if (ch >= '0' && ch <= '9') y = ch - '0';  
    else return 0;  
    return y;
}
void HTTPrequest::parsePost_() {//post解析
	if (method_ == "POST" && header_["Content-Type"] == "application/x-www-form-urlencoded") {
		ParseFromUrlencoded_();
		if (DEFAULT_HTML_TAG.count(path_)) {
			int tag = DEFAULT_HTML_TAG.find(path_)->second;
			LOG_DEBUG("Tag:%d", tag);
			if (tag == 0 || tag == 1) {
				bool isLogin = (tag == 1);
				if (UserVerify(post_["username"], post_["password"], isLogin)) //身份验证 哈希表 字段
				{
					path_ = "/welcome.html";
				}
				else {
					path_ = "/error.html";
				}
			}
		}
	}
}
bool HTTPrequest::UserVerify(const string &name, const string &pwd, bool isLogin) {
	if (name == "" || pwd == "") { return false; }
	LOG_INFO("Verify name:%s pwd:%s", name.c_str(), pwd.c_str());
	MYSQL* sql;//sql查询
	SqlConnRAII(&sql, SqlConnPool::Instance());// 从sql 连接池中选取连接 放入sql
	assert(sql);

	bool flag = false;
	unsigned int j = 0;
	char order[256] = { 0 };
	MYSQL_FIELD *fields = nullptr;
	MYSQL_RES *res = nullptr;

	if (!isLogin) { flag = true; }
	/* 查询用户及密码 */ //验证密码行为
	snprintf(order, 256, "SELECT username, password FROM user WHERE username='%s' LIMIT 1", name.c_str());
	LOG_DEBUG("%s", order);

	if (mysql_query(sql, order)) {
		mysql_free_result(res);
		return false;
	}
	res = mysql_store_result(sql);
	j = mysql_num_fields(res);
	fields = mysql_fetch_fields(res);

	while (MYSQL_ROW row = mysql_fetch_row(res)) {
		LOG_DEBUG("MYSQL ROW: %s %s", row[0], row[1]);
		string password(row[1]);
		/* 注册行为 且 用户名未被使用*/
		if (isLogin) {
			if (pwd == password) { flag = true; }
			else {
				flag = false;
				LOG_DEBUG("pwd error!");
			}
		}
		else {
			flag = false;
			LOG_DEBUG("user used!");
		}
	}
	mysql_free_result(res);

	/* 注册行为 且 用户名未被使用*/
	if (!isLogin && flag == true) {
		LOG_DEBUG("regirster!");
		bzero(order, 256);
		snprintf(order, 256, "INSERT INTO user(username, password) VALUES('%s','%s')", name.c_str(), pwd.c_str());
		LOG_DEBUG("%s", order);
		if (mysql_query(sql, order)) {
			LOG_DEBUG("Insert error!");
			flag = false;
		}
		flag = true;
	}
	SqlConnPool::Instance()->FreeConn(sql);//释放连接 放入sql连接池
	LOG_DEBUG("UserVerify success!!");
	return flag;
}
string HTTPrequest::urlDecode(const string& str)
{
//将键值对的参数用 & 连接起来，如果有空格，将空格转换为 + 加号;有特殊符号，将特殊符号转换为 ASCII HEX 值
    string decode_str="";
    size_t length = str.length();  
    for (size_t i = 0; i < length; i++) {  
        if (str[i] == '+') {
            decode_str += ' ';  
        } else if (i + 2 < length && str[i] == '%') {
            unsigned char high = FromHex((unsigned char)str[++i]);  
            unsigned char low = FromHex((unsigned char)str[++i]);  
            decode_str += high*16 + low;  
        } else {
            decode_str += str[i];  
        }
    }
    return decode_str;  
}

void HTTPrequest::ParseFromUrlencoded_() {
    cout<<body_<<endl;
    //LOG_DEBUG("Body:%s, len:%d", body_.c_str(), body_.size());
        if(body_.size() == 0) { return; }
    //string decoded_body{urlDecode(body_)};
   //LOG_DEBUG("Body:%s, len:%d", decoded_body.c_str(), decoded_body.size());
        string key, value;
        int num = 0;
        int n = body_.size();
        int i = 0, j = 0;
        for(; i < n; i++) {
            char ch = body_[i];
            switch (ch) {
            case '=':
                key = urlDecode(body_.substr(j, i - j));
                j = i + 1;
                break;
            case '&':
                value = urlDecode(body_.substr(j, i - j));
                j = i + 1;
                post_[key] = value;
				LOG_DEBUG("%s = %s", key.c_str(), value.c_str());
                break;
            default:
                break;
            }
        }
        assert(j <= i);
        if(post_.count(key) == 0 && j < i) {
            value = urlDecode(body_.substr(j, i - j));//存入最后一个key -value 
            post_[key] = value;
			LOG_DEBUG("%s = %s", key.c_str(), value.c_str());
        } 
}

string HTTPrequest::path() const{
    return path_;
}

string& HTTPrequest::path(){
    return path_;
}
string HTTPrequest::method() const {
    return method_;
}

string HTTPrequest::version() const {
    return version_;
}

string HTTPrequest::getPost(const string& key) const {
    assert(key != "");
    if(post_.count(key) == 1) {
        return post_.find(key)->second;
    }
    return "";
}

string HTTPrequest::getPost(const char* key) const {
    assert(key != nullptr);
    if(post_.count(key) == 1) {
        return post_.find(key)->second;
    }
    return "";
}