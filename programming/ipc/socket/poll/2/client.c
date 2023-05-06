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
 * client.c
 * rt 2018.10
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

#include "common.h"


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
			perror("server terminated prematurely");
			exit(1);
		}
		if(fputs(recvline, stdout) == EOF)
		{
			perror("fputs error");
			exit(1);
		}

		bzero(sendline, MAX_LINE);
	}
}

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	
	if(argc != 2)
	{
		perror("Usage: tcpip <IPAddress>");
		exit(1);
	}

	if((sockfd = tcpsocket_client(argv[1])) == -1)
	{
		perror("socket error");
		exit(1);
	}	

	str_cli(sockfd);
	exit(0);

}
