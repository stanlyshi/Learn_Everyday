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
 ** Copyright (c) 2006-2010 ICT/CAS.
 ** All rights reserved.
 **
 ** File name: lte_sema.c
 ** Description: Source file for semaphore functions.
 **
 ** Current Version: 1.2
 ** $Revision: 1.1.1.1 $
 ** Author:Zhang zidi (zhangzidi@ict.ac.cn)
 ** Date: 2009.07.17
 **
 ******************************************************************************/
#include "sbs_sema.h"
#include "sbs_malloc.h"


/* Dependencies ------------------------------------------------------------- */

/* Constants ---------------------------------------------------------------- */

/* Types -------------------------------------------------------------------- */

/* Macros ------------------------------------------------------------------- */

/* Globals ------------------------------------------------------------------ */

/* Functions ---------------------------------------------------------------- */

/*******************************************************************************
 * Initialize binary semaphore.
 *
 * Input: options: SEM_FULL, or SEM_EMPTY.
 *
 * Output: return semaphore created,
 *                NULL, if create failed.
 ******************************************************************************/
SemaType create_semb(INT32 options)
{

    SemaType sem_p;


    if (!(sem_p = (SemaType)sbs_malloc(sizeof(sem_t)))) {
        return NULL;
    }
    if (sem_init(sem_p, 0, options)) {
        sbs_free(sem_p);
        return NULL;
    }
    return sem_p;
}

/*******************************************************************************
 * Initialize count semaphore.
 *
 * Input: init_count: initial count.
 *
 * Output: return semaphore created.
 *                NULL, if create failed.
 ******************************************************************************/

SemaType create_semc(INT32 init_count)
{

    SemaType sem_p;

    if (!(sem_p = (SemaType)sbs_malloc(sizeof(sem_t)))) {
        return NULL;
    }
    if (sem_init(sem_p, 0, init_count)) {
        sbs_free(sem_p);
        return NULL;
    }


    return sem_p;
}

/*******************************************************************************
 * Delete semaphore.
 *
 * Input: sem_p: pointer to semaphore to be deleted.
 *
 * Output: return Return_ALL_RIGHT, or -1 if error occurs.
 ******************************************************************************/

INT32 delete_sem(SemaType sem_p)
{
    INT32 ret = 0;

    ret = sem_destroy(sem_p);
    sbs_free(sem_p);
    return ret;
}

/*******************************************************************************
 * Wait on semaphore.
 *
 * Input: sem_p: pointer to semaphore.
 *        timeout: waiting time value, may be WAIT_FOREVER, NO_WAIT,
 *                 or other values in unit of usec.
 *                 
 * Output: return Return_ALL_RIGHT, or -1 if error occurs, or -2 if interruptible return.
 ******************************************************************************/
INT32 take_sem(SemaType sem_p, INT32 timeout)
{

    INT32 poll_time;

    switch (timeout) {
        case WAIT_FOREVER:
            return sem_wait(sem_p);
        case NO_WAIT:
            return sem_trywait(sem_p);
        default:
            if (timeout < 0) {
                return Return_FAIL;
            }

            while (sem_trywait(sem_p) !=0) {
                if (timeout < 0) {
                    return Return_FAIL;
                }
                if (timeout > SEM_WAIT_POLL_TIME) {
                    poll_time = SEM_WAIT_POLL_TIME;
                    timeout -= SEM_WAIT_POLL_TIME;
                } else {
                    poll_time = timeout;
                    timeout = -1;
                }
                usleep(poll_time);
            }
            return Return_ALL_RIGHT;
	}
}

/*******************************************************************************
 * Signal a semaphore.
 *
 * Input: sem_p: pointer to semaphore.
 * 
 * Output: return Return_ALL_RIGHT, or -1 if error occurs.
 ******************************************************************************/
INT32 give_sem(SemaType sem_p)
{
	return sem_post(sem_p);
}
/*******************************************************************************
 * Get the value of the semaphore.
 *
 * Input: sem_p: pointer to semaphore.
 *
 * Output:
 * 	int: the number
 ******************************************************************************/
INT32 get_sem_value(SemaType sem_p) {
	INT32 value = 0;
	sem_getvalue(sem_p, &value);
	return value;
}
