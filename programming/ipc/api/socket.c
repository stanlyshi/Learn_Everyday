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
 * socket
 *
 * 套接字 = 传输层协议 + 端口号 + IP地址
 *
 * int socket(int domain, int type, int protocol);
 * domain: 地址类型
 * type: 设置通信协议类型
 * protocol: socket所使用的协议编号
 */
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int s;
	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("connect");
		exit(1);
	}
	else
	{
		printf("a socket was create.\n");
		printf("socket number: %d\n", s);
	}
	/*用错误的参数创建套接字*/
	if((s = socket(AF_INET, 123, 0)) < 0)
	{
		perror("connect");
		exit(1);
	}
	else
	{
		printf("a socket was create.\n");
		printf("socket number: %d\n", s);
	}


}
