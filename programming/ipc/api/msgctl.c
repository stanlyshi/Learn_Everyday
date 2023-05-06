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
 *	msgctl函数提供在一个消息队列上的各种操作
 */

#include <sys/msg.h>

int msgctl(int msqid, int cmd, struct msqid_ds *buff);
/**
 *	返回：	
 *		成功：0
 *		错误：-1
 *
 *	msgctl函数提供3个命令：
 *		IPC_RMID	从系统中删除由msqid指定的消息队列
 *		IPC_SET		给所有指定的消息队列设置其msqid_ds结构的一下四个成员：
 *					msg_perm.uid	msg_perm.gid	msg_perm.mode
 *					msg_perm.qbytes
 *		IPC_STAT	（通过buff参数）给调用者返回与所指定消息队列对应的当前
 *					msqid_ds结构。
 */
