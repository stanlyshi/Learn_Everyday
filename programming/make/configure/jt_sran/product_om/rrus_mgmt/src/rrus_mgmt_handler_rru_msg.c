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
 *File name: rrus_mgmt_handler_rru_msg.c
 *Description: All handlers of messages from rru.
 *
 *Current Version: 0.l
 *Author: zhanghenghao (zhanghenghao@sylincom.com)
 *Date: 2018.08.26
 *
 ************************************************************************/


#include "sbs_type.h"
#include "sbs_log.h"
#include "om_common_h.h"
#include "rrus_mgmt_api_rru.h"
#include "rrus_mgmt_api_cfig_mgmt.h"
#include "rrus_mgmt_api_timer.h"
#include "rrus_mgmt_rru_entity.h"
#include "rrus_mgmt_basic_func.h"
#include "rrus_mgmt_common.h"

extern INT32 handle_msg_param_cfg_req(UINT32 rru_id, void* msg);
extern INT32 handle_msg_time_delay_measure_req(UINT32 rru_id, void* msg);
extern INT32 handle_msg_time_delay_cfg_req(UINT32 rru_id, void* msg);
extern INT32 handle_msg_cpri_time_measure_requ(UINT16 cpri_index, void* msg);
extern INT32 send_msg_to_om(UINT16 module_id, UINT8 *msg_p, UINT32 msg_len);


/////////////////心跳&告警/////////////////////
INT32 handle_msg_rru_hub(UINT32 rru_id, void* msg)
{
	//处理RRU在位心跳消息
	log_msg(LOG_INFO, RRUS_MGMT, "handle rru hub.\n");
	INT32 entity_id;
	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no such an entity.\n");
		return FUNC_ERR;
	}

	if(g_rru_entity_table[entity_id]->logic_info.lost_hub_count > 0)
	{
		g_rru_entity_table[entity_id]->logic_info.lost_hub_count--;
	}

	sbs_timer_schedule(rru_hub_timeout[entity_id], 3, 0);
//	printf("current hub lost count:%d\n",g_rru_entity_table[entity_id]->logic_info.lost_hub_count);


    return FUNC_OK;
}



INT32 handle_msg_alarm_query_rsp(UINT32 rru_id, void *msg)
{
	handle_msg_alarm_report(rru_id, msg);
	return FUNC_OK;
}


INT32 handle_msg_alarm_report(UINT32 rru_id, void* msg)
{
	//处理告警上报
	log_msg(LOG_INFO, RRUS_MGMT, "handle alarm report.\n");
	INT32 entity_id;
	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no such an entity.\n");
		return FUNC_ERR;
	}
	//需要的信息存入entity
	MSG_RRU_BBU_ALARM_REPORT *pmsg = (MSG_RRU_BBU_ALARM_REPORT*)msg;
	printf("valid_flag:%d\n",ntohs(pmsg->ie_alarm_report.valid_flag));
	printf("alarm_code:%d\n",ntohl(pmsg->ie_alarm_report.alarm_code));
	printf("alarm_sub_code:%d\n",ntohl(pmsg->ie_alarm_report.alarm_sub_code));
	printf("alarm_state:%d\n",ntohl(pmsg->ie_alarm_report.alarm_state));


    return FUNC_OK;
}


