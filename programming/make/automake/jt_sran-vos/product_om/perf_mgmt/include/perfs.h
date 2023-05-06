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
** File name: perfs.h
** Description:  performance statistics module's  inner header, the coder just include "perfs.h" in prfm_mgmt module
**              source codes
**
** Current Version: 
** $Revision$
** Author: Rong Tao
** Date: 2019.02 - 
**
***********************************************************************************************************************/
#ifndef _PERFS_H
#define _PERFS_H

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "perfs_types.h"
#include "perfs_config.h"
#include "perfs_sm.h"
#include "perfs_msgs.h"
#include "perfs_l1l2l3_perf.h"
#include "perfs_cell_perf.h"


#include "perf_mgmt_api.h"
#include "cell_mgmt_api.h"
#include "inet_mgmt_api.h"


/* Constants ---------------------------------------------------------------------------------------------------------*/
#define PERF_OK     VOS_OK
#define PERF_ERR    VOS_ERROR
#define PERF_YES    VOS_YES
#define PERF_NO     VOS_NO
#define PERF_WARN   2

#define PERF_ANSI_GRAY       "\x1b[1;37;40m"
#define PERF_ANSI_CYAN       "\x1b[1;36m"
#define PERF_ANSI_PURPLE     "\x1b[1;35m"
#define PERF_ANSI_BLUE       "\x1b[1;34m"
#define PERF_ANSI_YELLOW     "\x1b[1;33m"
#define PERF_ANSI_GREEN      "\x1b[1;32m"
#define PERF_ANSI_RED        "\x1b[1;31m"
#define PERF_ANSI_BOLD       "\x1b[1;30m"
#define PERF_ANSI_RESET      "\x1b[0m"

/**
 *  For log
 */
typedef enum {
    PERF_LOG_TYPE_MIN,
    PERF_LOG_ERROR,
    PERF_LOG_INFO,
    PERF_LOG_SUCCESS,
    PERF_LOG_WARNING,
    PERF_LOG_DEBUG,
    PERF_LOG_SIMPLE,
    PERF_LOG_TYPE_MAX,
}LOG_PERF_TYPE;


/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
/**
 *  Swap VOS functions
 */
#define Malloc(size)    MallocVos(size, perfs_module.moduleID)

#define TimerCreateLoop(callbackFn, arg, ms) \
            TimerCreateLoopVos(perfs_module.moduleID, perfs_msgqid, callbackFn, arg, ms)
#define TimerCreateNoLoop(callbackFn, arg, ms) \
            TimerCreateNoLoopVos(perfs_module.moduleID, perfs_msgqid, callbackFn, arg, ms)
#define TimerChange(timer, ms)  TimerChangeVos(perfs_module.moduleID, timer, ms)
#define TimerDelete(timer)      TimerDeleteVos(perfs_module.moduleID, timer)




/* Globals -----------------------------------------------------------------------------------------------------------*/
extern vos_module_t     perfs_module;
extern ULONG            perfs_msgqid;     /* perf mgmt msgq id */

/* Functions ---------------------------------------------------------------------------------------------------------*/
/**
 *  Terminal logging and debug
 */
#ifdef _PERF_LOGGING
#include <libgen.h>
 #define log_perf_err(fmt...)  log_perf(STDERR, PERF_LOG_ERROR,Basename(__FILE__), __func__, __LINE__, fmt)
 #define log_perf_succ(fmt...) log_perf(STDOUT, PERF_LOG_SUCCESS,Basename(__FILE__), __func__, __LINE__,fmt)
 #define log_perf_info(fmt...) log_perf(STDOUT, PERF_LOG_INFO,Basename(__FILE__), __func__, __LINE__,fmt)
 #define log_perf_warn(fmt...) log_perf(STDERR, PERF_LOG_WARNING,Basename(__FILE__), __func__, __LINE__,fmt)
 #define log_perf_simple(fmt...) log_perf(STDOUT, PERF_LOG_SIMPLE,Basename(__FILE__), __func__, __LINE__,fmt)
#else
 #define log_perf_err(fmt...)  do{}while(0)
 #define log_perf_succ(fmt...) do{}while(0)
 #define log_perf_info(fmt...) do{}while(0)
 #define log_perf_warn(fmt...) do{}while(0)
 #define log_perf_simple(fmt...) do{}while(0)
#endif /*<_PERF_LOGGING>*/

#ifdef _PERF_DEBUG
#include <libgen.h>
 #define log_perf_debug() log_perf(STDOUT, PERF_LOG_DEBUG, Basename(__FILE__), __func__, __LINE__,"\n")
#else
 #define log_perf_debug() do{}while(0)
#endif /*<_PERF_DEBUG>*/


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
/**
 *  Terminal logging and debug
 */
INT32 log_perf(FILE *fp, LOG_PERF_TYPE type, 
                CONST CHAR *__file, CONST CHAR *__func, CONST UINT32 __line, CONST CHAR *fmt, ...);

/**
 *  performance statistics module initialized function
 */
INT32 perfs_module_init();
INT32 perfs_clean_up();
INT32 perfs_check_paras_valid();
INT32 perfs_task_init();
INT32 perfs_task_main_ctrl(VOID *arg);

/**
 *  Initialized message queue
 */
INT32 perfs_msgqueue_init();

/**
 *  Initialized cell entity
 */
INT32 perfs_cells_init();

/**
 *  Initialized state machine
 */
INT32 perfs_sm_init();

/**
 *  Register to cell_mgmt
 */
void register_to_cell_mgmt_callback(UINT32 cell_id, ULONG mask, cell_info_t *arg);

INT32 get_cell_perf(UINT32 cell_id, cell_perf_t *cell_perf);

INT32 display_cell_performance(FILE *fp, UINT32 cell_id);


#endif /*<_PERFS_H>*/
