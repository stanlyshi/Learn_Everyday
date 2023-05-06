/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:32 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:44 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:28 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:03 CST. */
/*
 * fpga_msg_receive_fn.h
 *
 *  Created on: Aug 31, 2018
 *      Author: root
 */

#ifndef PRODUCT_OM_BORD_MGMT_H_FPGA_MSG_RECEIVE_FN_H_
#define PRODUCT_OM_BORD_MGMT_H_FPGA_MSG_RECEIVE_FN_H_

#include "sbs_type.h"
#include <sys/socket.h>        	//socket
#include <sys/ioctl.h>        	//ioctl
#include <net/if.h>        	//ifreq
#include <linux/if_packet.h>    //sockaddr_sll
#include <linux/if_ether.h> //ETH_P_ALL
#include <netinet/in.h>//struct sockaddr_in
#include <arpa/inet.h>//inet_addr,inet_aton,inet_ntoa


typedef struct
{
struct sockaddr_ll raw_serveraddr;
struct sockaddr_in udp_serveraddr;
struct sockaddr_in udp_clientaddr;
INT32 raw_fd;
INT32 un_fd;
INT32 udp_fd;
INT32 raw_len;
INT32 udp_len;
}Sesson;
extern Sesson sesson;
extern struct sockaddr clientaddr;
//void *fpga_msg_receive_fn(void);
#endif /* PRODUCT_OM_BORD_MGMT_H_FPGA_MSG_RECEIVE_FN_H_ */
