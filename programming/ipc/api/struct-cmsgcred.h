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
 *	cmsgcred结构体
 *
 *	接收发送者的凭证。
 *
 *	FreeBSD使用在头文件socket.h中定义的smsgcred结构传递凭证
 *
 *		P337
 */

#include <sys/socket.h>

struct cmsgcred
{
	pid_t	cmcred_pid;
	uid_t	cmcred_uid;
	uid_t	cmcred_euid;
	gid_t	cmcred_gid;
	short	cmcred_ngroups;
	gid_t	cmcred_groups[CMGROUP_MAX];
}