/////////////////初始化//////////////////////////
INT32 handle_msg_rru_ip_req(void* buf)
{
	//处理RRU IP地址请求
	INT32 i;

	RRU_BBU_RRU_IP_REQ *pmsg = (RRU_BBU_RRU_IP_REQ*)buf;

	if(id_to_entity(pmsg->rru_id) == -1)
	{
		log_msg(LOG_INFO, RRUS_MGMT, "handle msg rru ip req.\n");

		for (i = 0; i < MAX_RRU_NUM; i++)
		{
			if(g_rru_entity_table[i] == NULL)
			{
				g_rru_entity_table[i] = (RruEntity*)malloc(sizeof(RruEntity));
				g_ne_id = 1;
				g_rru_entity_table[i]->logic_info.bbu_id = g_ne_id;
				g_rru_entity_table[i]->logic_info.rru_id = pmsg->rru_id;
				printf("RRU ID reported:%0x\n",g_rru_entity_table[i]->logic_info.rru_id);
				memcpy(&g_rru_entity_table[i]->logic_info.rru_mac, &pmsg->rru_mac,sizeof(UINT8)*6);

				RRU_ID *tmp_rru_id= (RRU_ID*)&g_rru_entity_table[i]->logic_info.rru_id;
				g_rru_entity_table[i]->logic_info.fpga_id = (UINT32)tmp_rru_id->chip_id;
				g_rru_entity_table[i]->logic_info.cpri_port = (UINT8)tmp_rru_id->fiber_num;
				g_rru_entity_table[i]->logic_info.rru_level = (UINT8)tmp_rru_id->rru_level;
				memcpy(&g_rru_entity_table[i]->logic_info.rru_ip ,RRU_IP_1,sizeof(UINT8)*16);
				log_msg(LOG_INFO, RRUS_MGMT,"entity num :%d rru_id:%d mac:%s\n",i, g_rru_entity_table[i]->logic_info.rru_id, g_rru_entity_table[i]->logic_info.rru_mac);

				g_rru_entity_table[i]->logic_info.stream_no = 0;

				if(i == MAX_RRU_NUM-1)
				{
					log_msg(LOG_ERR, RRUS_MGMT,"exceed max number of rru req,\n");
					return FUNC_ERR;
				}

				break;
			}
		}
	}else{
		log_msg(LOG_ERR, RRUS_MGMT, "repeated rru ip req.\n");

	}

	//生成响应消息，分配ip
	BBU_RRU_RRU_IP_RSP *msg_ip_rsp = (BBU_RRU_RRU_IP_RSP*)malloc(sizeof(BBU_RRU_RRU_IP_RSP));
	msg_ip_rsp->rru_id = pmsg->rru_id;
//	msg_ip_rsp->bbu_id = htonl(g_ne_id);
	msg_ip_rsp->bbu_id = htonl(1);
	memcpy(&msg_ip_rsp->rru_mac, &pmsg->rru_mac, sizeof(UINT8)*6);

	ip_to_array(msg_ip_rsp->rru_ip, (char*)&RRU_IP_1);
	ip_to_array(msg_ip_rsp->bbu_ip, (char*)&RRU_SERVER_IP);
	ip_to_array(msg_ip_rsp->sub_mask, (char*)&SUB_MASK);
//	ip_to_array(&msg_ip_rsp->bbu_ip, g_ne_ip_root);
//	ip_to_array(&msg_ip_rsp->sub_mask, g_om_ip_netmask);


	send_msg_to_inet(MODULE_RRUS_MGMT, 0, udp_sockfd, 0, (struct sockaddr *)&udp_client, (char*)msg_ip_rsp, sizeof(BBU_RRU_RRU_IP_RSP));
	free(msg_ip_rsp);

	g_rru_entity_table[i]->logic_info.rru_cur_state = STATE_WAIT_CHANNEL_BUILD_REQ;
	log_msg(LOG_INFO, RRUS_MGMT, "sent ip address response with socket:%d\n",udp_sockfd);
//
//	MSG_RRU_MAC_REQ *rru_mac = (MSG_RRU_MAC_REQ*)malloc(sizeof(MSG_RRU_MAC_REQ));
//	rru_mac->msg_head.msg_type = htons(MSG_TYPE_DATA_FPGA_RRUS_MGMT);
//	rru_mac->msg_head.msg_code = htons(CODE_RRUS_MGMT_DATA_FPGA_RRU_MAC_REQ);
//	rru_mac->msg_head.msg_len = htonl(sizeof(MSG_RRU_MAC_REQ));
//
//	rru_mac->msg_head.fpga_id = htonl(g_rru_entity_table[i]->logic_info.fpga_id);
////	rru_mac->msg_head.fpga_id = htonl(1);
//	rru_mac->rru_mac_req.fiber_num = htonl((UINT32)g_rru_entity_table[i]->logic_info.cpri_port);
////	rru_mac->rru_mac_req.fiber_num = htonl(1);
//	rru_mac->rru_mac_req.level = g_rru_entity_table[i]->logic_info.rru_level;
//
//	printf("rru mac req fpga_id:%d\n", g_rru_entity_table[i]->logic_info.fpga_id);
//	printf("rru mac req fiber_num:%d\n", g_rru_entity_table[i]->logic_info.cpri_port);
//	printf("rru mac req level:%d\n", g_rru_entity_table[i]->logic_info.rru_level);
//
//	memcpy(&rru_mac->rru_mac_req.rru_mac,&g_rru_entity_table[i]->logic_info.rru_mac, sizeof(UINT8)*6);
//	send_msg_mac(rru_mac, sizeof(MSG_RRU_MAC_REQ));

//	free(rru_mac);


	return FUNC_OK;
}


