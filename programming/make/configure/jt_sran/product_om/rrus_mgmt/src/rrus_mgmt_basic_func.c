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
 *File name: rru_agent_basic_func.c
 *Description: the basic functions for rrus_mgmt.
 *
 *Current Version: 0.l
 *Author: 
 *Date: 2018.06.05
 *
 ************************************************************************/
#include <netinet/in.h>
#include <time.h>
#include "sbs_type.h"
#include "sbs_log.h"
#include "om_common_h.h"
#include "rrus_mgmt_basic_func.h"
#include "rrus_mgmt_cpri_entity.h"
#include "rrus_mgmt_ir.h"
#include "rrus_mgmt_rru_entity.h"
#include "rrus_mgmt_common.h"

/* Functions ---------------------------------------------------------------- */

/*******************************************************************************
* Show the system time.
*
* Output:  return -1,failed;
*          return 0, success
******************************************************************************/

struct tm* system_time()
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	printf("created timer pointer:%p\n",timeinfo);
    return timeinfo;
}

/*******************************************************************************
* Get the RRU id from Ir msg.
*
* Output:  return -1,failed;
*          return others, success
******************************************************************************/
UINT32 get_rru_id_from_ir(void *msg, UINT32 len)
{
    UINT32 rru_id;

    if (msg == NULL)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Message is NULL pointer in %s.\n", __FUNCTION__);
        return FUNC_ERR;
    }
    IrMsgHead temp_head;
    memcpy(&temp_head, (IrMsgHead*)msg, sizeof(IrMsgHead));
    rru_id = ntohl(temp_head.rru_id);

	return rru_id;
}


/*******************************************************************************
* Get the msg code from Ir msg.
*
* Output:  return -1,failed;
*          return others, success
******************************************************************************/
INT32 get_msg_code_from_ir(void* buf, UINT32 len)
{
	UINT32 msg_code;
    if (buf == NULL)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Message is NULL pointer in %s.\n", __FUNCTION__);
        return FUNC_ERR;
    }
    IrMsgHead temp_head;
    memcpy(&temp_head, buf, sizeof(IrMsgHead));
    msg_code = ntohl(temp_head.msg_code);
    return msg_code;
}

/*******************************************************************************
* Get the RRU event from Ir msg.
*
* Output:  return -1,failed;
*          return others, success
******************************************************************************/
UINT32 get_rru_event_from_ir(void *msg, UINT32 len)
{
    UINT32 msg_code;
    if (msg == NULL)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Message is NULL pointer.\n");
        return FUNC_ERR;
    }
    IrMsgHead temp_head;
    memcpy(&temp_head, (IrMsgHead*)msg, sizeof(IrMsgHead));
    msg_code = ntohl(temp_head.msg_code);

    switch(msg_code)
    {
        case MSG_CODE_RRU_BBU_CHANNEL_BUILD_REQ:
            return EVENT_CHANNEL_BUILD_REQ;

        case MSG_CODE_RRU_BBU_CHANNEL_BUILD_CFG_RSP:
            return EVENT_CHANNEL_BUILD_CFG_RSP;

        case MSG_CODE_RRU_BBU_VER_UPDATE_RESULT_IND:
            return EVENT_VER_UPDATE_RESULT_IND;

        case MSG_CODE_RRU_BBU_VER_QUERY_RSP:
            return EVENT_VER_QUERY_RSP;

//        case MSG_CODE_RRU_BBU_VER_DOWNLOAD_RSP:
//            return EVENT_VER_DOWNLOAD_RSP;

        case MSG_CODE_RRU_BBU_VER_DOWNLOAD_RESULT_IND:
            return EVENT_VER_DOWNLOAD_RESULT_IND;

        case MSG_CODE_RRU_BBU_VER_ACTIVATE_RSP:
            return EVENT_VER_ACTIVATE_RSP;

        case MSG_CODE_RRU_BBU_STATUS_QUERY_RSP:
            return EVENT_STATUS_QUERY_RSP;

        case MSG_CODE_RRU_BBU_PARAM_QUERY_RSP:
            return EVENT_PARAM_QUERY_RSP;

        case MSG_CODE_RRU_BBU_PARAM_CFG_RSP:
            return EVENT_PARAM_CFG_RSP;

        case MSG_CODE_RRU_BBU_INIT_CAL_RESULT_REPORT:
            return EVENT_INIT_CAL_RESULT_REPORT;

        case MSG_CODE_RRU_BBU_TIME_DELAY_MEASURE_RSP:
            return EVENT_TIME_DELAY_MEASURE_RSP;

        case MSG_CODE_RRU_BBU_TIME_DELAY_CFG_RSP:
            return EVENT_TIME_DELAY_CFG_RSP;

        case MSG_CODE_RRU_BBU_ALARM_QUERY_RSP:
            return EVENT_ALARM_QUERY_RSP;

        case MSG_CODE_RRU_BBU_RING_TEST_RSP:
            return EVENT_RING_TEST_RSP;

        case MSG_CODE_RRU_BBU_TDLTE_CELL_CFG_RSP:
            return EVENT_CELL_CFG_RSP;

        default:
            log_msg(LOG_ERR, RRUS_MGMT, "Receive message's code is out of range!\n");
            return FUNC_ERR;
    }

    return FUNC_ERR;
}

