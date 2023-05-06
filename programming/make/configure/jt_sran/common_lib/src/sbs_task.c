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
/*******************************************************************************
 ** 
 ** Copyright (c) 2006-2015 ICT/CAS.
 ** All rights reserved.
 **
 ** File name: sbs_task.c
 ** Description: Source file of thread functions.
 **
 ** Current Version: 1.0
 ** Author: zhangzongshuai@ict.ac.cn
 ** Date: 2015-09
 **
 ******************************************************************************/

/* Dependencies ------------------------------------------------------------- */
#include "sbs_task.h"
#include <errno.h>
#include <string.h>
/* Constants ---------------------------------------------------------------- */

/* Types -------------------------------------------------------------------- */

/* Macros ------------------------------------------------------------------- */

/* Globals ------------------------------------------------------------------ */

/* Functions ---------------------------------------------------------------- */

/****************************************************************************************
 * Creates a thread.
 *
 * Input:
 *        prior: priority of a thread, prior must be in [1, 99] when policy is RR or FIFO
 *        policy: schedule policy of a thread. SCHED_RR or SCHED_FIFO or SCHED_OTHER
 *        state: set the new created thread to joinable or detached state,
 *               TASK_JOIN: joinable state,
 *               TASK_DETACH: detached state.
 *        fn: entry point of thread entry function.
 *        args_p: arguments of thread entry function.
 *
 * Output:
 *        >=0: identifier of created thread.
 *        -1: if create failed.
 *
 ****************************************************************************************/
TaskType create_pthread(INT32 prior, INT32 policy,
	INT32 state, FUNCPTR fn, void *args_p)
{
   	pthread_attr_t attr;
   	struct sched_param param;
   	TaskType taskid;		/* unsigned long int */
	INT32 ret;

	/* Set attributes for new thread */
	pthread_attr_init(&attr);

	/* set schedule policy */
	pthread_attr_setschedpolicy(&attr, policy);

	/* Set thread's priority */
	param.sched_priority = prior;
	pthread_attr_setschedparam(&attr, &param);

	/* PTHREAD_SCOPE_SYSTEM: kernel level thread
	 * PTHREAD_SCOPE_PROCESS: user level thread */
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	/*PTHREAD_CREATE_JOINABLE non detach state thread
	 *PTHREAD_CREATE_DETACHED : detach state thread */
   	if (state != TASK_JOIN) {
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	}

	/* PTHREAD_INHERIT_SCHED: inherit father thread's attr
	 * PTHREAD_EXPLICIT_SCHED: use pthread_create() attr */
	pthread_attr_setinheritsched(&attr,PTHREAD_EXPLICIT_SCHED);

	if ((ret = pthread_create(&taskid, &attr, (void *)fn, args_p)) != 0) {
		printf("fail to create pthread:ret:%d, error:%s \n", ret, strerror(errno));
		if(getuid() != 0){
			printf("Please run app with root user\n");
		}
		return -1;
	}else{
		/* show thread attr */
		//disp_pthread_attr(taskid, attr);

		/* release pthread_attr_t */
		pthread_attr_destroy(&attr);

		return taskid;
	}
}

/****************************************************************************************
* get thread's attribute, this function is an internal function, no meaning to other user
*
* Input:
*		thread's id and attr struct
*
* Output:
*		print the thread's attribute
*
* Return:
*		None
*
 ****************************************************************************************/
void disp_pthread_attr(pthread_t t_id, pthread_attr_t attr )
{
	INT32 policy;
	INT32 inherit;
	INT32 detach_state;
	UINT32 guard_size;
	INT32 scope;
	int rc;
	struct sched_param param;

	void * stack_addr;
	size_t stack_size;

	if(getuid() == 0){
		printf("The current user is root\n");
	}else {
		printf("The current user is not root\n");
	}

	/* get sched policy and prior */
	rc = pthread_getschedparam(t_id, &policy, &param);
	if(policy == SCHED_OTHER) {
		printf("thread sched policy:\t\tSCHED_OTHER\n");
	}else if(policy == SCHED_RR){
		printf("thread sched policy:\t\tSCHED_RR\n");
	}else if(policy == SCHED_FIFO) {
		printf("thread sched policy:\t\tSCHED_FIFO\n");
	}
	printf("thread sched priority:\t\t%d\n", param.sched_priority);

	/* thread inherit*/
	pthread_attr_getinheritsched(&attr, &inherit); 
	printf("thread inherit:\t\t\t%s\n", (inherit == PTHREAD_EXPLICIT_SCHED)
		?"PTHREAD_EXPLICIT_SCHED":"PTHREAD_INHERIT_SCHED");

	/* detachstate*/
	pthread_attr_getdetachstate(&attr,&detach_state);
	printf("thread detachstate:\t\t%s\n", (detach_state == PTHREAD_CREATE_DETACHED)
		?"PTHREAD_CREATE_DETACHED":"PTHREAD_CREATE_JOINABLE");

	/* scope */
	pthread_attr_getscope(&attr,&scope);
	printf("thread scope:\t\t\t%s\n", (scope == PTHREAD_SCOPE_SYSTEM)
		?"PTHREAD_SCOPE_SYSTEM":"PTHREAD_SCOPE_PROCESS");

	/* guard size */
	pthread_attr_getguardsize(&attr,&guard_size);
	printf("thread guard size:\t\t%d\n", guard_size);

	/* pthread_getattr_np() is a non-standard GNU extension that 
		retrieves the attributes of the thread specified in its 
		first argument */
	pthread_getattr_np(t_id, &attr);
    if( 0 == pthread_attr_getstack(&attr,(void*)&stack_addr,&stack_size) )
    {
		printf("thread stackSize:\t\t0x%x\n", stack_size);
		printf("thread stackAddr:\t\t0x%x\n", (UINT32)stack_addr);
    }
}

/*******************************************************************************
 * Delete a thread.
 *
 * Input: taskid: identifier of thread to be deleted.
 *
 * Output: return 0 : success
 *               -1 : error occurs.
 ******************************************************************************/
INT32 delete_thread(TaskType task_p)
{
	INT32 ret = 0;
	if (pthread_cancel(task_p)) {
    		ret =  -1;
    	}
	return ret;
}
