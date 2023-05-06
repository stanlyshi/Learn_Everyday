/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:21 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:07 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:48 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:19 CST. */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>

#include "common.h"

static int listenfd;

extern int client[MAC_MAX_NUM];


void handle_sigint(int signum)
{
    printf("Catch the SIGINT signal.\n");
    close(listenfd);
    unlink(PATH);
    exit(1);
}

int main()
{
    int connfd; 
    int ret = 0;
    int i;
    int len;
    struct sockaddr_un clt_addr, srv_addr;
    
    char msg[VCM_STR_LEN];
    int msglen = 0;
    
    
    int maxfd, maxi, nready; 
    fd_set readset, allset;
    int sockfd;
    
    signal(SIGINT, handle_sigint);
    
    listenfd = unsocket_server(PATH);
    
    unsocket_init();
    
    maxfd = listenfd;
    maxi = -1;
    
    
    FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
    
    chmod(PATH, 00777);
    
    while(1)
    {
        readset = allset;
		nready = select(maxfd+1, &readset, NULL, NULL, NULL);

        if(nready <= 0)
        {
            perror("select error");
            close(listenfd);
            unlink(PATH);
            break;
        }
        if(FD_ISSET(listenfd, &readset))
		{
            len = sizeof(clt_addr);
            connfd = accept(listenfd, (struct sockaddr*)&clt_addr, &len);
            if(connfd < 0)
            {
                perror("cannot accept client connect request.");
                close(listenfd);
                unlink(PATH);
                break;
            }

			for(i=0; i<MAC_MAX_NUM; ++i)
			{
				if(client[i] < 0)
				{
					client[i] = connfd;
					break;
				}
                if(MAC_MAX_NUM == i)
                {
                    perror("too many connection.\n");
                    exit(1);
                }
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
        }
        
        for(i=0; i<=maxi; ++i)
		{
            if((sockfd = client[i] ) < 0)
			{
				continue;
			}
            if(FD_ISSET(sockfd, &readset))
			{   
                memset(msg, 0, VCM_STR_LEN);
                msglen = read(sockfd, msg, sizeof(msg));
                if(msglen <= 0)
                {
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }
                printf("[%3d]:  %s\n", sockfd, msg);
                if(strcmp(msg, GOODBYE) != 0)
                {
                    write(sockfd, "Recv your message.", 20);
                }
            }
        }
        
        
    }
}