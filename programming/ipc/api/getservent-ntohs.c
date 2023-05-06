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

int main()
{
	struct servent *serv;

	while(serv = getservent())
	{
		printf("----\n");
		printf("name:		%s\n", serv->s_name);
		printf("aliases:	%s\n", serv->s_aliases[0]);
		printf("port:		%d\n", serv->s_port);
		printf("ntohs(prot):%d\n", ntohs(serv->s_port));
		printf("proto:		%s\n", serv->s_proto);
	}
}
