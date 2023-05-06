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
** File name: inet_mgmt_config.h 
** Description: INET_MGMT "#define"
**
** Current Version: 
** $Revision$
** Author: first    : Ye Guanwen    2018��8��29��
**         modify   : Rong Tao      2019.02
** Date: 2018.08 - 2019.02 - 
**
***********************************************************************************************************************/
#ifndef _MODULE_INET_MGMT_DEFINE_H
#define _MODULE_INET_MGMT_DEFINE_H
     
/* Dependencies ------------------------------------------------------------------------------------------------------*/
/* Constants ---------------------------------------------------------------------------------------------------------*/
#define INET_MGMT_NAME              "Inet Management Module"
#define INET_MGMT_MAIN_TASK_NAME    "Inet Management Main Task"
#define INET_MGMT_RECV_TASK_NAME    "Inet Management Receive Task"
#define INET_MGMT_SEND_TASK_NAME    "Inet Management Send Task"

/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
#define _INET_LOGGING   /* turn on logging message */


#define MAX_SOCK_NUM                    100

#define MAX_RECV_MSG_LEN                1500
#define LISTEN_LEN                      10  

#define MAX_INET_MSGQ_MSG_LEN           2048


#define INET_STR_LEN                    256
#define INET_MAX_MSGQ_MSG_NUM           256


#define G_OM_PORT_NAME                  "ens33"



/* Globals -----------------------------------------------------------------------------------------------------------*/

/* Functions ---------------------------------------------------------------------------------------------------------*/
/* Check Macros ------------------------------------------------------------------------------------------------------*/

#endif /*<_MODULE_INET_MGMT_DEFINE_H>*/
