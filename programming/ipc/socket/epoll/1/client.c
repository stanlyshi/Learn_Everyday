/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:14 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:03 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:44 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:16 CST. */
/**
 * client.c
 * 2018.07 rt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#define PORT 7777
#define MAX_LINE 2048

int max(int a, int b)
{
	return a>b?a:b;
}

/*readline 函数实现*/
ssize_t readline(int fd, char *vptr, size_t maxlen)
{
	ssize_t n, rc;
	char c, *ptr;

	ptr = vptr;

	for(n=1; n<maxlen; n++)
	{
		if((rc = read(fd, &c, 1)) == 1)
		{
			*ptr++ = c;
			if(c == '\n')
			{
				break;/*new line*/
			}
		}
		else if(rc == 0)
		{
			*ptr = 0;
			return n-1;/*EOF, n-1 bytes were read*/
		}
		else
		{
			return -1;/*error, errno set by read()*/
		}
	}
	*ptr = 0;
	return n;
}

/*普通客户端消息处理函数*/
void str_cli(int sockfd)
{
	/*发送和接收缓冲区*/
	char sendline[MAX_LINE], recvline[MAX_LINE];

	while(fgets(sendline, MAX_LINE, stdin) != NULL)
	{
		write(sockfd, sendline, strlen(sendline));
		bzero(recvline, MAX_LINE);
	
		if(readline(sockfd, recvline, MAX_LINE) == 0)
		{
			perror("server terminated prematurely\n");	
			exit(1);
		}

		if(fputs(recvline, stdout) == EOF)
		{
			perror("fputs error\n");
			exit(1);
		}
		bzero(sendline, MAX_LINE);
	}
}

int main(int argc, char **argv)
{
	/*声明套接字和链接服务器地址*/
	int sockfd;
	struct sockaddr_in servaddr;

	/*判断是否为合法输入*/
	if(argc != 2)
	{
		perror("Usage: tcpip <IPAddress>");
		exit(1);
	}

	/*1.创建套接字*/
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error\n");
		exit(1);
	}

	/*2.设置服务器连接地址*/
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0)
	{
		printf("inet_pton error for %s\n", argv[1]);
		exit(1);
	}
	
	/*3.发送连接服务器请求*/
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect error\n");
		exit(1);
	}	

	/*调用消息处理函数*/
	str_cli(sockfd);
	exit(0);

}






























