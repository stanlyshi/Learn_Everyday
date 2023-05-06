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
 * struct hostent *gethostbyname(const char *name);
 * 	struct hostent
 * 	{
 *		char *h_name;		//正式的主机名
 *		char **h_aliases;	//这个主机的别名
 *		int h_addrtype;		//主机名的类型
 *		int h_length;		//地址的长度
 *		char **h_addr_list;	//从域名服务器取得的主机的地址
 * 	}
 */
int main(int argc, char **argv)
{
	struct hostent *host;
	char hostname[] = "www.163.com";

	struct in_addr in;

	struct sockaddr_in addr_in;
	extern int h_errno;	/*错误号*/

	/*取得主机地址*/
	if((host = gethostbyname(hostname)) != NULL)
	{
		/*取得主机地址*/
		memcpy(&addr_in.sin_addr.s_addr, host->h_addr, 4);
		in.s_addr = addr_in.sin_addr.s_addr;

		printf("Domain name: %s\n", hostname);
		printf("IP length:   %d\n", host->h_length);
		printf("Type:  	     %d\n", host->h_addrtype);
		printf("IP:          %d\n", inet_ntoa(in));
	}
	/*出错的处理*/
	else
	{
		printf("Domain name: %s\n", hostname);
		printf("error:       %d\n", h_errno);
		printf("%s\n", hstrerror(h_errno));
	}


}
