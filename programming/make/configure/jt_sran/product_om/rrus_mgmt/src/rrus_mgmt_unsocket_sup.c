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
 *File name: rrus_mgmt_unsocket_sup.c
 *Description: unsocket of RRUS_MGMT and CELL_MGMT.
 *
 *Current Version: 0.l
 *Author: wangcheng (wangcheng2017@ict.ac.cn)
 *Date: 2018.09.14
 *
 ************************************************************************/

/* Includes ----------------------------------------------------------- */
#include "sbs_type.h"
#include "sbs_log.h"
#include "sbs_msgq.h"
#include "om_common_h.h"
#include "rrus_mgmt_common.h"
#include "rrus_mgmt_api_mac.h"

/* Declarations -------------------------------------------------------- */

extern MsgqType MSGQ_ID_RRUS_MGMT;

extern MsgqType MSGQ_ID_CELL_MGMT;

extern INT32 set_cell_fd_by_cell_id(UINT32 cell_id, INT32 fd);

extern INT32 get_cell_fd_by_cell_id(UINT32 cell_id);

/* Functions ---------------------------------------------------------- */
void unsocket_cb(INT32 sock_fd, struct sockaddr *clientaddr, char *msg_p, UINT32 msg_len)
{
	UINT32 cell_id;
	cell_id = ((OmMacHead*)msg_p)->cell_id;

	if (get_cell_fd_by_cell_id(cell_id) != sock_fd)
	{
		set_cell_fd_by_cell_id(cell_id, sock_fd);
		log_msg(LOG_INFO, SOCKET_COMMON, "Update cell_id %d 's fd = %d.\n", cell_id, sock_fd);
	}

	UINT16 msg_code;
	msg_code = ((OmMacHead*)msg_p)->cell_id;
	if (msg_code > CODE_RRUS_MGMT_MAC_MIN)
	{
		if (send_msgq(MSGQ_ID_RRUS_MGMT, (char*)msg_p, msg_len, WAIT_FOREVER, 1) == FUNC_ERR)
			log_msg(LOG_ERR, SOCKET_COMMON, "Send RRUS_MGMT MAC message to rrus_mgmt error.\n");
	}
	else
	{
		if (send_msgq(MSGQ_ID_CELL_MGMT, (char*)msg_p, msg_len, WAIT_FOREVER, 1) == FUNC_ERR)
			log_msg(LOG_ERR, SOCKET_COMMON, "Send CELL_MGMT MAC message to rrus_)mgmt error.\n");
	}
	
	return;
}