//INT32 send_rru_ip_rsp(UINT32 rru_id)
//{
//	INT32 entity_id;
//	if((entity_id = id_to_entity(rru_id)) == -1)
//	{
//		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
//		return FUNC_ERR;
//	}
//
//	//生成响应消息，分配ip
//	BBU_RRU_RRU_IP_RSP *msg_ip_rsp = (BBU_RRU_RRU_IP_RSP*)malloc(sizeof(BBU_RRU_RRU_IP_RSP));
//	msg_ip_rsp->rru_id = htonl(g_rru_entity_table[entity_id]->logic_info.rru_id);
////	msg_ip_rsp->bbu_id = htonl(g_ne_id);
//	msg_ip_rsp->bbu_id = htonl(1);
//	memcpy(&msg_ip_rsp->rru_mac, &g_rru_entity_table[entity_id]->logic_info.rru_mac, sizeof(UINT8)*6);
//
//	ip_to_array(msg_ip_rsp->rru_ip, (char*)&RRU_IP_1);
//	ip_to_array(msg_ip_rsp->bbu_ip, (char*)&RRU_SERVER_IP);
//	ip_to_array(msg_ip_rsp->sub_mask, (char*)&SUB_MASK);
////	ip_to_array(&msg_ip_rsp->bbu_ip, g_ne_ip_root);
////	ip_to_array(&msg_ip_rsp->sub_mask, g_om_ip_netmask);
//
//
//	send_msg_to_inet(MODULE_RRUS_MGMT, 0, udp_sockfd, 0, (struct sockaddr *)&udp_client, (char*)msg_ip_rsp, sizeof(BBU_RRU_RRU_IP_RSP));
//	free(msg_ip_rsp);
//
//	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_CHANNEL_BUILD_REQ;
//	log_msg(LOG_INFO, RRUS_MGMT, "sent ip address response with socket:%d\n",udp_sockfd);
//    return FUNC_OK;
//}




INT32 handle_msg_channel_build_req(UINT32 rru_id, void* msg)
{
	//处理通道建立请求消息
	log_msg(LOG_INFO, RRUS_MGMT, "handle channel build req(RRU:%d).\n",rru_id);
	INT32 entity_id;
	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_RRU_BBU_CHANNEL_BUILD_REQ* pmsg = (MSG_RRU_BBU_CHANNEL_BUILD_REQ*)msg;
	memcpy(&g_rru_entity_table[entity_id]->com_info.product_info, &pmsg->ie_rru_product_info.manufacturer, ntohs(pmsg->ie_rru_product_info.ie_len)-sizeof(UINT16)*2);
	memcpy(&g_rru_entity_table[entity_id]->com_info.channel_build_info, &pmsg->ie_channel_build_reason.reason, ntohs(pmsg->ie_channel_build_reason.ie_len)-sizeof(UINT16)*2);
	memcpy(&g_rru_entity_table[entity_id]->com_info.ability_info, &pmsg->ie_rru_ability.td_carrier_num, ntohs(pmsg->ie_rru_ability.ie_len)-sizeof(UINT16)*2);



	//生成通道建立配置消息
	MSG_BBU_RRU_CHANNEL_BUILD_CFG *msg_channel_build_cfg = (MSG_BBU_RRU_CHANNEL_BUILD_CFG*)malloc(sizeof(MSG_BBU_RRU_CHANNEL_BUILD_CFG));
	memset(msg_channel_build_cfg,0,sizeof(MSG_BBU_RRU_CHANNEL_BUILD_CFG));
	msg_channel_build_cfg->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_CHANNEL_BUILD_CFG);
	msg_channel_build_cfg->ir_msg_head.rru_id = htonl(rru_id);
	msg_channel_build_cfg->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	msg_channel_build_cfg->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_CHANNEL_BUILD_CFG));
	msg_channel_build_cfg->ir_msg_head.stream_no = htonl(g_rru_entity_table[entity_id]->logic_info.stream_no+1);

	msg_channel_build_cfg->ie_system_time.ie_flag = htons(IE_FLAG_SYS_TIME);
	msg_channel_build_cfg->ie_system_time.ie_len = htons(sizeof(IE_SYSTEM_TIME));
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	msg_channel_build_cfg->ie_system_time.second = timeinfo->tm_sec;
	msg_channel_build_cfg->ie_system_time.minute = timeinfo->tm_min;
	msg_channel_build_cfg->ie_system_time.hour = timeinfo->tm_hour;
	msg_channel_build_cfg->ie_system_time.day = timeinfo->tm_mday;
	msg_channel_build_cfg->ie_system_time.month = timeinfo->tm_mon;
	msg_channel_build_cfg->ie_system_time.year = htons(timeinfo->tm_year);

	msg_channel_build_cfg->ie_access_resp_address.ie_flag = htons(IE_FLAG_ACCESS_RSP_ADDR);
	msg_channel_build_cfg->ie_access_resp_address.ie_len = htons(sizeof(IE_ACCESS_RESP_ADDRESS));
	ip_to_array((UINT8*)&msg_channel_build_cfg->ie_access_resp_address.bbu_ftp_ip, (char*)&RRU_SERVER_IP);
