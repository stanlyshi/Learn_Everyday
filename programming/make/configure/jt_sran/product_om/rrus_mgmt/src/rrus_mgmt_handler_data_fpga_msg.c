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
 *File name: rrus_mgmt_handler_data_fpga_msg.c
 *Description: All handlers of messages from data_fpga.
 *
 *Current Version: 0.l
 *Author: zhanghenghao (zhanghenghao@sylincom.com)
 *Date: 2018.08.26
 *
 ************************************************************************/


#include "sbs_type.h"
#include "sbs_log.h"
#include "om_common_h.h"
#include "rrus_mgmt_api_data_fpga.h"
#include "rrus_mgmt_api_timer.h"
#include "rrus_mgmt_cpri.h"
#include "rrus_mgmt_cpri_entity.h"
#include "rrus_mgmt_basic_func.h"
#include "rrus_mgmt_rru_entity.h"

typedef struct
{
	UINT8 port_state;
	UINT8 path_state;
	UINT16 padding;
}CPRI_STATE;

extern INT32 handle_msg_cpri_time_cfg_req(UINT16 cpri_index, void* msg);
extern INT32 handle_msg_time_delay_measure_req(UINT32 rru_id, void* msg);
//extern INT32 send_msg_to_om(UINT16 module_id, UINT8 *msg_p, UINT32 msg_len);

INT32 handle_data_fpga_rru_mac_rsp(UINT16 cpri_index, void* buf)
{
	log_msg(LOG_INFO, RRUS_MGMT,"handle rru mac rsp(%d)\n",cpri_index);
	INT32 entity_id;
	if((entity_id = id_to_cpri_entity(cpri_index)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no cpri entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_RRU_MAC_RSP *rru_mac_rsp = (MSG_RRU_MAC_RSP*)buf;

	printf("rru mac rsp fpga_id:%d\n", ntohl(rru_mac_rsp->msg_head.fpga_id));
	printf("rru mac rsp fiber_num:%d\n", ntohl(rru_mac_rsp->rru_mac_rsp.fiber_num));
	printf("rru mac rsp level:%d\n", ntohl(rru_mac_rsp->rru_mac_rsp.level));

	if(ntohl(rru_mac_rsp->rru_mac_rsp.result) == 0)
	{
		log_msg(LOG_INFO, RRUS_MGMT,"bind rru mac with cpri(%d) success.\n",cpri_index);

	}else{
		log_msg(LOG_ERR, RRUS_MGMT,"bind rru mac with cpri(%d) failed.\n",cpri_index);
		return FUNC_ERR;
	}

	EqIndex *rru_id = (EqIndex*)&cpri_index;

	UINT32 RRU_ID;
	memcpy(&RRU_ID, rru_id, sizeof(UINT32));

	printf("RRU ID received:%d\n",RRU_ID);
//
//	INT32 rru_entity_id;
//	if((rru_entity_id = id_to_entity(RRU_ID)) == -1)
//	{
//		log_msg(LOG_ERR, RRUS_MGMT, "no rru entity in %s.\n", __FUNCTION__);
//		return FUNC_ERR;
//	}

//	send_rru_ip_rsp(RRU_ID);

	 return FUNC_OK;
}


INT32 handle_data_fpga_frnum_rsp(void* buf, UINT32 len)
{
    return FUNC_OK;
}


INT32 handle_data_fpga_cpri_init_cfg_rsp(UINT16 cpri_index, void* buf)
{
	log_msg(LOG_INFO, RRUS_MGMT,"handle cpri init cfg resp(%d)\n",cpri_index);
	INT32 entity_id;
	if((entity_id = id_to_cpri_entity(cpri_index)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no cpri entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	MSG_CPRI_INIT_CFG_RSP *msg_init_resp = (MSG_CPRI_INIT_CFG_RSP*)buf;

	if(ntohl(msg_init_resp->init_cfg_rsp.result) == 0)
	{
		log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) init cfg success\n",cpri_index);

		cpri_state_query[entity_id] = (struct sbs_timer_list*)malloc(sizeof(struct sbs_timer_list));
		cpri_state_query[entity_id]->data = (void*)&g_cpri_entity_table[entity_id]->cpri_index;
		cpri_state_query[entity_id]->cb = (void*)timer_cb_send_cpri_state_query_to_rrus_mgmt;
		sbs_timer_schedule(cpri_state_query[entity_id], 60, 0);

	}else{
		log_msg(LOG_ERR, RRUS_MGMT,"cpri(%d) init config failed\n",cpri_index);
	}

    return FUNC_OK;
}

INT32 handle_data_fpga_axc_channel_cfg_rsp(UINT16 cpri_index, void* buf)
{
	MSG_CPRI_AXC_CHANNEL_CFG_RSP *msg_axc_resp = (MSG_CPRI_AXC_CHANNEL_CFG_RSP*)buf;

	if(ntohl(msg_axc_resp->axc_channel_cfg_rsp.result) == 0)
	{
		log_msg(LOG_INFO, RRUS_MGMT,"axc cfg success\n");
		handle_msg_cpri_time_cfg_req(cpri_index,NULL);
	}else{
		log_msg(LOG_ERR, RRUS_MGMT,"cpri(%d) axc config failed",cpri_index);
	}
    return FUNC_OK;
}

INT32 handle_data_fpga_cpri_state_query_rsp(UINT16 cpri_index, void* buf)
{
	MSG_CPRI_STATE_QUERY_RSP *msg_state_resp = (MSG_CPRI_STATE_QUERY_RSP*)buf;

	CPRI_STATE *pmsg =  (CPRI_STATE*)&msg_state_resp->state_query_rsp.state;
	switch(pmsg->port_state)
	{
		case(CPRISTATEA):
		{
			log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) port state:A\n",cpri_index);
			break;
		}
		case(CPRISTATEB):
		{
			log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) port state:B\n",cpri_index);
			break;
		}
		case(CPRISTATEC):
		{
			log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) port state:C\n",cpri_index);
			break;
		}
		case(CPRISTATED):
		{
			log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) port state:D\n",cpri_index);
			break;
		}
		case(CPRISTATEE):
		{
			log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) port state:E\n",cpri_index);
			break;
		}
		case(CPRISTATEF):
		{
			log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) port state:F\n",cpri_index);
			break;
		}
		case(CPRISTATEG):
		{
			log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) port state:G\n",cpri_index);
			break;
		}
		default:
			log_msg(LOG_ERR, RRUS_MGMT,"cpri(%d) not a valid port state\n",cpri_index);
			break;
	}
	switch(pmsg->path_state)
	{
		case(CPRISTATENORMAL):
		{
			log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) path state normal\n",cpri_index);
			break;
		}
		case(CPRISTATELOS):
		{
			log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) path state LOS\n",cpri_index);
			break;
		}
		case(CPRISTATELOF):
		{
			log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) path state LOF\n",cpri_index);
			break;
		}
	}

    return FUNC_OK;
}

