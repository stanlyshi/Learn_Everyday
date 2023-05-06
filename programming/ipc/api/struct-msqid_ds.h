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
 *	对于系统中的每个消息队列，内核维护一个定义在sys/msg.h头文件中的信息结构
 */
struct msqid_ds
{
	struct ipc_perm;
	struct msg *msg_first;
	struct msg *msg_last;

	msglen_t msg_cbytes;
	msgqnum_t msg_qnum;
	msglen_t msg_qbytes;

	pid_t msg_lspid;
	pid_t msg_lrpid;

	time_t msg_stime;
	time_t msg_rtime;
	time_t msg_ctime;
};
