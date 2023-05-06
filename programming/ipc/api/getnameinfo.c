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
 *	getnameinfo函数
 *
 *	和getaddrinfo是互补函数，以一个套接字地址为参数，返回描述其中的主机
 *	的一个字符串和描述其中的服务器的另一个字符串。
 *	本函数以协议无关的方式提供这些信息，调用者不必关系存放的套接字地址结构
 *	中的协议地址的类型，因为这谢谢姐有本函数自行处理。
 *
 */

#include <netdb.h>

int getnameinfo(const struct sockaddr *sockaddr, socklen_t addrlen,
				char *host, socklen_t hostlen,
				char serv, socklen_t servlen, int flags);

