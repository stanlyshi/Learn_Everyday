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
 *	pthread_detach
 *
 *		一个线程可以使joinable（可汇合的，默认值），也可以是detached（脱离的），
 *	当一个可汇合的线程终止时，他的线程ID和退出状态将留存到另一个线程对它调用
 *	pthread_join。脱离的线程却像守护进程，当他们终止时，所有相关资源都被释放，
 *	我们不能等待他们终止。如果一个线程需要知道另一个线程什么时候终止，那最好
 *	保持第二个线程的可汇合状态。
 *
 *	pthread_detach函数吧指定的线程转变为脱离状态。
 */

#include <pthread.h>

int pthread_detach(pthread_t tid);

