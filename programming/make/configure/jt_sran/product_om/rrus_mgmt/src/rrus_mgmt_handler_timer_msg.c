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
 *File name: rrus_mgmt_handler_timer_msg.c
 *Description: All handlers of messages from timer.
 *
 *Current Version: 0.l
 *Author: 
 *Date: 2018.08.26
 *
 ************************************************************************/
#include<arpa/inet.h>
#include "sbs_type.h"
#include "sbs_log.h"
#include "om_common_h.h"
#include "rrus_mgmt_api_timer.h"
#include "rrus_mgmt_ir.h"
#include "rrus_mgmt_basic_func.h"

struct sbs_timer_list* bbu_hub;
struct sbs_timer_list* rru_hub_timeout[MAX_RRU_NUM];
struct sbs_timer_list* rru_cell_cfg_rsp_timeout[MAX_RRU_NUM];
struct sbs_timer_list* rru_normal_msg_rsp_timeout[MAX_RRU_NUM];
struct sbs_timer_list* rru_param_query[MAX_RRU_NUM];
struct sbs_timer_list* rru_state_query[MAX_RRU_NUM];
struct sbs_timer_list* cpri_state_query[MAX_CPRI_NUM];
struct sbs_timer_list* cpri_msg_rsp_timeout[MAX_CPRI_NUM];

extern INT32 send_msg_to_inet(UINT16 module_id,UINT32 streamflag,INT32 fd,UINT32 ip,struct sockaddr *clientaddr,char *msg_p,UINT32 msg_len);

extern INT32 send_msg_to_om(UINT16 module_id, UINT8 *msg_p, UINT32 msg_len);

extern INT32 handle_msg_rru_reg(UINT32 rru_id, UINT32 ind);

INT32 timer_cb_send_bbu_hub_msg_to_rrus_mgmt()
{
	BBU_HUB *timer = (BBU_HUB*)malloc(sizeof(BBU_HUB));
	timer->head.msg_type = MODULE_TIMER;
	timer->head.msg_code = CODE_TIMER_BBU_HUB;
	timer->head.msg_len = sizeof(BBU_HUB);
	if(send_msg_to_om(MODULE_RRUS_MGMT, (UINT8*)timer, sizeof(BBU_HUB)) == FUNC_ERR)
	{
		log_msg(LOG_INFO, RRUS_MGMT, "cd to rru mgmt failed\n");

	}
	free(timer);
    return FUNC_OK;
}

INT32 timer_cb_send_rru_hub_timeout_to_rrus_mgmt(UINT32 *rru_id)
{
	RRU_HUB_TIMEOUT *timer = (RRU_HUB_TIMEOUT*)malloc(sizeof(RRU_HUB_TIMEOUT));
	timer->head.msg_type = MODULE_TIMER;
	timer->head.msg_code = CODE_TIMER_RRU_HUB_TIMEOUT;
	timer->head.msg_len = sizeof(RRU_HUB_TIMEOUT);
	timer->rru_id = *rru_id;

	if(send_msg_to_om(MODULE_RRUS_MGMT, (UINT8*)timer, sizeof(RRU_HUB_TIMEOUT)) == FUNC_ERR)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "cd to rru mgmt failed\n");

	}
	free(timer);
	return FUNC_OK;
}

INT32 timer_cb_send_rru_cell_cfg_rsp_timeout_to_rrus_mgmt(UINT32 *rru_id)
{
	RRU_CELL_CFG_RSP_TIMEOUT *timer = (RRU_CELL_CFG_RSP_TIMEOUT*)malloc(sizeof(RRU_CELL_CFG_RSP_TIMEOUT));
	timer->head.msg_type = MODULE_TIMER;
	timer->head.msg_code = CODE_TIMER_RRU_CELL_CFG_RSP_TIMEOUT;
	timer->head.msg_len = sizeof(RRU_CELL_CFG_RSP_TIMEOUT);
	timer->rru_id = *rru_id;
	if(send_msg_to_om(MODULE_RRUS_MGMT, (UINT8*)timer, sizeof(RRU_CELL_CFG_RSP_TIMEOUT)) == FUNC_ERR)
	{
		log_msg(LOG_INFO, RRUS_MGMT, "cd to rru mgmt failed\n");

	}

	free(timer);
    return FUNC_OK;
}

