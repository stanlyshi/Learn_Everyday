/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:14 CST. */
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

//const struct in6_addr in6addr_any;


int tcpsocket_server()
{
    struct sockaddr_in6 servaddr;
    
    int listenfd;
    
    if((listenfd = socket(AF_INET6, SOCK_STREAM, 0))<0)
    {
		return -1;
    } 
    
    bzero(&servaddr, sizeof(servaddr));
    
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_addr = in6addr_any;
    servaddr.sin6_port = htons(PORT);
    
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

int tcpsocket_client(const char *ipv6)
{
    int sockfd;
	struct sockaddr_in6 servaddr;
    
    if(ipv6 == NULL)
    {
        return -1;
    }
    
    if((sockfd = socket(AF_INET6, SOCK_STREAM, 0)) == -1)
	{
		return -1;
	}
    
    bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family = AF_INET;
	servaddr.sin6_port = htons(PORT);
	
	if(inet_pton(AF_INET6, ipv6, &servaddr.sin6_addr) < 0)
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