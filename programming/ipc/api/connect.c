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
/**
 * 请求连接connect
 * int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
 * 	sockfd: 已经建立的socket
 * 	serv_addr：指向sockaddr的结构体，存储了远程服务器IP与端口信息
 * 	addrlen：是sockaddr结构体的内存长度
 *
 * 函数会将本地的socket连接到服务器的serv_addr所指向的服务器IP与端口
 * 如果连接成功返回值为0，失败返回-1.
 * 函数可能发生的错误：
 * 		EBADF：参数sockfd不是一个合法的socket
 * 		EFAULT：参数serv_addr指针指向了一个无法读取的内存空间
 * 		ENOTSOCK：参数sockfd是文件描述次，不是正常的socket
 * 		ECONNREFUSED：拒绝访问
 * 		ETIMEDOUT：操作超时
 * 		ENETUNREACH：无法传送数据报至指定的主机
 * 		EAFNOSUPPORT：sockaddr结构的sa_family不正确
 * 		EALREADY：socket不能阻断，但是以前的链接操作还未完成
 * 头文件
 * 	#include <sys/type.h>
 * 	#include <sys/socket.h>
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*定义一个表示端口的常量*/
#define PORT 5678

#define REMOTE_IP "127.0.0.1"


int main()
{
	int s;
	struct sockaddr_in addr;
	
	char msgbuffer[256];

	/*建立一个socket*/
	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket: ");
		exit(1);
	}
	else
	{
		printf("socket created\n");
		printf("socket id: %d \n", s);
	}	

	/*清空表示地址的结构体变量*/
	bzero(&addr, sizeof(addr));
	/*设置addr的成员*/
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(REMOTE_IP);

	/*链接到远程服务器*/
	if(connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		perror("connect");
		exit(1);
	}
	else
	{
		printf("connected\n");
		printf("remote ip: %s", REMOTE_IP);
		printf("remote port: %d\n", PORT);
	}
	exit(0);
}