INT32 timer_cb_send_rru_normal_msg_rsp_timeout_to_rrus_mgmt(UINT32 *rru_id)
{
	RRU_NORMAL_MSG_RSP_TIMEOUT *timer = (RRU_NORMAL_MSG_RSP_TIMEOUT*)malloc(sizeof(RRU_NORMAL_MSG_RSP_TIMEOUT));
	timer->head.msg_type = MODULE_TIMER;
	timer->head.msg_code = CODE_TIMER_RRU_CELL_CFG_RSP_TIMEOUT;
	timer->head.msg_len = sizeof(RRU_NORMAL_MSG_RSP_TIMEOUT);
	timer->rru_id = *rru_id;
	if(send_msg_to_om(MODULE_RRUS_MGMT, (UINT8*)timer, sizeof(RRU_NORMAL_MSG_RSP_TIMEOUT)) == FUNC_ERR)
	{
		log_msg(LOG_INFO, RRUS_MGMT, "cd to rru mgmt failed\n");

	}
	free(timer);
    return FUNC_OK;
}

INT32 timer_cb_send_rru_param_query_to_rrus_mgmt(UINT32 *rru_id)
{
	RRU_PARAM_QUERY *timer = (RRU_PARAM_QUERY*)malloc(sizeof(RRU_PARAM_QUERY));
	timer->head.msg_type = MODULE_TIMER;
	timer->head.msg_code = CODE_TIMER_RRU_PARAM_QUERY;
	timer->head.msg_len = sizeof(RRU_PARAM_QUERY);
	timer->rru_id = *rru_id;
	if(send_msg_to_om(MODULE_RRUS_MGMT, (UINT8*)timer, sizeof(RRU_PARAM_QUERY)) == FUNC_ERR)
	{
		log_msg(LOG_INFO, RRUS_MGMT, "cd to rru mgmt failed\n");

	}
	free(timer);
    return FUNC_OK;
}

INT32 timer_cb_send_rru_state_query_to_rrus_mgmt(UINT32 *rru_id)
{
	RRU_STATE_QUERY *timer = (RRU_STATE_QUERY*)malloc(sizeof(RRU_STATE_QUERY));
	timer->head.msg_type = MODULE_TIMER;
	timer->head.msg_code = CODE_TIMER_RRU_STATE_QUERY;
	timer->head.msg_len = sizeof(RRU_STATE_QUERY);
	timer->rru_id = *rru_id;
	if(send_msg_to_om(MODULE_RRUS_MGMT, (UINT8*)timer, sizeof(RRU_STATE_QUERY)) == FUNC_ERR)
	{
		log_msg(LOG_INFO, RRUS_MGMT, "cd to rru mgmt failed\n");

	}
	free(timer);
    return FUNC_OK;
}

INT32 timer_cb_send_cpri_state_query_to_rrus_mgmt(UINT16 *cpri_id)
{
	TIMER_CPRI_STATE_QUERY *timer = (TIMER_CPRI_STATE_QUERY*)malloc(sizeof(TIMER_CPRI_STATE_QUERY));
	timer->head.msg_type = MODULE_TIMER;
	timer->head.msg_code = CODE_TIMER_CPRI_STATE_QUERY;
	timer->head.msg_len = sizeof(TIMER_CPRI_STATE_QUERY);
	timer->cpri_index = *cpri_id;
	if(send_msg_to_om(MODULE_RRUS_MGMT, (UINT8*)timer, sizeof(TIMER_CPRI_STATE_QUERY)) == FUNC_ERR)
	{
		log_msg(LOG_INFO, RRUS_MGMT, "cd to rru mgmt failed\n");

	}
	free(timer);
    return FUNC_OK;
}

