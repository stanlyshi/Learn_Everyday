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
 *File name: rrus_mgmt_handler_main_ctrl_msg.c
 *Description: All handlers of messages from main_ctrl.
 *
 *Current Version: 0.l
 *Author: zhanghenghao (zhanghenghao@sylincom.com)
 *Date: 2018.08.26
 *
 ************************************************************************/


#include "sbs_type.h"
#include "sbs_log.h"
#include "om_common_h.h"
#include "rrus_mgmt_api_main_ctrl.h"
#include "rrus_mgmt_cpri_entity.h"
#include "rrus_mgmt_cpri.h"
#include "rrus_mgmt_common.h"
#include "rrus_mgmt_basic_func.h"

INT32 handle_imar_cpri_init(void* buf, UINT32 len)
{
	log_msg(LOG_INFO, RRUS_MGMT, "handle cpri init trigger\n");
	INT32 i;
	for (i = 0; i < MAX_CPRI_NUM; i++)
	{
		if(g_cpri_entity_table[i] != NULL)
		{
			handle_msg_cpri_init_cfg(g_cpri_entity_table[i]->cpri_index, NULL);
		}
	}
    return FUNC_OK;
}

INT32 handle_msg_cpri_init_cfg(UINT16 cpri_index, void* msg)
{
	log_msg(LOG_INFO, RRUS_MGMT,"handle cpri init config\n");
	INT32 entity_id;
	UINT32 RRU_ID_START;
	if((entity_id = id_to_cpri_entity(cpri_index)) == FUNC_ERR)
	{
	 	log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
	 	return FUNC_ERR;
	}

	MSG_CPRI_INIT_CFG *msg_cpri_cfg = (MSG_CPRI_INIT_CFG*)malloc(sizeof(MSG_CPRI_INIT_CFG));
	msg_cpri_cfg->msg_head.msg_type = htons(MSG_TYPE_DATA_FPGA_RRUS_MGMT);
	msg_cpri_cfg->msg_head.msg_code = htons(CODE_RRUS_MGMT_DATA_FPGA_CPRI_INIT_CFG);
	msg_cpri_cfg->msg_head.msg_len = htonl(sizeof(MSG_CPRI_INIT_CFG));
	EqIndex *index = (EqIndex*)(&cpri_index);
	msg_cpri_cfg->msg_head.fpga_id = htonl((UINT32)index->fpga_id);
	printf("cpri init fpga_id:%d\n", index->fpga_id);
	msg_cpri_cfg->init_cfg.fiber_num = htonl((UINT32)index->fiber_num);
	printf("cpri init fiber_num:%d\n", index->fiber_num);
	msg_cpri_cfg->init_cfg.cpri_speed = htonl(g_cpri_entity_table[entity_id]->CpriSpeed);
//	msg_cpri_cfg->init_cfg.rru_id_start = htonl(g_cpri_entity_table[entity_id]->RRU_ID_Start);

	index->rru_id = 1;
	memcpy(&RRU_ID_START, index, sizeof(UINT32));
	printf("RRU_ID_START:%0x\n",RRU_ID_START);
	msg_cpri_cfg->init_cfg.rru_id_start = htonl(RRU_ID_START);
	send_msg_mac(msg_cpri_cfg, sizeof(MSG_CPRI_INIT_CFG));
	free(msg_cpri_cfg);

	return FUNC_OK;
}
