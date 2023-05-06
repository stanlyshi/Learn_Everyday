/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:10 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:01 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:42 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:14 CST. */
/** System V message queue identifier
 * msg queue 
 *  msg_server.c
 *  2018.07 2018.10
 *
 *  msgctl(2),   msgrcv(2),   msgsnd(2),  ftok(3),  capabili‐
       ties(7), mq_overview(7), svipc(7)

 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include "msg.h"


int main()
{
	int msqid;
	key_t key;

	struct msg_form msg;

	
	/*获取key值*/
	if((key = ftok(MSG_FILE, 123)) < 0)
	{
		perror("ftok error");
		exit(1);
	}
	
	/*打印key值*/
	printf("Message Queue - Server key is: %d\n", key);

	/*创建消息队列*/
	if((msqid = msgget(key, IPC_CREAT|0777)) == -1)
	{
		perror("msgget error");
		exit(1);
	}

	/*打印消息队列ID和进程ID*/
	printf("My msqid id: %d\n", msqid);
	printf("My pid is: %d\n", getpid());

	/*循环读取消息*/
	for(;;)
	{
		/*返回类型为888的第一个消息*/
		msg.mtype = 888;
		msgrcv(msqid, &msg, sizeof(msg)-sizeof(msg.mtype), msg.mtype, 0);
		
		if(msg.mtext[0] == 'q')
		{
			break;
		}
		msg_print("server", msg);

		/*客户端接收的消息类型*/
		msg.mtype = 999;
		sprintf(msg.mtext, "hello, I'm server %d", getpid());
		msg.f = 6.28;
		
		msgsnd(msqid, &msg, sizeof(msg)-sizeof(msg.mtype), 0);
	}
	return 0;
}
