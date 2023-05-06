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
 *	socketpair函数创建连个随后关联起来的套将诶子。
 *	本函数仅适用于Unix域套接字。
 */

#include <sys/socket.h>

int socketpair(int family, int type, int protocol, int sockfd[2]);

/**
 *	返回值：
 *		成功：0
 *		出错：-1
 *
 *	family必须为AF_LOCAL
 *	protocol参数必须为0；
 *	type参数既可以是SOCK_STREAM，也可以是SOCK_DGRAM。
 *	新创建的两个套接字以sockfd[0]和sockfd[1]返回。
 *
 *	本函数类似于Unix的pipe函数，会返回两个彼此连接的描述符，事实上，
 *	源自Berkeley的实现通过执行与socketpair一样的内部操作给出pipe接口。
 */
