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
 *File name: rrus_mgmt_handler_cfig_mgmt_msg.c
 *Description: All handlers of messages from cfig_mgmt.
 *
 *Current Version: 0.l
 *Author: zhanghenghao (zhanghenghao@sylincom.com)
 *Date: 2018.08.26
 *
 ************************************************************************/

#include <arpa/inet.h>
#include "sbs_type.h"
#include "sbs_log.h"
#include "om_common_h.h"
#include "rrus_mgmt_basic_func.h"
#include "rrus_mgmt_api_cfig_mgmt.h"
#include "rrus_mgmt_api_timer.h"
#include "rrus_mgmt_cpri_entity.h"
#include "rrus_mgmt_cpri.h"
#include "rrus_mgmt_rru_entity.h"
#include "rrus_mgmt_ir.h"
#include "rrus_mgmt_common.h"

CpriEntity* g_cpri_entity_table[MAX_CPRI_NUM];

INT32 handle_msg_cpri_time_measure_requ(UINT16 cpri_index, void* msg);
INT32 handle_msg_cpri_time_cfg_req(UINT16 cpri_index, void* msg);
INT32 handle_msg_param_cfg_req(UINT32 rru_id, void* msg);
INT32 handle_msg_ver_query_req(UINT32 rru_id, void* msg);

INT32 handle_msg_time_delay_measure_req(UINT32 rru_id, void* msg);
INT32 handle_msg_time_delay_cfg_req(UINT32 rru_id, void* msg);

INT32 handle_msg_rru_reset(UINT32 rru_id, void *msg);

INT32 handle_icr_init_info_cpri(void* buf, UINT32 len)
{
	log_msg(LOG_INFO, RRUS_MGMT, "handle initial info--cpri speed\n");
	INT32 i;
	ICR_INIT_INFO_CPRI_SPEED *pmsg = (ICR_INIT_INFO_CPRI_SPEED*)buf;

	for (i = 0; i < MAX_CPRI_NUM; i++)
	{
		if(g_cpri_entity_table[i] == NULL)
		{
			g_cpri_entity_table[i] = (CpriEntity*)malloc(sizeof(CpriEntity));
			g_cpri_entity_table[i]->cpri_index = pmsg->cpri_index;
			g_cpri_entity_table[i]->CpriSpeed = pmsg->cpri_speed;

			break;
		}
	}


    return FUNC_OK;
}

INT32 handle_icr_init_info_rru(void* buf, UINT32 len)
{
	log_msg(LOG_INFO, RRUS_MGMT, "handle initial info--rru\n");
	INT32 i;
	ICR_INIT_INFO_RRU *pmsg = (ICR_INIT_INFO_RRU*)buf;

	for (i = 0; i < MAX_RRU_PARAM_NUM; i++)
	{
		if(g_rru_param_table[i] == NULL)
		{
			g_rru_param_table[i] = (RRU_PARAM_TABLE*)malloc(sizeof(RRU_PARAM_TABLE));
			memcpy(g_rru_param_table[i],&pmsg->serial_no,sizeof(RRU_PARAM_TABLE));
			log_msg(LOG_INFO, RRUS_MGMT,"rru parameter table saved\n");

			int j = 0;
			printf("rru param:serial_no:");
			for (j; j < 16; j++)
			{
				printf("%c",g_rru_param_table[i]->serial_no[j]);
			}
			printf("\n");
			if(i == MAX_RRU_PARAM_NUM-1)
			{
				log_msg(LOG_ERR, RRUS_MGMT,"cannot store more RRU parameter\n");
				return FUNC_ERR;
			}
			break;
		}
	}

    return FUNC_OK;
}




INT32 handle_icr_cpri_ring_test_req(void* buf, UINT32 len)
{
	log_msg(LOG_INFO, RRUS_MGMT,"cpri ring test query\n");

	MSG_CPRI_RING_TEST_REQ *msg_ring_req = (MSG_CPRI_RING_TEST_REQ*)malloc(sizeof(MSG_CPRI_RING_TEST_REQ));
	msg_ring_req->msg_head.msg_type = htons(MSG_TYPE_DATA_FPGA_RRUS_MGMT);
	msg_ring_req->msg_head.msg_code = htons(CODE_RRUS_MGMT_DATA_FPGA_RING_TEST_REQ);
	msg_ring_req->msg_head.msg_len = htonl(sizeof(MSG_CPRI_RING_TEST_REQ));

	ICR_CPRI_RING_TEST *pmsg = (ICR_CPRI_RING_TEST*)buf;
	EqIndex *index = (EqIndex*)&pmsg->cpri_index;
	msg_ring_req->msg_head.fpga_id = htonl((UINT32)index->fpga_id);
	msg_ring_req->ring_test.fiber_num = htonl((UINT32)index->fiber_num);

	send_msg_mac(msg_ring_req, sizeof(MSG_CPRI_RING_TEST_REQ));
	free(msg_ring_req);

    return FUNC_OK;
}

INT32 handle_icr_cpri_ser_test_req(void* buf, UINT32 len)
{
	log_msg(LOG_INFO, RRUS_MGMT,"cpri ser test query\n");
	MSG_CPRI_SER_TEST_REQ *msg_ser_req = (MSG_CPRI_SER_TEST_REQ*)malloc(sizeof(MSG_CPRI_SER_TEST_REQ));
	msg_ser_req->msg_head.msg_type = htons(MSG_TYPE_DATA_FPGA_RRUS_MGMT);
	msg_ser_req->msg_head.msg_code = htons(CODE_RRUS_MGMT_DATA_FPGA_SER_TEST_REQ);
	msg_ser_req->msg_head.msg_len = htonl(sizeof(MSG_CPRI_SER_TEST_REQ));

	ICR_CPRI_SER_TEST *pmsg = (ICR_CPRI_SER_TEST*)buf;
	EqIndex *index = (EqIndex*)&pmsg->cpri_index;
	msg_ser_req->msg_head.fpga_id = htonl((UINT32)index->fpga_id);
	msg_ser_req->ser_test_req.fiber_num = htonl((UINT32)index->fiber_num);
	msg_ser_req->ser_test_req.ctl_word = pmsg->ctrl_word;

	send_msg_mac(msg_ser_req, sizeof(MSG_CPRI_SER_TEST_REQ));
	free(msg_ser_req);

    return FUNC_OK;
}





