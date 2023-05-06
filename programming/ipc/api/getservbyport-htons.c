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
#include <netdb.h>
/**
 *	struct servent *getservbyport(int port, char *proto);
 *  port: 端口号
 *	proto: 某个服务器使用的协议
 *	如果没有查到这个服务，返回NULL
 *
 *	需要包含incude<netdb.h>
 */
int main()
{
	struct servent *ser;

	if(ser = getservbyport(htons(23), "tcp"))
	{
		printf("name:		%s\n", ser->s_name);
		printf("port:		%d\n", ntohs(ser->s_port));
		printf("protocol:	%s\n", ser->s_proto);
		printf("alias:		%s\n", ser->s_aliases[0]);
	}	
	else
	{
		printf("there is no such a service\n");
	}
	
	printf("---\n");
	if(ser = getservbyport(htons(80), "tcp"))
	{
		printf("name:		%s\n", ser->s_name);
		printf("port:		%d\n", ntohs(ser->s_port));
		printf("protocol:	%s\n", ser->s_proto);
		printf("alias:		%s\n", ser->s_aliases[0]);
	}	
	else
	{
		printf("there is no such a service\n");
	}
	
	printf("---\n");
	if(ser = getservbyport(htons(999), "tcp"))
	{
		printf("name:		%s\n", ser->s_name);
		printf("port:		%d\n", ntohs(ser->s_port));
		printf("protocol:	%s\n", ser->s_proto);
		printf("alias:		%s\n", ser->s_aliases[0]);
	}	
	else
	{
		printf("there is no such a service\n");
	}



}	
