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
 * om_common.c
 *
 *  Created on: Sep 20, 2018
 *      Author: root
 */

#include "sbs_type.h"
#include "sbs_msgq.h"
#include "sbs_log.h"
#include "om_common_h.h"


INT32 send_msg_to_om(UINT16 module_id, UINT8 *msg_p, UINT32 msg_len)
{
	 switch(module_id)
	 {
		   case MODULE_BORD_MGMT:
				if (0 != send_msgq(MSGQ_ID_BORD_MGMT,msg_p,msg_len, WAIT_FOREVER, 1))
				{
						 log_msg(LOG_WARNING,INET_MGMT,"send message error to BRDM\n");
						 return -1;
				}
				break;
		   case MODULE_CELL_MGMT:
				if (0 != send_msgq(MSGQ_ID_CELL_MGMT,msg_p,msg_len, WAIT_FOREVER, 1))
				{
						 log_msg(LOG_WARNING,INET_MGMT,"send message error to CELL_MGMT\n");
						 return -1;
				}
				break;
		   case MODULE_RRUS_MGMT:
				if (0 != send_msgq(MSGQ_ID_RRUS_MGMT,msg_p,msg_len, WAIT_FOREVER, 1))
				{
						 log_msg(LOG_WARNING,INET_MGMT,"send message error to CELL_MGMT\n");
						 return -1;
				}

				break;
		   case MODULE_MAIN_CTRL:
				if (0 != send_msgq(MSGQ_ID_MAIN_CTRL,msg_p,msg_len, WAIT_FOREVER, 1))
				{
						 log_msg(LOG_WARNING,INET_MGMT,"send message error to CELL_MGMT\n");
						 return -1;
				}

				break;
		   case MODULE_CFIG_MGMT:
				if (0 != send_msgq(MSGQ_ID_CFIG_MGMT,msg_p,msg_len, WAIT_FOREVER, 1))
				{
						 log_msg(LOG_WARNING,INET_MGMT,"send message error to CELL_MGMT\n");
						 return -1;
				}

				break;
		   case MODULE_INET_MGMT:
				if (0 != send_msgq(MSGQ_ID_INET_MGMT,msg_p,msg_len, WAIT_FOREVER, 1))
				{
						 log_msg(LOG_WARNING,INET_MGMT,"send message error to CELL_MGMT\n");
						 return -1;
				}

				break;
		   case MODULE_TIMER:
				if (0 != send_msgq(MSGQ_ID_TIMER_MGMT,msg_p,msg_len, WAIT_FOREVER, 1))
				{
						 log_msg(LOG_WARNING,INET_MGMT,"send message error to TIMER_MGMT\n");
						 return -1;
				}

				break;
		   default:
					log_msg(LOG_WARNING,INET_MGMT,"wrong module type!\n");
					return -1;
		   break;
	 }

	 return 0;
}
