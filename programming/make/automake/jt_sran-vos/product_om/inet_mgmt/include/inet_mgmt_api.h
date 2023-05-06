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
** File name: inet_mgmt_api.h
** Description:  
**
** Current Version: 
** $Revision$
** Author: first    : yeguanwen 2018��8��29��
**         modify   : Rong Tao  2019.02
** Date: 2018.08 - 2019.02 - 
**
***********************************************************************************************************************/
#ifndef _MODULE_INET_MGMT_API_H
#define _MODULE_INET_MGMT_API_H

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include <sys/socket.h>

#include "inet_mgmt_types.h"
#include "inet_mgmt_config.h"


/* Constants ---------------------------------------------------------------------------------------------------------*/
#define CALLBACK_FUNC_VALID             1
#define CALLBACK_FUNC_INVALID           0
#define MSGQ_VALID                      1
#define MSGQ_INVALID                    0
#define SOCK_STREAM_VALID               1
#define SOCK_STREAM_INVALID             0
#define IP_VALID                        1
#define IP_INVALID                      0
#define SOCKADDR_NULL                   NULL

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
extern INT32 inet_mgmt_module_init();
extern INT32 inet_mgmt_clean_up();

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
extern INT32 send_msg_to_inet(UINT16 module_id,UINT32 streamflag,INT32 fd,UINT32 ip,struct sockaddr *clientaddr,
                              char *msg_p,UINT32 msg_len);

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
extern INT32 socket_tcp_register(MsgqType msgq_id, pCallBackFun p, UINT32 callback_flag, UINT16 port);
extern INT32 socket_udp_register(MsgqType msgq_id, pCallBackFun p, UINT32 callback_flag, UINT16 port);
extern INT32 unsocket_register(MsgqType msgq_id, pCallBackFun p, UINT32 callback_flag, char *path, UINT32 stream_flag);
extern INT32 raw_socket_register(MsgqType msgq_id, pCallBackFun p, UINT32 callback_flag,UINT32 protocol);
extern INT32 socket_unregister(INT32 sockfd);

#endif /*<_MODULE_INET_MGMT_API_H>*/
