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
 *	sctp_getpaddrs：返回对端的所有地址
 */

#include <netinet/sctp.h>

int sctp_getpaddrs(int sockfd, sctp_addoc_t id, struct sockaddr **addrs);

/**
 *	getpeername函数用于sctp中时，仅仅返回主目的地址。
 *	如果需要知道对端的所有地址，就用这个函数就行了。
 *
 *	返回值：
 *		成功：存放在addrs中的对端地址数；
 *		错误：-1
 */