//	msg_channel_build_cfg->ie_access_resp_address.bbu_ftp_ip;

	msg_channel_build_cfg->ie_rru_run_mode_set.ie_flag = htons(IE_FLAG_RRU_OPER_MODE_SET);
	msg_channel_build_cfg->ie_rru_run_mode_set.ie_len = htons(sizeof(IE_RRU_RUN_MODE_SET));
	msg_channel_build_cfg->ie_rru_run_mode_set.run_mode = htonl(RRU_MODE_NORMAL);

	msg_channel_build_cfg->ie_soft_ver_check_result.ie_flag = htons(IE_FLAG_SW_VER_REQ);
	msg_channel_build_cfg->ie_soft_ver_check_result.ie_len = htons(sizeof(IE_SOFT_VER_CHECK_RESULT));
	msg_channel_build_cfg->ie_soft_ver_check_result.sw_ver_type = 0;
	//judge the need of ver update, if no need:
	if(memcmp(&pmsg->ie_rru_soft_ver_info.sw_ver,&g_rru_entity_table[entity_id]->com_info.soft_ware_info.sw_ver,sizeof(INT8)*40) != 0)
	{
		msg_channel_build_cfg->ie_soft_ver_check_result.check_result = htonl(RRU_SOFT_VERSION_NOT_SAME);
		//if needs ver update:启动定时器等待版本更新结果指示 RRU_BBU_VER_UPDATE_RESULT_IND
		g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_VER_UPDATE_RESULT_IND;
		rru_normal_msg_rsp_timeout[entity_id] = (struct sbs_timer_list*)malloc(sizeof(struct sbs_timer_list));
		rru_normal_msg_rsp_timeout[entity_id]->data = (void*)&g_rru_entity_table[entity_id]->logic_info.rru_id;;
		rru_normal_msg_rsp_timeout[entity_id]->cb = (void*)timer_cb_send_rru_normal_msg_rsp_timeout_to_rrus_mgmt;
		sbs_timer_schedule(rru_normal_msg_rsp_timeout[entity_id], 9, 0);
	}else{
		msg_channel_build_cfg->ie_soft_ver_check_result.check_result = htonl(RRU_SOFT_VERSION_SAME);
	}
//	msg_channel_build_cfg->ie_soft_ver_check_result.file_path
//	msg_channel_build_cfg->ie_soft_ver_check_result.file_name
//	msg_channel_build_cfg->ie_soft_ver_check_result.file_len
//	msg_channel_build_cfg->ie_soft_ver_check_result.file_time
//	msg_channel_build_cfg->ie_soft_ver_check_result.file_ver

	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)msg_channel_build_cfg, sizeof(MSG_BBU_RRU_CHANNEL_BUILD_CFG));
	free(msg_channel_build_cfg);

	log_msg(LOG_INFO, RRUS_MGMT, "sent channel build cfg req(RRU:%d).\n",rru_id);
	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_CHANNEL_BUILD_CFG_RSP;
	return FUNC_OK;
}

INT32 handle_msg_channel_build_cfg_rsp(UINT32 rru_id, void* msg)
{
	//处理通道配置响应消息
	log_msg(LOG_INFO, RRUS_MGMT, "handle channel build cfg rsp(RRU:%d).\n",rru_id);
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}


	MSG_RRU_BBU_CHANNEL_BUILD_CFG_RSP* pmsg = (MSG_RRU_BBU_CHANNEL_BUILD_CFG_RSP*)msg;

	//CfgResult为0，成功:
	if(ntohl(pmsg->ie_channel_build_cfg_ret.cfg_result) == CHANNEL_BUILD_CFG_FAILED)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "channel build cfg rsp failed\n", __FUNCTION__);
		return FUNC_ERR;
	}

//
	//BBU收到通道建立配置应答消息后发起心跳，间隔3s
	if(g_rru_entity_table[0] != NULL && g_rru_entity_table[1] == NULL)
	{
		bbu_hub = (struct sbs_timer_list*)malloc(sizeof(struct sbs_timer_list));
		bbu_hub->data = NULL;
		bbu_hub->cb = (void*)timer_cb_send_bbu_hub_msg_to_rrus_mgmt;
		sbs_timer_schedule(bbu_hub, 3, 0);
	}

	g_rru_entity_table[entity_id]->logic_info.lost_hub_count = 0;

	//触发该RRU的心跳超时定时器
	rru_hub_timeout[entity_id] = (struct sbs_timer_list*)malloc(sizeof(struct sbs_timer_list));
	rru_hub_timeout[entity_id]->data = (void*)&g_rru_entity_table[entity_id]->logic_info.rru_id;
	rru_hub_timeout[entity_id]->cb = (void*)timer_cb_send_rru_hub_timeout_to_rrus_mgmt;
	sbs_timer_schedule(rru_hub_timeout[entity_id], 3, 0);


	//RRU level equals to 1
//	if(g_rru_entity_table[entity_id]->logic_info.rru_level == 1)
//	{
//		handle_msg_cpri_time_measure_requ(g_rru_entity_table[entity_id]->logic_info.cpri_index,NULL);
//	}else{
//		handle_msg_time_delay_measure_req(rru_id, NULL);
//	}

	handle_msg_param_cfg_req(rru_id,NULL);

	return FUNC_OK;
}


