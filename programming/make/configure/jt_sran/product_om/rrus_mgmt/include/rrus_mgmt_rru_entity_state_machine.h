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
 *File name: rrus_mgmt_rru_entity_state_machine.h
 *Description: the state of rru.
 *
 *Current Version: 0.l
 *Author: WangCheng (wangcheng2017@ict.ac.cn)
 *Date: 2018.06.05
 *
 ************************************************************************/

#ifndef RRU_ENTITY_STATE_MACHINE_H
#define RRU_ENTITY_STATE_MACHINE_H

/* Dependencies ---------------------------------------------------------- */
#include "sbs_type.h"

/* Declarations----------------------------------------------------------- */


typedef INT32 (*IrMsgHandleFn)(UINT32 RruId, void*);


typedef struct
{
    char            *state_name;
    char            *event_name;
    IrMsgHandleFn   p_handle_fn;
}RruStateTable;

typedef enum
{
    STATE_WAIT_CHANNEL_BUILD_REQ = 1,
    STATE_WAIT_CHANNEL_BUILD_CFG_RSP,
    STATE_WAIT_VER_UPDATE_RESULT_IND,
    STATE_RRU_NORMAL,
    STATE_WAIT_RRU_VER_QUERY_RSP,
    STATE_WAIT_VER_DOWNLOAD_RSP,
    STATE_WAIT_VER_ACTIVATE_RSP,
    STATE_WAIT_STATUS_QUERY_RSP,
    STATE_WAIT_PARAM_QUERY_RSP,
    STATE_WAIT_PARAM_CFG_RSP,
    STATE_WAIT_TIME_DELAY_MEASURE_RSP,
    STATE_WAIT_TIME_DELAY_CFG_RSP,
    STATE_WAIT_ALARM_QUERY_RSP,
    STATE_WAIT_RING_TEST_RSP,
    STATE_WAIT_CELL_CFG_RSP,
    RRU_STATE_MAX
}RruStateMachine;

typedef enum
{
    EVENT_CHANNEL_BUILD_REQ = 1,
    EVENT_CHANNEL_BUILD_CFG_RSP,
    EVENT_VER_UPDATE_RESULT_IND,
    EVENT_VER_QUERY_REQ,
    EVENT_VER_QUERY_RSP,
    EVENT_VER_DOWNLOAD_REQ,
    EVENT_VER_DOWNLOAD_RESULT_IND,
    EVENT_VER_ACTIVATE_IND,
    EVENT_VER_ACTIVATE_RSP,
    EVENT_STATUS_QUERY_REQ,
    EVENT_STATUS_QUERY_RSP,
    EVENT_PARAM_QUERY_REQ,
    EVENT_PARAM_QUERY_RSP,
    EVENT_PARAM_CFG_REQ,
    EVENT_PARAM_CFG_RSP,
    EVENT_INIT_CAL_RESULT_REPORT,
    EVENT_TIME_DELAY_MEASURE_REQ,
    EVENT_TIME_DELAY_MEASURE_RSP,
    EVENT_TIME_DELAY_CFG_REQ,
    EVENT_TIME_DELAY_CFG_RSP,
    EVENT_ALARM_REPORT,
    EVENT_ALARM_QUERY_REQ,
    EVENT_ALARM_QUERY_RSP,
    EVENT_RESET_IND,
    EVENT_RING_TEST_REQ,
    EVENT_RING_TEST_RSP,
    EVENT_CELL_CFG_REQ,
    EVENT_CELL_CFG_RSP,
    RRU_EVENT_MAX
}RruEventType;

extern RruStateTable g_rru_state_table[RRU_STATE_MAX][RRU_EVENT_MAX];
extern char* rru_event_name[RRU_EVENT_MAX];
extern char* rru_state_name[RRU_STATE_MAX];


/* Functions ------------------------------------------------------------- */

extern INT32 add_rru_state_table_entry(RruStateMachine rru_state, RruEventType event,
								IrMsgHandleFn *handle_fn, UINT8 *state_name, UINT8 *event_name);


/**********************************************
** init RRU state machine.
**
** Input:
** 		None.
** Output:
** 		None.
** Return:
**      success: 0
**      else -1:
**
***********************************************/
extern INT32 init_rru_sm(void);


/**********************************************
** exec rru state machine.
**
** Input:
**		vbs_state: the rru's state.
**		event: event from sm.
**		msg: content of msg.
** Output:
** 		None.
** Return:
**      success: 0
**      else -1:
**
***********************************************/
extern INT32 exec_rru_sm(UINT32 RruId, RruEventType event, void *msg);



#endif
