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
#include <netdb.h>
/**
 * struct hostent *gethostbyaddr(const void *addr, socklen_t len, int type)
 *	addr: 是一个保存了IP地址的字符串
 *	len是这个IP地址的长度
 *	type的值一般为AF_INET
 *	函数的返回值：如果失败则返回NULL
 *
 */
int main(int argc, char **argv)
{
	struct hostent *host;
	char addr[] = "127.0.0.1";
	struct in_addr in;
	struct sockaddr_in addr_in;
	extern int h_errno;

	if((host = gethostbyaddr(addr, sizeof(addr), AF_INET))
			 != (struct hostent *)NULL)
	{
		memcpy(&addr_in.sin_addr.s_addr, host->h_addr, 4);
		in.s_addr = addr_in.sin_addr.s_addr;
	
		printf("Domain name: %s\n", host->h_name);
		printf("IP length:   %d\n", host->h_length);
		printf("Type:        %d\n", host->h_addrtype);
		printf("IP:          %s\n", inet_ntoa(in));
	}
	else
	{
		printf("error: %d\n", h_errno);
		printf("%s\n", hstrerror(h_errno));
	}
}
