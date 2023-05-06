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
/******************************************************************************
 ** 
 ** Copyright (c) 2015 ICT/CAS.
 ** All rights reserved.
 **
 ** File name: sbs_task.h
 ** Description: Header file of thread functions.
 **
 ** Current Version: 1.0
 ** Author: zhangzongshuai@ict.ac.cn
 ** Date: 2015-09
 **
 ******************************************************************************/
/* Dependencies ------------------------------------------------------------- */

#ifndef SBS_TASK_H
#define SBS_TASK_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "sbs_type.h"

/* Constants ---------------------------------------------------------------- */
#define TASK_JOIN 1		/* Set the new created task to joinable state */
#define TASK_DETACH 0	/* Set the new created task to detached state */


/* Types -------------------------------------------------------------------- */
typedef pthread_t TaskType;

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
	INT32 state, FUNCPTR fn, void *args_p);

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
void disp_pthread_attr(pthread_t t_id, pthread_attr_t attr );

/*******************************************************************************
 * Delete a thread.
 *
 * Input: taskid: identifier of thread to be deleted.
 *
 * Output: return 0 : success
 *               -1 : error occurs.
 ******************************************************************************/
INT32 delete_thread(TaskType task_p);

#endif