INT32 timer_cb_send_cpri_msg_rsp_timeout_to_rrus_mgmt(UINT16 *cpri_id)
{
	CPRI_MSG_RSP_TIMEOUT *timer = (CPRI_MSG_RSP_TIMEOUT*)malloc(sizeof(CPRI_MSG_RSP_TIMEOUT));
	timer->head.msg_type = MODULE_TIMER;
	timer->head.msg_code = CODE_TIMER_CPRI_MSG_RSP_TIMEOUT;
	timer->head.msg_len = sizeof(CPRI_MSG_RSP_TIMEOUT);
	timer->cpri_index = *cpri_id;
	if(send_msg_to_om(MODULE_RRUS_MGMT, (UINT8*)timer, sizeof(CPRI_MSG_RSP_TIMEOUT)) == FUNC_ERR)
	{
		log_msg(LOG_INFO, RRUS_MGMT, "cd to rru mgmt failed\n");

	}
	free(timer);
    return FUNC_OK;
}






INT32 handle_timer_bbu_hub_msg(void* buf, UINT32 len)
{
//	log_msg(LOG_INFO, RRUS_MGMT, "sent hub to RRUS\n");
	//发送心跳消息给所有在线的RRU
	INT32 i,live_rru = 0;
	for (i = 0; i < MAX_RRU_NUM; i++)
	{
		if(g_rru_entity_table[i] != NULL && g_rru_entity_table[i]->logic_info.rru_cur_state > STATE_WAIT_CHANNEL_BUILD_CFG_RSP)
		{
			MSG_BBU_RRU_HB *msg_bbu_hub = (MSG_BBU_RRU_HB*)malloc(sizeof(MSG_BBU_RRU_HB));
			msg_bbu_hub->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_HB);
			msg_bbu_hub->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_HB));
			msg_bbu_hub->ir_msg_head.rru_id = htonl(g_rru_entity_table[i]->logic_info.rru_id);
			msg_bbu_hub->ir_msg_head.bbu_id = htonl(g_rru_entity_table[i]->logic_info.bbu_id);
			msg_bbu_hub->ir_msg_head.fiber_port = htonl(g_rru_entity_table[i]->logic_info.cpri_port);
			msg_bbu_hub->ir_msg_head.stream_no = htonl(g_rru_entity_table[i]->logic_info.stream_no+1);

			struct sockaddr_in rruaddr;
			inet_aton((char*)&g_rru_entity_table[i]->logic_info.rru_ip,&rruaddr.sin_addr);
			//rruaddr.sin_addr.s_addr = htonl(rruaddr.sin_addr.s_addr);
			send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)msg_bbu_hub, sizeof(MSG_BBU_RRU_HB));
			free(msg_bbu_hub);

			live_rru++;
			log_msg(LOG_INFO, RRUS_MGMT, "sent hub to RRU(%d)\n",g_rru_entity_table[i]->logic_info.rru_id);

		}
	}
	if(live_rru == 0)
	{
		//无RRU在线，停止发送
		log_msg(LOG_INFO, RRUS_MGMT, "NO RRU alive.\n");
		sbs_timer_del(bbu_hub);
		free(bbu_hub);
		return FUNC_OK;
	}

	sbs_timer_schedule(bbu_hub, 3, 0);

    return FUNC_OK;
}

