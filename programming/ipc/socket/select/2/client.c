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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:16 CST. */
/**
 * select socket client.c
 *  2018.10 rt
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

int main(int argc, char *argv[])
{
	int sockfd;
    int maxfd;
	fd_set rset;
    
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
    
	char sendline[MAX_LINE], recvline[MAX_LINE];
	
	FD_ZERO(&rset);
	
	while(1)
	{
		/*将文件描述符和套接字描述符添加到rset描述符集*/
		FD_SET(fileno(stdin), &rset);
		FD_SET(sockfd, &rset);
		maxfd = max(fileno(stdin), sockfd) + 1;
		select(maxfd, &rset, NULL, NULL, NULL);

		if(FD_ISSET(fileno(stdin), &rset))
		{
			if(fgets(sendline, MAX_LINE, stdin) == NULL)
			{
				printf("read nothing~\n");
				close(sockfd);/*all done*/
				return;
			}

			sendline[strlen(sendline) - 1] = '\0';
			write(sockfd, sendline, strlen(sendline));
		}
        
		if(FD_ISSET(sockfd, &rset))
		{
			int len = read(sockfd, recvline, MAX_LINE);
            if(len <= 0)
                break;
            printf("recv: %s\n", recvline);
		}
	}

	return 1;
}
