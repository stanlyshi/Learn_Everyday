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
#include <sys/socket.h>

int getsockopt(int sockfd, int level, int optname, void *optval,
				socklen_t *optlen);

int setsockopt(int sockfd, int level, int optname, const void *optval,
				socklen_t *optlen);

/**
 *	sockfd：已经打开的套接字；
 *	level：级别，制定系统中解释选项的代码或为通用套接字代码，
 *			或为某个特定的代码；
 *	optval：一个指向某个变量的指针，
 */