/*******************************************************************************
* xx.xx.xx.xx转化�?xxxx"
*
******************************************************************************/
void ip_to_array(UINT8 *ip_to, char *ip)
{
	//xx.xx.xx.xx转化成"xxxx"
	int m, n = 0,len;
	for(m = 0; m< strlen(ip);m++)
	{
		if(ip[m] == '.')
		{
			n++;
		}
	}
	if(n == 3 || n == 5)
	{
		len = n + 1;
	}else{
		log_msg(LOG_ERR, RRUS_MGMT,"Not a vaild address");
		return;
	}
	UINT8 a[len];
	a[0] = atoi(ip);
	int j,k = 0,l = 0;
	for(j = 0; j< strlen(ip);j++)
	{
		if(ip[j] == '.')
		{
			k++;
			l += 1;
			a[l] = atoi(ip+k);
		}else{
			k++;
		}
	}
	memcpy(ip_to, (UINT8 *)a, sizeof(a));
    return;
}

/*******************************************************************************
* 通过rru_id寻找相应entity_id
*
* Output:  return -1,failed;
*          return >0, success
******************************************************************************/
INT32 id_to_entity(UINT32 rru_id)
{
	//通过rru_id寻找相应entity_id
	INT32 i;
	for (i = 0; i < MAX_RRU_NUM; i++)
	{
		if(g_rru_entity_table[i] != NULL)
		{
			if(g_rru_entity_table[i]->logic_info.rru_id == rru_id)
			{
				return i;
			}
		}
	}
    return FUNC_ERR;
}


/*******************************************************************************
* 通过cpri_index寻找相应entity_id
*
* Output:  return -1,failed;
*          return >0, success
******************************************************************************/
INT32 id_to_cpri_entity(UINT16 cpri_index)
{
	//通过cpri_index寻找相应entity_id
	INT32 i;
	for (i = 0; i < MAX_CPRI_NUM; i++)
	{
		if(g_cpri_entity_table[i] != NULL)
		{
			if(g_cpri_entity_table[i]->cpri_index == cpri_index)
			{
				return i;
			}
		}
	}
	return FUNC_ERR;
}



