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
 * mnt_mgmt_init.h
 *
 *  Created on: Aug 21, 2018
 *      Author: root
 */

#ifndef MNT_MGMT_INIT_H_
#define MNT_MGMT_INIT_H_

#include "sbs_type.h"



#define MSG_RECEIVE_PRIOR      1
#define Debug
//#define Test
#define LOCAL_UDP_PORT 		35000
#define CLIENT_UDP_PORT 	35001

#define PROTOCOL_TYPE 		0x10DB

extern UINT8 cpu_data_mac1[6];
extern UINT8 cpu_data_mac2[6];
extern UINT8 cpu_sre_mac[6];
extern UINT8 cpu_om_mac[6];
//UINT32 udp_sock_fd;
UINT32 raw_sock_fd;
INT32 bord_mgmt_init();
INT32 device_id_init();
INT32 heart_beat_record_init();
INT32 conver_macaddr_in_string_to_byte();
#endif
