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
 * 接收连接函数
 * int accept(int s, struct sockaddr *addr, int *addrlen);
 * 	s: 处于监听状态的socket
 * 	addr 一个sockaddr结构体指针
 * 	addrlen：是sockaddr的内存长度
 *
 * 当accept函数就收一个连接时，或返回一个新的socket编号，以后的数据传输与读取
 * 就是通过这个新的socket编号来处理，原理参数中的socket可以继续使用，接收
 * 连接收，远程主机的地址和端口信息将会保存到addr所指向的结构体内，如果处理
 * 失败，返回-1.
 * 函数可能产生如下的错误：
 * 		EBADF：参数s不是一个合法的socket代码
 * 		EFAULT：参数uaddr指针西乡无法存取的内存
 * 		ENOTSOCK：参数s为一文件描述词，不是socket
 * 		EOPNOTSUPP：指定的socket不是SOCK_STREAM
 * 		EPERM：防火墙拒绝这个链接
 * 		ENOMEN：核心内存不足
 * 头文件
 * #include <sys/type.h>
 * #include <sys/socket.h>
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT 5678
#define MAX 10

int main()
{
	int sockfd, newsockfd, is_connected[MAX], fd;

	struct sockaddr_in addr;
	int addr_len = sizeof(struct sockaddr_in);
	fd_set myreadfds;

	char msgbuffer[256];

	char msg[] = "This is the message from server.Connection\n";

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket: ");
		exit(1);
	}
	else
	{
		printf("socket created\nsocket id: %d\n", sockfd);
	}

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	/*绑定端口号*/
	if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("connect");
		exit(1);
	}
	else
	{
		printf("connected\nlocal port: %d\n", PORT);
	}

	/*监听一个端口号*/
	if(listen(sockfd, 3) < 0)
	{
		perror("listen");
		exit(1);
	}
	else
	{
		printf("listenning\n");
	}

	/*接收一个连接*/
	//这是服务器的API函数
	//所以说只运行这个函数不行，需要从客户端的端口获取消息才行
	if((newsockfd = accept(sockfd, (struct sockaddr *)&addr, &addr_len)) < 0)
	{
		perror("accept: ");
		exit(1);
	}
	else
	{
		printf("accept a new connection\n");
	}
	
	exit(0);
}
