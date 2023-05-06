/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:28 CST. */
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
/**********************************************************************************************************************
** 
** Copyright (c) 2008-2018 ICT/CAS.
** All rights reserved.
**
** File name: vcm.h
** Description:
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao
** Date: 2019.03 - 
**
***********************************************************************************************************************/
#ifndef _VCM_H
#define _VCM_H 1

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm_config.h"
#include "vcm_types.h"
#include "vcm_cell_entity.h"
#include "vcm_sm.h"
#include "vcm_errno.h"
#include "vcm_msgs.h"
#include "vcm_tools.h"
#include "vcm_sys_wrapper.h"
#include "vcm_alarm.h"
#include "vcm_db.h"
#include "vcm_plugin.h"

 
#include "cell_mgmt_api.h"
#include "inet_mgmt_api.h"


#if !defined _CELL_MGMT_API_H
 #error "Never include vcm.h directly, include cell_mgmt_api.h instead."
#endif


/* Constants ---------------------------------------------------------------------------------------------------------*/
#define VCM_OK              VOS_OK
#define VCM_ERR             VOS_ERROR
#define VCM_YES             VOS_YES
#define VCM_NO              VOS_NO
#define VCM_WARN            2

#define VCM_ANSI_GRAY       "\x1b[1;37;40m"
#define VCM_ANSI_CYAN       "\x1b[1;36m"
#define VCM_ANSI_PURPLE     "\x1b[1;35m"
#define VCM_ANSI_BLUE       "\x1b[1;34m"
#define VCM_ANSI_YELLOW     "\x1b[1;33m"
#define VCM_ANSI_GREEN      "\x1b[1;32m"
#define VCM_ANSI_RED        "\x1b[1;31m"
#define VCM_ANSI_BOLD       "\x1b[1;30m"
#define VCM_ANSI_RESET      "\x1b[0m"


/**
 *  For log
 */
typedef enum {
    VCM_LOG_TYPE_MIN,
    VCM_LOG_ERROR,
    VCM_LOG_INFO,
    VCM_LOG_SUCCESS,
    VCM_LOG_WARNING,
    VCM_LOG_DEBUG,
    VCM_LOG_SIMPLE,
    VCM_LOG_TYPE_MAX,
}LOG_VCM_TYPE;

/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
extern vos_module_t     vcm_module;
extern ULONG            vcm_msgqid;     /* cell mgmt msgq id */

/**
 *  Swap VOS functions
 */
#define Malloc(size)    MallocVos(size, vcm_module.moduleID)

#define TimerCreateLoop(callbackFn, arg, ms) \
            TimerCreateLoopVos(vcm_module.moduleID, vcm_msgqid, callbackFn, arg, ms)
#define TimerCreateNoLoop(callbackFn, arg, ms) \
            TimerCreateNoLoopVos(vcm_module.moduleID, vcm_msgqid, callbackFn, arg, ms)
#define TimerChange(timer, ms)  TimerChangeVos(vcm_module.moduleID, timer, ms)
#define TimerDelete(timer)      TimerDeleteVos(vcm_module.moduleID, timer)



/* define FUNCTIONs */
#define VCM_FUNC_DEFINE1(name, ret, t1, v1) \
	ret name(t1 v1)
#define VCM_FUNC_DEFINE2(name, ret, t1, v1, t2, v2) \
	ret name(t1 v1, t2 v2)
#define VCM_FUNC_DEFINE3(name, ret, t1, v1, t2, v2, t3, v3) \
	ret name(t1 v1, t2 v2, t3 v3)
#define VCM_FUNC_DEFINE4(name, ret, t1, v1, t2, v2, t3, v3, t4, v4) \
	ret name(t1 v1, t2 v2, t3 v3, t4 v4)
#define VCM_FUNC_DEFINE5(name, ret, t1, v1, t2, v2, t3, v3, t4, v4, t5, v5) \
	ret name(t1 v1, t2 v2, t3 v3, t4 v4, t5 v5)
#define VCM_FUNC_DEFINE6(name, ret, t1, v1, t2, v2, t3, v3, t4, v4, t5, v5, t6, v6) \
	ret name(t1 v1, t2 v2, t3 v3, t4 v4, t5 v5, t6 v6)

#define VCM_FUNC_CALL(name, ...) name(__VA_ARGS__)

#define __called_once() \
    do{ \
        static int __call_times =  0; \
        __call_times++; \
        if(__call_times>1) { \
            log_vcm_err("Error: Must just call \"%s\" once.\n", __func__); \
            Exit(VCM_ERR); \
        } \
    }while(0)


/**
 *  Check pointer is NULL or not
 */
#define CHECK_POINTER(p)                        \
    if(NULL == p) {                             \
        log_vcm_err("NULL pointer Error.\n");   \
        __vcm_set_errno(_VCM_ENULLPOINTER);     \
        return VCM_ERR;                         \
    } 
/**
 *  Check file is exist or not
 */
#define CHECK_F_EXIST(f)                            \
    do{                                             \
        CHECK_POINTER(f);                           \
        if(-1 == ACCESS_IS_EXIST(f)) {              \
            log_vcm_err("%s doesn't Exist.\n", f);  \
            __vcm_set_errno(_VCM_EFNOTEXIST);       \
            return VCM_ERR;                         \
        }                                           \
    }while(0)
    
/**
 *  Check file is readable or not
 */
#define CHECK_F_READABLE(f)                             \
    do{                                                 \
        CHECK_POINTER(f);                               \
        if(-1 == ACCESS_IS_READABLE(f)) {               \
            log_vcm_err("%s doesn't readable.\n", f);   \
            __vcm_set_errno(_VCM_EFNOTREADABLE);        \
            return VCM_ERR;                             \
        }                                               \
    }while(0)
    