/////////////////设备管理//////////////////////////

INT32 handle_msg_param_query_rsp(UINT32 rru_id, void* msg)
{
	//处理参数查询响应
	log_msg(LOG_INFO, RRUS_MGMT, "handle param query rsp(RRU:%d).\n",rru_id);
	INT32 entity_id;
	UINT16 IE_len;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}


	IrMsgHead *tmpHead = (IrMsgHead*)malloc(sizeof(IrMsgHead));
	memcpy(tmpHead, msg, sizeof(IrMsgHead));
	IE_len = ntohl(tmpHead->msg_len) - sizeof(IrMsgHead);
	void *cur_p = msg + sizeof(IrMsgHead);
	IEHead *tmpIEHead = (IEHead*)malloc(sizeof(IEHead));
	while(IE_len > 0)
	{
		memcpy(tmpIEHead, cur_p, sizeof(IEHead));
		printf("param query rsp IE code:%d\n",htons(tmpIEHead->ie_flag));
		char ie[htons(tmpIEHead->ie_len)];
		memcpy(ie,cur_p, sizeof(ie));
		ie_query_rsp(entity_id,htons(tmpIEHead->ie_flag),ie);
		cur_p = cur_p + htons(tmpIEHead->ie_len);
		IE_len = IE_len - htons(tmpIEHead->ie_len);
	}

	free(tmpHead);
	free(tmpIEHead);

	return FUNC_OK;

}

INT32 handle_msg_param_cfg_rsp(UINT32 rru_id, void* msg)
{
	log_msg(LOG_INFO, RRUS_MGMT, "handle param cfg rsp(RRU:%d).\n",rru_id);
	INT32 entity_id;
	UINT16 IE_len;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}



	IrMsgHead *tmpHead = (IrMsgHead*)malloc(sizeof(IrMsgHead));
	memcpy(tmpHead, msg, sizeof(IrMsgHead));
	IE_len = ntohl(tmpHead->msg_len) - sizeof(IrMsgHead);
	IEHead *tmpIEHead = (IEHead*)malloc(sizeof(IEHead));
	void *cur_p = msg + sizeof(IrMsgHead);
	while(IE_len > 0)
	{

		memcpy(tmpIEHead, cur_p, sizeof(IEHead));
		printf("processed the parameter cfg with IE code:%d\n",htons(tmpIEHead->ie_flag));
		char ie[htons(tmpIEHead->ie_len)];
		memcpy(ie,cur_p, sizeof(ie));
		if(ie_cfg_rsp(htons(tmpIEHead->ie_flag),ie) == 1)
		{
			log_msg(LOG_ERR, RRUS_MGMT, "param cfg failed(%d).\n", htons(tmpIEHead->ie_flag),__FUNCTION__);
		}
		cur_p = cur_p + htons(tmpIEHead->ie_len);
		IE_len = IE_len - htons(tmpIEHead->ie_len);
	}

	free(tmpHead);
	free(tmpIEHead);



//	//BBU收到通道建立配置应答消息后发起心跳，间隔3s
//	if(g_rru_entity_table[0] != NULL && g_rru_entity_table[1] == NULL)
//	{
//		bbu_hub = (struct sbs_timer_list*)malloc(sizeof(struct sbs_timer_list));
//		bbu_hub->data = NULL;
//		bbu_hub->cb = (void*)timer_cb_send_bbu_hub_msg_to_rrus_mgmt;
//		sbs_timer_schedule(bbu_hub, 3, 0);
//	}
//
//	g_rru_entity_table[entity_id]->logic_info.lost_hub_count = 0;
//
//	//触发该RRU的心跳超时定时器
//	rru_hub_timeout[entity_id] = (struct sbs_timer_list*)malloc(sizeof(struct sbs_timer_list));
//	rru_hub_timeout[entity_id]->data = (void*)&g_rru_entity_table[entity_id]->logic_info.rru_id;
//	rru_hub_timeout[entity_id]->cb = (void*)timer_cb_send_rru_hub_timeout_to_rrus_mgmt;
//	sbs_timer_schedule(rru_hub_timeout[entity_id], 3, 0);


	handle_msg_time_delay_measure_req(rru_id, NULL);


	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_TIME_DELAY_MEASURE_RSP;

	//send rru register msg to cell module
	handle_msg_rru_reg(rru_id, 0);

	//send trap msg to cfig module
	ICR_RRU_INIT_RESULT_TRAP *init_trap = (ICR_RRU_INIT_RESULT_TRAP*)malloc(sizeof(ICR_RRU_INIT_RESULT_TRAP));
	init_trap->head.msg_type = MODULE_RRUS_MGMT;
	init_trap->head.msg_code = TRAP_RRU_INIT_RESULT;
	init_trap->head.msg_len = sizeof(ICR_RRU_INIT_RESULT_TRAP);

	init_trap->rru_id = (UINT16)rru_id;
	memcpy(&init_trap->serial_no, &g_rru_entity_table[entity_id]->com_info.product_info.serial_no, sizeof(INT8)*16);
	if(send_msg_to_om(MODULE_CFIG_MGMT, (UINT8*)init_trap, sizeof(ICR_RRU_INIT_RESULT_TRAP)) == FUNC_ERR)
	{
		log_msg(LOG_INFO, RRUS_MGMT, "sent rru init to cfig mgmt failed\n");

	}
	free(init_trap);

	return FUNC_OK;
}



