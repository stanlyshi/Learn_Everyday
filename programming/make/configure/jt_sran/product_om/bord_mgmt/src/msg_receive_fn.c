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
 * msg_receive_fn.c
 *
 *  Created on: Aug 26, 2018
 *      Author: root
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <assert.h>
#include <arpa/inet.h>
#include "sbs_msgq.h"
#include "sbs_log.h"
#include "om_common_h.h"
#include "mnt_mgmt_api.h"
#include "mnt_mgmt_init.h"
#include "mnt_mgmt_msg_handle.h"
#include "msg_receive_fn.h"
#include "fpga_msg_receive_fn.h"
void proc_fpga_msg(INT8 *arg1)
{
switch(ntohs(*(UINT16 *)(arg1+2)))
{
case OM_RX_BORD_MGMT_TIME_SYNC_REPORT:
    handle_fn_time_sync_report_req(arg1);
    break;
case OM_RX_BORD_MGMT_HEAL_THRES_QUERY_RSP:
    handle_fpga_heal_thres_query_rsp(arg1);
    break;

case OM_RX_BORD_MGMT_HEAL_INFO_QUERY_RSP:
    handle_fn_fpga_heal_info_query_rsp(arg1);
    break;
case OM_RX_BORD_MGMT_TEMP_THRES_CFG_RSP:
    handle_fn_fpga_temp_thres_cfg_rsp(arg1);
    break;
case OM_RX_BORD_MGMT_VOL_THRES_CFG_RSP:
    handle_fn_fpga_vol_thres_cfg_rsp(arg1);
    break;
case OM_RX_BORD_MGMT_FAN_THRES_CFG_RSP:
    handle_fn_fpga_fan_thres_cfg_rsp(arg1);
    break;
case OM_RX_BORD_MGMT_GPS_CFG_RSP:
    handle_fn_gps_cfg_rsp(arg1);
    break;
case OM_RX_BORD_MGMT_CLOCK_SOURCE_CFG_RSP:
    handle_fn_clock_source_cfg_rsp(arg1);
    break;
case OM_RX_BORD_MGMT_FW_UPDATE_RSP:
    //handle_fn_fpga1_fw_update_rsp(arg1);
    break;
case OM_RX_BORD_MGMT_CHANNEL_MONITOR:
    handle_fn_fpga1_heartbeat_msg(arg1);//fpga阈值查询请求发送
    break;
case OM_RX_BORD_MGMT_BOARD_POWER_RSP:
    handle_fn_fpga_board_power_rsp(arg1);
    break;
case OM_RX_BORD_MGMT_CHIP_POWER_RSP:
    handle_fn_fpga_chip_power_rsp(arg1);
    break;
default:
	printf("ERROR:fpga send msgcode invalid!\n");
	break;
}

}

void proc_timer_msg(INT8 *arg2)
{
	switch(*(UINT16 *)(arg2+2))
	{
	 case HEART_BEAT_BORD_TIMER_MSG:
	         handle_fn_heartb_timer_msg();
	         break;
	 case QUERY_FPGA_CPU_HEAL_TIMER_MSG:
	         handle_fn_query_fpga_cpu_heal_info_timer_msg();
	         break;
	 default:
		     log_msg(LOG_ERR, BORD_MGMT, "timer massage code invalid!\n");
	         break;
    }

}