INT32 handle_timer_rru_hub_timeout(void* buf, UINT32 len)
{
	RRU_HUB_TIMEOUT *rru_hub_t = (RRU_HUB_TIMEOUT*)buf;
	INT32 entity_id;
	if((entity_id = id_to_entity(rru_hub_t->rru_id)) == FUNC_ERR)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no such an entity.\n");
		return FUNC_ERR;
	}


	if(g_rru_entity_table[entity_id]->logic_info.lost_hub_count == 9)
	{
		sbs_timer_del(rru_hub_timeout[entity_id]);
		free(rru_hub_timeout[entity_id]);

		INT32 i;
		for (i = 0; i < MAX_RRU_NUM; i++)
		{
			if(g_rru_cfig_table[i] == NULL)
			{
				g_rru_cfig_table[i] = (RruEntity*)malloc(sizeof(RruEntity));
				memcpy(g_rru_cfig_table[i],g_rru_entity_table[entity_id],sizeof(RruEntity));
				memset(&g_rru_cfig_table[i]->logic_info,0,sizeof(RruLogicInfo));
				log_msg(LOG_INFO, RRUS_MGMT, "stored information of RRU(%d),waiting reconnection....\n",rru_hub_t->rru_id);

				//wait the delete msg from cfig module
				free(g_rru_cfig_table[i]);
				break;
			}
			if(i == MAX_RRU_NUM -1)
			{
				log_msg(LOG_INFO, RRUS_MGMT, "exceed the max number of RRU parameter base\n",rru_hub_t->rru_id);
				return FUNC_OK;
			}
		}
		free(g_rru_entity_table[entity_id]);
		g_rru_entity_table[entity_id] = NULL;

//		handle_msg_rru_reg(rru_hub_t->rru_id, 1);
	}else{
		g_rru_entity_table[entity_id]->logic_info.lost_hub_count++;
		printf("RRU(%d) lost hub count:%d",g_rru_entity_table[entity_id]->logic_info.rru_id,g_rru_entity_table[entity_id]->logic_info.lost_hub_count);
		sbs_timer_schedule(rru_hub_timeout[entity_id], 3, 0);
	}

    return FUNC_OK;
}

INT32 handle_timer_rru_cell_cfg_rsp_timeout(void* buf, UINT32 len)
{
	log_msg(LOG_ERR, RRUS_MGMT, "rru cell cfg rsp timeout\n");
	RRU_CELL_CFG_RSP_TIMEOUT *cell_timeout = (RRU_CELL_CFG_RSP_TIMEOUT*)buf;
	INT32 entity_id;
	if((entity_id = id_to_entity(cell_timeout->rru_id)) == FUNC_ERR)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no such an entity.\n");
		return FUNC_ERR;
	}

	sbs_timer_del(rru_cell_cfg_rsp_timeout[entity_id]);
	free(rru_cell_cfg_rsp_timeout[entity_id]);
	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_RRU_NORMAL;

    return FUNC_OK;
}

INT32 handle_timer_rru_normal_msg_rsp_timeout(void* buf, UINT32 len)
{
	log_msg(LOG_ERR, RRUS_MGMT, "rru msg rsp timeout\n");
	RRU_NORMAL_MSG_RSP_TIMEOUT *normal_timeout = (RRU_NORMAL_MSG_RSP_TIMEOUT*)buf;
	INT32 entity_id;
	if((entity_id = id_to_entity(normal_timeout->rru_id)) == FUNC_ERR)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no such an entity.\n");
		return FUNC_ERR;
	}
	sbs_timer_del(rru_normal_msg_rsp_timeout[entity_id]);
	free(rru_normal_msg_rsp_timeout[entity_id]);
	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_RRU_NORMAL;


    return FUNC_OK;
}

