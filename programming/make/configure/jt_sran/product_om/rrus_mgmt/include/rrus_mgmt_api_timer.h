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
 *File name: rrus_mgmt_api_timer.h
 *Description: the all api of rrus_mgmt with timer.
 *
 *Current Version: 0.l
 *Author: 
 *Date: 2018.08.30
 *
 ************************************************************************/

#ifndef RRUS_MGMT_TIMER_API_H
#define RRUS_MGMT_TIMER_API_H

#include "sbs_type.h"
#include "sbs_timer.h"
#include "rrus_mgmt_rru_entity.h"
#include "rrus_mgmt_cpri_entity.h"

/* Message Code of RRUS_MGMT with TIMER */
//#define CODE_TIMER_RRU_HUB_TIMEOUT                  0
//#define CODE_TIMER_BBU_HUB                          1
//#define CODE_TIMER_RRU_CELL_CFG_RSP_TIMEOUT         2
//#define CODE_TIMER_RRU_NORMAL_MSG_RSP_TIMEOUT       3
//#define CODE_TIMER_RRU_PARAM_QUERY                  4
//#define CODE_TIMER_RRU_STATE_QUERY                  5
//#define CODE_TIMER_CPRI_STATE_QUERY                 6
//#define CODE_TIMER_CPRI_MSG_RSP_TIMEOUT             7
typedef enum
{
	CODE_TIMER_RRU_HUB_TIMEOUT,
	CODE_TIMER_BBU_HUB,
	CODE_TIMER_RRU_CELL_CFG_RSP_TIMEOUT,
	CODE_TIMER_RRU_NORMAL_MSG_RSP_TIMEOUT,
	CODE_TIMER_RRU_PARAM_QUERY,
	CODE_TIMER_RRU_STATE_QUERY,
	CODE_TIMER_CPRI_STATE_QUERY,
	CODE_TIMER_CPRI_MSG_RSP_TIMEOUT
}RRUM_TimerType;

typedef struct
{

	RRUM_TimerType		type;
    INT32               seconds;
    INT32               micro_seconds;
    void                (*call_back_fn)(void*);
    void                *data;
}RRU_TimerMsg;


typedef struct 
{
    CommMsgHead     head;
    UINT32          rru_id;
}RRU_HUB_TIMEOUT;

typedef struct 
{
	CommMsgHead     head;
}BBU_HUB;

typedef struct 
{
	CommMsgHead     head;
    UINT32          rru_id;
}RRU_CELL_CFG_RSP_TIMEOUT;

typedef struct 
{
	CommMsgHead     head;
    UINT32          rru_id;
}RRU_NORMAL_MSG_RSP_TIMEOUT;

typedef struct 
{
	CommMsgHead     head;
    UINT32          rru_id;
}RRU_PARAM_QUERY;

typedef struct 
{
	CommMsgHead     head;
    UINT32          rru_id;
}RRU_STATE_QUERY;

typedef struct 
{
	CommMsgHead     head;
    UINT16			cpri_index;
}TIMER_CPRI_STATE_QUERY;

typedef struct 
{
	CommMsgHead     head;
    UINT16			cpri_index;
}CPRI_MSG_RSP_TIMEOUT;

extern struct sbs_timer_list* bbu_hub;
extern struct sbs_timer_list* rru_hub_timeout[MAX_RRU_NUM];
extern struct sbs_timer_list* rru_cell_cfg_rsp_timeout[MAX_RRU_NUM];
extern struct sbs_timer_list* rru_normal_msg_rsp_timeout[MAX_RRU_NUM];
extern struct sbs_timer_list* rru_param_query[MAX_RRU_NUM];
extern struct sbs_timer_list* rru_state_query[MAX_RRU_NUM];
extern struct sbs_timer_list* cpri_state_query[MAX_CPRI_NUM];
extern struct sbs_timer_list* cpri_msg_rsp_timeout[MAX_CPRI_NUM];

//INT32 rrus_mgmt_timer_init();

extern INT32 timer_cb_send_bbu_hub_msg_to_rrus_mgmt();

extern INT32 timer_cb_send_rru_hub_timeout_to_rrus_mgmt(UINT32 *rru_id);

extern INT32 timer_cb_send_rru_cell_cfg_rsp_timeout_to_rrus_mgmt(UINT32 *rru_id);

extern INT32 timer_cb_send_rru_normal_msg_rsp_timeout_to_rrus_mgmt(UINT32 *rru_id);

extern INT32 timer_cb_send_rru_param_query_to_rrus_mgmt(UINT32 *rru_id);

extern INT32 timer_cb_send_rru_state_query_to_rrus_mgmt(UINT32 *rru_id);

extern INT32 timer_cb_send_cpri_state_query_to_rrus_mgmt(UINT16 *cpri_id);

extern INT32 timer_cb_send_cpri_msg_rsp_timeout_to_rrus_mgmt(UINT16 *cpri_id);



extern INT32 handle_timer_bbu_hub_msg(void*, UINT32);

extern INT32 handle_timer_rru_hub_timeout(void*, UINT32);

extern INT32 handle_timer_rru_cell_cfg_rsp_timeout(void*, UINT32);

extern INT32 handle_timer_rru_normal_msg_rsp_timeout(void*, UINT32);

extern INT32 handle_timer_rru_param_query(void*, UINT32);

extern INT32 handle_timer_rru_state_query(void*, UINT32);

extern INT32 handle_timer_cpri_state_query(void*, UINT32);

extern INT32 handle_timer_cpri_msg_rsp_timeout(void*, UINT32);


#endif
