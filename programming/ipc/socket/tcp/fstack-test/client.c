/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:16 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:05 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:45 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:17 CST. */
/**
 * client.c 客户端代码
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "common.h"

#if 0
/*readline函数实现*/
ssize_t readline(int fd, char *vptr, size_t maxlen)
{
	ssize_t n, rc;
	char c, *ptr;

	ptr = vptr;

	for(n = 1; n < maxlen; n++)
	{
		if((rc = read(fd, &c, 1)) == 1)
		{
			*ptr++ = c;
			if(c == '\n') 
			{
				break;//新的一行，等同于fgets()
			}
		}
		else if(rc == 0)
		{
			*ptr = 0;
			return (n-1);//EOF, 读取了(n-1)字节的数据
		}
		else
		{
			return -1;//读取错误, read()的返回值
		}
	}
	*ptr = 0;//字符串结尾，等同于NULL
	return n;
}
#endif


int main(int argc, char *argv[])
{
   	/*声明套接字和链接服务器地址*/
	int sockfd;
	struct sockaddr_in servaddr;

	/*判断是否为合法输入*/
	if(argc != 2)
	{
		perror("usage: tcpcli <IP address>");
		exit(1);
	}

	/*1.创建套接字*/
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error");
		exit(1);
	}

	/*2.设置连接服务器地址结构*/
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0)
	{
		printf("inet_pton error for %s\n", argv[1]);
		exit(1);
	}
	
	/*3.发送连接服务器请求*/
	if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect error");
		exit(1);
	}	

	/*4.消息处理*/
	char sendline[MAX_LINE], recvline[MAX_LINE];
	
	printf("Input: ");
#if 1
	while(fgets(sendline, MAX_LINE, stdin) != NULL)
	{
		write(sockfd, sendline, strlen(sendline));

		printf("Client Send: %s\n", sendline);
		
#if 0
		if(readline(sockfd, recvline, MAX_LINE) == 0)
		{
			perror("server terminated prematurely");
			exit(1);
		}
#else
		read(sockfd, recvline, MAX_LINE);
#endif
		printf("Client Recv: %s\n", recvline);
		/* if(fputs(recvline, stdout) == EOF)
		{
			perror("fputs error");
			exit(1);
		} */
		printf("Input>>");
	}
#else
	
#endif
	/*5.关闭套接字*/
	close(sockfd);



	return 1; 
}
