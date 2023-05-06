/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:15 CST. */
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
#include <pthread.h>

#include "common.h"

int tcpsocket_server()
{
    struct sockaddr_in servaddr;
    
    int listenfd;
    
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0))<0)
    {
		return -1;
    } 
    
    bzero(&servaddr, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    
    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
		return -1;
    }
    
    if(listen(listenfd, LISTENQ) < 0)
    {
		return -1;
    }  
    
    return listenfd;
}

int tcpsocket_client(const char *ipv4)
{
    int sockfd;
	struct sockaddr_in servaddr;
    
    if(ipv4 == NULL)
    {
        return -1;
    }
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		return -1;
	}
    
    bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	
	if(inet_pton(AF_INET, ipv4, &servaddr.sin_addr) < 0)
	{
		return -1;
	}
	
	if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		return -1;
	}	
    return sockfd;
}

void *recv_message(void *fd)
{
	int sockfd = *(int*)fd;
    int n;
    char buf[MAX_LINE];
    
	while(1)
	{
		memset(buf, 0, MAX_LINE);
        
		if((n = recv(sockfd, buf, MAX_LINE, 0)) == -1)
		{
			perror("recv error.\n");
			exit(1);
		}

		if(strncmp(buf, QUIT, strlen(QUIT)) == 0)
		{
			printf("closed.\n");
			close(sockfd);
			exit(0);
		}

		printf("recv : %s", buf);
	}
}

void send_message(int sockfd, pthread_t tid)
{
    char msg[MAX_LINE];
	memset(msg, 0, MAX_LINE);
	
	while(fgets(msg, MAX_LINE, stdin) != NULL)
	{
		if(strncmp(msg, QUIT, strlen(QUIT)) == 0)
		{
			memset(msg, 0, MAX_LINE);
			strcpy(msg, QUIT);
			send(sockfd, msg, strlen(msg), 0);
			pthread_cancel((pthread_t )tid);
			close(sockfd);
			exit(0);
		}

		if(send(sockfd, msg, strlen(msg), 0) == -1)
		{
			perror("send error.\n");
			continue;
		}
	}
}