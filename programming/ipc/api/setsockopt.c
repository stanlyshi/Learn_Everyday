/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:04 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:58 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:39 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:11 CST. */
/**
 *	int setsockopt(	int s,	//已经打开的socket
 *					int level,//设置的网络层
 *					int optname,//
 *					const void *optval,
 *					socklen_t optlen
 *				);
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

int main()
{
	/*定义相关变量*/
	int s;
	int val = 1, len, i;
	len = sizeof(int);

	/*创建套接字*/
	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("connect\n");
		exit(1);
	}
	else
	{
		printf("a socket was create\n");
		printf("socket number: %d\n", s);
	}
	/*设置套接字的一个参数*/
	i = setsockopt(s, SOL_SOCKET, SO_TYPE, &val, len);
	if(i == 0)//判断结果情况
	{
		printf("set socket ok \n");
	}
	else
	{
		printf("set socket error\n");
	}
	/*用错误的方法设置一个套接字*/
	setsockopt(100, SOL_SOCKET, SO_TYPE, &val, len);
	perror("socket:");
}
