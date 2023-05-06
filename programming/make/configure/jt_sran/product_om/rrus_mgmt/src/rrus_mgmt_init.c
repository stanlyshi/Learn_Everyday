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
*File name: rrus_mgmt_init.h
*Description: the entrance of rrus_mgmt.
*
*Current Version: 0.l
*Author: wangcheng (wangcheng2017@ict.ac.cn)
*Date: 2018.08.23
*
************************************************************************/

/* Dependencies ------------------------------------------------------------- */
#include "sbs_task.h"
#include "om_common_h.h"
#include "sbs_type.h"
#include "sbs_log.h"
#include "sbs_msgq.h"

/* Declarations ---------------------------------------------------------- */
extern TaskType THREAD_RRUS_MGMT;

extern TaskType THREAD_RRUS_MGMT_INET_SUP;

extern UINT8 flag_thread_rrus_mgmt;

extern UINT8 flag_thread_rrus_mgmt_inet_sup;

extern MsgqType MSGQ_ID_RRUS_MGMT;

extern MsgqType MSGQ_ID_RRUS_MGMT_INET_SUP;

extern void *rrus_mgmt_entrance();

extern void *rrus_mgmt_inet_sup_entrance();

extern INT32 init_rru_sm(void);

/* Functions ------------------------------------------------------------- */
 extern INT32 socket_unregister(INT32 sockfd);

extern INT32 udp_sockfd;

extern INT32 tcp_sockfd;

/* Functions ------------------------------------------------------------- */
/**********************************************
** the clean function of RRUS_MGMT module.
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
INT32 rrus_mgmt_clean_up()
{
	if(socket_unregister(udp_sockfd) == -1)
	{
		return FUNC_ERR;
	}

	if(socket_unregister(tcp_sockfd) == -1)
	{
		return FUNC_ERR;
	}

	return FUNC_OK;
}

/**********************************************
** the init entrance of RRUS_MGMT module.
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
 INT32 rrus_mgmt_module_init()
 {
	 MSGQ_ID_RRUS_MGMT = open_msgq(RRUS_MGMT_MSGQ_NAME, MAX_MSGQ_MSG_NUM, MAX_MSGQ_MSG_LEN);
     if (FUNC_ERR == MSGQ_ID_RRUS_MGMT)
     {
         log_msg(LOG_ERR, RRUS_MGMT, "open RRUS_MGMT_MSG_QUEUE_ID failed.\n");
         return FUNC_ERR;
     }

	 MSGQ_ID_RRUS_MGMT_INET_SUP = open_msgq(RRUS_MGMT_INET_SUP_MSGQ_NAME, MAX_MSGQ_MSG_NUM, MAX_MSGQ_MSG_LEN);
     if (FUNC_ERR == MSGQ_ID_RRUS_MGMT_INET_SUP)
     {
         log_msg(LOG_ERR, RRUS_MGMT, "open RRUS_MGMT_INET_SUP_MSG_QUEUE_ID failed.\n");
         return FUNC_ERR;
     }
	 
     // RRUS_MGMT thread
     if ((THREAD_RRUS_MGMT = create_pthread(1, SCHED_RR, 1, rrus_mgmt_entrance, NULL)) == FUNC_ERR)
     {
         log_msg(LOG_ERR, RRUS_MGMT, "Create THREAD_RRUS_MGMT failed.\n");
		 
         return FUNC_ERR;
     }
     else
     {
         log_msg(LOG_INFO, RRUS_MGMT, "Create THREAD_RRUS_MGMT success, the thread_id is %d\n", THREAD_RRUS_MGMT);
         flag_thread_rrus_mgmt = THREAD_VALID;
     }

     // RRUS_MGMT_INET_SUP thread
     if ((THREAD_RRUS_MGMT_INET_SUP = create_pthread(1, SCHED_RR, 1, rrus_mgmt_inet_sup_entrance, NULL)) == FUNC_ERR)
     {
         log_msg(LOG_ERR, RRUS_MGMT, "Create THREAD_RRUS_MGMT_INET_SUP failed.\n");
		 
         return FUNC_ERR;
     }
     else
     {
         log_msg(LOG_INFO, RRUS_MGMT, "Create THREAD_RRUS_MGMT_INET_SUP success, the thread_id is %d\n", THREAD_RRUS_MGMT_INET_SUP);
         flag_thread_rrus_mgmt_inet_sup = THREAD_VALID;
     }

     if (init_rru_sm() == FUNC_ERR)
     {
    	 log_msg(LOG_ERR, RRUS_MGMT, "RRU state machine init failed.\n");
    	 return FUNC_ERR;
     }
     return FUNC_OK;
}

