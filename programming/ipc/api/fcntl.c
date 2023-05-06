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
 *	file control
 */
#include <fcntl.h>

int fcntl(int fd, int cmd, .../*int arg*/);

/**
 *	fcntl函数提供了与网络编程相关的如下特性：
 *
 *		1.非阻塞式IO：通过使用F_SETFL命令设置O_NONBLOCK文件状态标识，
 *						我们可以把一个套接字设置为非阻塞型。
 *		2.信号驱动式IO：通过使用F_SETFL命令设置O_ASYNC文件状态标识，
 *						我们可以把一个套接字设置成一但其状态发生变化，
 *						内核就产生一个SIGIO信号。
 *
 * 	O_NONBLOCK：非阻塞式IO
 *	O_ASYNC：信号驱动式IO
 *
 *
 *	成功返回：取决于cmd
 *	出错返回：-1
 */

/** 百度百科 **
 *
 *	fcntl()用来操作文件描述符的一些特性。
 *	fcntl不仅可以施加建议性锁，还可以施加强制性锁。
 *	fcntl还能对文件的某一记录进行上锁，也就是记录锁。
 */