/*******************************************************************************
* Add the Mac head and send through raw socket
*
* Output:  return -1,failed;
*          return >0, success
******************************************************************************/
INT32 send_msg_mac(void* msg, UINT32 msg_len)
{
	char buf[MAX_MSGQ_MSG_LEN];
	memset(buf,0, sizeof(buf));
	CpriMacHead *tmpHead = (CpriMacHead*)malloc(sizeof(CpriMacHead));
	ip_to_array(tmpHead->des_mac,CPRI_MAC);
	ip_to_array(tmpHead->src_mac,OM_MAC);
	tmpHead->pro_type = htons(CPRI_MAC_PROTO);
	memcpy(buf,tmpHead,sizeof(CpriMacHead));
	memcpy(buf+sizeof(CpriMacHead),msg,msg_len);
	if(send_msg_to_inet(MODULE_RRUS_MGMT, 0, cpri_raw_fd, 0, NULL, (char*)buf, sizeof(CpriMacHead)+msg_len) == FUNC_ERR)
	{
		return FUNC_ERR;
	}

	free(tmpHead);
	return FUNC_OK;
}



/*******************************************************************************
* print the ir msg
*
* Output:  return -1,failed;
*          return >0, success
******************************************************************************/
INT8 print_ir_msg(UINT16 msg_code, UINT8* msg_p)
{
	if(msg_p == NULL)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "NULL pointer of it msg.\n");
		return FUNC_ERR;
	}

	IrMsgHead *ir_head = (IrMsgHead*)msg_p;
	printf("/******************************************************************************\n");
	printf("msg_code:%d\n", ntohl(ir_head->msg_code));
	printf("msg_len:%d\n", ntohl(ir_head->msg_len));
	printf("rru_id:%0x\n", ntohl(ir_head->rru_id));
	printf("bbu_id:%0x\n", ntohl(ir_head->bbu_id));
	printf("fiber_port:%d\n", ir_head->fiber_port);
	printf("stream_no:%d\n", ntohl(ir_head->stream_no));

	switch(msg_code)
	{
		case MSG_CODE_RRU_BBU_CHANNEL_BUILD_REQ:
		{
			MSG_RRU_BBU_CHANNEL_BUILD_REQ *msg = (MSG_RRU_BBU_CHANNEL_BUILD_REQ*)msg_p;

			printf("\nie_rru_product_info.ie_flag:%d\n",ntohs(msg->ie_rru_product_info.ie_flag));
			printf("ie_rru_product_info.ie_len:%d\n",ntohs(msg->ie_rru_product_info.ie_len));
			int i = 0;
			printf("ie_rru_product_info.serial_no:");
			for (i; i < 16; i++)
			{
				printf("%c",msg->ie_rru_product_info.serial_no[i]);
			}
			printf("\n");
			//printf("ie_rru_product_info.serial_no:%d\n",msg->ie_rru_product_info.serial_no);

			printf("\nie_channel_build_reason.ie_flag:%d\n",ntohs(msg->ie_channel_build_reason.ie_flag));
			printf("ie_channel_build_reason.ie_len:%d\n",ntohs(msg->ie_channel_build_reason.ie_len));
			printf("ie_channel_build_reason.reason:%d\n",msg->ie_channel_build_reason.reason);
			printf("ie_channel_build_reason.alarm_code:%d\n",ntohl(msg->ie_channel_build_reason.alarm_code));

			printf("\nie_rru_ability.ie_flag:%d\n",ntohs(msg->ie_rru_ability.ie_flag));
			printf("ie_rru_ability.ie_len:%d\n",ntohs(msg->ie_rru_ability.ie_len));
			printf("ie_rru_ability.td_carrier_num:%d\n",ntohl(msg->ie_rru_ability.td_carrier_num));
			printf("ie_rru_ability.lte_carrier_num:%d\n",ntohl(msg->ie_rru_ability.lte_carrier_num));

			printf("\nie_rru_hard_type_ver_info.ie_flag:%d\n",ntohs(msg->ie_rru_hard_type_ver_info.ie_flag));
			printf("ie_rru_hard_type_ver_info.ie_len:%d\n",ntohs(msg->ie_rru_hard_type_ver_info.ie_len));

			printf("\nie_rru_soft_ver_info.ie_flag:%d\n",ntohs(msg->ie_rru_soft_ver_info.ie_flag));
			printf("ie_rru_soft_ver_info.ie_len:%d\n",ntohs(msg->ie_rru_soft_ver_info.ie_len));

			break;
		}

		case MSG_CODE_RRU_BBU_CHANNEL_BUILD_CFG_RSP:
		{
			MSG_RRU_BBU_CHANNEL_BUILD_CFG_RSP *msg = (MSG_RRU_BBU_CHANNEL_BUILD_CFG_RSP*)msg_p;

			printf("\nie_channel_build_cfg_ret.ie_flag:%d\n",ntohs(msg->ie_channel_build_cfg_ret.ie_flag));
			printf("ie_channel_build_cfg_ret.ie_len:%d\n",ntohs(msg->ie_channel_build_cfg_ret.ie_len));
			printf("ie_channel_build_cfg_ret.cfg_result:%d\n",ntohl(msg->ie_channel_build_cfg_ret.cfg_result));
			break;
		}

		case MSG_CODE_RRU_BBU_PARAM_CFG_RSP:
		{
			MSG_RRU_BBU_PARAM_CFG_RSP *msg = (MSG_RRU_BBU_PARAM_CFG_RSP*)msg_p;

			printf("\nie_system_time_rsp.ie_flag:%d\n",ntohs(msg->ie_system_time_rsp.ie_flag));
			printf("ie_system_time_rsp.ie_len:%d\n",ntohs(msg->ie_system_time_rsp.ie_len));
			printf("ie_system_time_rsp.result:%d\n",ntohl(msg->ie_system_time_rsp.result));

			printf("\nie_cpu_usage_check_cycle_cfg_rsp.ie_flag:%d\n",ntohs(msg->ie_cpu_usage_check_cycle_cfg_rsp.ie_flag));
			printf("ie_cpu_usage_check_cycle_cfg_rsp.ie_len:%d\n",ntohs(msg->ie_cpu_usage_check_cycle_cfg_rsp.ie_len));
			printf("ie_cpu_usage_check_cycle_cfg_rsp.result:%d\n",ntohl(msg->ie_cpu_usage_check_cycle_cfg_rsp.result));

			printf("\nie_vswr_thres_cfg_rsp.ie_flag:%d\n",ntohs(msg->ie_vswr_thres_cfg_rsp.ie_flag));
			printf("ie_vswr_thres_cfg_rsp.ie_len:%d\n",ntohs(msg->ie_vswr_thres_cfg_rsp.ie_len));
			printf("ie_vswr_thres_cfg_rsp.result:%d\n",ntohl(msg->ie_vswr_thres_cfg_rsp.result));

			printf("\nie_overtemp_thres_cfg_rsp.ie_flag:%d\n",ntohs(msg->ie_overtemp_thres_cfg_rsp.ie_flag));
			printf("ie_overtemp_thres_cfg_rsp.ie_len:%d\n",ntohs(msg->ie_overtemp_thres_cfg_rsp.ie_len));
			printf("ie_overtemp_thres_cfg_rsp.result:%d\n",ntohl(msg->ie_overtemp_thres_cfg_rsp.result));

			printf("\nie_antenna_cfg_rsp.ie_flag:%d\n",ntohs(msg->ie_antenna_cfg_rsp.ie_flag));
			printf("ie_antenna_cfg_rsp.ie_len:%d\n",ntohs(msg->ie_antenna_cfg_rsp.ie_len));
			printf("ie_antenna_cfg_rsp.result:%d\n",ntohl(msg->ie_antenna_cfg_rsp.result));

			printf("\nie_rf_path_state_cfg_rsp.ie_flag:%d\n",ntohs(msg->ie_rf_path_state_cfg_rsp.ie_flag));
			printf("ie_rf_path_state_cfg_rsp.ie_len:%d\n",ntohs(msg->ie_rf_path_state_cfg_rsp.ie_len));
			printf("ie_rf_path_state_cfg_rsp.result:%d\n",ntohl(msg->ie_rf_path_state_cfg_rsp.result));

			break;
		}

		case MSG_CODE_RRU_BBU_TIME_DELAY_MEASURE_RSP:
		{
			MSG_RRU_BBU_TIME_DELAY_MEASURE_RSP *msg = (MSG_RRU_BBU_TIME_DELAY_MEASURE_RSP*)msg_p;

			printf("\nie_time_delay_rsp.ie_flag:%d\n",ntohs(msg->ie_time_delay_rsp.ie_flag));
			printf("ie_time_delay_rsp.ie_len:%d\n",ntohs(msg->ie_time_delay_rsp.ie_len));
			printf("ie_time_delay_rsp.fiber_no:%d\n",msg->ie_time_delay_rsp.fiber_no);
			printf("ie_time_delay_rsp.t_offset:%d\n",ntohl(msg->ie_time_delay_rsp.t_offset));
			printf("ie_time_delay_rsp.tb_delay_dl:%d\n",ntohl(msg->ie_time_delay_rsp.tb_delay_dl));
			printf("ie_time_delay_rsp.tb_delay_ul:%d\n",ntohl(msg->ie_time_delay_rsp.tb_delay_ul));
			printf("ie_time_delay_rsp.t2a:%d\n",ntohl(msg->ie_time_delay_rsp.t2a));
			printf("ie_time_delay_rsp.ta3:%d\n",ntohl(msg->ie_time_delay_rsp.ta3));
			printf("ie_time_delay_rsp.n_frame:%d\n",ntohl(msg->ie_time_delay_rsp.n_frame));
			break;
		}

		case MSG_CODE_RRU_BBU_TIME_DELAY_CFG_RSP:
		{
			MSG_RRU_BBU_TIME_DELAY_CFG_RSP *msg = (MSG_RRU_BBU_TIME_DELAY_CFG_RSP*)msg_p;

			printf("\nie_time_delay_cfg_rsp.ie_flag:%d\n",ntohs(msg->ie_time_delay_cfg_rsp.ie_flag));
			printf("ie_time_delay_cfg_rsp.ie_len:%d\n",ntohs(msg->ie_time_delay_cfg_rsp.ie_len));
//			printf("ie_time_delay_cfg_rsp.fiber_no:%d\n",msg->ie_time_delay_cfg_rsp.fiber_no);
			printf("ie_time_delay_cfg_rsp.cfg_result:%d\n",ntohl(msg->ie_time_delay_cfg_rsp.cfg_result));

			break;
		}

		case MSG_CODE_RRU_BBU_TDLTE_CELL_CFG_RSP:
		{
			MSG_RRU_BBU_CELL_CFG_RSP_BUILD *msg = (MSG_RRU_BBU_CELL_CFG_RSP_BUILD*)msg_p;

			printf("\nie_cell_cfg_rsp.ie_flag:%d\n",ntohs(msg->ie_cell_cfg_rsp.ie_flag));
			printf("ie_cell_cfg_rsp.ie_len:%d\n",ntohs(msg->ie_cell_cfg_rsp.ie_len));
			printf("ie_cell_cfg_rsp.local_cell_id:%d\n",ntohl(msg->ie_cell_cfg_rsp.local_cell_id));
			printf("ie_cell_cfg_rsp.result:%d\n",ntohl(msg->ie_cell_cfg_rsp.result));

			printf("\nie_freq_cfg_rsp.ie_flag:%d\n",ntohs(msg->ie_freq_cfg_rsp.ie_flag));
			printf("ie_freq_cfg_rsp.ie_len:%d\n",ntohs(msg->ie_freq_cfg_rsp.ie_len));
			printf("ie_freq_cfg_rsp.local_cell_id:%d\n",ntohl(msg->ie_freq_cfg_rsp.local_cell_id));
			printf("ie_freq_cfg_rsp.freq_cfg_cmd:%d\n",msg->ie_freq_cfg_rsp.freq_cfg_cmd);
			printf("ie_freq_cfg_rsp.carrier_no:%d\n",msg->ie_freq_cfg_rsp.carrier_no);
			printf("ie_cell_cfg_rsp.result:%d\n",ntohl(msg->ie_cell_cfg_rsp.result));

			break;
		}

		case MSG_CODE_RRU_BBU_STATUS_QUERY_RSP:
		{
			MSG_RRU_BBU_STATUS_QUERY_RSP *msg = (MSG_RRU_BBU_STATUS_QUERY_RSP*)msg_p;

			printf("\nie_rf_path_state_query_rsp.ie_flag:%d\n",ntohs(msg->ie_rf_path_state_query_rsp.ie_flag));
			printf("ie_rf_path_state_query_rsp.ie_len:%d\n",ntohs(msg->ie_rf_path_state_query_rsp.ie_len));
			printf("ie_rf_path_state_query_rsp.rf_path_no:%d\n",msg->ie_rf_path_state_query_rsp.rf_path_no);
			printf("ie_rf_path_state_query_rsp.up_path_state:%d\n",ntohl(msg->ie_rf_path_state_query_rsp.up_path_state));
			printf("ie_rf_path_state_query_rsp.dw_path_state:%d\n",ntohl(msg->ie_rf_path_state_query_rsp.dw_path_state));

			printf("\nie_system_clock_state_rsp.ie_flag:%d\n",ntohs(msg->ie_system_clock_state_rsp.ie_flag));
			printf("ie_system_clock_state_rsp.ie_len:%d\n",ntohs(msg->ie_system_clock_state_rsp.ie_len));
			printf("ie_system_clock_state_rsp.state:%d\n",ntohl(msg->ie_system_clock_state_rsp.state));

			break;
		}

		case MSG_CODE_RRU_BBU_PARAM_QUERY_RSP:
		{
			MSG_RRU_BBU_PARAM_QUERY_RSP *msg = (MSG_RRU_BBU_PARAM_QUERY_RSP*)msg_p;

			printf("\nie_system_time.ie_flag:%d\n",ntohs(msg->ie_system_time.ie_flag));
			printf("ie_system_time.ie_len:%d\n",ntohs(msg->ie_system_time.ie_len));
			printf("ie_system_time.year:%d\n",ntohs(msg->ie_system_time.year));

			printf("\nie_cpu_usage_rsp.ie_flag:%d\n",ntohs(msg->ie_cpu_usage_rsp.ie_flag));
			printf("ie_cpu_usage_rsp.ie_len:%d\n",ntohs(msg->ie_cpu_usage_rsp.ie_len));
			printf("ie_cpu_usage_rsp.cpu_usage:%d\n",ntohl(msg->ie_cpu_usage_rsp.cpu_usage));

			printf("\nie_cpu_usage_check_cycle_rsp.ie_flag:%d\n",ntohs(msg->ie_cpu_usage_check_cycle_rsp.ie_flag));
			printf("ie_cpu_usage_check_cycle_rsp.ie_len:%d\n",ntohs(msg->ie_cpu_usage_check_cycle_rsp.ie_len));
			printf("ie_cpu_usage_check_cycle_rsp.period:%d\n",ntohl(msg->ie_cpu_usage_check_cycle_rsp.period));

			printf("\nie_rru_temp_rsp.ie_flag:%d\n",ntohs(msg->ie_rru_temp_rsp.ie_flag));
			printf("ie_rru_temp_rsp.ie_len:%d\n",ntohs(msg->ie_cpu_usage_check_cycle_rsp.ie_len));
			printf("ie_rru_temp_rsp.rf_path_no:%d\n",msg->ie_rru_temp_rsp.rf_path_no);
			printf("ie_rru_temp_rsp.temp_type:%d\n",ntohl(msg->ie_rru_temp_rsp.temp_type));
			printf("ie_rru_temp_rsp.temp_val:%d\n",ntohl(msg->ie_rru_temp_rsp.temp_val));

			printf("\nie_vswr_state_rsp.ie_flag:%d\n",ntohs(msg->ie_vswr_state_rsp.ie_flag));
			printf("ie_vswr_state_rsp.ie_len:%d\n",ntohs(msg->ie_vswr_state_rsp.ie_len));
			printf("ie_vswr_state_rsp.rf_path_no:%d\n",msg->ie_vswr_state_rsp.rf_path_no);
			printf("ie_vswr_state_rsp.vswr_val:%d\n",ntohl(msg->ie_vswr_state_rsp.vswr_val));

			printf("\nie_vswr_thres_rsp.ie_flag:%d\n",ntohs(msg->ie_vswr_thres_rsp.ie_flag));
			printf("ie_vswr_thres_rsp.ie_len:%d\n",ntohs(msg->ie_vswr_thres_rsp.ie_len));
			printf("ie_vswr_thres_rsp.vswr_thres1:%d\n",ntohl(msg->ie_vswr_thres_rsp.vswr_thres1));
			printf("ie_vswr_thres_rsp.vswr_thres2:%d\n",ntohl(msg->ie_vswr_thres_rsp.vswr_thres2));

			printf("\nie_overtemp_thres_rsp.ie_flag:%d\n",ntohs(msg->ie_overtemp_thres_rsp.ie_flag));
			printf("ie_overtemp_thres_rsp.ie_len:%d\n",ntohs(msg->ie_overtemp_thres_rsp.ie_len));
			printf("ie_overtemp_thres_rsp.low_thres:%d\n",ntohl(msg->ie_overtemp_thres_rsp.low_thres));
			printf("ie_overtemp_thres_rsp.up_thres:%d\n",ntohl(msg->ie_overtemp_thres_rsp.up_thres));

			printf("\nie_output_power_rsp.ie_flag:%d\n",ntohs(msg->ie_output_power_rsp.ie_flag));
			printf("ie_output_power_rsp.ie_len:%d\n",ntohs(msg->ie_output_power_rsp.ie_len));
			printf("ie_output_power_rsp.rf_path_no:%d\n",msg->ie_output_power_rsp.rf_path_no);
			printf("ie_output_power_rsp.power_val:%d\n",ntohs(msg->ie_output_power_rsp.power_val));

			printf("\nie_state_machine_rsp.ie_flag:%d\n",ntohs(msg->ie_state_machine_rsp.ie_flag));
			printf("ie_state_machine_rsp.ie_len:%d\n",ntohs(msg->ie_state_machine_rsp.ie_len));
			printf("ie_state_machine_rsp.state:%d\n",msg->ie_state_machine_rsp.state);
			break;
		}
		default:
			log_msg(LOG_ERR, RRUS_MGMT, "invalid msg_code for print_ir_msg.\n");
			return FUNC_ERR;
	}
	printf("******************************************************************************/\n");

	return FUNC_OK;
}




