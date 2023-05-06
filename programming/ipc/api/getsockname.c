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
 *	返回与某个套接字关联的本地协议地址
 *
 *	getsockname()
 *	返回：
 *		成功：0
 *		出错：-1
 *
 *	函数装填由localaddr指针所指的套接字地址结构
 */
#include <sys/socket.h>

int getsockname(int sockfd, 
				struct sockaddr *localaddr, 
				socklen_t *addrlen);

/**
 * 获取套接字的地址族
 *	UNIX网络编程 卷1中的lib/sockfd_to_family.c中的内容
 */
int
sockfd_to_family(int sockfd)
{
	struct sockaddr_storage ss;
	socklen_t len;

	len = sizeof(ss);

	if(getsockname(sockfd, (struct sockaddr *)&ss, &len) < 0)
	{
		return -1;
	}
	return ss.ss_family;
}