INT32 handle_msg_status_query_rsp(UINT32 rru_id, void* msg)
{
	//处理状态查询响应
	log_msg(LOG_INFO, RRUS_MGMT, "handle status query rsp(RRU:%d).\n",rru_id);
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}


	//需要的信息存入entity
	MSG_RRU_BBU_STATUS_QUERY_RSP* pmsg = (MSG_RRU_BBU_STATUS_QUERY_RSP*)msg;
	g_rru_entity_table[entity_id]->com_info.rru_state_info.up_path_state[pmsg->ie_rf_path_state_query_rsp.rf_path_no] = pmsg->ie_rf_path_state_query_rsp.up_path_state;
	g_rru_entity_table[entity_id]->com_info.rru_state_info.clock_state = ntohs(pmsg->ie_system_clock_state_rsp.state);


	return FUNC_OK;

}


/////////////////版本管理//////////////////////////
INT32 handle_msg_ver_query_rsp(UINT32 rru_id, void* msg)
{

	//处理版本查询响应
	log_msg(LOG_INFO, RRUS_MGMT, "handle ver query rsp(RRU:%d).\n",rru_id);
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_RRU_BBU_VER_QUERY_RSP* pmsg = (MSG_RRU_BBU_VER_QUERY_RSP*)msg;
	//size
	if(memcmp(&pmsg->ie_rru_soft_ver_info.sw_ver,&g_rru_entity_table[entity_id]->com_info.soft_ware_info.sw_ver,sizeof(INT8)*40) != 0)
	{
		log_msg(LOG_INFO, RRUS_MGMT, "different software version.\n");
		handle_msg_ver_download_req(rru_id,NULL);
		g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_WAIT_VER_DOWNLOAD_RSP;
		return FUNC_OK;
	}

	log_msg(LOG_INFO, RRUS_MGMT, "no need to update.\n");
	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_RRU_NORMAL;
	return FUNC_OK;
}

INT32 handle_msg_ver_download_rsp(UINT32 rru_id, void* msg)
{
	//处理版本下载
	log_msg(LOG_INFO, RRUS_MGMT, "handle ver download rsp(RRU:%d).\n",rru_id);
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	//判断
	MSG_RRU_BBU_VER_DOWNLOAD_RSP* pmsg = (MSG_RRU_BBU_VER_DOWNLOAD_RSP*)msg;
	if(ntohl(pmsg->ie_ver_download_rsp.download_result) == VER_REJECT_DOWNLOAD_OTHER)
	{
		log_msg(LOG_ERR, RRUS_MGMT, " software cannot be download.%s.\n", __FUNCTION__);

	}
	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_RRU_NORMAL;

	return FUNC_OK;
}

INT32 handle_msg_ver_download_result_ind(UINT32 rru_id, void* msg)
{
	//处理版本下载结果指示
	log_msg(LOG_INFO, RRUS_MGMT, "handle ver download result ind(RRU:%d).\n",rru_id);
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	//判断
	MSG_RRU_BBU_VER_DOWNLOAD_RESULT_IND* pmsg = (MSG_RRU_BBU_VER_DOWNLOAD_RESULT_IND*)msg;

	switch(ntohl(pmsg->ie_ver_download_result_ind.download_result))
	{
		case(VER_DOWNLOAD_FINISH):
		{
			log_msg(LOG_INFO, RRUS_MGMT, "VER DOWNLOAD FINISHED\n");
			break;
		}
		case(VER_FILE_NOT_EXIST):
		{
			log_msg(LOG_ERR, RRUS_MGMT, "VER FILE NOT EXIST\n");
			break;
		}
		case(VER_DOWNLOAD_TIMEOUT):
		{
			log_msg(LOG_ERR, RRUS_MGMT, "VER DOWNLOAD TIMEOUT\n");
			break;
		}
		case(VER_FILE_TOO_LARGE):
		{
			log_msg(LOG_ERR, RRUS_MGMT, "VER FILE TOO LARGE\n");
			break;
		}
		default:
			log_msg(LOG_ERR, RRUS_MGMT, "Not a valid result\n", __FUNCTION__);
			return FUNC_ERR;
	}

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_RRU_NORMAL;
	return FUNC_OK;
}


