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
 *	shmctl函数提供了一个共享内存区的多种操作
 */

#include <sys/shm.h>

int shmctl(int shmid, int cmd, struct shmid_ds *buff);
	//成功：0
	//失败：-1
	
	//IPC_RMID：从系统中删除shmid标识的共享内存区并删除它
	//IPC_SET：给所指定的共享内存区设置其shmid_ds结构的以下三个成员：
	//			shm_perm.uid, shm_perm.gid, shm_perm.mode。他们的值来自buff
	//			参数指向的结构中的响应成员。shm_ctime的值也用当前时间替换。
	//IPC_STAT：（通过buff参数）向调用者返回指定共享内存区当前的shmid_ds结构。