INT32 handle_msg_cpri_time_measure_requ(UINT16 cpri_index, void* msg)
{
	log_msg(LOG_INFO, RRUS_MGMT,"cpri time delay measure query\n");

	MSG_CPRI_TIME_DELAY_MEASURE_REQ *msg_time_mea_req = (MSG_CPRI_TIME_DELAY_MEASURE_REQ*)malloc(sizeof(MSG_CPRI_TIME_DELAY_MEASURE_REQ));
	msg_time_mea_req->msg_head.msg_type = htons(MSG_TYPE_DATA_FPGA_RRUS_MGMT);
	msg_time_mea_req->msg_head.msg_code = htons(CODE_RRUS_MGMT_DATA_FPGA_TIME_DELAY_MEASURE_REQ);
	msg_time_mea_req->msg_head.msg_len = htonl(sizeof(MSG_CPRI_AXC_CHANNEL_CFG));
	EqIndex *index = (EqIndex*)(&cpri_index);
	msg_time_mea_req->msg_head.fpga_id = htonl((UINT32)index->fpga_id);
	msg_time_mea_req->time_delay_measure.fiber_num = htonl((UINT32)index->fiber_num);

	send_msg_mac(msg_time_mea_req, sizeof(MSG_CPRI_TIME_DELAY_MEASURE_REQ));
	free(msg_time_mea_req);

	return FUNC_OK;
}


INT32 handle_icr_cpri_time_cfg_req(void* buf, UINT32 len)
{
	ICR_CPRI_TIME_DELAY_CFG *pmsg = (ICR_CPRI_TIME_DELAY_CFG*)buf;
	handle_msg_cpri_time_cfg_req(pmsg->cpri_index, (void*)buf);
	return FUNC_OK;
}


INT32 handle_msg_cpri_time_cfg_req(UINT16 cpri_index, void* msg)
{
	log_msg(LOG_INFO, RRUS_MGMT,"cpri time delay cfg query\n");

	MSG_CPRI_TIME_DELAY_CFG *msg_time_cfg_req = (MSG_CPRI_TIME_DELAY_CFG*)malloc(sizeof(MSG_CPRI_TIME_DELAY_CFG));
	msg_time_cfg_req->msg_head.msg_type = htons(MSG_TYPE_DATA_FPGA_RRUS_MGMT);
	msg_time_cfg_req->msg_head.msg_code = htons(CODE_RRUS_MGMT_DATA_FPGA_TIME_DELAY_CFG_REQ);
	msg_time_cfg_req->msg_head.msg_len = htonl(sizeof(MSG_CPRI_TIME_DELAY_CFG));
	EqIndex *index = (EqIndex*)(&cpri_index);
	msg_time_cfg_req->msg_head.fpga_id = htonl((UINT32)index->fpga_id);
	msg_time_cfg_req->time_delay_cfg.fiber_num = htonl((UINT32)index->fiber_num);

	if(msg != NULL)
		//get time cfg info from cfg module
	{
		ICR_CPRI_TIME_DELAY_CFG *pmsg = (ICR_CPRI_TIME_DELAY_CFG*)msg;
		msg_time_cfg_req->time_delay_cfg.axc_ind = htonl((UINT32)pmsg->axc_ind);
		msg_time_cfg_req->time_delay_cfg.time_delay_ul = htonl((UINT32)pmsg->time_delay_dl);
		msg_time_cfg_req->time_delay_cfg.time_delay_dl = htonl((UINT32)pmsg->time_delay_dl);
	}else{
		//compute automatically during init
		msg_time_cfg_req->time_delay_cfg.axc_ind = htonl(1);
		msg_time_cfg_req->time_delay_cfg.time_delay_ul = htonl(1);
		msg_time_cfg_req->time_delay_cfg.time_delay_dl = htonl(1);
	}


	send_msg_mac(msg_time_cfg_req, sizeof(MSG_CPRI_TIME_DELAY_CFG));
	free(msg_time_cfg_req);

	return FUNC_OK;
}

INT32 handle_icr_single_send_cfg(void* buf, UINT32 len)
{
	ICR_SINGLE_SEND_CFG *pmsg = (ICR_SINGLE_SEND_CFG*)buf;

	MSG_SINGLE_MODE_CFG *single_mode = (MSG_SINGLE_MODE_CFG*)malloc(sizeof(MSG_SINGLE_MODE_CFG));
	single_mode->msg_head.msg_type = htons(MSG_TYPE_DATA_FPGA_RRUS_MGMT);
	single_mode->msg_head.msg_code = htons(CODE_RRUS_MGMT_DATA_FPGA_TIME_DELAY_CFG_REQ);
	single_mode->msg_head.msg_len = htonl(sizeof(MSG_CPRI_TIME_DELAY_CFG));
	EqIndex *index = (EqIndex*)(&pmsg->cpri_index);
	single_mode->msg_head.fpga_id = htonl((UINT32)index->fpga_id);
	single_mode->single_mode_cfg.fiber_num = htonl((UINT32)index->fiber_num);
	single_mode->single_mode_cfg.axc_ind = htonl(pmsg->axc_ind);
	single_mode->single_mode_cfg.on_or_off = htonl(pmsg->on_or_off);
	single_mode->single_mode_cfg.single_mode = htonl(pmsg->single_mode);

	send_msg_mac(single_mode, sizeof(MSG_SINGLE_MODE_CFG));
	free(single_mode);

	return FUNC_OK;
}

INT32 handle_icr_fpga_work_mode_cfg(void* buf, UINT32 len)
{

    return FUNC_OK;
}

INT32 handle_icr_iq_data_out_cfg(void* buf, UINT32 len)
{
    return FUNC_OK;
}





INT32 handle_msg_param_cfg_req(UINT32 rru_id, void* msg)
{
	log_msg(LOG_INFO, RRUS_MGMT, "handle param cfg req.\n");
	INT32 entity_id,i;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	MSG_BBU_RRU_PARAM_CFG_REQ *para_cfg = (MSG_BBU_RRU_PARAM_CFG_REQ*)malloc(sizeof(MSG_BBU_RRU_PARAM_CFG_REQ));
	para_cfg->ir_msg_head.rru_id = htonl(rru_id);
	para_cfg->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_PARAM_CFG_REQ);
	para_cfg->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_PARAM_CFG_REQ));
	para_cfg->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	para_cfg->ir_msg_head.fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	para_cfg->ir_msg_head.stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);

	i = 0;
	int j = 0;
	printf("rru param reported serial_no:");
	for (j; j < 16; j++)
	{
		printf("%c",g_rru_param_table[i]->serial_no[j]);
	}
	printf("\n");

	printf("serial no. difference:%d\n",memcmp(g_rru_entity_table[entity_id]->com_info.product_info.serial_no,g_rru_param_table[i]->serial_no,sizeof(INT8)*16));