void proc_cfg_msg(INT8 *arg3)
{

	switch(*(UINT16 *)(arg3 + 2))
	{
	case CFG_MGMT_BORD_MGMT_FPGA_FAN_THRES_CFG_REQ:
		handle_fn_fpga_fan_thres_cfg_req(arg3);
		break;
	case CFG_MGM_BORD_MGMT_FPGA_TEMP_THRES_CFG_REQ:
		handle_fn_fpga_temp_thres_cfg_req(arg3);
		break;
	case CFG_MGMT_BORD_MGMT_FPGA_VOL_THRES_CFG_REQ:
		handle_fn_fpga_vol_thres_cfg_req(arg3);
		break;
	case CFG_MGMT_BORD_MGMT_CPU_FAN_THRES_CFG_REQ:
		handle_fn_cpu_fan_thres_cfg_req(arg3);
		break;
	case CFG_MGMT_BORD_MGMT_CPU_VOL_THRES_CFG_REQ:
		handle_fn_cpu_vol_thres_cfg_req(arg3);
		break;
	case CFG_MGMT_BORD_MGMT_CPU_TEMP_THRES_CFG_REQ:
		handle_fn_cpu_temp_thres_cfg_req(arg3);
		break;
	case CFG_MGMT_BORD_MGMT_CPU_USG_THRES_CFG_REQ:
		handle_fn_cpu_usg_thres_cfg_req(arg3);
		break;
	case CFG_MGMT_BORD_MGMT_MEM_USG_THRES_CFG_REQ:
		handle_fn_mem_usg_thres_cfg_req(arg3);
		break;
	case CFG_MGMT_BORD_MGMT_FPGA_GPS_CFG_REQ:
		handle_fn_fpga_gps_cfg_req(arg3);
		break;
	case CFG_MGMT_BORD_MGMT_FPGA_CLOCK_SOURCE_CFG_REQ:
		handle_fn_fpga_clc_source_cfg_req(arg3);
		break;
	case CFG_MGMT_BORD_MGMT_FPGA_BOARD_POWER_CFG_REQ:
		handle_fn_fpga_brd_power_cfg_req(arg3);
		break;
	case CFG_MGMT_BORD_MGMT_FPGA_CHIP_POWER_CFG_REQ:
		handle_fn_fpga_chip_power_cfg_req(arg3);
		break;
	default:
		printf("ERROR:configuration massage code is invalid\n");
		break;
	}
}


void *bord_msg_receive_fn(void)
{
	INT8 buffer[MAX_MSGQ_MSG_LEN];
	INT32 len;

    while(1)
    {
    	sleep(1);
    	bzero(buffer,MAX_MSGQ_MSG_LEN);
    	len = receive_msgq(MSGQ_ID_BORD_MGMT,buffer,MAX_MSGQ_MSG_LEN,WAIT_FOREVER);

    	printf("len = %d.\n", len);
    	//perror("receive msgq");
    	if (len == -1)
    	{
    		log_msg(LOG_ERR, BORD_MGMT, "BORD_MGMT's massage queue receive massage error!\n");
    	}
    	else if(len == 0)
		{
		   continue;
		}
    	else
    	{
    		pthread_rwlock_wrlock(&brd_mnt_rwlock); //write lock
			if(ntohs(*(UINT16 *)(buffer+14)) == CTRL_FPGA_BORD_MGMT && ntohs(*(UINT16 *)(buffer+16)) == OM_RX_BORD_MGMT_BROADCAST_INFO_REQ) //fpga broadcast message
			{
				handle_fn_fpga_broadcast_info_req(buffer);
			}
			else if(ntohs(*(UINT16 *)(buffer+14)) == CTRL_FPGA_BORD_MGMT && ntohs(*(UINT16 *)(buffer+16)) == OM_RX_BORD_MGMT_DATA_CHANNEL_MONITOR)
			{
				handle_fn_data_fpga_heartbeat_msg(buffer); //处理data_fpga的心跳消息
			}
			else if(ntohs(*(UINT16 *)(buffer+14)) == CTRL_FPGA_BORD_MGMT && ntohs(*(UINT16 *)(buffer+16)) == OM_RX_BORD_MGMT_FPGA_IP_CFG_RSP)
			{
				handle_fn_om_udp_msg(buffer); //
			}
			else if(ntohs(*(UINT16 *)(buffer+14)) == MSG_TYPE_DATA_FPGA_BORD_MGMT && ntohs(*(UINT16 *)(buffer+16)) == DATA_FPGA_BORD_MGMT_FRNUM_RSP)
			{
				handle_fn_frame_info_report_cfg_rsp(buffer);
			}
			else
			{
				if(ntohs(*(UINT16 *)buffer) == CTRL_FPGA_BORD_MGMT)
				{
					proc_fpga_msg(buffer);
				}
				if(*(UINT16 *)buffer == TIMER_MSG)
				{
					proc_timer_msg(buffer);
				}
				if(*(UINT16 *)buffer == CFG_MGMT_BORD_MGMT)
				{
					proc_cfg_msg(buffer);
				}
			}

			pthread_rwlock_unlock(&brd_mnt_rwlock); //free lock
			}
    }
    close_msgq(BORD_MGMT_MSGQ_NAME,MSGQ_ID_BORD_MGMT);
    return (void *)NULL;
}
