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
 *	select socket server.c
 *   2018.10 rt
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
#include <signal.h>

#include "common.h"

int     client[FD_SETSIZE];
char    msg[MAX_LINE];
int     listenfd;

void sig_handler(int signum)
{
    close(listenfd);
    int i, sockfd;
    for(i=0;i<FD_SETSIZE;i++)
    if((sockfd = client[i] ) > 0)
    {
        close(sockfd);
    }
}


int main(int argc, char *argv[])
{
	int i, j, maxi, maxfd, connfd, sockfd;
	int nready;

	ssize_t n, ret;

	fd_set readset, allset, writeset;

    signal(SIGINT, &sig_handler);
    
    struct timeval tv;
	tv.tv_sec = 2;
    tv.tv_usec = 0;
	
	socklen_t clilen;

	struct sockaddr_in cliaddr;

	if((listenfd = tcpsocket_server()) == -1)
	{
		perror("socket error");
		exit(1);
	}

	maxfd   = listenfd;
	maxi    = -1;

	for(i=0; i<FD_SETSIZE; ++i)
	{
		client[i] = -1;
	}

	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
    printf("listenfd=%d\n", listenfd);
    
	while(1)
	{
		readset = allset;
        writeset = allset;
        debug();
		nready = select(maxfd+1, &readset, NULL, NULL, NULL);
        debug();
        
        printf("\033[31m nmaxfd \033[m: %d\n", maxfd);
        
        /*if client "connect" not do, this is False, not do if module */
		if(FD_ISSET(listenfd, &readset))
		{
            debug();
			clilen = sizeof(cliaddr);
			
			printf("Accept connection.\n");
            
			if((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen))
						< 0)
			{
				perror("accept error");
				exit(1);
			}
			printf("accept a new client: %s: %d\n", 
					inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);

			for(i=0; i<FD_SETSIZE; ++i)
			{
				if(client[i] < 0)
				{
					client[i] = connfd;
					break;
				}
			}
            
			if(FD_SETSIZE == i)
			{
				perror("too many connection.\n");
				exit(1);
			}
            
			FD_SET(connfd, &allset);
			
			if(connfd > maxfd)
			{
				maxfd = connfd;
			}
			if(i > maxi)
			{
				maxi = i;
			}
			if(--nready < 0)
			{
				continue;
			}

		}//if
        
		for(i=0; i<=maxi; ++i)
		{
			if((sockfd = client[i] ) < 0)
			{
				continue;
			}
			if(FD_ISSET(sockfd, &readset))
			{ 
                __srvSelectReadHandlerCB(sockfd, client, FD_SETSIZE);
                
				if(--nready <= 0)
				{
					break;
				}
			}//if
		}//for
        debug();
	}//while
    close(listenfd);
	return 1;
}//main
