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
 * UDP socket client.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>

const int SERV_PORT = 6000;
const int MAXLINE = 2048;

void dg_cli(FILE *fp, int sockfd, 
			const struct  sockaddr *pservaddr, 
			socklen_t servlen)
{
	int n;
	char sendline[MAXLINE], recvline[MAXLINE + 1];
	
	while(fgets(sendline, MAXLINE, fp) != NULL)
	{
		if(sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen) < 0)
		{
			perror("sendto error");
			exit(1);
		}
		if((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) < 0)
		{
			perror("recvfrom error");
			exit(1);
		}
		recvline[n] = '\0';
		fputs(recvline, stdout);
	}
}

int main(int argc, char *argv[])
{
	int sockfd, t;
	struct sockaddr_in servaddr;

	if(argc != 2)
	{
		perror("Usage: udpcli <IPAddress>");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);

	if((t = inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) <= 0)
	{
		perror("inet_pton error");
		exit(1);
	}
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket error");
		exit(1);
	}

	dg_cli(stdin, sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	exit(0);


	return 1;
}
