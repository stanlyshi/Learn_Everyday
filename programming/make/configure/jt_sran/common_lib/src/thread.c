/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:31 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:41 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:25 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:00 CST. */
/** Copyright (c) 2012-2013 ICT/CAS
 ** All rights reserved.
 **
 ** File Name:Threads.cpp
 ** Description:
 ** 	The entrance of the entrire software system;
 **      
 **
 ** Current Version: 1.0
 ** Author: ICT
 ** Data:2012-6-20
 **
 ************************************************************************/

#include "thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>
/**
  Block for the signal.
  Under Linux, spurious returns are possible.
 */ 
void signal_wait(struct signal* sg,struct mutex* wMutex)
{ 	
	assert(sg != NULL && wMutex != NULL );

	pthread_cond_wait(&sg -> mSignal,&wMutex->mMutex); 
}

void signal_main(struct signal* sg)
{ 
	assert( sg != NULL );
	
	pthread_cond_signal(&sg -> mSignal);
}

void signal_broadcast(struct signal* sg) 
{ 
	assert( sg != NULL );
	pthread_cond_broadcast(&sg -> mSignal); 
}




/** Join a thread that will stop on its own. */
void jion_thread(struct thread* tr) 
{
	assert( tr != NULL ); 
	int s = pthread_join(tr -> mThread,NULL); 
	assert(!s); 
}




struct mutex* init_mutex()
{
	int res;
	struct mutex* mutex ;

	mutex = (struct mutex*)malloc(sizeof( struct mutex ));
	if(!mutex){
		printf("Mutex is NULL\n");
		return NULL;
	}
	memset(mutex,0,sizeof(struct mutex));

	res = pthread_mutexattr_init(&mutex->mAttribs);
	assert(!res);
	res = pthread_mutexattr_settype(&mutex->mAttribs,PTHREAD_MUTEX_RECURSIVE);
	assert(!res);
	res = pthread_mutex_init(&mutex->mMutex,&mutex->mAttribs);
	assert(!res);
	
	return mutex;
}


void clean_mutex(struct mutex* mutex)
{
	int res;
	assert(mutex != NULL);
	
	pthread_mutex_destroy(&mutex -> mMutex);
	res = pthread_mutexattr_destroy(&mutex -> mAttribs);
}


static struct timespec ictGetTimeout(unsigned timeout)
{
	struct timeval now;
	unsigned sec,msec;
	struct timespec ret;

	/* timeout */
	sec =  timeout / 1000;
	msec = timeout % 1000;

	/* now */
	gettimeofday(&now,NULL);

	now.tv_usec += msec * 1000;
	now.tv_sec += sec;

	/*eclipse */	
	if( now.tv_usec > 1000000 ){
		now.tv_usec -= 1000000;
		now.tv_sec += 1;
	}

	ret.tv_sec = now.tv_sec;
	ret.tv_nsec = 1000 * (long)now.tv_usec;

	return ret;
}



struct signal* init_signal() 
{ 
	
	struct signal* sg ;
	
	sg = (struct signal*)malloc(sizeof(struct signal));

	if( !sg ){
		printf(" No Memory for Signal\n");
		return NULL;
	}	
	int ret = pthread_cond_init(&sg -> mSignal,NULL);
	if( ret!=0 ){
		printf("pthread_cond_init error\n");
		free(sg);
		return NULL;
	}
	return sg;
}
void clean_signal(struct signal* sg) 
{ 
	pthread_cond_destroy(&sg -> mSignal); 
	free(sg);
}



/** Block for the signal up to the cancellation timeout. */
void wait_for_timeout(struct signal* sg,struct mutex* wMutex, unsigned timeout) 
{
	struct timespec waitTime ;


	assert(sg != NULL && wMutex != NULL);
	
	waitTime = ictGetTimeout(timeout);
	pthread_cond_timedwait(&sg -> mSignal,&wMutex->mMutex,&waitTime);
}


/** Create a thread in a non-running state. */
struct thread* init_thread(int wStackSize )
{
	struct thread* thread;
	
	thread = (struct thread*) malloc(sizeof(struct thread));
	if( !thread ){
		printf("No memory for Thread.\n");
		return NULL;
	}


	if( wStackSize < 1024 ){
		wStackSize = 65536*4;
	}	
	thread -> mThread = (pthread_t)0;
	thread ->  mStackSize=wStackSize;
	
	return thread;
}


/**
  Destroy the Thread.
  It should be stopped and joined.
 */
void clean_thread(struct thread* th) 
{ 	
	assert( th != NULL );
	pthread_attr_destroy(&th->mAttrib);
	free(th); 
	th = NULL;
}


/** Start the thread on a task. */
void start_thread(struct thread* thread,void *(*task)(void*), void *arg)
{
	int res;
	
	assert(thread -> mThread==((pthread_t)0));
	
	res = pthread_attr_init(&thread->mAttrib);
	assert(!res);
	res = pthread_attr_setstacksize(&thread -> mAttrib, thread->mStackSize);
	assert(!res);
	res = pthread_create(&thread -> mThread, &thread -> mAttrib, task, arg);
	assert(!res);
}


/** Start the thread on a task. */
void start_realtime_thread(struct thread* thread,int sched_priority,void *(*task)(void*), void *arg)
{
	int res;
	int rr_min_priority,rr_max_priority;

	
	assert(thread -> mThread==((pthread_t)0));


	rr_min_priority = sched_get_priority_min(SCHED_RR);
	rr_max_priority = sched_get_priority_max(SCHED_RR);

	if( sched_priority >  rr_max_priority || sched_priority < rr_min_priority){
		printf("sched_priority is error\n");
		return ;
	}	
	thread->param.sched_priority = sched_priority;


	res = pthread_attr_init(&thread->mAttrib);
	assert(!res);
	res = pthread_attr_setstacksize(&thread -> mAttrib, thread->mStackSize);
	assert(!res);
	
	res = pthread_attr_setschedpolicy(&thread -> mAttrib,SCHED_RR);
	assert(!res);
	res = pthread_attr_setschedparam(&thread -> mAttrib,&thread->param);
	assert(!res);
	res = pthread_attr_setinheritsched(&thread -> mAttrib,PTHREAD_EXPLICIT_SCHED);
	assert(!res);

	res = pthread_create(&thread -> mThread, &thread -> mAttrib, task, arg);
	assert(!res);
}

// vim: ts=4 sw=4
