/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:04 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:58 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:39 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:11 CST. */
/*



*/


/*sockaddr和sockaddr_in是相同的，只是结构不同，可相互转化*/
struct sockaddr
{
	/**
     * 通信的地址类型
	 * TCP/IP : AF_INET
	 */
	unsigned short int sa_family;
	/*保存IP地址和端口信息*/
	char sa_data[14];
};

struct sockaddr_in
{
	/*与sockaddr结构体中的sa_family相同*/
	unsigned short int sin_family;
	/*套接字使用的端口号*/
	uint16_t sin_port;
	/*需要访问的IP地址*/
	struct in_addr sin_addr;
	/*未使用的字段，填充为0*/
	unsigned char int sin_zero[8];
};


/*套接字类型*/
/**
SOCK_STREAM: TCP
SOCK_DGRAM: UDP
SOCK_RAW: 原始套接字
*/


/*域名返回IP地址的函数*/
struct hostent *gethostbyname(const char *name);

/*结构体hostent定义如下*/
struct hostent
{
	char *h_name;		/*正式的主机名*/
	char **h_aliases;	/*这个主机的别名*/
	int h_addrtype;		/*主机名的类型*/
	int h_length;		/*地址的长度*/
	char **h_addr_list;	/*从域名服务器取得的主机的地址*/
}

/*
解析域名时，可能没有这个域名或者服务器发生错误，
可能返回的信息如下（可以用error来捕获）
*/
HOST_NOT_FOUND	/*主机没有找到*/
NO_ADDRESS 		/*没有IP地址*/
NO_DATA			/*没有数据*/
NO_RECOVERY		/*域名服务器发生错误*/
TRY_AGAIN		/*请稍后再重试*/

/*用IP返回域名*/
struct hostent * gethostbyaddr(const void *addr, socklen_t len, int type);
//addr是一个保存了IP地址的字符串
//len是这个IP地址的长度
//type的值一般为AF_INET
//函数返回值：如果失败则返回NULL
//

/*网络协议与网络服务*/
/*按名称取得一个协议的数据*/
struct protoent *getprotobyname(char *name);
//使用这个函数之前需要包含netdb.h
#include <netdb.h>
//其中name是一个协议名称字符串，返回值是protoent的结构体指针
//结构体protoent的定义方法如下：
struct protoent
{
	char *p_name;		/*协议的名称*/
	char **p_aliases;	/*协议的别名*/
	int p_proto;		/*协议的序号*/
};


/*网络服务*/
//取得系统支持的网络服务getservent
struct servent *getservent(void);

//servent结构体的原型
struct servent
{
	char *s_name;		/*这个服务的名称*/
	char **s_aliases;	/*这个服务可能的别名*/
	int s_port;			/*这个服务可能的端口*/
	char *s_proto;		/*这个服务使用的协议*/
}

//用名称取得系统所支持的服务：getservbyname
struct servent *getservbyname(char *name, char *proto);
//name为服务器的名称
//proto是某服务使用的协议


//由端口取得服务名称
struct servent *getservbyport(int port, char *proto);
//port是一个端口的编号，用ntohs函数转换
//proto是一个表示协议的字符串
//函数返回这个端口服务的servent类型的指针
//servent定义同上


//将网络地址转换成整数：
long inet_addr(char *cp);
//使用这个函数需要包含下面的头文件
//sys/socket.h
//netinet/in.h
//arpa/inet.h


//IP转化为长整形
long inet_addr(char *cp);


//将长整形转化为一个十进制网络地址
char *inet_ntoa(struct in_addr in);
//in是一个in_addr类型的结构体，定义如下
struct in_addr
{
	uint32_t s_addr; //是一个长整形数
}
//需要包含
//sys/socket.h
//netinet/in.h
//arpa/inet.h


//计算机中的32位长整形数转换为网络字符顺序的32位长整形数
//计算机中的16位长整形数转换为网络字符顺序的16位长整形数
//网络字符顺序的32位长整形数转换为计算机中的32位长整形数
//网络字符顺序的16位长整形数转换为计算机中的16位长整形数
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);
//需要包含
//arpa/inet.h
//整理老说就是网络上的字符数字和计算机上的不一样，用这几个函数转换就行



//网络编程中的错误处理
void herror(const char *s);
//需要包含
//netdb.h


//错误处理
void herror(char *s);
//先输出这个字符串，然后打印错误信息
//需要包含：netdb.h

//捕获错误编号
extern int h_errno;
//输出错误
char *hstrerror(int err);
//需要包含: netdb.h


//套接字 = 传输层协议 + 端口号 + IP地址
int socket(int domain, int type, int protocol);
//domain：指定使用何种的地址类型
//type：  设置通信协议类型
//protocol：用来指定socket所用的传输协议编号，一般为0
//socket函数需要包含的头文件
#include <sys/socket.h>
#include <sys/type.h>
//如果套接字成功，则返回这些编号，如果不成功返回-1
EPROTONOSUPPORT：参数domain指定的类型不支持参数type或protocol指定的协议
ENFILE：核心内存不足，无法建立新的socket结构
EMFILE：进程文件表一处，无法再建立新的套接字
EACCESS：权限不足
ENOBUFS、ENOMEN：内存不足
EINVAL：参数不合法
//只创建套接字无法进行正常的网络体系，需要与IP地址和端口号连接起来


//获得socket状态
int getsockopt(int s, int level, 
			   int optname, void *optval, socklen_t optlen);
//s：已经建立的socket编号
//level：需要设置的网络层
//optname：是需要获取的选项，可以设置成SO_DEBUG等
//optval：取得的某个参数的返回指针，
//optlen：optval的内存长度
//函数getsockopt如果成功返回1，不成功返回-1，错误信息如下
//EBADF：参数s不是合法的socket代码
//ENOTSOCK：参数s为一打开文件的编号，不是一个socket
//ENOPROTOOPT：参数optname指定的选项不正确
//EFAULT：参数optval指针指向的内存空间无法读取
//需要包含的头文件
#include <sys/socket.h>
#include <sys/types.h>


//设置socket状态
int socket(int s, int level, int optname, const void *optval, socklen_t optlen);


//无连接的套接字通信UDP
int sendto(int s, void *msg, int len, unsigned int flags, struct sockaddr *to, int tolen);

struct sockaddr_in
{
	unsigned short int sin_family; //与sockaddr结构体中的sa_family相同
	uint16_t sin_port;	//套接字使用的端口号
	struct in_addr sin_addr; //需要访问的IP地址
	unsigned char sin_zero[8]; //未使用的字段，填充为0
};

struct in_addr
{
	uint32_t s_addr;
};


























