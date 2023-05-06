/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:20 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:07 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:47 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:18 CST. */
#include<stdio.h>#include<stdlib.h>#include<sys/socket.h>#include<arpa/inet.h>#include<string.h>#include "common.h"int main(int argc,char* argv[]){	int sock;	struct sockaddr_in addr,sender_addr;	char buf[BUF_SIZE];	int str_len;	int sender_addr_sz;	struct ip_mreq join_addr;	const int loopback = 0;	sock=socket(AF_INET,SOCK_DGRAM,0);	memset(&addr,0,sizeof(addr));	addr.sin_family=AF_INET;	addr.sin_addr.s_addr=htonl(INADDR_ANY);	addr.sin_port=htons(atoi(argv[2]));	setsockopt(sock,IPPROTO_IP,IP_MULTICAST_LOOP,(char*)&loopback,sizeof(loopback));	join_addr.imr_multiaddr.s_addr=inet_addr(argv[1]);
	join_addr.imr_interface.s_addr=htonl(INADDR_ANY);
	setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,(void*)&join_addr,sizeof(join_addr));

	if(bind(sock,(struct sockaddr*)&addr,sizeof(addr))==-1)
		error_handling("bind error");
	while(1)
	{
		str_len=recvfrom(sock,buf,BUF_SIZE-1,0,NULL,0);
		if(!str_len)
			break;
		buf[str_len]=0;
		fputs(buf,stdout);
	}
	close(sock);
	return 0;
}
