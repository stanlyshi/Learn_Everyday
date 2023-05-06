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
 *  API: getaddrinfo()
 *
 * 	int getaddrinfo(
 * 					const char *hostname,
 * 					const char *service,
 * 					const struct addrinfo *hints,
 * 					struct addrinfo **result
 * 					);
 *		hostname: name of host
 *		service: name of service or 十进制的串口号字符串
 *		hints: 服务线索
 *		result：返回结果
 *	返回：
 *		成功：0
 *		出错：-1
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main()
{
	struct addrinfo hints, *res = NULL;

	int rc;
	
	memset(&hints, 0, sizeof(hints));

	/*设置addrinfo结构体中的参数*/
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	/*调用getaddinfo函数*/
	rc = getaddrinfo("127.0.0.1", "123", &hints, &res);
	if(rc != 0)
	{
		perror("getaddrinfo error");
		exit(1);
	}
	else
	{
		printf("getaddrinfo success\n");
	}
}

















