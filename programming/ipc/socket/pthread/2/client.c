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
 *	client.c
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
#include <pthread.h>

#include "common.h"

int main(int argc, char **argv)
{
	int sockfd;
	pthread_t recv_tid;
	struct sockaddr_in servaddr;

	if(argc != 2)
	{
		perror("Usage: tcpip <IP Address>");
		exit(1);
	}

	if((sockfd = tcpsocket_client(argv[1])) == -1)
	{
		perror("socket error");
		exit(1);
	}
	
	if(pthread_create(&recv_tid, NULL, recv_message, &sockfd) == -1)
	{
		perror("pthread create error.\n");
		exit(1);
	}
    
	send_message(sockfd, recv_tid);
    
	return 1;
}//main
