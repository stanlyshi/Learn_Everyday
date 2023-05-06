/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:40 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:24 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:00 CST. */
/************************************************************************
 **
 **
 ** Copyright (c) 2012-2013 ICT/CAS
 ** All rights reserved.
 **
 ** File Name:Threads.h
 ** Description:
 ** 	The entrance of the entrire software system;
 **      
 **
 ** Current Version: 1.0
 ** Author: ICT
 ** Data:2012-6-20
 **
 ************************************************************************/


#ifndef SBS_THREADS_H
#define SBS_THREADS_H

#include <pthread.h>
#include <assert.h>
#include <unistd.h>

struct mutex;

/** A class for recursive mutexes based on pthread_mutex. */
struct mutex {

	pthread_mutex_t mMutex;
	pthread_mutexattr_t mAttribs;
};

struct mutex* init_mutex();

void clean_mutex(struct mutex* mutex);

#define lock(mutex) do{ \
						pthread_mutex_lock(& ( mutex.mMutex));\
					}while(0)
#define unlock(mutex) do{ \
						pthread_mutex_unlock(&(mutex.mMutex)); \
					  }while(0)





/** A C++ interthread signal based on pthread condition variables. */
struct signal {
	pthread_cond_t mSignal;
};

struct signal* init_signal();

void clean_signal(struct signal* sg );
/**
  Block for the signal up to the cancellation timeout.
  Under Linux, spurious returns are possible.
 */
void wait_for_timeout(struct signal *sg,struct mutex* wMutex, unsigned timeout);

/**
  Block for the signal.
  Under Linux, spurious returns are possible.
 */ 
void signal_wait(struct signal* sg,struct mutex* wMutex);

void signal_main(struct signal* sg);

void signal_broadcast(struct signal* sg); 




/** A C++ wrapper for pthread threads.  */
struct thread {
	pthread_t mThread;
	pthread_attr_t mAttrib;
	struct sched_param param; 
	// FIXME -- Can this be reduced now?
	int mStackSize;
	
};

struct thread* init_thread(int wStackSize );
void clean_thread(struct thread* th); 

/** Start the thread on a task. */
void start_thread(struct thread* thread,void *(*task)(void*), void *arg);

/** Join a thread that will stop on its own. */
void jion_thread(struct thread* tr); 

void start_realtime_thread(struct thread* thread,int sched_priority,void *(*task)(void*), void *arg);

#endif
// vim: ts=4 sw=4

