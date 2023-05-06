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
/*sockaddr 和 sockaddr_in是相同的，*/
struct sockaddr_in
{
	unsigned short int sin_family;	//与sockaddr结构体中sa_family相同
	uint16_t sin_port;				//套接字使用的端口号
	struct in_addr sin_addr;		//需要访问的IP地址
	unsigned char int sin_zero[8];	//未使用的字段，填充为0
};

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
struct in_addr
{
	uint32_t s_addr;//是一个长整形数
};
