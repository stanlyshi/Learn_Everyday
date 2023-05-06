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
/*******************************************************************************
 ** 
 ** Copyright (c) 2006-2010 ICT/CAS.
 ** All rights reserved.
 **
 ** File name: lte_sema.h
 ** Description: Header file for sema functions.
 **
 ** Old Version: 1.0
 ** Author: Jihua Zhou (jhzhou@ict.ac.cn)
 ** Date: 2006.01.06
 **
 ******************************************************************************/


#ifndef SBS_SEMA_H
#define SBS_SEMA_H

/* Dependencies ------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include "sbs_type.h"
#include "sbs_err.h"
/* Constants ---------------------------------------------------------------- */
#define SEM_FULL 1
#define SEM_EMPTY 0
#define WAIT_FOREVER -1
#define NO_WAIT 0
#define SEM_WAIT_POLL_TIME 1000

/* Types -------------------------------------------------------------------- */
typedef sem_t *SemaType;

/* Macros ------------------------------------------------------------------- */

/* Globals ------------------------------------------------------------------ */

/* Functions ---------------------------------------------------------------- */
SemaType create_semb(INT32 options);

SemaType create_semc(INT32 init_count);


INT32 delete_sem(SemaType sem_p);

INT32 take_sem(SemaType sem_p, INT32 timeout);

INT32 give_sem(SemaType sem_p);

INT32 get_sem_value(SemaType sem_p);


#endif
