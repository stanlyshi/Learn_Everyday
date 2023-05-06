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
#include <stdio.h> /* These are the usual header files */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h> /* for close() */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "delay_test_server.h"

#define PORT 8888 /* Port that will be opened */
#define MAXDATASIZE 1000 /* Max number of bytes of data */

int  main()
{
	int sockfd; /* socket descriptors */
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in client; /* client's address information */
	socklen_t sin_size;

	char recvmsg[MAXDATASIZE]; /* buffer for message */
	char condition[] = "quit";
	sin_size=sizeof(struct sockaddr_in);


	bzero(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(PORT);
	server.sin_addr.s_addr = htonl (INADDR_ANY);

	/* data assignment */

	simuue();

	/* Creating UDP socket */

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {

		/* handle exception */
		perror("Creating socket failed.");
		exit(1);

	}

	/*  link   */

	if (bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
		/* handle exception */
		perror("Bind error.");
		exit(1);
	}


	while (1) {

		/* receive control data  */
		num = recvfrom(sockfd,recvmsg,MAXDATASIZE,0,(struct sockaddr *)&client,&sin_size);

		if (num < 0){
			perror("recvfrom error\n");
			exit(1);
		}
		if(strcmp(recvmsg,condition)==0) 
			break;
		data_ue_1=packagedata();
		sendto(sockfd,(char*)&data_ue_1,sizeof(data_ue_1),0,(struct sockaddr *)&client,sin_size);
		msgprintf(client);

	}

	/* close listenfd */

	close(sockfd);

	return 0;
}


void simuue(){
	ln=10;
	for(i = 0 ; i <99 ; i ++)
	{
		lst[i]=i;
	}
	for(i = 0 ; i <100 ; i ++)
	{
		ue[i]=(int*)malloc(sizeof(int));
		*ue[i]=i+100;
	}
}

data_ue packagedata(){

    data_ue data_ue_1;
    data_ue_1._ln=ln;
    for(i = 0 ; i < 99 ; i ++)
    {
      data_ue_1._lst[i]=lst[i];
    }
    for(i = 0 ; i < 100; i ++)
    {
        data_ue_1._ue[i]=*ue[i];
    }
    return data_ue_1;
}

void msgprintf(struct sockaddr_in client){
	
	printf("%d\n",num);
	printf("%s\n",recvmsg);
	printf("You got a message (%s) from %s\n",recvmsg,inet_ntoa(client.sin_addr) ); 
	
	for(i = 0 ; i < 99 ; i ++)
	{
	   	printf("Server Return Reverse Message: %d\n",data_ue_1._lst[i]);
	}
	for(i = 0 ; i < 100 ; i ++)
	{
	    printf("Server Return Reverse Message: %d\n",*ue[i]);
	}
}
