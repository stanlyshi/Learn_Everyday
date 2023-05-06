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
 *	使用semget打开一个信号量集后，
 *	使用semop函数对其中一个或多个信号量进行操作
 */

#include <sys/sem.h>

int semop(int semid, struct sembuf *opsptr, size_t nops);
	//成功：0
	//失败：-1
	

//其中sembuf结构体为
struct sembuf
{
	short	sem_num;
	short	sem_op;
	short	sem_flg;
};
/**
 *	nops：参数指出由opsptr指向的sembuf结构数组总元素的数目。
 *
 */
