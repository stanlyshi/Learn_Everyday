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
 *	当有一个消息被放置到空队列中时，该信号产生，主线程被阻止，
 *	信号处理程序执行并设置mqflag比那辆，主线程再次执行，反向mq_flag
 *	为非0，意识读出该消息。
 *	更为建议的办法之一是阻塞在某个函数中，仅仅等待该函数信号的递交，
 *	而不是让内核执行一个只为设置一个标志的信号处理程序。
 *	sigwait函数提供了这种能力：
 *
 */

#include <signal.h>

int sigwait(const sigset_t *set, int *sig);
	//成功：返回0
	//出错：Exxx值
	

