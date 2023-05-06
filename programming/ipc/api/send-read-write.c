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
 * 信息发送函数send
 * int send(int s, const void *msg, int len, unsigned int flags);
 * 		s:已经建立的socket	
 * 		msg：需要发送的指针
 * 		len：需要发送数据的长度
 * 		flags：一般为0，可能的数值为：
 * 			MSG_OOB：传送的数据已out-of-band的方式发送
 * 			MSG_DONTROUTE：取消路由表查询
 * 			MSG_DONTWAIT：设置为不可阻断传输
 * 			MSG_NOSIGNAL：此传输不可悲sigpipe信号中断
 * 如果数据成功，函数返回一传送的字符个数，否则返回-1
 * 函数可能发生的错误：
 * 		EBADF：参数s不是一个正确的socket
 * 		EFAULT：参数中的指针指向了不可读取的内存空间
 * 		ENOTSOCK：参数是一个文件，而不是一个socket
 * 		EINTR：被信号所中断
 * 		EAGAIN：此操作会终端进程，但是socket不允许中断
 * 		ENOBUFS：系统的换种内存不足
 * 		ENOMEM：核心内存不足
 * 		EINVAL：传给系统调用的参数不正确
 * 头文件
 * 	#include <sys/types.h>
 * 	#include <sys/socket.h>
 */

/**
 * 数据传输函数write和read
 * ssize_t write(int fd, const void *buf, size_t count);
 * ssize_t read (int fd,       void *buf, size_t count);
 * 		fd: 已经建立的socket
 * 		buf：指针
 * 		count：buf长度
 * read函数读取字节时，会把读取的内容保存到buf指向的内存中，然后返回读取到的
 * 		字节数；
 * write函数传输数据，会把buf指针指向的内存中的数据发送到socket连接的远程主机，
 * 		然后返回实际发送字节的个数。
 * 头文件：
 * 	#include <unistd.h>
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define PORT 6677

int main()
{
	int sockfd, newsockfd, fd;
	struct sockaddr_in addr;
	int addr_len = sizeof(struct sockaddr_in);

	fd_set myreadfds;
	char msgbuffer[256];
	char msg[] = "This is the message from server.Connection\n";

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(1);
	}
	else 
	{
		printf("socket created\n");
		printf("socked id: %d\n", sockfd);
	}

	/*清空zero所在的内存*/
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("connect");
		exit(1);
	}	
	else
	{
		printf("connected\n");
		printf("local port: %d\n", PORT);
	}

	/*监听一个端口号*/
	if(listen(sockfd, 3) < 0)
	{
		perror("listen error");
		exit(1);
	}
	else 
	{
		printf("listenning....\n");
	}

	/*接收一个连接*/
	if((newsockfd = accept(sockfd, (struct sockaddr*)&addr, &addr_len)) < 0)
	{
		perror("accept");
		exit(1);
	}
	else
	{
		printf("connect from: %s\n", inet_ntoa(addr.sin_addr));
	
		/*接收数据*/
		if(read(newsockfd, msgbuffer, sizeof(msgbuffer)) <= 0)
		{
			perror("accept");
			exit(1);
		}
		else
		{
			printf("message: %s\n", msgbuffer);
		}
	}

}


