/**
 *  Check file is writeable or not
 */
#define CHECK_F_WRITEABLE(f)                            \
    do{                                                 \
        CHECK_POINTER(f);                               \
        if(-1 == ACCESS_IS_WRITEABLE(f)) {              \
            log_vcm_err("%s doesn't writeable.\n", f);  \
            __vcm_set_errno(_VCM_EFNOTWRITEABLE);       \
            return VCM_ERR;                             \
        }                                               \
    }while(0)
    
/**
 *  Check file is executeable or not
 */
#define CHECK_F_EXECUTEABLE(f)                              \
    do{                                                     \
        CHECK_POINTER(f);                                   \
        if(-1 == ACCESS_IS_EXECUTABLE(f)) {                 \
            log_vcm_err("%s doesn't executeable.\n", f);    \
            __vcm_set_errno(_VCM_EFNOTEXECUTEABLE);         \
            return VCM_ERR;                                 \
        }                                                   \
    }while(0)

/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/
/**
 *  Terminal logging and debug
 */
#ifdef _VCM_LOGGING
#include <libgen.h>
 #define log_vcm_err(fmt...)  log_vcm(STDERR, VCM_LOG_ERROR,Basename(__FILE__), __func__, __LINE__, fmt)
 #define log_vcm_succ(fmt...) log_vcm(STDOUT, VCM_LOG_SUCCESS,Basename(__FILE__), __func__, __LINE__,fmt)
 #define log_vcm_info(fmt...) log_vcm(STDOUT, VCM_LOG_INFO,Basename(__FILE__), __func__, __LINE__,fmt)
 #define log_vcm_warn(fmt...) log_vcm(STDERR, VCM_LOG_WARNING,Basename(__FILE__), __func__, __LINE__,fmt)
 #define log_vcm_simple(fmt...) log_vcm(STDOUT, VCM_LOG_SIMPLE,Basename(__FILE__), __func__, __LINE__,fmt)
#else
 #define log_vcm_err(fmt...)  do{}while(0)
 #define log_vcm_succ(fmt...) do{}while(0)
 #define log_vcm_info(fmt...) do{}while(0)
 #define log_vcm_warn(fmt...) do{}while(0)
 #define log_vcm_simple(fmt...) do{}while(0)
#endif /*<_VCM_LOGGING>*/


#ifdef _VCM_DEBUG
#include <libgen.h>
 #define log_vcm_debug() log_vcm(STDOUT, VCM_LOG_DEBUG, Basename(__FILE__), __func__, __LINE__,"\n")
#else
 #define log_vcm_debug() do{}while(0)
#endif /*<_VCM_DEBUG>*/

INT32 log_vcm(FILE *fp, LOG_VCM_TYPE type, 
                CONST CHAR *__file, CONST CHAR *__func, CONST UINT32 __line, CONST CHAR *fmt, ...);



/**
 *  cell management module initialized function
 */
INT32 vcm_module_init();
INT32 vcm_clean_up();
INT32 vcm_check_paras_valid();
INT32 vcm_task_init();
INT32 vcm_task_main_ctrl(VOID *arg);


/**
 *  Display functions
 */
INT32 vcm_set_display(FILE *fp, vcm_set set, int start, int end, CONST CHAR flag);
INT32 vcm_sm_display(FILE *fp);


/**
 *  Board Management Clock Synchronization
 *  Sync & Ansync
 */
INT32 vcm_handle_bord_clock_sync();
INT32 vcm_handle_bord_clock_async();


/**
 *  L1: phy layer initial, configure, terminate function
 */
INT32 vcm_L1_init(CONST UINT32 cell_id);
INT32 vcm_L1_cfg(CONST UINT32 cell_id);
INT32 vcm_L1_kill(CONST UINT32 cell_id);
INT32 vcm_L1_clean_up();
VOID  vcm_L1_socket_callback(INT32 sock_fd, struct sockaddr_storage *clientaddr, UINT8 *msg_p, UINT32 msg_len);


/**
 *  L2L3: mac layer initial, configure, terminate function
 */
INT32 vcm_L2L3_init(CONST UINT32 cell_id);
INT32 vcm_L2L3_cfg(CONST UINT32 cell_id);
INT32 vcm_L2L3_kill(CONST UINT32 cell_id);
INT32 vcm_L2L3_clean_up();
VOID  vcm_L2L3_socket_callback(INT32 sock_fd, struct sockaddr_storage *clientaddr, UINT8 *msg_p, UINT32 msg_len);


/**
 *  Module register
 */
INT32 vcm_module_register_init();
INT32 vcm_task_module_register_ctrl(VOID *arg);
INT32 vcm_module_register(UINT32 module_id, ULONG mask, vcm_register_cb_f cb);
INT32 vcm_module_register_update(UINT32 module_id, ULONG mask, vcm_register_cb_f cb);
INT32 vcm_register_thing_happend(UINT32 cell_id, ULONG mask);
INT32 vcm_module_unregister(UINT32 module_id);


/**
 *  RRU: register, unregister, etc.
 *  I think that it's shouldn't use rru register message to be the argument parameter, just use rru_info_t structure
 *  for a parameter.
 */
INT32 vcm_handle_rru_reg_req(CONST vcm_outer_msg_rrum_rru_reg_req *msg);
INT32 vcm_rru_register(CONST vcm_outer_msg_rrum_rru_reg_req *rru);
INT32 vcm_rru_unregister(CONST vcm_outer_msg_rrum_rru_reg_req *rru);


 
#endif /*<_VCM_H>*/
