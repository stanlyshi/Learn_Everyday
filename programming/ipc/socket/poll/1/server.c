/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:14 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:04 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:44 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:16 CST. */
/**
 *	server.c
 *  rt 2018.07
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <poll.h>
#include <limits.h> /*for OPEN_MAX*/
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#ifndef OPEN_MAX
#define OPEN_MAX 1024
#endif

#ifndef INFTIM
#define INFTIM -1
#endif

#define PORT 8888
#define MAX_LINE 2048
#define LISTENQ 20

int main(int argc, char **argv)
{
	int i, maxi, listenfd, connfd, sockfd;
	int nready;
	ssize_t n, ret;

	struct pollfd client[OPEN_MAX];

	char buf[MAX_LINE];

	socklen_t clilen;

	struct sockaddr_in servaddr, cliaddr;

	/*1.得到监听描述符*/
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	/*2.绑定套接字*/
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	/*3.监听*/
	listen(listenfd, LISTENQ);

	/*4.设置poll*/
	client[0].fd = listenfd;
	client[0].events = POLLRDNORM;
	for(i=1; i<OPEN_MAX; ++i)
	{
		client[i].fd = -1;
	}
	maxi = 0;

	/*5.进入服务器接收请求死循环*/
	while(1)
	{
		nready = poll(client, maxi+1, INFTIM);
		if(client[0].revents & POLLRDNORM)
		{
			/*接收客户端请求*/
			clilen = sizeof(cliaddr);
			printf("\naccept connection~\n");

			if((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, 
															 &clilen)) < 0)
			{
				perror("accept error\n");
				exit(1);
			}

			printf("accept a new client: %s: %d\n", 
					inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);

			/*讲客户连接套接字描述符添加到数组*/
			for(i=1; i<OPEN_MAX; ++i)
			{
				if(client[i].fd < 0)
				{
					client[i].fd = connfd;
					break;
				}
			}

			if(OPEN_MAX == i)
			{
				perror("too many connection\n");
				exit(1);
			}

			/*该描述符等待的事件*/
			client[i].events = POLLRDNORM;

			if(i>maxi)
			{
				maxi = i;
			}
			if(--nready < 0)
			{
				continue;
			}
		}

		for(i=1; i<=maxi; ++i)
		{
			if((sockfd = client[i].fd) < 0)
			{
				continue;
			}	
			/*改连接描述符实际发生的事件*/
			if(client[i].revents & (POLLRDNORM | POLLERR))
			{
				/*处理客户请求*/
				printf("\nreading the socket~~~ \n");

				bzero(buf, MAX_LINE);
				if((n = read(sockfd, buf, MAX_LINE)) <= 0)
				{
					close(sockfd);
					client[i].fd = -1;
				}
				else
				{
					printf("clint[%d] send message: %s\n", i, buf);
					if((ret = write(sockfd, buf, n)) != n)
					{
						printf("error writing to the sockfd!\n");
						break;
					}
				}
				if(--nready <= 0)
				{
					break;
				}
			}
		}
	}
	exit(0);
}
