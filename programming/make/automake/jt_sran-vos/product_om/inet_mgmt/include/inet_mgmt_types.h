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
** File name: inet_mgmt_types.h
** Description:  
**
** Current Version: 
** $Revision$
** Author: first    : yeguanwen 2018��8��29��
**         modify   : Rong Tao  2019.02
** Date: 2018.08 - 2019.02 - 
**
***********************************************************************************************************************/
#ifndef _MODULE_INET_MGMT_TYPES_H
#define _MODULE_INET_MGMT_TYPES_H

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include <mqueue.h>
#include "vos_types.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/
#ifdef _VOS_TYPES_H_
 #define INET_OK     VOS_OK
 #define INET_ERR    VOS_ERROR
 #define INET_YES    VOS_YES
 #define INET_NO     VOS_NO
 #define INET_WARN   2
#else
 #define INET_OK      0
 #define INET_ERR     -1
 #define INET_YES     1
 #define INET_NO      0
 #define INET_WARN   2
#endif

/* Types -------------------------------------------------------------------------------------------------------------*/
typedef mqd_t MsgqType;

typedef void(*pCallBackFun)(INT32 sock_fd, struct sockaddr_storage *clientaddr, UINT8 *msg_p, UINT32 msg_len);

typedef enum{
	TCP,
	UDP,
	UNSOCKET,
	RAWSOCKET
}SocketType;


typedef struct{
	SocketType      socket_type;
	MsgqType        msgq;
	pCallBackFun    pFun;
	INT32           listenfd;
}ListenFdInfo;


typedef struct{
	SocketType      socket_type;
	MsgqType        msgq;
	pCallBackFun    pFun;
	UINT32          ip;
	INT32           conn_fd;
}TcpInfo;

typedef struct{
	SocketType      socket_type;
	pCallBackFun    pFun;
	INT32           sock_fd;
	MsgqType        msgq;
	struct sockaddr clientaddr;
}SockPara;

/*inet_mgmt message struct*/
typedef struct{
	UINT32          flag;
	INT32           fd;
	UINT32          ip;
	struct sockaddr clientaddr;
	MsgqType        msgq;
	UINT32          msg_len;
	UINT8*           msg_p;
}InetMsg;


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

#endif /*<_MODULE_INET_MGMT_TYPES_H>*/

