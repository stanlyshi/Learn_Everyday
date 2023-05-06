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
 *	Posix消息队列允许异步事件通知，以告知合适有一个消息放置到了某个空消息队列中。
 *	这种通知有两种方式选择：
 *		1.产生一个信号；
 *		2.创建一个线程来执行一个指定的函数。
 *
 *	这种通知通过调用mq_notify建立。
 */

#include <mqueue.h>

int mq_notify(mqd_t mqdes, const struct sigevent *notification);
	//成功：返回0
	//错误：-1
	
/**
 *	结构体sigevent定义在signal.h中：
 */
#include <signal.h>
union {
	int sival_int;
	void *sival_ptr;
};

struct sigevent
{
	int sigev_notify;
	int sigev_signo;
	union sigval sigev_value;
	
	void (*sigev_notify_funtion)(union sigval);
	pthread_attr_t *sigev_notify_attributes;
}
