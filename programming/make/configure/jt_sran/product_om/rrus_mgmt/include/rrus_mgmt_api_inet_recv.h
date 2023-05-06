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
 *File name: rrus_mgmt_inet_recv_api.h
 *Description: api of INET_RECV with RRUS_MGMT.
 *
 *Current Version: 0.l
 *Author: wangcheng2017@ict.ac.cn
 *Date: 2018.08.25
 *
 ************************************************************************/
#ifndef RRUS_MGMT_INET_RECV_API_H
#define RRUS_MGMT_INET_RECV_API_H

/*******************************include**********************************/
#include "sbs_type.h"

/******************************functions*********************************/

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
extern INT32 rrus_mgmt_inet_init();


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
extern INT32 INET_RECV_send_rru_broadcast_message_to_RRUS_MGMT(void* buf, UINT32 length);



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
extern INT32 INET_RECV_send_rru_message_to_RRUS_MGMT(void* buf, UINT32 length);


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
extern INT32 INET_RECV_send_fpga_message_to_RRUS_MGMT(void* buf, UINT32 length);


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
extern INT32 INET_RECV_send_mac_message_to_RRUS_MGMT(void* buf, UINT32 length);






#endif