INT32 handle_data_fpga_time_delay_measure_rsp(UINT16 cpri_index, void* buf)
{
	log_msg(LOG_INFO, RRUS_MGMT,"cpri time delay measure resp\n");
	MSG_CPRI_TIME_DELAY_MEASURE_RSP *msg_time_measure = (MSG_CPRI_TIME_DELAY_MEASURE_RSP*)buf;
	INT32 entity_id,rru_entity_id;

	if((entity_id = id_to_cpri_entity(cpri_index)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	g_cpri_entity_table[entity_id]->T14 = msg_time_measure->time_delay_measure_rsp.T14;


	UINT32 RRU_ID = g_cpri_entity_table[entity_id]->RRU_ID_Start;

	if((rru_entity_id = id_to_entity(RRU_ID)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	handle_msg_time_delay_measure_req(RRU_ID, NULL);
    return FUNC_OK;
}

INT32 handle_data_fpga_time_delay_cfg_rsp(UINT16 cpri_index, void* buf)
{
	MSG_CPRI_TIME_DELAY_CFG_RSP *msg_time_cfg_resp = (MSG_CPRI_TIME_DELAY_CFG_RSP*)buf;

	if(ntohl(msg_time_cfg_resp->time_delay_cfg_rsp.result) == 0)
	{
		log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) time delay cfg success\n",cpri_index);
	}else{
		log_msg(LOG_ERR, RRUS_MGMT,"cpri(%d) time delay cfg failed",cpri_index);
	}

    return FUNC_OK;
}

INT32 handle_data_fpga_ring_test_rsp(UINT16 cpri_index, void* buf)
{
	MSG_CPRI_RING_TEST_RSP *msg_ring_resp = (MSG_CPRI_RING_TEST_RSP*)buf;

	if(ntohl(msg_ring_resp->ring_test_rsp.result) == 0)
	{
		log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) ring test success\n",cpri_index);
	}else{
		log_msg(LOG_ERR, RRUS_MGMT,"cpri(%d) ring test failed\n",cpri_index);
	}
    return FUNC_OK;
}

INT32 handle_data_fpga_ser_test_rsp(UINT16 cpri_index, void* buf)
{
	MSG_CPRI_SER_TEST_RSP *msg_ser_resp = (MSG_CPRI_SER_TEST_RSP*)buf;
	log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) ser test result:%d\n", cpri_index, ntohl(msg_ser_resp->ser_test_rsp.total_high));

	return FUNC_OK;
}



INT32 handle_data_fpga_work_mode_cfg_rsp(void* buf, UINT32 len)
{
    return FUNC_OK;
}

INT32 handle_data_fpga_single_send_cfg_rsp(UINT16 cpri_index, void* buf)
{
	MSG_SINGLE_MODE_CFG_RSP *single_rsp = (MSG_SINGLE_MODE_CFG_RSP*)buf;
	if(ntohl(single_rsp->single_mode_cfg_rsp.result) == 0)
	{
		log_msg(LOG_INFO, RRUS_MGMT,"cpri(%d) single send cfg success\n",cpri_index);
	}else{
		log_msg(LOG_ERR, RRUS_MGMT,"cpri(%d) single send cfg failed\n",cpri_index);
	}
    return FUNC_OK;
}

INT32 handle_data_fpga_iq_out_cfg_rsp(void* buf, UINT32 len)
{
    return FUNC_OK;
}

