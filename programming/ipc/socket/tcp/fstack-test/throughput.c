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
#include <sys/time.h>

#include "common.h"

int main(int argc, char *argv[])
{
   	/*声明套接字和链接服务器地址*/
	int sockfd;
	struct sockaddr_in servaddr;

	/*判断是否为合法输入*/
	if(argc != 2)
	{
		perror("usage: tcpcli <IP address>");
		exit(1);
	}

	/*1.创建套接字*/
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket error");
		exit(1);
	}

	/*2.设置连接服务器地址结构*/
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0)
	{
		printf("inet_pton error for %s\n", argv[1]);
		exit(1);
	}
	
	/*3.发送连接服务器请求*/
	if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect error");
		exit(1);
	}	

	/*4.消息处理*/
	char sendline[MAX_LINE] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ123456"};
	char recvline[MAX_LINE] = {0};
	ssize_t n = 0, pkgcount = 0;
	struct timeval before, after;
	static int flag = 0;

	printf("Ready to send .\n");

	while(pkgcount<1000)
	{
		if(flag == 0) {
			gettimeval(&before);
			//printf("before time: %ld, %ld\n", before.tv_sec, before.tv_usec);	
		}
		//printf("write. pkgcount = %ld\n", pkgcount);
		n += write(sockfd, sendline, MAX_LINE);
		//n += read(sockfd, recvline, MAX_LINE);
	
		flag = 1;
		pkgcount ++;
	}
	gettimeval(&after);

	printf("before time: %ld, %ld\n", before.tv_sec, before.tv_usec);
	printf(" after time: %ld, %ld\n", after.tv_sec, after.tv_usec);
	printf("Total %.3lf Mbps, bytes = %ld(bits:%ld)\n", 8*n*1.0/((after.tv_sec-before.tv_sec)*1000000
								+after.tv_usec-before.tv_usec), n, n*8);

	/*5.关闭套接字*/
	close(sockfd);



	return 1; 
}