INT32 handle_msg_ver_update_result_ind(UINT32 rru_id, void* msg)
{
	//处理版本更新结果指示，发送空消息
	log_msg(LOG_INFO, RRUS_MGMT, "handle ver update result ind(RRU:%d).\n",rru_id);
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_RRU_BBU_VER_UPDATE_RESULT_IND* pmsg = (MSG_RRU_BBU_VER_UPDATE_RESULT_IND*)msg;

	switch(ntohl(pmsg->ie_ver_update_result_ind.update_result))
	{
		case(VER_UPDATE_SUCCEED):
		{
			log_msg(LOG_INFO, RRUS_MGMT, "VER UPDATE FINISHED\n");
			break;
		}
		case(FILE_NOT_EXSIT):
		{
			log_msg(LOG_ERR, RRUS_MGMT, "FILE NOT EXIST\n");
			break;
		}
		case(VER_UPDATE_TIMEOUT):
		{
			log_msg(LOG_ERR, RRUS_MGMT, "VER UPDATE TIMEOUT\n");
			break;
		}
		case(FILE_TOO_LARGE):
		{
			log_msg(LOG_ERR, RRUS_MGMT, "FILE TOO LARGE\n");
			break;
		}
		default:
			log_msg(LOG_ERR, RRUS_MGMT, "Not a valid result\n", __FUNCTION__);
			return FUNC_ERR;
	}
	//received this message->delete the rsp timer
//	sbs_timer_del(rru_rsp_timeout_timer[entity_id]);
//	free(rru_rsp_timeout_timer[entity_id]);

	//生成版本更新结果响应消息
	MSG_BBU_RRU_VER_UPDATE_RESULT_RSP *msg_ver_update_result_rsp = (MSG_BBU_RRU_VER_UPDATE_RESULT_RSP*)malloc(sizeof(MSG_BBU_RRU_VER_UPDATE_RESULT_RSP));
	msg_ver_update_result_rsp->ir_msg_head.rru_id = htonl(rru_id);
	msg_ver_update_result_rsp->ir_msg_head.msg_code = htonl(MSG_CODE_BBU_RRU_VER_UPDATE_RESULT_RSP);
	msg_ver_update_result_rsp->ir_msg_head.msg_len = htonl(sizeof(MSG_BBU_RRU_VER_UPDATE_RESULT_RSP));
	msg_ver_update_result_rsp->ir_msg_head.bbu_id = htonl(g_rru_entity_table[entity_id]->logic_info.bbu_id);
	msg_ver_update_result_rsp->ir_msg_head.fiber_port = g_rru_entity_table[entity_id]->logic_info.cpri_port;
	msg_ver_update_result_rsp->ir_msg_head.stream_no = pmsg->ir_msg_head.stream_no;


	struct sockaddr_in rruaddr;
	inet_aton((char*)&g_rru_entity_table[entity_id]->logic_info.rru_ip,&rruaddr.sin_addr);
	send_msg_to_inet(MODULE_RRUS_MGMT, 1, 0, rruaddr.sin_addr.s_addr, NULL, (char*)msg_ver_update_result_rsp, sizeof(MSG_BBU_RRU_VER_UPDATE_RESULT_RSP));
	free(msg_ver_update_result_rsp);
	log_msg(LOG_INFO, RRUS_MGMT, "ver update finished, waiting RRU restart\n");
	handle_msg_rru_reset(rru_id, NULL);

	return FUNC_OK;
}

INT32 handle_msg_ver_activate_rsp(UINT32 rru_id, void* msg)
{
	//处理版本激活响应
	//实现，建议重启。
	log_msg(LOG_INFO, RRUS_MGMT, "handle ver activate rsp(RRU:%d).\n",rru_id);
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	MSG_RRU_BBU_VER_ACTIVATE_RSP* pmsg = (MSG_RRU_BBU_VER_ACTIVATE_RSP*)msg;
	switch(ntohl(pmsg->ie_ver_activate_rsp.activate_result))
	{
		case(VER_ACTIVATE_SUCCEED):
		{
			log_msg(LOG_INFO, RRUS_MGMT, "VER ACTIVATE SUCCESSED\n");
			break;
		}
		case(VER_ACTIVATE_NOT_EXIST):
		{
			log_msg(LOG_ERR, RRUS_MGMT, "VER ACTIVATE NOT EXIST\n");
			break;
		}
		case(VER_ACTIVATE_FILE_BROKEN):
		{
			log_msg(LOG_ERR, RRUS_MGMT, "VER ACTIVATE FILE BROKEN\n");
			break;
		}
		case(VER_ACTIVATE_SOFT_HARD_MISMATCH):
		{
			log_msg(LOG_ERR, RRUS_MGMT, "VER SOFT/HARD MISMATCH\n");
			break;
		}
		case(VER_ACTIVATE_FAILED):
		{
			log_msg(LOG_ERR, RRUS_MGMT, "VER ACTIVATE FAILED\n");
			break;
		}
		default:
			log_msg(LOG_ERR, RRUS_MGMT, "Not a valid result\n", __FUNCTION__);
			return FUNC_ERR;
	}

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_RRU_NORMAL;
	handle_msg_rru_reset(rru_id, NULL);
	return FUNC_OK;
}

