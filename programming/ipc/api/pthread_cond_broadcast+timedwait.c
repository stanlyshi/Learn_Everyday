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
 *	
 */

#include <pthread.h>

int pthread_cond_broadcast(pthread_cond_t *cptr);

int pthread_cond_timedwait(pthread_cond_t *cptr,
							pthread_mutex_t *mptr,
							const struct timespec *abstime);

/**
 *	pthread_cond_broadcast
 *			通常pthread_cond_signal函数只唤醒等待在响应条件变量上的线程。
 *			在某些情况下一个线程有多个其他线程应被唤醒。这时，他可调用
 *			pthread_cond_broadcast唤醒阻塞在响应条件变量上的所有线程。
 *
 *	pthread_cond_timedwait
 *			允许线程就阻塞实践设置一个限制值。
 *			abstime参数是一个timespec结构：
 *					struct timespec
 *					{
 *						time_t tv_sec;
 *						long tv_nsec;
 *					};
 *
 */

#include <pthread.h>
int pthread_cond_broadcast (pthread_cond_t * cptr);
int pthread_cond_timedwait (pthread_cond_t * cptr, pthread_mutex_t *mptr, const struct timespec *abstime);
//Both return: 0 if OK, positive Exxx value on error
//
//pthread_cond_timedwait lets a thread place a limit on how long it will block. abstime is a timespec structure (as we defined with the pselect function, Section 6.9(See 8.4.9)) that specifies the system time when the function must return, even if the condition variable has not been signaled yet. If this timeout occurs, ETIME is returned.

struct timeval tv;
struct timespec ts;
if (gettimeofday(&tv, NULL) < 0)
	err_sys("gettimeofday error");
ts.tv_sec = tv.tv_sec + 5; /* 5 seconds in future */
ts.tv_nsec = tv.tv_usec * 1000; /* microsec to nanosec */
pthread_cond_timedwait( ..., &ts);




/**
 *	Summary
 *
 *	The creation of a new thread is normally faster than the creation of a new process with fork. This alone can be an advantage in heavily used network servers. Threads programming, however, is a new paradigm that requires more discipline.
 *	All threads in a process share global variables and descriptors, allowing this information to be shared between different threads. But this sharing introduces synchronization problems and the Pthread synchronization primitives that we must use are mutexes and condition variables. Synchronization of shared data is a required part of almost every threaded application.
 *	When writing functions that can be called by threaded applications, these functions must be thread-safe. Thread-specific data is one technique that helps with this, and we showed an example with our readline function.
 *	We return to the threads model in Chapter 30(See 9.19) with another server design in which the server creates a pool of threads when it starts. An available thread from this pool handles the next client request.
 */
