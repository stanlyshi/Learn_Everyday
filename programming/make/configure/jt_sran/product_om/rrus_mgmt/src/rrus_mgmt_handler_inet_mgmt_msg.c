/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:34 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:46 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:30 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:04 CST. */
/************************************************************************
 *Copyright (c) 2018 Sylincom.
 *All rights reserved.
 *
 *File name: rrus_mgmt_handler_inet_mgmt_msg.c
 *Description: All handlers of messages from inet_mgmt.
 *
 *Current Version: 0.l
 *Author: wangcheng (wangcheng2017@ict.ac.cn)
 *Date: 2018.08.26
 *
 ************************************************************************/

/* Includes**************************************************************/
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netpacket/packet.h>
#include "sbs_log.h"
#include "sbs_type.h"
#include "sbs_msgq.h"
#include "om_common_h.h"
#include "rrus_mgmt_common.h"
#include "rrus_mgmt_api_inet_recv.h"
#include "rrus_mgmt_api_inet_send.h"
#include "rrus_mgmt_cpri.h"

/* Declarations ------------------------------------------------------- */

extern INT32 raw_socket_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, UINT16 protocol);

extern INT32 unsocket_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, char *path, UINT32 stream_flag);

extern void unsocket_cb(INT32 sock_fd, struct sockaddr *clientaddr, char *msg_p, UINT32 msg_len);

extern MsgqType MSGQ_ID_RRUS_MGMT;

/* Variables ---------------------------------------------------------- */
INT32 cpri_raw_fd;
struct sockaddr_ll cpri_sll;


/* Functions----------------------------------------------------------- */
/**********************************************
** rrus_mgmt init fd register to inet_mgmt.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/
void rawsocket_cb(INT32 sock_fd, struct sockaddr *clientaddr, char *msg_p, UINT32 msg_len)
{
	CommMsgHead	msg_head;
	UINT8 buf[msg_len + sizeof(msg_head)-sizeof(CpriMacHead)];

	msg_head.msg_type = MODULE_DATA_FPGA;
	msg_head.msg_code = 0;
	msg_head.msg_len = 0;

	memcpy(buf, &msg_head, sizeof(msg_head));
	memcpy(buf + sizeof(msg_head), msg_p + sizeof(CpriMacHead), msg_len - sizeof(CpriMacHead));

	if (sock_fd == cpri_raw_fd)
	{
		if (send_msgq(MSGQ_ID_RRUS_MGMT, (char*)(buf), sizeof(buf), WAIT_FOREVER, 1) == FUNC_ERR)
		{
			log_msg(LOG_ERR, RRUS_MGMT, "Send raw socket message to rrus_mgmt error.\n");
			return;
		}
	}
	else
		log_msg(LOG_ERR, RRUS_MGMT, "raw socket cpri_raw_fd not euqal to sock_fd param.\n");
	return;
}



INT32 rrus_mgmt_inet_init()
{
    if ((cpri_raw_fd = raw_socket_register(NULL_MSGQ, (pCallBackFun*)rawsocket_cb, PCALLB_VALID, CPRI_MAC_PROTO)) == FUNC_ERR)
    {
		log_msg(LOG_ERR, RRUS_MGMT, "Raw socket of cpri register error and thread return.\n");
		return FUNC_ERR;
	}

	if ((FUNC_ERR == unsocket_register(NULL_MSGQ, (pCallBackFun*)unsocket_cb, PCALLB_VALID, UNSOCKET_PATH, STREAM_CHAR)))
	{
		log_msg(LOG_ERR, RRUS_MGMT, "Unsocket register error and thread return.\n");
		return FUNC_ERR;
	}
	
	return FUNC_OK;
}


/**********************************************
** INET_RECV invoke this function to send RRU broadcast message to RRUS_MGMT message queue.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/
INT32 INET_RECV_send_rru_broadcast_message_to_RRUS_MGMT(void* buf, UINT32 length)
{
    return FUNC_OK;
}



/**********************************************
** INET_RECV invoke this function to send RRU message to RRUS_MGMT message queue.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/
INT32 INET_RECV_send_rru_message_to_RRUS_MGMT(void* buf, UINT32 length)
{
    return FUNC_OK;
}


/**********************************************
** INET_RECV invoke this function to send data_fpga message to RRUS_MGMT message queue.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/
INT32 INET_RECV_send_fpga_message_to_RRUS_MGMT(void* buf, UINT32 length)
{
    return FUNC_OK;
}


/**********************************************
** INET_RECV invoke this function to send mac message to RRUS_MGMT message queue.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      error : -1
**
***********************************************/
INT32 INET_RECV_send_mac_message_to_RRUS_MGMT(void* buf, UINT32 length)
{
    return FUNC_OK;
}

