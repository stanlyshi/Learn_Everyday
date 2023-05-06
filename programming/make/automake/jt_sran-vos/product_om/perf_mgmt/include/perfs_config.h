/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 19日 星期二 14:31:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 19日 星期二 14:31:24 CST. 
*/
/***********************************************************************************************************************
** 
** Copyright (c) 2008-2019 ICT/CAS.
** All rights reserved.
**
** File name: perfs_config.h
** Description:  performance statistics module's  config
**
** Current Version: 
** $Revision$
** Author: Rong Tao
** Date: 2019.02 - 
**
***********************************************************************************************************************/
#ifndef _PERFS_CONFIG_H
#define _PERFS_CONFIG_H


/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "perfs_types.h"


/* Constants ---------------------------------------------------------------------------------------------------------*/
#define PERF_MGMT_NAME                  "Performance Statistics Module"
#define PERF_MGMT_MAIN_TASK_NAME        "Performance Statistics Main Task"


#define _PERF_LOGGING                    /* turn on logging message */
#define _PERF_DEBUG                      /* turn on perf_mgmt debug information on linux terminal */
#define _PERF_DISPLAY                    /* show perf cell entiry, state machine on stdout\stderr .etc. */
#define _PERF_TURN_ON_TIMER              /* turn on the timers of perf_mgmt in sm produre */


#define Timeout_perf_ms         4000 /*ms*/

/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/

/* Globals -----------------------------------------------------------------------------------------------------------*/

/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/






#endif /*<_PERFS_CONFIG_H>*/