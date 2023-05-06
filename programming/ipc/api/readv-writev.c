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
 *	readv   writev
 *
 *	readv和writev类似于read和write，不过readv和writev允许单个系统调用读入
 *	到或写出自一个或多个缓冲区。
 *	这些操作分别为分散读 和 集中写 ，因为来自读操作的输入数据被分散
 *	到多个应用缓冲区中，而来自多个应用缓冲区的输入数据则集中提供给单个写操作。
 */

#include <sys/uio.h>

ssize_t readv(int fileds, const struct iovec *iov, int iovcnt);
ssize_t writev(int fileds, const struct iovec *iov, int iovcnt);

/**
 *	iovec结构体定义
 */
struct iovec
{
	void *iov_base;		//buffer的起始地址
	size_t	iov_len;	//buffer的大小
}
