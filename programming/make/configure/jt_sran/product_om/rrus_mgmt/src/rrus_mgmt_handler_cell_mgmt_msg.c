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
 *File name: rrus_mgmt_handler_cell_mgmt_msg.c
 *Description: All handlers of messages from cell_mgmt.
 *
 *Current Version: 0.l
 *Author: zhanghenghao (zhanghenghao@sylincom.com)
 *Date: 2018.08.26
 *
 ************************************************************************/



#include "rrus_mgmt_api_cell_mgmt.h"
#include "sbs_type.h"
#include "om_common_h.h"
#include "sbs_log.h"
#include "rrus_mgmt_cpri.h"
#include "rrus_mgmt_common.h"
#include "rrus_mgmt_basic_func.h"
#include "rrus_mgmt_rru_entity.h"

extern INT32 send_msg_to_om(UINT16 module_id, UINT8 *msg_p, UINT32 msg_len);

INT32 handle_icer_cpri_axc_cfg(void* buf, int len)
{
	log_msg(LOG_INFO, RRUS_MGMT,"handle cpri AxC config\n");
	ICER_CPRI_AXC_CFG *pmsg = (ICER_CPRI_AXC_CFG*)buf;


	MSG_CPRI_AXC_CHANNEL_CFG *msg_axc_cfg = (MSG_CPRI_AXC_CHANNEL_CFG*)malloc(sizeof(MSG_CPRI_AXC_CHANNEL_CFG));
	msg_axc_cfg->msg_head.msg_type = htons(MSG_TYPE_DATA_FPGA_RRUS_MGMT);
	msg_axc_cfg->msg_head.msg_code = htons(CODE_RRUS_MGMT_DATA_FPGA_AXC_CHANNEL_CFG);
	msg_axc_cfg->msg_head.msg_len = htonl(sizeof(MSG_CPRI_AXC_CHANNEL_CFG));
	EqIndex *index = (EqIndex*)(&pmsg->rru_id);
	msg_axc_cfg->msg_head.fpga_id = htonl((UINT32)index->fpga_id);
	msg_axc_cfg->axc_channel_cfg.fiber_num = htonl((UINT32)index->fiber_num);
	msg_axc_cfg->axc_channel_cfg.axc_speed = htonl(pmsg->axc_speed);
	msg_axc_cfg->axc_channel_cfg.axc_ind = htonl(1);
//	if(pmsg->cfg_ind == 0)
//	{
//		msg_axc_cfg->axc_channel_cfg.axc_ind = 0;
//	}else{
////		msg_axc_cfg->axc_channel_cfg.axc_ind = 1;
////		need to be reconfirmed
//	}

	send_msg_mac(msg_axc_cfg,sizeof(MSG_CPRI_AXC_CHANNEL_CFG));
	free(msg_axc_cfg);
    return FUNC_OK;
}


INT32 handle_msg_rru_reg(UINT32 rru_id, UINT32 ind)
{
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	ICER_RRU_REG *rru_reg = (ICER_RRU_REG*)malloc(sizeof(ICER_RRU_REG));
	rru_reg->head.msg_type = MODULE_RRUS_MGMT;
	rru_reg->head.msg_code = CODE_ICER_RRU_REG_REQ;
	rru_reg->head.msg_len = sizeof(ICER_RRU_REG);

	memcpy(&rru_reg->rru_sn, g_rru_entity_table[entity_id]->com_info.product_info.serial_no, sizeof(INT8)*16);

	rru_reg->rru_id = rru_id;
	rru_reg->rru_reg = ind;

	if(send_msg_to_om(MODULE_CELL_MGMT, (UINT8*)rru_reg, sizeof(ICER_RRU_REG)) == FUNC_ERR)
	{
		log_msg(LOG_INFO, RRUS_MGMT, "sent rru init to cfig mgmt failed\n");

	}
	free(rru_reg);

    return FUNC_OK;
}


