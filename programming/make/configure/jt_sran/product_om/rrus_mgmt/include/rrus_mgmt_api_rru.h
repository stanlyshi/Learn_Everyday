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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:04 CST. */
/************************************************************************
 *Copyright (c) 2018 Sylincom.
 *All rights reserved.
 *
 *File name: rrus_mgmt_api_rru.h
 *Description: the api of rrus_mgmt with rru.
 *
 *Current Version: 0.l
 *Author: zhanghenghao (zhanghenghao@sylincom.com)
 *Date: 2018.09.04
 *
 ************************************************************************/

#ifndef RRUS_MGMT_API_RRU_H
#define RRUS_MGMT_API_RRU_H
/* Dependencies---------------------------------------------------------*/
#include "sbs_type.h"


/* Functions------------------------------------------------------------*/


/////////////////初始化管理//////////////////////////
INT32 handle_msg_rru_ip_req(void* msg);
INT32 send_rru_ip_rsp(UINT32 rru_id);

INT32 send_bbu_hub_to_rrus();

INT32 handle_msg_channel_build_req(UINT32 rru_id, void* msg);

INT32 handle_msg_channel_build_cfg_rsp(UINT32 rru_id, void* msg);

INT32 handle_msg_ver_update_result_ind(UINT32 rru_id, void* msg);

INT32 handle_msg_param_cfg_rsp_init(UINT32 rru_id, void* msg);

INT32 handle_msg_init_cal_result_report(UINT32 rru_id, void* msg);

/////////////////设备管理//////////////////////////
INT32 handle_msg_param_query_req(UINT32 rru_id, void* msg);

INT32 handle_msg_param_query_rsp(UINT32 rru_id, void* msg);

INT32 handle_msg_param_cfg_req(UINT32 rru_id, void* msg);

INT32 handle_msg_param_cfg_rsp(UINT32 rru_id, void* msg);

INT32 handle_msg_status_query_req(UINT32 rru_id, void* msg);

INT32 handle_msg_status_query_rsp(UINT32 rru_id, void* msg);

INT32 handle_msg_ver_query_req(UINT32 rru_id, void* msg);

INT32 handle_msg_ver_query_rsp(UINT32 rru_id, void* msg);

INT32 handle_msg_ver_download_req(UINT32 rru_id, void* msg);

INT32 handle_msg_ver_download_result_ind(UINT32 rru_id, void* msg);

INT32 handle_msg_ver_activate_ind(UINT32 rru_id, void* msg);

INT32 handle_msg_ver_activate_rsp(UINT32 rru_id, void* msg);

/////////////////配置管理//////////////////////////
INT32 handle_msg_ring_test_req(UINT32 rru_id, void* msg);

INT32 handle_msg_ring_test_rsp(UINT32 rru_id, void* msg);

INT32 handle_msg_cell_cfg_req(UINT32 rru_id, void* msg);

INT32 handle_msg_cell_cfg_rsp(UINT32 rru_id, void* msg);

INT32 handle_msg_reset_ind(UINT32 rru_id, void* msg);

INT32 handle_msg_time_delay_measure_req(UINT32 rru_id, void* msg);

INT32 handle_msg_time_delay_measure_rsp(UINT32 rru_id, void* msg);

INT32 handle_msg_time_delay_cfg_req(UINT32 rru_id, void* msg);

INT32 handle_msg_time_delay_cfg_rsp(UINT32 rru_id, void* msg);



INT32 handle_msg_rru_hub(UINT32 rru_id, void *msg);


INT32 handle_msg_alarm_query_req(UINT32 rru_id, void *msg);
INT32 handle_msg_alarm_query_rsp(UINT32 rru_id, void *msg);

INT32 handle_msg_alarm_report(UINT32 rru_id, void *msg);





#endif