//	for (i = 0; i < MAX_RRU_PARAM_NUM; i++)
//	{
//		if(g_rru_param_table[i] != NULL)
//		{
//			if(memcmp(g_rru_entity_table[entity_id]->com_info.product_info.serial_no,
//					g_rru_param_table[i]->serial_no,sizeof(INT8)*16) == 0)
//			{
				log_msg(LOG_INFO, RRUS_MGMT,"found the parameter file of rru(%s)\n",g_rru_entity_table[entity_id]->com_info.product_info.serial_no);

				para_cfg->ie_cpu_usage_check_cycle_cfg.ie_flag = htons(IE_FLAG_CPU_USAGE_PERIOD_CFG);
				para_cfg->ie_cpu_usage_check_cycle_cfg.ie_len = htons(sizeof(IE_CPU_USAGE_CHECK_CYCLE_CFG));
				para_cfg->ie_cpu_usage_check_cycle_cfg.period = htonl(g_rru_param_table[i]->period);

				para_cfg->ie_vswr_thres_cfg.ie_flag = htons(IE_FLAG_VSWR_THRESHOLD_CFG);
				para_cfg->ie_vswr_thres_cfg.ie_len = htons(sizeof(IE_VSWR_THRES_CFG));
				para_cfg->ie_vswr_thres_cfg.vswr_thres1 = htonl(g_rru_param_table[i]->vswr_thres1);
				para_cfg->ie_vswr_thres_cfg.vswr_thres1 = htonl(g_rru_param_table[i]->vswr_thres1);

				para_cfg->ie_overtemp_thres_rsp.ie_flag = htons(IE_FLAG_TEMP_THRESHOLD_CFG);
				para_cfg->ie_overtemp_thres_rsp.ie_len = htons(sizeof(IE_OVERTEMP_THRES_CFG));
				para_cfg->ie_overtemp_thres_rsp.temp_type = 0;
				para_cfg->ie_overtemp_thres_rsp.up_thres = htonl(g_rru_param_table[i]->up_thres);
				para_cfg->ie_overtemp_thres_rsp.low_thres = htonl(g_rru_param_table[i]->low_thres);

				para_cfg->ie_antenna_cfg.ie_flag = htons(IE_FLAG_ANT_CFG);
				para_cfg->ie_antenna_cfg.ie_len = htons(sizeof(IE_ANTENNA_CFG));
				para_cfg->ie_antenna_cfg.ant_mode = g_rru_param_table[i]->ant_mode;
				para_cfg->ie_antenna_cfg.ant_set_no = g_rru_param_table[i]->ant_set_no;
				memcpy(&para_cfg->ie_antenna_cfg.ant_state_u, &g_rru_param_table[i]->ant_state,sizeof(UINT16));

				para_cfg->ie_rf_path_state_cfg.ie_flag = htons(IE_FLAG_RF_PATH_STATE);
				para_cfg->ie_rf_path_state_cfg.ie_len = htons(sizeof(IE_RF_PATH_STATE_CFG));
//				UINT32 p;
//				for(p=0;p<8;p++)
//				{
//					para_cfg->ie_rf_path_state_cfg.up_path_state[p] = htonl(g_rru_param_table[i]->up_path_state[p]);
//					para_cfg->ie_rf_path_state_cfg.dw_path_state[p] = htonl(g_rru_param_table[i]->dw_path_state[p]);
//				}
//				break;
//			}
//		}
//		if(i == MAX_RRU_PARAM_NUM-1)
//		{
//			log_msg(LOG_ERR, RRUS_MGMT,"cannot find the parameter file of rru(%s)\n",g_rru_entity_table[entity_id]->com_info.product_info.serial_no);
//			return FUNC_ERR;
//		}
//	}

	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)para_cfg, sizeof(MSG_BBU_RRU_PARAM_CFG_REQ));
	free(para_cfg);
	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_PARAM_CFG_RSP;

	return FUNC_OK;
}


INT32 handle_icr_rru_param_cpu_usage_period(void* buf, UINT32 len)
{
	INT32 entity_id;
	ICR_RRU_PARAM_CPU_USAGE_PERIOD *pmsg = (ICR_RRU_PARAM_CPU_USAGE_PERIOD*)buf;

	if((entity_id = id_to_entity((UINT32)pmsg->rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	char para_cpu[sizeof(IrMsgHead) + sizeof(IE_CPU_USAGE_CHECK_CYCLE_CFG)];

	IrMsgHead *para_cfg_head = (IrMsgHead*)malloc(sizeof(IrMsgHead));
	para_cfg_head->rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	para_cfg_head->msg_code = htonl(MSG_CODE_BBU_RRU_PARAM_QUERY_REQ);
	para_cfg_head->msg_len = htonl(sizeof(IrMsgHead) + sizeof(IE_CPU_USAGE_CHECK_CYCLE_CFG));
	para_cfg_head->bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	para_cfg_head->fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	para_cfg_head->stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);

	memcpy(para_cpu,para_cfg_head,sizeof(IrMsgHead));

	IE_CPU_USAGE_CHECK_CYCLE_CFG *para_cfg_body = (IE_CPU_USAGE_CHECK_CYCLE_CFG*)malloc(sizeof(IE_CPU_USAGE_CHECK_CYCLE_CFG));
	para_cfg_body->ie_flag = htons(IE_FLAG_CPU_USAGE_PERIOD_CFG);
	para_cfg_body->ie_len = htons(sizeof(IE_CPU_USAGE_CHECK_CYCLE_CFG));
	para_cfg_body->period = htonl(pmsg->period);
	memcpy(para_cpu+sizeof(IrMsgHead),para_cfg_body,sizeof(IE_CPU_USAGE_CHECK_CYCLE_CFG));


	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)para_cpu, sizeof(IrMsgHead) + sizeof(IE_CPU_USAGE_CHECK_CYCLE_CFG));
	free(para_cfg_head);
	free(para_cfg_body);
	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_PARAM_CFG_RSP;

    return FUNC_OK;
}


