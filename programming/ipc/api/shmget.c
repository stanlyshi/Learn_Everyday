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
 *	shmget函数创建一个新的共享内存区，或者访问一个已存在的共享内存区
 */

#include <sys/shm.h>

int shmget(key_t key, size_t size, int oflag);
	//成功：共享内存区对象
	//失败：-1
	
	//key可以是ftok的返回值，也可以是IPC_PRIVATE
	//size以字节为单位指定内存区大小
	//oflag是读写权值得组合
	//