INT32 handle_timer_rru_param_query(void* buf, UINT32 len)
{
	//处理参数查询
	log_msg(LOG_INFO, RRUS_MGMT, "handle param query req.\n");
	INT32 entity_id;

	RRU_PARAM_QUERY *param= (RRU_PARAM_QUERY*)buf;

	if((entity_id = id_to_entity(param->rru_id)) == FUNC_ERR)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_BBU_RRU_PARAM_QUERY_REQ *para_query = (MSG_BBU_RRU_PARAM_QUERY_REQ*)malloc(sizeof(MSG_BBU_RRU_PARAM_QUERY_REQ));
	para_query->ir_msg_head.rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	para_query->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_PARAM_QUERY_REQ);
	para_query->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_PARAM_QUERY_REQ));
	para_query->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	para_query->ir_msg_head.fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;

	para_query->ie_system_time_query.ie_flag = htons(IE_FLAG_SYS_TIME_QUERY_REQ);
	para_query->ie_system_time_query.ie_len = htons(sizeof(IE_SYSTEM_TIME_QUERY));

	para_query->ie_cpu_usage_query.ie_flag = htons(IE_FLAG_CPU_USAGE_QUERY_REQ);
	para_query->ie_cpu_usage_query.ie_len = htons(sizeof(IE_CPU_USAGE_QUERY));

	para_query->ie_cpu_usage_check_cycle_query.ie_flag = htons(IE_FLAG_CPU_USAGE_PERIOD_QUERY_REQ);
	para_query->ie_cpu_usage_check_cycle_query.ie_len = htons(sizeof(IE_CPU_USAGE_CHECK_CYCLE_QUERY));

	para_query->ie_rru_temp_query.ie_flag = htons(IE_FLAG_RRU_TEMPERATUR_QUERY_REQ);
	para_query->ie_rru_temp_query.ie_len = htons(sizeof(IE_RRU_TEMP_QUERY));
	para_query->ie_rru_temp_query.temp_type = htonl(RF_PASS_TEMP);

	para_query->ie_vswr_state_query.ie_flag = htons(IE_FLAG_VSWR_STATE_QUERY_REQ);
	para_query->ie_vswr_state_query.ie_len = htons(sizeof(IE_VSWR_STATE_QUERY));
	para_query->ie_vswr_state_query.rf_path_no = 0;//Rf no.

	para_query->ie_vswr_thres_query.ie_flag = htons(IE_FLAG_VSWR_THRESHOLD_QUERY_REQ);
	para_query->ie_vswr_thres_query.ie_len = htons(sizeof(IE_VSWR_THRES_QUERY));

	para_query->ie_overtemp_thres_query.ie_flag = htons(IE_FLAG_TEMP_THRESHOLD_QUERY_REQ);
	para_query->ie_overtemp_thres_query.ie_len = htons(sizeof(IE_OVERTEMP_THRES_QUERY));

	para_query->ie_output_power_query.ie_flag = htons(IE_FLAG_OUTPUT_POWER_QUERY_REQ);
	para_query->ie_output_power_query.ie_len = htons(sizeof(IE_OUTPUT_POWER_QUERY));
	para_query->ie_output_power_query.rf_path_no = 0;

	para_query->ie_state_machine_query.ie_flag = htons(IE_FLAG_STATE_MACHINE_QUERY_REQ);
	para_query->ie_state_machine_query.ie_len = htons(sizeof(IE_STATE_MACHINE_QUERY));

	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)para_query, sizeof(MSG_BBU_RRU_PARAM_QUERY_REQ));
	free(para_query);

	if(g_rru_entity_table[entity_id] == NULL)
	{
		sbs_timer_del(rru_param_query[entity_id]);
		free(rru_param_query[entity_id]);
	}else{
		sbs_timer_schedule(rru_param_query[entity_id], 60, 0);
	}

    return FUNC_OK;
}