INT32 handle_icr_rru_param_vswr_thres(void* buf, UINT32 len)
{
	INT32 entity_id;
	ICR_RRU_PARAM_VSWR_THRES *pmsg = (ICR_RRU_PARAM_VSWR_THRES*)buf;

	if((entity_id = id_to_entity((UINT32)pmsg->rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	char para_vswr[sizeof(IrMsgHead) + sizeof(IE_VSWR_THRES_CFG)];

	IrMsgHead *para_cfg_head = (IrMsgHead*)malloc(sizeof(IrMsgHead));
	para_cfg_head->rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	para_cfg_head->msg_code = htonl(MSG_CODE_BBU_RRU_PARAM_QUERY_REQ);
	para_cfg_head->msg_len = htonl(sizeof(IrMsgHead) + sizeof(IE_VSWR_THRES_CFG));
	para_cfg_head->bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	para_cfg_head->fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	para_cfg_head->stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);

	memcpy(para_vswr,para_cfg_head,sizeof(IrMsgHead));

	IE_VSWR_THRES_CFG *para_cfg_body = (IE_VSWR_THRES_CFG*)malloc(sizeof(IE_VSWR_THRES_CFG));
	para_cfg_body->ie_flag = htons(IE_FLAG_VSWR_THRESHOLD_CFG);
	para_cfg_body->ie_len = htons(sizeof(IE_VSWR_THRES_CFG));
	para_cfg_body->vswr_thres1 = htons(pmsg->VswrThres1);
	para_cfg_body->vswr_thres2 = htons(pmsg->VswrThres2);
	memcpy(para_vswr+sizeof(IrMsgHead),para_cfg_body,sizeof(IE_VSWR_THRES_CFG));

	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)para_vswr, sizeof(IrMsgHead) + sizeof(IE_VSWR_THRES_CFG));
	free(para_cfg_head);
	free(para_cfg_body);

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_PARAM_CFG_RSP;
    return FUNC_OK;
}


INT32 handle_icr_rru_param_temp_thres(void* buf, UINT32 len)
{
	INT32 entity_id;
	ICR_RRU_PARAM_TEMP_THRES *pmsg = (ICR_RRU_PARAM_TEMP_THRES*)buf;

	if((entity_id = id_to_entity((UINT32)pmsg->rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	char para_temp[sizeof(IrMsgHead) + sizeof(IE_OVERTEMP_THRES_CFG)];

	IrMsgHead *para_cfg_head = (IrMsgHead*)malloc(sizeof(IrMsgHead));
	para_cfg_head->rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	para_cfg_head->msg_code = htonl(MSG_CODE_BBU_RRU_PARAM_QUERY_REQ);
	para_cfg_head->msg_len = htonl(sizeof(IrMsgHead) + sizeof(IE_OVERTEMP_THRES_CFG));
	para_cfg_head->bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	para_cfg_head->fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	para_cfg_head->stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);

	memcpy(para_temp,para_cfg_head,sizeof(IrMsgHead));

	IE_OVERTEMP_THRES_CFG *para_cfg_body = (IE_OVERTEMP_THRES_CFG*)malloc(sizeof(IE_OVERTEMP_THRES_CFG));
	para_cfg_body->ie_flag = htons(IE_FLAG_TEMP_THRESHOLD_CFG);
	para_cfg_body->ie_len = htons(sizeof(IE_OVERTEMP_THRES_CFG));
	para_cfg_body->temp_type = RF_PASS_TEMP;
	para_cfg_body->up_thres = htonl(pmsg->UpThres);
	para_cfg_body->low_thres = htonl(pmsg->LowThres);

	memcpy(para_temp+sizeof(IrMsgHead),para_cfg_body,sizeof(IE_OVERTEMP_THRES_CFG));


	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)para_temp, sizeof(IrMsgHead) + sizeof(IE_OVERTEMP_THRES_CFG));
	free(para_cfg_head);
	free(para_cfg_body);

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_PARAM_CFG_RSP;
    return FUNC_OK;
}


INT32 handle_icr_rru_param_ant_cfg(void* buf, UINT32 len)
{
	INT32 entity_id;
	ICR_RRU_PARAM_ANT_CFG *pmsg = (ICR_RRU_PARAM_ANT_CFG*)buf;

	if((entity_id = id_to_entity((UINT32)pmsg->rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	char para_ant[sizeof(IrMsgHead) + sizeof(IE_ANTENNA_CFG)];

	IrMsgHead *para_cfg_head = (IrMsgHead*)malloc(sizeof(IrMsgHead));
	para_cfg_head->rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	para_cfg_head->msg_code = htonl(MSG_CODE_BBU_RRU_PARAM_QUERY_REQ);
	para_cfg_head->msg_len = htonl(sizeof(IrMsgHead) + sizeof(IE_ANTENNA_CFG));
	para_cfg_head->bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	para_cfg_head->fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	para_cfg_head->stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);

	memcpy(para_ant,para_cfg_head,sizeof(IrMsgHead));

	IE_ANTENNA_CFG *para_cfg_body = (IE_ANTENNA_CFG*)malloc(sizeof(IE_ANTENNA_CFG));
	para_cfg_body->ie_flag = htons(IE_FLAG_ANT_CFG);
	para_cfg_body->ie_len = htons(sizeof(IE_ANTENNA_CFG));
	para_cfg_body->ant_mode = pmsg->AntMode;
	para_cfg_body->ant_set_no = pmsg->AntSetNo;
	memcpy(&para_cfg_body->ant_state_u, &pmsg->AntState,sizeof(UINT16));


	memcpy(para_ant+sizeof(IrMsgHead),para_cfg_body,sizeof(IE_ANTENNA_CFG));


	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)para_ant, sizeof(IrMsgHead) + sizeof(IE_ANTENNA_CFG));
	free(para_cfg_head);
	free(para_cfg_body);

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_PARAM_CFG_RSP;
    return FUNC_OK;
}


INT32 handle_icr_rru_param_channel_state(void* buf, UINT32 len)
{
	INT32 entity_id;
	ICR_RRU_PARAM_CHANNEL_STATE *pmsg = (ICR_RRU_PARAM_CHANNEL_STATE*)buf;

	if((entity_id = id_to_entity((UINT32)pmsg->rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	char para_channel[sizeof(IrMsgHead) + sizeof(IE_RF_PATH_STATE_CFG)];

	IrMsgHead *para_cfg_head = (IrMsgHead*)malloc(sizeof(IrMsgHead));
	para_cfg_head->rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	para_cfg_head->msg_code = htonl(MSG_CODE_BBU_RRU_PARAM_QUERY_REQ);
	para_cfg_head->msg_len = htonl(sizeof(IrMsgHead) + sizeof(IE_RF_PATH_STATE_CFG));
	para_cfg_head->bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	para_cfg_head->fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	para_cfg_head->stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);

	memcpy(para_channel,para_cfg_head,sizeof(IrMsgHead));

	IE_RF_PATH_STATE_CFG *para_cfg_body = (IE_RF_PATH_STATE_CFG*)malloc(sizeof(IE_RF_PATH_STATE_CFG));
	para_cfg_body->ie_flag = htons(IE_FLAG_ANT_CFG);
	para_cfg_body->ie_len = htons(sizeof(IE_FLAG_RF_PATH_STATE));
	memcpy(&para_cfg_body->up_path_state, &pmsg->up_path_state,sizeof(UINT32)*16);


	memcpy(para_channel+sizeof(IrMsgHead),para_cfg_body,sizeof(IE_RF_PATH_STATE_CFG));

	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)para_channel, sizeof(IrMsgHead) + sizeof(IE_RF_PATH_STATE_CFG));
	free(para_cfg_head);
	free(para_cfg_body);
	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_PARAM_CFG_RSP;

    return FUNC_OK;
}


