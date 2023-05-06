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

/*readline函数实现*/
ssize_t readline(int fd, char *vptr, size_t maxlen)
{
	ssize_t n, rc;
	char c, *ptr;

	ptr = vptr;

	for(n = 1; n < maxlen; n++)
	{
		if((rc = read(fd, &c, 1)) == 1)
		{
			*ptr++ = c;
			if(c == '\n') 
			{
				break;//新的一行，等同于fgets()
			}
		}
		else if(rc == 0)
		{
			*ptr = 0;
			return (n-1);//EOF, 读取了(n-1)字节的数据
		}
		else
		{
			return -1;//读取错误, read()的返回值
		}
	}
	*ptr = 0;//字符串结尾，等同于NULL
	return n;
}