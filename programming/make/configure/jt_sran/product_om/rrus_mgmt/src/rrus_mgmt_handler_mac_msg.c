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
 *File name: rrus_mgmt_handler_mac_msg.c
 *Description: All handlers of messages from mac.
 *
 *Current Version: 0.l
 *Author: zhanghenghao (zhanghenghao@sylincom.com)
 *Date: 2018.08.26
 *
 ************************************************************************/


#include "sbs_type.h"
#include "sbs_log.h"
#include "om_common_h.h"
#include "rrus_mgmt_basic_func.h"
#include "rrus_mgmt_api_mac.h"
#include "rrus_mgmt_rru_entity.h"
#include "rrus_mgmt_common.h"

INT32 handle_imr_cell_build_cfg_req(void* buf, UINT32 len)
{
	//处理小区配置
	log_msg(LOG_INFO, RRUS_MGMT, "handle cell build cfg req.\n");
	INT32 entity_id;

	IMR_CELL_BUILD_CFG *pmsg = (IMR_CELL_BUILD_CFG*)buf;

	UINT32 rru_id;//get by cell_id
	rru_id = pmsg->head.cell_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_BBU_RRU_CELL_CFG_REQ_BUILD *cell_build_req = (MSG_BBU_RRU_CELL_CFG_REQ_BUILD*)malloc(sizeof(MSG_BBU_RRU_CELL_CFG_REQ_BUILD));
	cell_build_req->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_TDLTE_CELL_CFG_REQ);
	cell_build_req->ir_msg_head.rru_id = htonl(rru_id);
	cell_build_req->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_CELL_CFG_REQ_BUILD));
	cell_build_req->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	cell_build_req->ir_msg_head.fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;

	cell_build_req->ie_cell_cfg.ie_flag = htons(IE_FLAG_CELL_CFG_REQ);
	cell_build_req->ie_cell_cfg.ie_len = htons(sizeof(IE_CELL_CFG));
	cell_build_req->ie_cell_cfg.cell_cfg_cmd = pmsg->cell_build_cell.cell_cdg_cmd;
	cell_build_req->ie_cell_cfg.local_cell_id = htonl(pmsg->cell_build_cell.lc_id);
	cell_build_req->ie_cell_cfg.single_carrier_cell_power = htons(pmsg->cell_build_cell.single_carrier_cell_power);
	cell_build_req->ie_cell_cfg.ant_set_no = pmsg->cell_build_cell.ant_set_no;
	cell_build_req->ie_cell_cfg.freq_ie_num = pmsg->cell_build_cell.freq_ie_num;

	cell_build_req->ie_freq_cfg.ie_flag = htons(IE_FLAG_FREQ_CFG_REQ);
	cell_build_req->ie_freq_cfg.ie_len = htons(sizeof(IE_FREQ_CFG));
	cell_build_req->ie_freq_cfg.freq_cfg_cmd = pmsg->cell_build_freq.freq_cdg_cmd;
	cell_build_req->ie_freq_cfg.local_cell_id = htonl(pmsg->cell_build_freq.lc_id);
	cell_build_req->ie_freq_cfg.carrier_no = pmsg->cell_build_freq.carrier_no;
	cell_build_req->ie_freq_cfg.mid_freq = htonl(pmsg->cell_build_freq.mid_freq);
	cell_build_req->ie_freq_cfg.freq_attribute = htonl(pmsg->cell_build_freq.freq_attribute);
	cell_build_req->ie_freq_cfg.ts_info = pmsg->cell_build_freq.tsinfo;
	cell_build_req->ie_freq_cfg.special_subframe_cfg = pmsg->cell_build_freq.special_subframe_cfg;
	cell_build_req->ie_freq_cfg.carrier_width = htonl(pmsg->cell_build_freq.carrier_width);
	cell_build_req->ie_freq_cfg.circ_len_ch = pmsg->cell_build_freq.circ_len_ch;


	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)cell_build_req, sizeof(MSG_BBU_RRU_CELL_CFG_REQ_BUILD));
	free(cell_build_req);

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_CELL_CFG_RSP;


    return FUNC_OK;
}

INT32 handle_imr_cell_delete_req(void* buf, UINT32 len)
{
	log_msg(LOG_INFO, RRUS_MGMT, "handle cell build cfg req.\n");
	INT32 entity_id;

	IMR_CELL_DELETE_REQ *pmsg = (IMR_CELL_DELETE_REQ*)buf;

	UINT32 rru_id;//get by cell_id

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	MSG_BBU_RRU_CELL_CFG_REQ_DELETE *cell_delete_req = (MSG_BBU_RRU_CELL_CFG_REQ_DELETE*)malloc(sizeof(MSG_BBU_RRU_CELL_CFG_REQ_DELETE));
	cell_delete_req->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_TDLTE_CELL_CFG_REQ);
	cell_delete_req->ir_msg_head.rru_id = htonl(rru_id);
	cell_delete_req->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_CELL_CFG_REQ_DELETE));
	cell_delete_req->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	cell_delete_req->ir_msg_head.fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;

	cell_delete_req->ie_cell_cfg.ie_flag = htons(IE_FLAG_CELL_CFG_REQ);
	cell_delete_req->ie_cell_cfg.ie_len = htons(sizeof(IE_CELL_CFG));
	cell_delete_req->ie_cell_cfg.cell_cfg_cmd = 2;
	memcpy(&cell_delete_req->ie_cell_cfg.local_cell_id,&pmsg->lc_id,sizeof(IMR_CELL_DELETE_REQ));

	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)cell_delete_req, sizeof(MSG_BBU_RRU_CELL_CFG_REQ_DELETE));
	free(cell_delete_req);

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_CELL_CFG_RSP;

	return FUNC_OK;
}




