/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:33 CST. */
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
/*
 * inet_mgmt.h
 *
 *  Created on: 2018年8月29日
 *      Author: yeguanwen
 */

#ifndef PRODUCT_OM_INET_MGMT_INET_MGMT_H_INET_MGMT_H_
#define PRODUCT_OM_INET_MGMT_INET_MGMT_H_INET_MGMT_H_


#ifndef NETPORT_H_
#define NETPORT_H_

#include <stdio.h>
#include <sys/socket.h>

#include "sbs_type.h"
#include "om_common_h.h"  /*common header file*/

#define IP_LEN                   16
#define MAX_SOCK_NUM             100

#define MSG_RECEIVE_PRIO         1
#define MAX_RECV_MSG_LEN         1500
#define LISTEN_LEN               10 //research



#define MAX_INET_MSGQ_MSG_LEN    MAX_MSGQ_MSG_LEN

MsgqType NETPORT_MSGQ_ID;


/*RRU Msg mgmt*/

/************RRU_MGMT provide****************/
#define RRU_MSG        0
#define DATA_FPGA_MSG  1
#define UDP_BROADCAST  2
#define MAC_MSG        3
/*send msg to om_rru */
//extern  INT32 send_to_RRUS_MGMT(char *msg_p, UINT32 msg_len, UINT32 flag);
extern  INT32 send_tcp_to_RRUS_MGMT(char *msg_p,UINT32 msg_len,UINT32 flag);
extern  INT32 send_udp_to_RRUS_MGMT(char *msg_p,UINT32 msg_len,UINT32 flag);
extern  INT32 send_mac_to_RRUS_MGMT(char *msg_p,UINT32 msg_len,UINT32 flag);
/********************************************/

typedef void(*pCallBackFun)(INT32 sock_fd, struct sockaddr *clientaddr, char *msg_p, UINT32 msg_len);

typedef enum{
	TCP,
	UDP,
	UNSOCKET,
	RAWSOCKET
}SocketType;


typedef struct{
	/*UINT16 module_id;*/
	SocketType socket_type;
	MsgqType msgq;
	pCallBackFun pFun;
	INT32 listenfd;
}ListenFdInfo;


typedef struct{
	/*UINT16 module_id;*/
	SocketType socket_type;
	MsgqType msgq;
	pCallBackFun pFun;
	UINT32 ip;
	INT32 conn_fd;
}TcpInfo;

typedef struct{
	SocketType socket_type;
	pCallBackFun pFun;
	//UINT8 addrUsed;
	INT32 sock_fd;
	/*UINT16 module_id;*/
	MsgqType msgq;
	struct sockaddr clientaddr;
}SockPara;

/*inet_mgmt message struct*/
typedef struct{
	UINT32 flag;
	INT32 fd;
	UINT32  ip;
	struct sockaddr clientaddr;
	/*UINT16 module_id;*/
	MsgqType msgq;
	UINT32 msg_len;
	char* msg_p;
}InetMsg;


/*function declaration*/
INT32 inet_mgmt_mgmt_init();
INT32 inet_mgmt_start();
//void *inet_mgmt_start(void*);
void *msg_send_fn(void*arg);
void *msg_receive_fn(void*arg);

INT32 send_msg_to_module(MsgqType msgq/*UINT16 module_id*/,UINT32 socktype,char *msg_p,UINT32 msg_len);
INT32 send_msg_to_inet(UINT16 module_id,UINT32 streamflag,INT32 fd,UINT32 ip,struct sockaddr *clientaddr,char *msg_p,UINT32 msg_len);

/*register function declaration*/

INT32 socket_tcp_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, UINT16 port);
INT32 socket_udp_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, UINT16 port);
INT32 unsocket_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag, char *path, UINT32 stream_flag);
INT32 raw_socket_register(MsgqType msgq, pCallBackFun p, UINT32 callback_flag,UINT32 protocol);
INT32 socket_unregister(INT32 sockfd);

#endif /* NETPORT_H_ */


#endif /* PRODUCT_OM_INET_MGMT_INET_MGMT_H_INET_MGMT_H_ */