INT32 handle_icr_rru_ring_test_req(void* buf, UINT32 len)
{
	//处理回环测试
	log_msg(LOG_INFO, RRUS_MGMT, "handle ring test req.\n");
	INT32 entity_id;
	ICR_RRU_RING_TEST *pmsg = (ICR_RRU_RING_TEST*)buf;

	if((entity_id = id_to_entity((UINT32)pmsg->rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_BBU_RRU_RING_TEST_REQ *ring_req = (MSG_BBU_RRU_RING_TEST_REQ*)malloc(sizeof(MSG_BBU_RRU_RING_TEST_REQ));
	ring_req->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_RING_TEST_REQ);
	ring_req->ir_msg_head.rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	ring_req->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_RING_TEST_REQ));
	ring_req->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	ring_req->ir_msg_head.fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	ring_req->ir_msg_head.stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);


	ring_req->ie_ring_test_req.ie_flag = htons(IE_FLAG_RING_TEST_REQ);
	ring_req->ie_ring_test_req.ie_len = htons(sizeof(IE_RING_TEST_REQ));
	ring_req->ie_ring_test_req.ring_test_type = htonl(RING_TEST_OTHER);
	ring_req->ie_ring_test_req.check_period = htons(pmsg->check_period);//ns
	ring_req->ie_ring_test_req.port_no = pmsg->port_no;


	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)ring_req, sizeof(MSG_BBU_RRU_RING_TEST_REQ));
	free(ring_req);

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_RING_TEST_RSP;

    return FUNC_OK;
}

INT32 handle_icr_rru_reset_req(void* buf, UINT32 len)
{
	log_msg(LOG_INFO, RRUS_MGMT, "handle reset ind.\n");

	ICR_RRU_RESET_REQ *pmsg = (ICR_RRU_RESET_REQ*)buf;
	handle_msg_rru_reset((UINT32)pmsg->rru_id, NULL);

	return FUNC_OK;

}

INT32 handle_msg_rru_reset(UINT32 rru_id, void *msg)
{

	INT32 entity_id;
	if((entity_id = id_to_entity((UINT32)rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_BBU_RRU_RESET_IND *reset_ind = (MSG_BBU_RRU_RESET_IND*)malloc(sizeof(MSG_BBU_RRU_RESET_IND));
	reset_ind->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_RESET_IND);
	reset_ind->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	reset_ind->ir_msg_head.fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	reset_ind->ir_msg_head.rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	reset_ind->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_RESET_IND));
	reset_ind->ir_msg_head.stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);



	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)reset_ind, sizeof(MSG_BBU_RRU_RESET_IND));
	free(reset_ind);

	log_msg(LOG_INFO, RRUS_MGMT, "reset ind sent\n");
	free(g_rru_entity_table[entity_id]);
	g_rru_entity_table[entity_id] = NULL;

    return FUNC_OK;
}



INT32 handle_icr_rru_soft_update_req(void* buf, UINT32 len)
{
	log_msg(LOG_INFO, RRUS_MGMT, "ver query trigger.\n");
	ICR_RRU_SW_UPDATE_REQ *pmsg = (ICR_RRU_SW_UPDATE_REQ*)buf;
	handle_msg_ver_query_req((UINT32)pmsg->rru_id,NULL);
    return FUNC_OK;
}


INT32 handle_msg_ver_query_req(UINT32 rru_id, void* msg)
{
	//处理版本查询
	log_msg(LOG_INFO, RRUS_MGMT, "handle ver query req.\n");
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_BBU_RRU_VER_QUERY_REQ *ver_query = (MSG_BBU_RRU_VER_QUERY_REQ*)malloc(sizeof(MSG_BBU_RRU_VER_QUERY_REQ));
	ver_query->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_VER_QUERY_REQ);
	ver_query->ir_msg_head.rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	ver_query->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_VER_QUERY_REQ));
	ver_query->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	ver_query->ir_msg_head.fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	ver_query->ir_msg_head.stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);


	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)ver_query, sizeof(MSG_BBU_RRU_VER_QUERY_REQ));
	free(ver_query);

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_RRU_VER_QUERY_RSP;

	return FUNC_OK;
}

INT32 handle_msg_ver_download_req(UINT32 rru_id, void* msg)
{
	//处理版本下载
	log_msg(LOG_INFO, RRUS_MGMT, "handle ver download req.\n");
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_BBU_RRU_VER_DOWNLOAD_REQ *ver_download = (MSG_BBU_RRU_VER_DOWNLOAD_REQ*)malloc(sizeof(MSG_BBU_RRU_VER_DOWNLOAD_REQ));
	ver_download->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_VER_DOWNLOAD_REQ);
	ver_download->ir_msg_head.rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	ver_download->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_VER_DOWNLOAD_REQ));
	ver_download->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	ver_download->ir_msg_head.fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	ver_download->ir_msg_head.stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);

	ver_download->ie_soft_ver_check_result.ie_flag = htons(IE_FLAG_SW_VER_REQ);
	ver_download->ie_soft_ver_check_result.ie_len = htons(sizeof(IE_SOFT_VER_CHECK_RESULT));
	ver_download->ie_soft_ver_check_result.sw_ver_type = 0;
	memcpy(&ver_download->ie_soft_ver_check_result.check_result,&g_rru_entity_table[entity_id]->com_info.soft_ver_check_result_info,
			sizeof(SoftVerCheckResultInfo));

	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)ver_download, sizeof(MSG_BBU_RRU_VER_DOWNLOAD_REQ));
	free(ver_download);

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_VER_DOWNLOAD_RSP;

	return FUNC_OK;
}

INT32 handle_msg_ver_activate_ind(UINT32 rru_id, void* msg)
{
	//处理版本激活
	log_msg(LOG_INFO, RRUS_MGMT, "handle ver activate ind.\n");
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_BBU_RRU_VER_ACTIVATE_IND *ver_activate = (MSG_BBU_RRU_VER_ACTIVATE_IND*)malloc(sizeof(MSG_BBU_RRU_VER_ACTIVATE_IND));
	ver_activate->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_VER_ACTIVATE_IND);
	ver_activate->ir_msg_head.rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	ver_activate->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_VER_ACTIVATE_IND));
	ver_activate->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	ver_activate->ir_msg_head.fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	ver_activate->ir_msg_head.stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);

	ver_activate->ie_rru_soft_ver_info.ie_flag = htons(IE_FLAG_SW_VER);
	ver_activate->ie_rru_soft_ver_info.ie_len = htons(sizeof(IE_RRU_SOFT_VER_INFO));
	memcpy(&ver_activate->ie_rru_soft_ver_info.sw_ver,&g_rru_entity_table[entity_id]->com_info.soft_ware_info,
			sizeof(SoftWareInfo));

	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)ver_activate, sizeof(MSG_BBU_RRU_VER_ACTIVATE_IND));
	free(ver_activate);

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_VER_ACTIVATE_RSP;

	return FUNC_OK;
}


