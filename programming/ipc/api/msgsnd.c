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
 *	用msgget打开一个消息队列后，我们使用msgsnd往其上放置一个消息
 */

#include <sys/msg.h>

int msgsnd(int msgid, const void *ptr, size_t length, int flag);
	//成功：0
	//错误：-1
/**
 *	msgid：由msgget返回的标识符
 *	ptr：一个结构体指针，他有如下的模板，定义在sys/msg.h里面
 *		struct msgbuf{
 *			long mtype;
 *			char mtext[1];
 *		};
 */
