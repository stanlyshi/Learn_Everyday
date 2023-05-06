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
 *	close()
 *
 *	通常close()函数也用来关闭套接字socket，并终止TCP连接
 */

#include <unistd.h>

int close(int sockfd);

/**
 *	close一个TCP套接字的默认行为是把该套接字标记成已关闭，然后立即
 *	返回到调用进程，该套接字描述符不能再由调用进程使用，也就是说它不能
 *	在座位read和write的第一个参数。
 */