INT32 handle_msg_time_delay_measure_req(UINT32 rru_id, void* msg)
{
	//处理时延测量
	log_msg(LOG_INFO, RRUS_MGMT, "handle time delay measure req.\n");
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_BBU_RRU_TIME_DELAY_MEASURE_REQ *time_req = (MSG_BBU_RRU_TIME_DELAY_MEASURE_REQ*)malloc(sizeof(MSG_BBU_RRU_TIME_DELAY_MEASURE_REQ));
	time_req->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_TIME_DELAY_MEASURE_REQ);
	time_req->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	time_req->ir_msg_head.fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	time_req->ir_msg_head.rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	time_req->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_TIME_DELAY_MEASURE_REQ));
	time_req->ir_msg_head.stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);

	time_req->ie_time_delay_req.ie_flag = htons(IE_FLAG_TIME_DELAY_MEAS_REQ);
	time_req->ie_time_delay_req.ie_len = htons(sizeof(IE_TIME_DELAY_REQ));
	time_req->ie_time_delay_req.fiber_no = 1;
	printf("ie_time_delay_req.ie_flag:%d\n",ntohs(time_req->ie_time_delay_req.ie_flag));
	printf("ie_time_delay_req.ie_len:%d\n",ntohs(time_req->ie_time_delay_req.ie_len));
	printf("ie_time_delay_req.fiber_no:%d\n",time_req->ie_time_delay_req.fiber_no);
	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)time_req, sizeof(MSG_BBU_RRU_TIME_DELAY_MEASURE_REQ));
	free(time_req);

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_TIME_DELAY_MEASURE_RSP;

	return FUNC_OK;
}


INT32 handle_icr_rru_time_delay_cfg(void* buf, UINT32 len)
{

    return FUNC_OK;
}

INT32 handle_msg_time_delay_cfg_req(UINT32 rru_id, void* msg)
{
	//判断时延配置命令
	log_msg(LOG_INFO, RRUS_MGMT, "handle time delay cfg req.\n");
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_BBU_RRU_TIME_DELAY_CFG_REQ* time_cfg = (MSG_BBU_RRU_TIME_DELAY_CFG_REQ*)malloc(sizeof(MSG_BBU_RRU_TIME_DELAY_CFG_REQ));
	time_cfg->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_TIME_DELAY_CFG_REQ);
	time_cfg->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	time_cfg->ir_msg_head.fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	time_cfg->ir_msg_head.rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
	time_cfg->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_TIME_DELAY_CFG_REQ));
	time_cfg->ir_msg_head.stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);

	time_cfg->ie_time_delay_cfg_req.ie_flag = htons(IE_FLAG_TIME_DELAY_CFG_REQ);
	time_cfg->ie_time_delay_cfg_req.ie_len = htons(sizeof(IE_TIME_DELAY_CFG_REQ));
	time_cfg->ie_time_delay_cfg_req.fiber_no = 1;
	if(g_rru_entity_table[entity_id]->logic_info.rru_level == 1)
	{
		INT32 cpri_entity;
		if((cpri_entity = id_to_cpri_entity(g_rru_entity_table[entity_id]->logic_info.cpri_index)) == -1)
		{
		 	log_msg(LOG_ERR, RRUS_MGMT, "no cpri entity in %s.\n", __FUNCTION__);
		 	return FUNC_ERR;
		}
		UINT32 T14;
		T14 = g_cpri_entity_table[cpri_entity]->T14;
		UINT32 Toffset;
		Toffset = g_rru_entity_table[entity_id]->com_info.time_delay_info.t_offset;
		time_cfg->ie_time_delay_cfg_req.ul_cal_rru = (T14-Toffset)/2 + g_rru_entity_table[entity_id]->com_info.time_delay_info.t2a;
		time_cfg->ie_time_delay_cfg_req.dl_cal_rru = (T14-Toffset)/2 + g_rru_entity_table[entity_id]->com_info.time_delay_info.ta3;

	}


	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)time_cfg, sizeof(MSG_BBU_RRU_TIME_DELAY_CFG_REQ));
	free(time_cfg);

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_TIME_DELAY_CFG_RSP;

	return FUNC_OK;
}



INT32 get_first_rru_id(UINT16 *rru_id)
{
	if(g_rru_entity_table[0] != NULL)
	{
		memcpy(rru_id,&g_rru_entity_table[0]->logic_info.rru_id,sizeof(UINT16));
	}else{
		rru_id = NULL;
	}
	return FUNC_OK;
}

