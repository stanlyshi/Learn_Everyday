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
#include<stdio.h>#include<stdlib.h>#include<sys/socket.h>#include<arpa/inet.h>#include<string.h>
#include "common.h"
int main(int argc ,char* argv[]){	int sock;	struct sockaddr_in addr;	int time_live;	FILE* fp;	char buf[BUF_SIZE];	const int loopback = 0;	if(argc!=3)	{		printf("Usage <addr> <port>\n",argv[1],argv[2]);		exit(1);	}
	sock=socket(AF_INET,SOCK_DGRAM,0);	if(sock==-1)	{		error_handling("socket error");	}
	memset(&addr,0,sizeof(addr));	addr.sin_family=AF_INET;	addr.sin_addr.s_addr=inet_addr(argv[1]);	addr.sin_port=htons(atoi(argv[2]));
	setsockopt(sock,IPPROTO_IP,IP_MULTICAST_LOOP,(char*)&loopback,sizeof(loopback));
	time_live=64;//设置生存时间为64，也就是最多经过64个路由器的转发	setsockopt(sock,IPPROTO_IP,IP_MULTICAST_TTL,(void*)&time_live,sizeof(time_live));
	fp=fopen("news.txt","r");	if(fp==NULL)		error_handling("fopen error");

	while(!feof(fp))	{		fgets(buf,BUF_SIZE,fp);		sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&addr,sizeof(addr));		sleep(2);	}	close(fp);	close(sock);	return 0;}