INT32 handle_timer_rru_state_query(void* buf, UINT32 len)
{
	//处理状态查询
	log_msg(LOG_INFO, RRUS_MGMT, "handle status query req.\n");
	INT32 entity_id;

	RRU_STATE_QUERY *state = (RRU_STATE_QUERY*)buf;

	if((entity_id = id_to_entity(state->rru_id)) == FUNC_ERR)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	MSG_BBU_RRU_STATUS_QUERY_REQ *status_req = (MSG_BBU_RRU_STATUS_QUERY_REQ*)malloc(sizeof(MSG_BBU_RRU_STATUS_QUERY_REQ));
	status_req->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_STATUS_QUERY_REQ);
	status_req->ir_msg_head.rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	status_req->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_STATUS_QUERY_REQ));
	status_req->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	status_req->ir_msg_head.fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;

	status_req->ie_rf_path_state_query.ie_flag = htons(IE_FLAG_RF_PATH_STATE_QUERY_REQ);
	status_req->ie_rf_path_state_query.ie_len = htons(sizeof(IE_RF_PATH_STATE_QUERY));
	status_req->ie_rf_path_state_query.rf_path_no = 1;

	status_req->ie_system_time_state.ie_flag = htons(IE_FLAG_CLOCK_STATE_QUERY_REQ);
	status_req->ie_system_time_state.ie_len = htons(sizeof(IE_SYSTEM_TIME_STATE));

	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)status_req, sizeof(MSG_BBU_RRU_STATUS_QUERY_REQ));
	free(status_req);

	if(g_rru_entity_table[entity_id] == NULL)
	{
		sbs_timer_del(rru_state_query[entity_id]);
		free(rru_state_query[entity_id]);
	}else{
		sbs_timer_schedule(rru_state_query[entity_id], 60, 0);
	}

    return FUNC_OK;
}

INT32 handle_timer_cpri_state_query(void* buf, UINT32 len)
{
	log_msg(LOG_INFO, RRUS_MGMT,"cpri state query\n");

	TIMER_CPRI_STATE_QUERY *cpri_state = (TIMER_CPRI_STATE_QUERY*)buf;

	INT32 entity_id;
	if((entity_id = id_to_cpri_entity(cpri_state->cpri_index)) == FUNC_ERR)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no cpri entity in %s.\n", __FUNCTION__);
		//sbs_timer_del(cpri_state_query[entity_id]);
		return FUNC_ERR;
	}

	MSG_CPRI_STATE_QUERY *msg_state_req = (MSG_CPRI_STATE_QUERY*)malloc(sizeof(MSG_CPRI_STATE_QUERY));
	msg_state_req->msg_head.msg_type = htons(MSG_TYPE_DATA_FPGA_RRUS_MGMT);
	msg_state_req->msg_head.msg_code = htons(CODE_RRUS_MGMT_DATA_FPGA_CPRI_STATE_QUERY);
	msg_state_req->msg_head.msg_len = htonl(sizeof(MSG_CPRI_STATE_QUERY));

	EqIndex *cpri = (EqIndex*)&cpri_state->cpri_index;
	msg_state_req->msg_head.fpga_id = htonl((UINT32)cpri->fpga_id);
	msg_state_req->state_query.fiber_num = htonl((UINT32)cpri->fiber_num);

	send_msg_mac(msg_state_req, sizeof(MSG_CPRI_STATE_QUERY));

	free(msg_state_req);

	if(cpri_state_query[entity_id] == NULL)
	{
		sbs_timer_del(cpri_state_query[entity_id]);
		//free(cpri_state_query[entity_id]);
		return FUNC_OK;
	}else{
		sbs_timer_schedule(cpri_state_query[entity_id], 60, 0);
	}

    return FUNC_OK;
}

INT32 handle_timer_cpri_msg_rsp_timeout(void* buf, UINT32 len)
{
	CPRI_MSG_RSP_TIMEOUT *cpri_timeout = (CPRI_MSG_RSP_TIMEOUT*)buf;

	INT32 entity_id;
	if((entity_id = id_to_cpri_entity(cpri_timeout->cpri_index)) == FUNC_ERR)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no cpri entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	log_msg(LOG_ERR, RRUS_MGMT, "cpri(%d) msg timeout.\n", cpri_timeout->cpri_index);

	sbs_timer_del(cpri_msg_rsp_timeout[entity_id]);
	free(cpri_msg_rsp_timeout[entity_id]);
    return FUNC_OK;
}