INT32 get_next_rru_id(UINT16 *rru_id)
{
	UINT16 RRU_ID;
	memcpy(&RRU_ID,rru_id,sizeof(UINT16));
	INT32 entity_id;

	if((entity_id = id_to_entity((UINT32)RRU_ID)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	if(g_rru_entity_table[entity_id+1] != NULL)
	{
		memcpy(rru_id,&g_rru_entity_table[0]->logic_info.rru_id,sizeof(UINT16));
	}else{
		rru_id = NULL;
	}
	return FUNC_OK;
}

INT32 check_exist_rru_id(UINT16 rru_id)
{
	INT32 entity_id;

	if((entity_id = id_to_entity((UINT32)rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	return FUNC_OK;
}


INT32 get_cpri_channel_state(UINT16 cpri_index , void *msg)
{
	INT32 entity_id;

	if((entity_id = id_to_cpri_entity(cpri_index)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	memcpy(msg, &g_cpri_entity_table[entity_id]->State,sizeof(UINT32));

	return FUNC_OK;
}

INT32 get_cpri_time_delay_measure(UINT16 cpri_index , void *msg)
{
	INT32 entity_id;

	if((entity_id = id_to_cpri_entity(cpri_index)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	memcpy(msg, &g_cpri_entity_table[entity_id]->T14,sizeof(UINT32));

	return FUNC_OK;
}


INT32 get_cpri_ser_test_result(UINT16 cpri_index ,UINT8 pos, void *msg)
{
	INT32 entity_id;

	if((entity_id = id_to_cpri_entity(cpri_index)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	switch(pos)
	{
		case 1:
		{
			memcpy(msg, &g_cpri_entity_table[entity_id]->total_high,sizeof(UINT32));
			break;
		}
		case 2:
		{
			memcpy(msg, &g_cpri_entity_table[entity_id]->total_low,sizeof(UINT32));
			break;
		}
		case 3:
		{
			memcpy(msg, &g_cpri_entity_table[entity_id]->error_high,sizeof(UINT32));
			break;
		}
		case 4:
		{
			memcpy(msg, &g_cpri_entity_table[entity_id]->error_low,sizeof(UINT32));
			break;
		}
		default:
		{
			log_msg(LOG_ERR, RRUS_MGMT, "not a valid position indicator\n");
			return FUNC_ERR;
		}
	}

	return FUNC_OK;
}

INT32 get_rru_logic_info(UINT16 rru_id, UINT8 pos, void *msg, UINT32 *len)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	/*typedef struct
	{
		UINT32			net_unit_id;
	    UINT32          bbu_id;
		UINT32			fpga_id;
		UINT8			cpri_port;
		UINT8			bbu_ip[16];
		UINT8			rru_ip[16];
		UINT8			rru_mac[18];
		UINT8			sub_mask[16];
	}ICR_RRU_LOGIC_INFO;*/
	switch(pos)
	{
//		case 1:
//		{
//			memcpy(msg, &g_rru_entity_table[entity_id]->logic_info,sizeof(UINT32));
//			break;
//		}
		case 2:
		{
			memcpy(msg, &g_rru_entity_table[entity_id]->logic_info.bbu_id,sizeof(UINT32));
			*len = (UINT32)sizeof(UINT32);
			break;
		}
		case 3:
		{
			memcpy(msg, &g_rru_entity_table[entity_id]->logic_info.fpga_id,sizeof(UINT32));
			*len = (UINT32)sizeof(UINT32);
			break;
		}
		case 4:
		{
			memcpy(msg, &g_rru_entity_table[entity_id]->logic_info.cpri_port,sizeof(UINT8));
			*len = (UINT32)sizeof(UINT8);
			break;
		}
		case 5:
		{
			memcpy(msg, &g_rru_entity_table[entity_id]->logic_info.bbu_ip,sizeof(UINT8)*16);
			*len = (UINT32)(sizeof(UINT8)*16);
			break;
		}
		case 6:
		{
			memcpy(msg, &g_rru_entity_table[entity_id]->logic_info.rru_ip,sizeof(UINT8)*16);
			*len = (UINT32)(sizeof(UINT8)*16);
			break;
		}
		case 7:
		{
			memcpy(msg, &g_rru_entity_table[entity_id]->logic_info.rru_mac,sizeof(UINT8)*18);
			*len = (UINT32)(sizeof(UINT8)*18);
			break;
		}
		case 8:
		{
			memcpy(msg, &g_rru_entity_table[entity_id]->logic_info.sub_mask,sizeof(UINT8)*16);
			*len = (UINT32)(sizeof(UINT8)*16);
			break;
		}
		default:
		{
			log_msg(LOG_ERR, RRUS_MGMT, "not a valid position indicator\n");
			return FUNC_ERR;
		}
	}
	return FUNC_OK;
}

INT32 get_rru_product_info(UINT16 rru_id, UINT8 pos, void *msg, UINT32 *len)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	switch(pos)
	{
		case 1:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.product_info.manufacturer,sizeof(UINT8)*16);
			break;
		}
		case 2:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.product_info.publisher,sizeof(UINT8)*16);
			break;
		}
		case 3:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.product_info.serial_no,sizeof(UINT8)*16);
			break;
		}
		case 4:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.product_info.manufacture_date,sizeof(UINT8)*16);
			break;
		}
		case 5:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.product_info.last_service_date,sizeof(UINT8)*16);
			break;
		}
		case 6:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.product_info.extra_info,sizeof(UINT8)*16);
			break;
		}
		default:
		{
			log_msg(LOG_ERR, RRUS_MGMT, "not a valid position indicator\n");
			return FUNC_ERR;
		}
	}

	*len = (UINT32)(sizeof(UINT8)*16);

	return FUNC_OK;
}

INT32 get_rru_channel_build_reason(UINT16 rru_id, UINT8 pos, void *msg)
{

	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	switch(pos)
	{
		case 1:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.channel_build_info.reason,sizeof(UINT8));
			break;
		}
		case 2:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.channel_build_info.AlarmCode,sizeof(UINT32));
			break;
		}
		default:
		{
			log_msg(LOG_ERR, RRUS_MGMT, "not a valid position indicator\n");
			return FUNC_ERR;
		}
	}
	return FUNC_OK;
}


INT32 get_rru_ability_info(UINT16 rru_id, UINT8 pos, void *msg)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	switch(pos)
	{
		case 1:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.ability_info.td_carrier_num,sizeof(UINT32));
			break;
		}
		case 2:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.ability_info.lte_carrier_num,sizeof(UINT32));
			break;
		}
		case 3:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.ability_info.ant_num,sizeof(UINT8));
			break;
		}
		case 4:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.ability_info.max_power,sizeof(UINT16));
			break;
		}
		case 5:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.ability_info.master_or_slave,sizeof(UINT8));
			break;
		}
		case 6:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.ability_info.max_dw_time_delay,sizeof(UINT32));
			break;
		}
		case 7:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.ability_info.max_up_time_delay,sizeof(UINT32));
			break;
		}
		case 8:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.ability_info.support_mode,sizeof(UINT16));
			break;
		}
		case 9:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.ability_info.ant_clbrt_flag,sizeof(UINT8));
			break;
		}
		case 10:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.ability_info.main_send_eme_path,sizeof(UINT8));
			break;
		}
		case 11:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.ability_info.slave_send_eme_path,sizeof(UINT8));
			break;
		}
		case 12:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.ability_info.main_recv_eme_path,sizeof(UINT8));
			break;
		}
		case 13:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.ability_info.slave_recv_eme_path,sizeof(UINT8));
			break;
		}
		default:
		{
			log_msg(LOG_ERR, RRUS_MGMT, "not a valid position indicator\n");
			return FUNC_ERR;
		}

	}
	return FUNC_OK;

}

