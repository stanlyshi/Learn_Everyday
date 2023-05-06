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
 *	sctp服务器可能希望捆绑与所在主机系统相关IP地址的一个子集。
 *	传统意义上，TCP服务器与UDP服务器要么捆绑做在主机的某个地址，
 *	要么捆绑所有地址，而不是捆绑这些地址的一个子集。
 *
 *	sctp_bindx函数允许sctp套接字捆绑一个特定地址子集。
 *
 */
#include <netient/sctp.h>

int sctp_bindx(int sockfd, const struct sockaddr *addrs, int addrcnt, int flags);

/**
 *	sockfd：由socket函数返回的套接字描述符；
 *	addrs：一个指向紧凑的地址列表的指针；
 *		每个套接字地址结构套接字地址结构紧跟在前一个套接字地址结构之后，
 *		中间没有填充字节。
 *	addrcnt：传递给sctp_bindx的地址个数由adrcnt参数指定。
 *	flags：参数指导sctp_bindx调用执行两种行为：卷1：P212
 *
 */
