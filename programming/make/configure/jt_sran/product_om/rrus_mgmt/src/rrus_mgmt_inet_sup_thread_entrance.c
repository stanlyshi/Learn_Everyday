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
 *File name: rrus_mgmt_inet_sup_thread_entrance.c
 *Description: the entrance of rru_mgmt_inet_sup_thread.
 *
 *Current Version: 0.l
 *Author: wangcheng (wangcheng2017@ict.ac.cn)
 *Date: 2018.09.13
 *
 ************************************************************************/
 
/* Dependencies ------------------------------------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include "sbs_type.h"
#include "sbs_msgq.h"
#include "sbs_log.h"
#include "om_common_h.h"
#include "rrus_mgmt_common.h"
#include "rrus_mgmt_ir.h"

/* Declarations ------------------------------------------------------------ */
extern MsgqType MSGQ_ID_RRUS_MGMT_INET_SUP;

extern MsgqType MSGQ_ID_RRUS_MGMT;

extern INT32 rrus_mgmt_inet_sup_init();

extern INT32 prehandle_send_rru_ip_broadcast_to_rrus_mgmt(void*, UINT32);

extern INT32 prehandle_send_rru_om_msg_to_rrus_mgmt(void*, UINT32);

extern INT32 socket_tcp_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, UINT16 port);

extern INT32 socket_udp_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, UINT16 port);



INT32 udp_sockfd;
struct sockaddr_in udp_client;

INT32 tcp_sockfd;


/* Functions --------------------------------------------------------------- */
/**********************************************
** init rrus_mgmt rru fd(tcp and udp).
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
INT32 rrus_mgmt_inet_sup_init()
{
    int udp_fd;
    if ((udp_fd = socket_udp_register(MSGQ_ID_RRUS_MGMT_INET_SUP, NULL, MSGQID_VALID, BBU_UDP_PORT)) == FUNC_ERR)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "UDP socket register error.\n");
		return FUNC_ERR;
    }
	udp_sockfd = udp_fd;

	//发出广播address
    bzero(&udp_client, sizeof(struct sockaddr_in));
    udp_client.sin_family = AF_INET;
    udp_client.sin_port = htons(RRU_UDP_PORT);
    udp_client.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	int tcp_listen_fd;
	if ((tcp_listen_fd = socket_tcp_register(MSGQ_ID_RRUS_MGMT_INET_SUP, NULL, MSGQID_VALID, BBU_RRU_TCP_PORT)) == FUNC_ERR)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "TCP socket register error.\n");
		return FUNC_ERR;
	}
	tcp_sockfd = tcp_listen_fd;

	return FUNC_OK;
    
}



/**********************************************
** pre handle rru ip broadcast message.
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
INT32 prehandle_send_rru_ip_broadcast_to_rrus_mgmt(void* msg, UINT32 len)
{
	CommMsgHead	msg_head;
	UINT8 buf[len + sizeof(msg_head)];
	
	msg_head.msg_type = MODULE_RRU;
	msg_head.msg_code = htons(MSG_CODE_RRU_BBU_RRU_IP_REQ);
	msg_head.msg_len = htonl(len + sizeof(msg_head));

	memcpy(buf, &msg_head, sizeof(msg_head));
	memcpy(buf + sizeof(msg_head), msg, len);
	if (send_msgq(MSGQ_ID_RRUS_MGMT, buf, sizeof(buf), WAIT_FOREVER, 1) == FUNC_ERR)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "Send rru ip broadcast to rrus_mgmt error.\n");
		return FUNC_ERR;
	}
	
	return FUNC_OK;
}

/**********************************************
** pre handle rru om message.
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
INT32 prehandle_send_rru_om_msg_to_rrus_mgmt(void* msg, UINT32 len)
{
	CommMsgHead	msg_head;
	UINT8 buf[len + sizeof(msg_head)];
	
	msg_head.msg_type = MODULE_RRU;
	msg_head.msg_code = htons((UINT16)ntohl(((IrMsgHead*)msg)->msg_code));
	msg_head.msg_len = htonl(len + sizeof(msg_head));

	memcpy(buf, &msg_head, sizeof(msg_head));
	memcpy(buf + sizeof(msg_head), msg, len);
	if (send_msgq(MSGQ_ID_RRUS_MGMT, buf, sizeof(buf), WAIT_FOREVER, 1) == FUNC_ERR)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "Send rru ip broadcast to rrus_mgmt error.\n");
		return FUNC_ERR;
	}
	
	return FUNC_OK;
}



/**********************************************
** the entrance of rrus_mgmt_inet_sup thread.
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
void *rrus_mgmt_inet_sup_entrance()
{
    char    buffer[MAX_MSGQ_MSG_LEN];      
    INT32   rl = 0;

    if (rrus_mgmt_inet_sup_init() == FUNC_ERR)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Function rrus_mgmt_inet_sup_init return error and thread return.\n");
		return NULL;
    }

    log_msg(LOG_INFO, RRUS_MGMT, "RRUS_MGMT_INET_SUP init success, enter while...\n");

    while (1)
    {
        usleep(250000);
        memset(buffer, 0, MAX_MSGQ_MSG_LEN);
        if ((rl = receive_msgq(MSGQ_ID_RRUS_MGMT_INET_SUP, buffer, MAX_MSGQ_MSG_LEN, WAIT_FOREVER)) == FUNC_ERR)
        {
            log_msg(LOG_ERR, RRUS_MGMT, "Receive message from RRUS_MGMT_INET_SUP_MSG_QUEUE failed in %s function at %d lines.\n", __FUNCTION__, __LINE__);
            continue;
        }
        else if (rl == RRU_IP_BROADCAST_LENGTH)
        {
            if (prehandle_send_rru_ip_broadcast_to_rrus_mgmt((void*)buffer, rl) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Send rru ip broadcast to rrus_mgmt error.\n");
            }
        }
        else
        {
            if (prehandle_send_rru_om_msg_to_rrus_mgmt((void*)buffer, rl) == FUNC_ERR)
            {
                log_msg(LOG_ERR, RRUS_MGMT, "Send rru om message to rrus_mgmt error.\n");
            }
        }
    }
}