INT32 get_rru_ware_info(UINT16 rru_id, UINT8 pos, void *msg, UINT32 *len)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	switch(pos)
	{
		case 1:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.hard_ware_info.hw_type,sizeof(INT8)*32);
			*len = (UINT32)(sizeof(INT8)*32);
			break;
		}
		case 2:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.hard_ware_info.hw_version,sizeof(INT8)*16);
			*len = (UINT32)(sizeof(INT8)*16);
			break;
		}
		case 3:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.soft_ware_info.sw_ver,sizeof(INT8)*40);
			*len = (UINT32)(sizeof(INT8)*40);
			break;
		}
		case 4:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.soft_ware_info.fw_ver,sizeof(INT8)*40);
			*len = (UINT32)(sizeof(INT8)*40);
			break;
		}
		default:
		{
			log_msg(LOG_ERR, RRUS_MGMT, "not a valid position indicator\n");
			return FUNC_ERR;
		}
	}
	return FUNC_OK;
}

//RRU参数查询——CPU占用率查询
INT32 get_rru_param_cpu_usage(UINT16 rru_id, void *msg)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	memcpy(msg,&g_rru_entity_table[entity_id]->com_info.rru_param_info.cpu_usage,sizeof(UINT32));

	return FUNC_OK;
}

//RRU参数查询——CPU占用率统计周期查询
INT32 get_rru_param_cpu_usage_period(UINT16 rru_id, void *msg)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	memcpy(msg,&g_rru_entity_table[entity_id]->com_info.rru_param_info.cpu_usage_period,sizeof(UINT32));

	return FUNC_OK;
}

//RRU参数查询——RRU射频通道温度查询
INT32 get_rru_param_temp_val(UINT16 rru_id, void *msg, UINT32 *len)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	memcpy(msg,&g_rru_entity_table[entity_id]->com_info.rru_param_info.temp_val,sizeof(UINT32)*8);
	*len = (UINT32)(sizeof(UINT32)*8);

	return FUNC_OK;
}

//RRU参数查询——RRU射频通道驻波比状态查询
INT32 get_rru_param_vswr_val(UINT16 rru_id, void *msg, UINT32 *len)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	memcpy(msg,&g_rru_entity_table[entity_id]->com_info.rru_param_info.vswr_val,sizeof(UINT32)*8);
	*len = (UINT32)(sizeof(UINT32)*8);
	return FUNC_OK;
}

//RRU参数查询——驻波比门限查询
INT32 get_rru_param_vswr_thres(UINT16 rru_id, UINT8 pos, void *msg)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	if(pos == 1)
	{
		memcpy(msg,&g_rru_entity_table[entity_id]->com_info.rru_param_info.vswr_thres1,sizeof(UINT32));
	}
	if(pos == 2)
	{
		memcpy(msg,&g_rru_entity_table[entity_id]->com_info.rru_param_info.vswr_thres2,sizeof(UINT32));
	}
	return FUNC_OK;
}


//RRU参数查询——过温门限查询响应
INT32 get_rru_param_temp_thres(UINT16 rru_id, UINT8 pos, void *msg)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	if(pos == 1)
	{
		memcpy(msg,&g_rru_entity_table[entity_id]->com_info.rru_param_info.up_thres,sizeof(INT32));
	}
	if(pos == 2)
	{
		memcpy(msg,&g_rru_entity_table[entity_id]->com_info.rru_param_info.low_thres,sizeof(INT32));
	}

	return FUNC_OK;
}


//RRU参数查询——射频通道输出功率查询响应
INT32 get_rru_param_power_val (UINT16 rru_id, void *msg, UINT32 *len)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	memcpy(msg,&g_rru_entity_table[entity_id]->com_info.rru_param_info.power_val,sizeof(UINT16)*8);
	*len = (UINT32)(sizeof(UINT16)*8);

	return FUNC_OK;
}

//RRU参数查询——状态机查询响应
INT32 get_rru_param_state(UINT16 rru_id, void *msg)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	memcpy(msg,&g_rru_entity_table[entity_id]->com_info.rru_param_info.state,sizeof(UINT8));
	return FUNC_OK;
}

//RRU射频状态查询	CFIG_MGMT通过该消息触发获取RRU射频通道状态
INT32 get_rru_channel_state(UINT16 rru_id, UINT8 pos, void *msg, UINT32 *len)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	if(pos == 1)
	{
		memcpy(msg,&g_rru_entity_table[entity_id]->com_info.rru_state_info.up_path_state,sizeof(UINT32)*8);
		*len = (UINT32)(sizeof(UINT32)*8);
	}

	if(pos == 2)
	{
		memcpy(msg,&g_rru_entity_table[entity_id]->com_info.rru_state_info.dw_path_state,sizeof(UINT32)*8);
		*len = (UINT32)(sizeof(UINT32)*8);
	}

	return FUNC_OK;
}


//RRU时钟状态查询	CFIG_MGMT通过该消息触发获取RRU时钟状态
INT32 get_rru_clock_state(UINT16 rru_id, void *msg)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	memcpy(msg,&g_rru_entity_table[entity_id]->com_info.rru_state_info.clock_state,sizeof(UINT32));
	return FUNC_OK;
}

//RRU时延信息查询	CFIG_MGMT通过该消息触发获取RRU时延信息
INT32 get_rru_time_delay_measure(UINT16 rru_id, UINT8 pos, void *msg)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	switch(pos)
	{
		case 1:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.time_delay_info.t_offset,sizeof(UINT32));
			break;
		}
		case 2:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.time_delay_info.tb_delay_dl,sizeof(UINT32));
			break;
		}
		case 3:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.time_delay_info.tb_delay_ul,sizeof(UINT32));
			break;
		}
		case 4:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.time_delay_info.t2a,sizeof(UINT32));
			break;
		}
		case 5:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.time_delay_info.ta3,sizeof(UINT32));
			break;
		}
		default:
		{
			log_msg(LOG_ERR, RRUS_MGMT, "not a valid position indicator\n");
			return FUNC_ERR;
		}
	}
	return FUNC_OK;
}


//RRU时延配置信息查询	CFIG_MGMT通过该消息触发获取RRU时延配置信息
INT32 get_rru_time_delay_cfg(UINT16 rru_id, UINT8 pos, void *msg)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	switch(pos)
	{
		case 1:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.time_delay_cfg_info.trxa_rxm,sizeof(UINT32));
			break;
		}
		case 2:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.time_delay_cfg_info.trxm_rxa,sizeof(UINT32));
			break;
		}
		case 3:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.time_delay_cfg_info.time_adv,sizeof(UINT32));
			break;
		}
		case 4:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.time_delay_cfg_info.dl_cal_rru,sizeof(UINT32));
			break;
		}
		case 5:
		{
			memcpy(msg,&g_rru_entity_table[entity_id]->com_info.time_delay_cfg_info.ul_cal_rru,sizeof(UINT32));
			break;
		}
		default:
		{
			log_msg(LOG_ERR, RRUS_MGMT, "not a valid position indicator\n");
			return FUNC_ERR;
		}
	}
	return FUNC_OK;
}


