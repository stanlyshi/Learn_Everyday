/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:32 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:45 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:28 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:03 CST. */
/*
 * mnt_mgmt_msg_handle.c
 *
 *  Created on: Aug 21, 2018
 *      Author: root
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "sbs_timer.h"
#include "sbs_msgq.h"
#include "sbs_task.h"
#include "sbs_log.h"
#include "mnt_mgmt_init.h"
#include "mnt_mgmt_msg_handle.h"
#include "mnt_mgmt_api.h"
#include "om_common_h.h"
#include "fpga_msg_receive_fn.h"
#include "msg_receive_fn.h"
#include "inet_mgmt.h"

/*********************************************************************************************

 Description :handle heart beat timeout message from heart beat monitor timer
 input :NULL
 output:NULL
 return:success FUNC_OK
        false FUNC_ERR

 *********************************************************************************************/
INT32 handle_fn_heartb_timer_msg()
{
#ifdef Debug
	log_msg(LOG_INFO, BORD_MGMT, "check FPGA heart beat counts record!!!!!!!\n");
#endif
	int i,j;
	for(i = 0;i < FpgaNum;i++)
	{
		for(j = 0;j < PortNum;j++)
		{
			if(fpgaheartbeatrcd[i][j].heartb_count == 0 && fpgaheartbeatrcd[i][j].fpga_port_state == 1)
			{

				log_msg(LOG_ERR, BORD_MGMT, "fpga_id=%d,port_id=%d no heart beat!\n",i,j);

				// send alarm
			}
			if(fpgaheartbeatrcd[i][j].heartb_count == 0 && fpgaheartbeatrcd[i][j].fpga_port_state == 0)
			{
				log_msg(LOG_SUMMARY, BORD_MGMT, "fpga_id=%d,port_id=%d power off || power on but all net port fault \n",i,j);
				/////
			}

			fpgaheartbeatrcd[i][j].heartb_count = 0;
		}

	}
	return FUNC_OK;
}

/*********************************************************************************************

 Description :handle function of timeout message from query FPGA information timer
 input :NULL
 output:NULL
 return:success FUNC_OK
         false FUNC_ERR

 *********************************************************************************************/
INT32 handle_fn_query_fpga_cpu_heal_info_timer_msg()
{
	get_cpu_server_res_usage();    //get CPU server computing resource utilization
    get_cpu_server_heal_info();  // get CPU server health information
    query_fpga_heal_info(); //get FPGA health information
    return FUNC_OK;
}


/***********************************************************************************************

 Description :add heart beat  count timer
 input : time(second)
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ***********************************************************************************************/
INT32 add_handle_heartb_msg_timer(UINT32 seconds)
{
	heartb_timer* hb_timer = (heartb_timer*)calloc(1,sizeof(heartb_timer));
	hb_timer->timer = (struct sbs_timer_list*)calloc(1,sizeof(struct sbs_timer_list));
	hb_timer->seconds = seconds;
	hb_timer->timer->cb = send_heartb_bord_timeout_msg;
	hb_timer->timer->data = hb_timer;
	sbs_timer_schedule(hb_timer->timer,seconds,0);
	return FUNC_OK;
}
/***********************************************************************************************

 Description :add query FPGA timer
 input : time(second)
 output:NULL
 return: success FUNC_OK
         false FUNC_ERR

 ***********************************************************************************************/
INT32 add_query_fpga_timer(UINT32 seconds)
{
	query_fpga_timer* query_timer = (query_fpga_timer*)calloc(1,sizeof(query_fpga_timer));

	if(get_cpu_start_time() == FUNC_ERR)
	{
		log_msg(LOG_ERR, BORD_MGMT, "open /proc/stat failed in function add_query_fpga_timer!\n");
		return FUNC_ERR;
	}
	query_timer->seconds = seconds;
	query_timer->timer = (struct sbs_timer_list*)calloc(1,sizeof(struct sbs_timer_list));//must don't free
	query_timer->timer->cb = send_query_fpga_timeout_msg;
	query_timer->timer->data = query_timer;
	sbs_timer_schedule(query_timer->timer,seconds,0);
	return FUNC_OK;
}
/***********************************************************************************************

 Description :send check fpga heartbeat counts timeout message to own message queue(callback function)
 input: NULL
 output:NULL
 return:void

 ***********************************************************************************************/
void send_heartb_bord_timeout_msg(void *data)
{
	heartb_timer* hb_ptr = (heartb_timer*)data;
    memset(&msghead,0,sizeof(MsgHead));
    msghead.msg_type = TIMER_MSG;
    msghead.msg_code = HEART_BEAT_BORD_TIMER_MSG;
    msghead.msg_len = sizeof(MsgHead);
    int len = send_msgq(MSGQ_ID_BORD_MGMT,(char *)&msghead, sizeof(msghead), WAIT_FOREVER, 1);
    if(len < 0)
    {
    	log_msg(LOG_ERR, BORD_MGMT, "send massage to massage queue error in function heartb_msg_timer's send_msgq() !\n");
    }
    sbs_timer_schedule(hb_ptr->timer,hb_ptr->seconds,0);
}

/************************************************************************************************

 Description :send query FPGA timeout message to own message queue(callback function)
 input: NULL
 output:NULL
 return: void

 ************************************************************************************************/
void send_query_fpga_timeout_msg(void *data)
{
	query_fpga_timer* ptr = (query_fpga_timer*)data;
	memset(&msghead,0,sizeof(MsgHead));
    msghead.msg_type = TIMER_MSG;
    msghead.msg_code = QUERY_FPGA_CPU_HEAL_TIMER_MSG;
    msghead.msg_len = sizeof(MsgHead);
    int len = send_msgq(MSGQ_ID_BORD_MGMT,(char *)&msghead, sizeof(msghead), WAIT_FOREVER, 1);
	if(len < 0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send massage to massage queue error in function query_fpga_msg_timer's send_msgq()!\n");
	}
   sbs_timer_schedule(ptr->timer, ptr->seconds, 0);
}