/*******************************************************************************
* process the ir message--parameter query response
*
* Output:  return -1,failed;
*          return >0, success
******************************************************************************/
INT8 ie_query_rsp(INT32 entity_id, UINT16 ie_flag, void* msg)
{
	switch(ie_flag)
	{
		case IE_FLAG_SYS_TIME:
		{
			IE_SYSTEM_TIME *p = (IE_SYSTEM_TIME*)msg;
//			p->year;
			break;
		}
		case IE_FLAG_CPU_USAGE_QUERY_RSP:
		{
			IE_CPU_USAGE_RSP *p = (IE_CPU_USAGE_RSP*)msg;
			g_rru_entity_table[entity_id]->com_info.rru_param_info.cpu_usage = p->cpu_usage;
			break;
		}
		case IE_FLAG_CPU_USAGE_PERIOD_QUERY_RSP:
		{
			IE_CPU_USAGE_CHECK_CYCLE_RSP *p = (IE_CPU_USAGE_CHECK_CYCLE_RSP*)msg;
			g_rru_entity_table[entity_id]->com_info.rru_param_info.period = p->period;
			break;
		}
		case IE_FLAG_RRU_TEMPERATUR_QUERY_RSP:
		{
			IE_RRU_TEMP_RSP *p = (IE_RRU_TEMP_RSP*)msg;
			g_rru_entity_table[entity_id]->com_info.rru_param_info.temp_val[p->rf_path_no] = p->temp_type;
			break;
		}
		case IE_FLAG_VSWR_STATE_QUERY_RSP:
		{
			IE_VSWR_STATE_RSP *p = (IE_VSWR_STATE_RSP*)msg;
			g_rru_entity_table[entity_id]->com_info.rru_param_info.vswr_val[p->rf_path_no] = p->vswr_val;
			break;
		}
		case IE_FLAG_VSWR_THRESHOLD_QUERY_RSP:
		{
			IE_VSWR_THRES_RSP *p = (IE_VSWR_THRES_RSP*)msg;
			g_rru_entity_table[entity_id]->com_info.rru_param_info.vswr_thres1 = p->vswr_thres1;
			g_rru_entity_table[entity_id]->com_info.rru_param_info.vswr_thres2 = p->vswr_thres2;
			break;
		}
		case IE_FLAG_TEMP_THRESHOLD_QUERY_RSP:
		{
			IE_OVERTEMP_THRES_RSP *p = (IE_OVERTEMP_THRES_RSP*)msg;
			g_rru_entity_table[entity_id]->com_info.rru_param_info.up_thres = p->up_thres;
			g_rru_entity_table[entity_id]->com_info.rru_param_info.low_thres = p->low_thres;
			break;
		}
		case IE_FLAG_OUTPUT_POWER_QUERY_RSP:
		{
			IE_OUTPUT_POWER_RSP *p = (IE_OUTPUT_POWER_RSP*)msg;
			g_rru_entity_table[entity_id]->com_info.rru_param_info.power_val[p->rf_path_no] = p->power_val;
			break;
		}
		case IE_FLAG_STATE_MACHINE_QUERY_RSP:
		{
			IE_STATE_MACHINE_RSP *p = (IE_STATE_MACHINE_RSP*)msg;
			g_rru_entity_table[entity_id]->com_info.rru_param_info.state = p->state;
			break;
		}
		default:
			log_msg(LOG_ERR, RRUS_MGMT, "invalid ie flag for parameter query rsp.\n");
			return FUNC_ERR;
	}
	return FUNC_OK;
}