/////////////////配置管理//////////////////////////

INT32 handle_msg_ring_test_rsp(UINT32 rru_id, void* msg)
{
	//处理回环测试响应
	log_msg(LOG_INFO, RRUS_MGMT, "handle ring test rsp(RRU:%d).\n",rru_id);
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_RRU_BBU_RING_TEST_RSP* pmsg = (MSG_RRU_BBU_RING_TEST_RSP*)msg;
	//判断
	if(ntohl(pmsg->ie_ring_test_rsp.ring_test_result) == RING_TEST_FAILED)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "ring test failed %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	log_msg(LOG_INFO, RRUS_MGMT, "ring test success %s.\n", __FUNCTION__);
	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_RRU_NORMAL;
	return FUNC_OK;

}


INT32 handle_msg_cell_cfg_rsp(UINT32 rru_id, void* msg)
{
	//处理小区配置响应消息
	//判断小区配置是否成功
	//找到小区实体存储信息
	log_msg(LOG_INFO, RRUS_MGMT, "handle cell cfg rsp(RRU:%d).\n",rru_id);
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_RRU_BBU_CELL_CFG_RSP_BUILD* pmsg = (MSG_RRU_BBU_CELL_CFG_RSP_BUILD*)msg;
	if(ntohl(pmsg->ie_cell_cfg_rsp.result) == CELL_CFG_FAILED)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "cell cfg failed %s.\n", __FUNCTION__);
	}
	if(ntohl(pmsg->ie_freq_cfg_rsp.result) == FREQ_CFG_FAILED)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "freq cfg failed %s.\n", __FUNCTION__);
	}

	log_msg(LOG_INFO, RRUS_MGMT, "cell cfg success %s.\n", __FUNCTION__);
	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_RRU_NORMAL;
	return FUNC_OK;

}


INT32 handle_msg_time_delay_measure_rsp(UINT32 rru_id, void* msg)
{
	//处理时延测量响应
	log_msg(LOG_INFO, RRUS_MGMT, "handle time delay measure rsp(RRU:%d).\n",rru_id);
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	//需要的信息存入entity
	MSG_RRU_BBU_TIME_DELAY_MEASURE_RSP* pmsg = (MSG_RRU_BBU_TIME_DELAY_MEASURE_RSP*)msg;
	memcpy(&g_rru_entity_table[entity_id]->com_info.time_delay_info,&pmsg->ie_time_delay_rsp.fiber_no,ntohs(pmsg->ie_time_delay_rsp.ie_len));

	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_RRU_NORMAL;
	handle_msg_time_delay_cfg_req(rru_id,NULL);

	return FUNC_OK;
}

INT32 handle_msg_time_delay_cfg_rsp(UINT32 rru_id, void* msg)
{
	//判断时延配置命令是否成功
	log_msg(LOG_INFO, RRUS_MGMT, "handle time delay cfg rsp(RRU:%d).\n",rru_id);
	INT32 entity_id;

	if((entity_id = id_to_entity(rru_id)) == -1)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "no entity in %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}

	MSG_RRU_BBU_TIME_DELAY_CFG_RSP* pmsg = (MSG_RRU_BBU_TIME_DELAY_CFG_RSP*)msg;
	//判断 result
	if(ntohl(pmsg->ie_time_delay_cfg_rsp.cfg_result) == TIME_DELAY_CFG_FAILED)
	{
		log_msg(LOG_ERR, RRUS_MGMT, "time delay cfg failed %s.\n", __FUNCTION__);
		return FUNC_ERR;
	}
	g_rru_entity_table[entity_id]->logic_info.rru_cur_state = STATE_RRU_NORMAL;
	//get from cfg mgmt

	rru_state_query[entity_id] = (struct sbs_timer_list*)malloc(sizeof(struct sbs_timer_list));
	rru_state_query[entity_id]->data = (void*)&g_rru_entity_table[entity_id]->logic_info.rru_id;
	rru_state_query[entity_id]->cb = (void*)timer_cb_send_rru_state_query_to_rrus_mgmt;
	sbs_timer_schedule(rru_state_query[entity_id], 60, 0);

	rru_param_query[entity_id] = (struct sbs_timer_list*)malloc(sizeof(struct sbs_timer_list));
	rru_param_query[entity_id]->data = (void*)&g_rru_entity_table[entity_id]->logic_info.rru_id;
	rru_param_query[entity_id]->cb = (void*)timer_cb_send_rru_param_query_to_rrus_mgmt;
	sbs_timer_schedule(rru_param_query[entity_id], 60, 0);

	return FUNC_OK;
}


