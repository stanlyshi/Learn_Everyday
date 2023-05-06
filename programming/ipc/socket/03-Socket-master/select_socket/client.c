/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:12 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:03 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:43 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:15 CST. */
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

#define PORT 8888
#define MAX_LINE 2048

int max(int a , int b)
{
	return a > b ? a : b;
}

/*readline函数实现*/
ssize_t readline(int fd, char *vptr, size_t maxlen)
{
	ssize_t	n, rc;
	char	c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ( (rc = read(fd, &c,1)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		} else if (rc == 0) {
			*ptr = 0;
			return(n - 1);	/* EOF, n - 1 bytes were read */
		} else
			return(-1);		/* error, errno set by read() */
	}

	*ptr = 0;	/* null terminate like fgets() */
	return(n);
}

/*普通客户端消息处理函数*/
void str_cli(int sockfd)
{
	/*发送和接收缓冲区*/
	char sendline[MAX_LINE] , recvline[MAX_LINE];
	while(fgets(sendline , MAX_LINE , stdin) != NULL)	
	{
		write(sockfd , sendline , strlen(sendline));

		bzero(recvline , MAX_LINE);
		if(readline(sockfd , recvline , MAX_LINE) == 0)
		{
			perror("server terminated prematurely");
			exit(1);
		}//if

		if(fputs(recvline , stdout) == EOF)
		{
			perror("fputs error");
			exit(1);
		}//if

		bzero(sendline , MAX_LINE);
	}//while
}

/*采用select的客户端消息处理函数*/
void str_cli2(FILE* fp , int sockfd)
{
	int maxfd;
	fd_set rset;
	/*发送和接收缓冲区*/
	char sendline[MAX_LINE] , recvline[MAX_LINE];

	FD_ZERO(&rset);
	while(1)
	{
		/*将文件描述符和套接字描述符添加到rset描述符集*/
		FD_SET(fileno(fp) , &rset);	
		FD_SET(sockfd , &rset);
		maxfd = max(fileno(fp) , sockfd) + 1;
		select(maxfd , &rset , NULL , NULL , NULL);
		
		if(FD_ISSET(fileno(fp) , &rset))
		{
			if(fgets(sendline , MAX_LINE , fp) == NULL)
			{
				printf("read nothing~\n");
				close(sockfd); /*all done*/
				return ;
			}//if
			sendline[strlen(sendline) - 1] = '\0';
			write(sockfd , sendline , strlen(sendline));
		}//if

		if(FD_ISSET(sockfd , &rset))
		{
			if(readline(sockfd , recvline , MAX_LINE) == 0)
			{
				
				perror("handleMsg: server terminated prematurely.\n");
				exit(1);			
			}//if
			
			if(fputs(recvline , stdout) == EOF)
			{
				perror("fputs error");
				exit(1);
			}//if
		}//if	
	}//while
}

int main(int argc , char **argv)
{
	/*声明套接字和链接服务器地址*/
    int sockfd;
    struct sockaddr_in servaddr;

    /*判断是否为合法输入*/
    if(argc != 2)
    {
        perror("usage:tcpcli <IPaddress>");
        exit(1);
    }//if

    /*(1) 创建套接字*/
    if((sockfd = socket(AF_INET , SOCK_STREAM , 0)) == -1)
    {
        perror("socket error");
        exit(1);
    }//if

    /*(2) 设置链接服务器地址结构*/
    bzero(&servaddr , sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if(inet_pton(AF_INET , argv[1] , &servaddr.sin_addr) < 0)
    {
        printf("inet_pton error for %s\n",argv[1]);
        exit(1);
    }//if

    /*(3) 发送链接服务器请求*/
    if(connect(sockfd , (struct sockaddr *)&servaddr , sizeof(servaddr)) < 0)
    {
        perror("connect error");
        exit(1);
    }//if

	/*调用普通消息处理函数*/
	str_cli(sockfd);	
	/*调用采用select技术的消息处理函数*/
	//str_cli2(stdin , sockfd);
	exit(0);
}
