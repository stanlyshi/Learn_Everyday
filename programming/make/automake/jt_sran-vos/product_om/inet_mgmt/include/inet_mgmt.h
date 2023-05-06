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
/***********************************************************************************************************************
** 
** Copyright (c) 2008-2019 ICT/CAS.
** All rights reserved.
**
** File name: inet_mgmt.h
** Description:  
**
** Current Version:   
** $Revision$
** Author: first    : Ye Guanwen    2018��8��29��
**         modify   : Rong Tao      2019.02
** Date: 2018.08 - 2019.02 - 
**
***********************************************************************************************************************/
#ifndef _MODULE_INET_MGMT_H 
#define _MODULE_INET_MGMT_H
 
/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include <sys/socket.h>
  
#include "inet_mgmt_config.h"

#include "inet_mgmt_types.h"
#include "inet_mgmt_config.h"


#include "vos_types.h"
#include "vos_lib.h"
#include "vos_string.h"
#include "vos_que.h"
#include "vos_task.h"
#include "vos_sem.h"
#include "vos_ctype.h"

#include "cell_mgmt_api.h"

  
/* Constants ---------------------------------------------------------------------------------------------------------*/
typedef enum log_inet_type_e {
    INET_LOG_TYPE_MIN,
    INET_LOG_ERROR,
    INET_LOG_INFO,
    INET_LOG_SUCCESS,
    INET_LOG_WARNING,
    INET_LOG_DEBUG,
    INET_LOG_SIMPLE,
    INET_LOG_TYPE_MAX,
}LOG_INET_TYPE;


/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
#ifdef _INET_LOGGING
#include <libgen.h>
#define log_inet_err(fmt...)    log_inet(STDERR, INET_LOG_ERROR,basename(__FILE__), __func__, __LINE__, fmt)
#define log_inet_info(fmt...)   log_inet(STDOUT, INET_LOG_INFO,basename(__FILE__), __func__, __LINE__, fmt)
#define log_inet_warn(fmt...)   log_inet(STDOUT, INET_LOG_WARNING,basename(__FILE__), __func__, __LINE__, fmt)
#define log_inet_succ(fmt...)   log_inet(STDOUT, INET_LOG_SUCCESS,basename(__FILE__), __func__, __LINE__, fmt)
#define log_inet_debug()        log_inet(STDOUT, INET_LOG_DEBUG,basename(__FILE__), __func__, __LINE__,"\n")

#else
#define log_inet_err(fmt...)    do{}while(0)
#define log_inet_info(fmt...)   do{}while(0)
#define log_inet_warn(fmt...)   do{}while(0)
#define log_inet_succ(fmt...)   do{}while(0)
#define log_inet_debug()        do{}while(0)
#endif 

/* Globals -----------------------------------------------------------------------------------------------------------*/
extern vos_module_t     inet_module;

#define Malloc(size)    MallocVos(size, inet_module.moduleID)

/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 log_inet(FILE *fp, LOG_INET_TYPE type, CONST CHAR *__file, CONST CHAR *__func, CONST UINT32 __line, CONST CHAR *fmt, ...);

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
/*function declaration*/
INT32 inet_mgmt_module_init();
INT32 inet_mgmt_clean_up();
INT32 inet_mgmt_start();

void *msg_send_fn(void*arg);
void *msg_receive_fn(void*arg);

INT32 send_msg_to_module(MsgqType msgq/*UINT16 module_id*/,UINT32 socktype,UINT8 *msg_p,UINT32 msg_len);
INT32 send_msg_to_inet(UINT16 module_id,UINT32 streamflag,INT32 fd,UINT32 ip,struct sockaddr *clientaddr,
                        UINT8 *msg_p,UINT32 msg_len);

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
/*register function declaration*/
INT32 socket_tcp_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, UINT16 port);
INT32 socket_udp_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, UINT16 port);
INT32 unsocket_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, char *path, UINT32 stream_flag);
INT32 raw_socket_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag,UINT32 protocol);
INT32 socket_unregister(INT32 sockfd);


#endif /*<_MODULE_INET_MGMT_H>*/
