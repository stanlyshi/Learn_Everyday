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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:04 CST. */
/************************************************************************
 *Copyright (c) 2018 Sylincom.
 *All rights reserved.
 *
 *File name: rrus_mgmt_common.h
 *Description: the common defines in rrus_mgmt.
 *
 *Current Version: 0.l
 *Author: wangcheng (wangcheng2017@ict.ac.cn)
 *Date: 2018.08.18
 *
 ************************************************************************/


#ifndef RRUS_MGMT_COMMON_H
#define RRUS_MGMT_COMMON_H

#include <netinet/in.h>
#include "sbs_msgq.h"
#include <sys/socket.h>


typedef void(*pCallBackFun)(INT32 sock_fd, struct sockaddr *clientaddr, char *msg_p, UINT32 msg_len);

extern INT32 unsocket_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, char *path, UINT32 stream_flag);

extern INT32 send_msg_to_inet(UINT16 module_id, UINT32 streamflag, INT32 fd, UINT32 ip, struct sockaddr *clientaddr, char *msg_p, UINT32 msg_len);

#define NULL_MSGQ		0
#define PCALLB_VALID	1
#define MSGQID_VALID	0
#define STREAM_CHAR		1
#define STREAM_DATA		0
#define UNSOCKET_PATH	"/tmp/om_rx"

extern INT32 udp_sockfd;
extern struct sockaddr_in udp_client;

extern INT32 cpri_raw_fd;
extern struct sockaddr_ll cpri_sll;


#define CPRI_MAC_PROTO           0x7080
//#define NETPORTNAME         "ens33"
#define DEBUG

#define RRU_IP_BROADCAST_LENGTH 10

// socket
#define BBU_UDP_PORT        33333
#define RRU_UDP_PORT        33334
#define BBU_RRU_TCP_PORT    30000
#define RRU_SERVER_IP       "10.21.66.66"
#define RRU_IP_1       "10.21.200.142"//need to be assigned
#define SUB_MASK       "255.255.0.0"
#define CPRI_MAC            "0.0.0.0.0.0"
//#define OM_MAC              "208.23.194.172.136.57"//10.21.100.142
#define OM_MAC              "232.154.143.141.243.106"

#endif
