/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:11 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:02 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:43 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:14 CST. */
/**
 *  UDP socket 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/ip.h>

const int SERV_PORT = 6000;
const int MAXLINE = 2048;

void dg_echo(int sockfd, struct sockaddr *pcliaddr, socklen_t clilen)
{
	int n;
	socklen_t len;
	char mesg[MAXLINE];

	for(;;)
	{
		len = clilen;
		if((n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len)) < 0)
		{
			perror("recvfrom error");
			exit(1);
		}

		if((n = sendto(sockfd, mesg, n, 0, pcliaddr, len)) < 0)
		{
			perror("sendto error");
			exit(1);
		}
	}
}

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr, cliaddr;

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;	
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket error");
		exit(1);
	}
	if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
	{
		perror("bind error");
		exit(1);
	}

	dg_echo(sockfd, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

	return 1;
}
