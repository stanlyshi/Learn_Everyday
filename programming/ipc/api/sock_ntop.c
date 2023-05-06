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
 *	inet_ntop的一个基本问题是：
 *		它要求调用者传递一个指向某个二进制地址的指针，
 *		而该指针地址通常包含在一个套接字地址结构中，
 *		这就要求调用者必须知道这个结构的格式和地址族，
 *		也就是说，为了使用这个函数，必须为IPv4编写如下代码：
 *
 *		struct sockaddr_in addr;
 *		inet_ntop(AF_INET, &addr.sin_addr, str, sizeof(str));
 *
 *		或者为IPv6编写如下代码：
 *
 *		struct sockaddr_in6 addr6;
 *		inet_ntop(AF_INET6, &addr6.sin6_addr, str, sizeof(str));
 *
 *	为了解决这个问题，UNIX Network Programming的作者们编写了一个函数
 *		sock_ntop(),它以指向某个套接字地质结构的指针为参数，查看该
 *		结构的内部，然后调用适当的参数返回改地址的表达式：
 *
 *		#include "unp.h"
 *
 *		char *sock_ntop(const struct sockaddr *sockaddr, socklen_t addrlen);
 */

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
	char protstr[8];
	static char str[128];
	
	switch(sa->sa_family)
	{
		case AF_INET:
		{
			struct sockaddr_in *sin = (struct sockaddr_in *)sa;

			if(inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			{
				return NULL;
			}
			if(ntohs(sin->sin_port != 0))
			{
				snprintf(portstr, sizeof(portstr), ":%d"
						 ,ntohs(sin->sin_port));
				strcat(str, portstr);
			}	
			return str;
		}
	}
}
