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
/**
 * server.c
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
#include <pthread.h>

#include "common.h"

int main(int argc, char **argv)
{
	int listenfd, connfd;
	socklen_t clilen;
	pthread_t recv_tid;
	struct sockaddr_in cliaddr;

	if((listenfd = tcpsocket_server()) == -1)
	{
		perror("socket error.\n");
		exit(1);
	}

	clilen = sizeof(cliaddr);
	if((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
	{
		perror("accept error.\n");
		exit(1);
	}

	printf("server: got connection from %s:%d\n", 
            inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);

	if(pthread_create(&recv_tid, NULL, recv_message, &connfd) == -1)
	{
		perror("pthread create error\n");
		exit(1);
	}
    
	send_message(connfd, recv_tid);
    
	return 1;
}
