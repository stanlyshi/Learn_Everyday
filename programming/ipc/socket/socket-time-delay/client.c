/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:16 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:05 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:45 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:17 CST. */
/*
 ============================================================================
 Name        : client.c
 Author      : cc
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<netdb.h>
#include<stdarg.h>
#include<string.h>
#include<sys/time.h>
#include "delay_test_client.h"

#define PORT 8888/* Open Port on Remote Host */
#define MAXDATASIZE 1000 /* Max number of bytes of data */

int main(int argc, char *argv[])
{
	data_ue * data_ue_2;
	struct timeval starttime,endtime;
	char recvbuf[MAXDATASIZE]; /* buf will store received text */
	char sendbuf[MAXDATASIZE];
	struct hostent *he; /* structure that will get information about remote host */
	struct sockaddr_in server; /* server's address information */

	if (argc !=2) { 
		printf("Usage: %s <IP Address> \n",argv[0]);
		exit(1);
	}

	if ((he=gethostbyname(argv[1]))==NULL){ /* calls gethostbyname() */
		printf("gethostbyname() error\n");
		exit(1);
	}
	if ((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1){ /* calls socket() */
		printf("socket() error\n");
		exit(1);
	}
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT); /* htons() is needed again */
	server.sin_addr = *((struct in_addr *)he->h_addr); /*he->h_addr passes "*he"'s info to "h_addr" */
	socklen_t len;
	len=sizeof(struct sockaddr_in);

	while (1) {
		/*send control data*/
		printf("input message:");
		fgets(sendbuf,40,stdin);
		sendto(fd,sendbuf,strlen(sendbuf),0,(struct sockaddr *)&server,len);

		/*timer start */

		gettimeofday(&starttime,0);

		/*receive data package*/

		if ((numbytes=recvfrom(fd,recvbuf,MAXDATASIZE,0,(struct sockaddr *)&server,&len)) == -1){ 
			printf("recvfrom() error\n");
			exit(1);
		}


		/*recover data */


		  data_ue_2=(data_ue*)recvbuf;
		  ln=data_ue_2->_ln;
			for(i = 0 ; i < 99 ; i ++)
			{
			  lst[i]=data_ue_2->_lst[i];
			}
			for(i = 0 ; i < 100 ; i ++)
			{
				ue[i]=(int*)malloc(sizeof(int));
				*ue[i]=data_ue_2->_ue[i];
			}
		  /*timer end */

		  gettimeofday(&endtime,0);

		  /* timer calculate */
		  timeuse =1000000*(endtime.tv_sec-starttime.tv_sec)+endtime.tv_usec-starttime.tv_usec;
		  timeuse/=1000;

		  msgprintf_client();
	}

	/* close fd */

	close(fd);

	return 0;

}

void msgprintf_client(){
	printf("Server Return Reverse Message: %d\n",ln); /* it prints server's welcome message */
	   
	for(i = 0 ; i < 99 ; i ++)
	{
	 	printf("Server Return Reverse Message: %d\n",lst[i]);
	}

	for(i = 0 ; i < 100 ; i ++)
	{
	   	printf("Server Return Reverse Message: %d\n",*ue[i]);
	}
	printf("%d\n",numbytes);
	printf("Timeuse is %f ms.\n",timeuse);
}

