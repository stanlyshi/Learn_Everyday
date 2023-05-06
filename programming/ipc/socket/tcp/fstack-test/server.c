/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:17 CST. */
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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:17 CST. */
/**
 *  server.c  服务器端代码
 *   2018.07
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

#include "common.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in cliaddr;
    
    int listenfd, connfd;
    pid_t childpid;

    char buf[MAX_LINE];
    
    socklen_t clilen;

    if((listenfd = tcpsocket_server())<0)
    {
		perror("socket error");
		exit(1);
    }
	
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,NULL,0);
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEPORT,NULL,0);
	clilen = sizeof(cliaddr);

    int val = 1;
	setsockopt(listenfd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));
	printf("val = %d\n", val);
    for(;;)
    {
        if((connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen)) < 0)
        {
            perror("accept error");
            exit(1);
        }
		if((childpid = fork()) == 0)
		{
	    	close(listenfd);
			setsockopt(connfd, SOL_TCP, TCP_NODELAY, &val, sizeof(val));


	    	ssize_t n;
            
            memset(buf, 0, MAX_LINE);
	    	while((n = read(connfd, buf, MAX_LINE)) > 0)
	    	{
				printf("Server Recv(%d): %s\n", n, buf);
                memset(buf, 0, MAX_LINE);
                //buf[0] = 'A';
                
				//write(connfd, buf, MAX_LINE);
	    	}
	    	exit(0);
		}
		close(connfd);
    }    
    
    close(listenfd);   

}//main