/*******************************************************************************
* return the ie flag of ir message--parameter configure response
*
* Output:  return -1,failed;
*          return >0, success
******************************************************************************/
INT8 ie_cfg_rsp(UINT16 ie_flag, void* msg)
{
	switch(ie_flag)
	{
		case IE_FLAG_CPU_USAGE_PERIOD_CFG_RSP:
		{
			IE_CPU_USAGE_CHECK_CYCLE_CFG_RSP *p = (IE_CPU_USAGE_CHECK_CYCLE_CFG_RSP*)msg;
			return ntohl(p->result);
		}
		case IE_FLAG_VSWR_THRESHOLD_CFG_RSP:
		{
			IE_VSWR_THRES_CFG_RSP *p = (IE_VSWR_THRES_CFG_RSP*)msg;
			return ntohl(p->result);
		}
		case IE_FLAG_TEMP_THRESHOLD_CFG_RSP:
		{
			IE_OVERTEMP_THRES_CFG_RSP *p = (IE_OVERTEMP_THRES_CFG_RSP*)malloc(sizeof(IE_OVERTEMP_THRES_CFG_RSP));
			return ntohl(p->result);
		}
		case IE_FLAG_ANT_CFG_RSP:
		{
			IE_ANTENNA_CFG_RSP *p = (IE_ANTENNA_CFG_RSP*)malloc(sizeof(IE_ANTENNA_CFG_RSP));
			return ntohl(p->result);
		}
		case IE_FLAG_RF_PATH_STATE_CFG_RSP:
		{
			IE_RF_PATH_STATE_CFG_RSP *p = (IE_RF_PATH_STATE_CFG_RSP*)malloc(sizeof(IE_RF_PATH_STATE_CFG_RSP));
			return ntohl(p->result);
		}
		default:
			log_msg(LOG_ERR, RRUS_MGMT, "invalid ie flag for parameter cfg rsp.\n");
			return FUNC_ERR;
	}

	return FUNC_OK;
}
