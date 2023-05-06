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
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 *	将一个长整形转化为一个十进制IP
 *	char *inet_ntoa(struct in_addr in);
 *	其中in为in_addr类型的结构体
 *	struct in_addr
 *	{
 *		uint32_t s_addr;
 *	}
 */

int main()
{
	struct in_addr in;
	long int ld = 16777343;
	in.s_addr = ld;
	printf("inet_ntoa: %s\n", inet_ntoa(in));

	return 1;
}

