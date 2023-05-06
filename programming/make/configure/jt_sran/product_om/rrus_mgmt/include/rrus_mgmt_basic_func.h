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
 *File name: rru_agent_basic_func.h
 *Description: the basic functions for rrus_mgmt.
 *
 *Current Version: 0.l
 *Author: WangCheng (wangcheng2017@ict.ac.cn)
 *Date: 2018.06.05
 *
 ************************************************************************/

#ifndef RRUS_MGMT_BASIC_FUNC_H
#define RRUS_MGMT_BASIC_FUNC_H


/* Dependencies ------------------------------------------------------------- */
#include "sbs_type.h"


/* Functions ---------------------------------------------------------------- */

/*******************************************************************************
* Show the system time.
*
* Output:  return -1,failed;
*          return 0, success
******************************************************************************/
extern INT32 show_system_time();

/*******************************************************************************
* Get the RRU id from Ir msg.
*
* Output:  return -1,failed;
*          return 0, success
******************************************************************************/
extern UINT32 get_rru_id_from_ir(void *msg, UINT32 len);

/*******************************************************************************
* Get the RRU event from Ir msg.
*
* Output:  return -1,failed;
*          return 0, success
******************************************************************************/
extern UINT32 get_rru_event_from_ir(void *msg, UINT32 len);

/*******************************************************************************
* xx.xx.xx.xx转化�?xxxx"
*
******************************************************************************/
extern void ip_to_array(UINT8 *ip_to, char *ip);

/*******************************************************************************
* 通过rru_id寻找相应entity_id
*
* Output:  return -1,failed;
*          return >0, success
******************************************************************************/
extern INT32 id_to_entity(UINT32 rru_id);

/*******************************************************************************
* 通过cpri_index寻找相应entity_id
*
* Output:  return -1,failed;
*          return >0, success
******************************************************************************/
extern INT32 id_to_cpri_entity(UINT16 cpri_index);



/*******************************************************************************
* attract the msg code from ir message
*
* Output:  return -1,failed;
*          return >0, success
******************************************************************************/
extern INT32 get_msg_code_from_ir(void*, UINT32);




/*******************************************************************************
* Add the Mac head and send through raw socket
*
* Output:  return -1,failed;
*          return >0, success
******************************************************************************/
extern INT32 send_msg_mac(void* msg, UINT32 msg_len);


/*******************************************************************************
* print the ir msg
*
* Output:  return -1,failed;
*          return >0, success
******************************************************************************/
extern INT8 print_ir_msg(UINT16 msg_code, UINT8* msg_p);

/*******************************************************************************
* process the ir message--parameter query response
*
* Output:  return -1,failed;
*          return >0, success
******************************************************************************/
extern INT8 ie_query_rsp(INT32 entity_id, UINT16 ie_flag, void* msg);


/*******************************************************************************
* return the ie flag of ir message--parameter configure response
*
* Output:  return -1,failed;
*          return >0, success
******************************************************************************/
extern INT8 ie_cfg_rsp(UINT16 ie_flag, void* msg);


#endif
