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
 *	条件变量
 */

#include <pthread.h>

int pthread_cond_wait(pthread_cond_t *cptr, pthread_mutex_t *mptr);
int pthread_cond_signal(pthread_cond_t *cptr);
//返回：
//		成功：0
//		失败：正数：Exxx值

/**
 *	我们需要一个让主循环进入睡眠，知道某个线程通知他有事可做才醒来的方法。
 *	条件变量结合互斥锁能够提供这个功能：互斥锁提供互斥机制，条件变量提供信号机制。
 *
 */

int ndone;
pthread_mutex_t ndone_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ndone_cond = PTHREAD_COND_INITIALIZER;

//A thread notifies the main loop that it is terminating by incrementing 
//the counter while its mutex lock is held and by signaling the condition variable.

pthread_mutex_lock(&ndone_mutex);
ndone++;
pthread_cond_signal(&ndone_cond);
pthread_mutex_unlock(&ndone_mutex);

//The main loop then blocks in a call to pthread_cond_wait, waiting to be signaled by a terminating thread.
//

while (nlefttoread > 0) {
	while (nconn < maxnconn && nlefttoconn > 0) {
		/* find file to read */
		//...
	}
	/* Wait for thread to terminate */
	pthread_mutex_lock(&ndone_mutex);
	while (ndone == 0)
		pthread_cond_wait (&ndone_cond, &ndone_mutex);
	for (i = 0; i < nfiles; i++) {
		if (file[i].f_flags & F_DONE) {
			pthread_join(file[i].f_tid, (void **) &fptr);
			/* update file[i] for terminated thread */
			//...
		}
	}
	pthread_mutex_unlock (&ndone_mutex);
}

/**
 *	Notice that the variable ndone is still checked only while the mutex is held. Then, if there is nothing to do, pthread_cond_wait is called. This puts the calling thread to sleep and releases the mutex lock it holds. Furthermore, when the thread returns from pthread_cond_wait (after some other thread has signaled it), the thread again holds the mutex.
 *
 */
