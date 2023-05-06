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
 *File name: rru_agent_rru_entity_state_machine.h
 *Description: the state of Rru.
 *
 *Current Version: 0.l
 *Author: wangcheng (wangcheng2017@ict.ac.cn)
 *Date: 2018.06.05
 *
 ************************************************************************/
/* Dependencies ---------------------------------------------------------- */
#include "sbs_type.h"
#include "sbs_log.h"
#include "om_common_h.h"
#include "rrus_mgmt_rru_entity_state_machine.h"
#include "rrus_mgmt_rru_entity.h"
#include "rrus_mgmt_api_rru.h"


RruEntity*       g_rru_cfig_table[MAX_RRU_NUM];

RruEntity*       g_rru_entity_table[MAX_RRU_NUM];
RruStateTable    g_rru_state_table[RRU_STATE_MAX][RRU_EVENT_MAX];
char*            rru_event_name[RRU_EVENT_MAX];
char*            rru_state_name[RRU_STATE_MAX];


/* Functions ------------------------------------------------------------- */

// macros
#define add_rru_state(state, event, handle_fn) \
	        add_rru_state_table_entry(state, event, handle_fn, #state, #event)
/**********************************************
** add rru's fn of state-event in state table.
**
** Input:
**      rru_state, event, handle_fn.
** Output:
**      None.
** Return:
**      success: 0
**      else -1:
**
***********************************************/

INT32 add_rru_state_table_entry(RruStateMachine rru_state, RruEventType event,
								IrMsgHandleFn *handle_fn, UINT8 *state_name, UINT8 *event_name)
{
    if (rru_state < 0 || rru_state >= RRU_STATE_MAX)
    {
        log_msg(LOG_ERR, MAIN, "rru_state is not in range, please check the rru_state, now is No.%d\n", rru_state);
        return FUNC_ERR;
    }
    if (event < 0 || event >= RRU_EVENT_MAX)
    {
        log_msg(LOG_ERR, MAIN, "event is not in range, please check the event, now is No.%d\n", event);
        return FUNC_ERR;
    }
    if (NULL == handle_fn)
    {
        log_msg(LOG_ERR, MAIN, "handle_fn is NULL pointer!\n");
        return FUNC_ERR;
    }
    g_rru_state_table[rru_state][event].p_handle_fn = handle_fn;
    rru_state_name[rru_state] = state_name;
    rru_event_name[event] = event_name;

    return FUNC_OK;
}

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
INT32 init_rru_sm(void)
{
    /* init rru table */
	memset(g_rru_state_table, 0, sizeof(g_rru_state_table));

	/*init pointer array.*/
	memset(rru_event_name, 0, sizeof(rru_event_name));
	memset(rru_state_name, 0, sizeof(rru_state_name));
    
    /*add fn to state table*/
    if (add_rru_state(STATE_WAIT_CHANNEL_BUILD_REQ, EVENT_CHANNEL_BUILD_REQ, 
            (IrMsgHandleFn*)handle_msg_channel_build_req) == -1)
    {
        goto add_rru_table_error;
    }

    if (add_rru_state(STATE_WAIT_CHANNEL_BUILD_CFG_RSP, EVENT_CHANNEL_BUILD_CFG_RSP, 
            (IrMsgHandleFn*)handle_msg_channel_build_cfg_rsp) == -1)
    {
        goto add_rru_table_error;
    }

    if (add_rru_state(STATE_WAIT_VER_UPDATE_RESULT_IND, EVENT_VER_UPDATE_RESULT_IND, 
            (IrMsgHandleFn*)handle_msg_ver_update_result_ind) == -1)
    {
        goto add_rru_table_error;
    }

    if (add_rru_state(STATE_RRU_NORMAL, EVENT_VER_QUERY_REQ, 
            (IrMsgHandleFn*)handle_msg_ver_query_req) == -1)
    {
        goto add_rru_table_error;
    }

    if (add_rru_state(STATE_WAIT_RRU_VER_QUERY_RSP, EVENT_VER_QUERY_RSP, 
            (IrMsgHandleFn*)handle_msg_ver_query_rsp) == -1)
    {
        goto add_rru_table_error;
    }

    if (add_rru_state(STATE_RRU_NORMAL, EVENT_VER_DOWNLOAD_REQ, 
            (IrMsgHandleFn*)handle_msg_ver_download_req) == -1)
    {
        goto add_rru_table_error;
    }

//    if (add_rru_state(STATE_WAIT_VER_DOWNLOAD_RSP, EVENT_VER_DOWNLOAD_RSP,
//            (IrMsgHandleFn*)handle_msg_ver_download_rsp) == -1)
//    {
//        goto add_rru_table_error;
//    }

    if (add_rru_state(STATE_WAIT_VER_UPDATE_RESULT_IND, EVENT_VER_DOWNLOAD_RESULT_IND,
            (IrMsgHandleFn*)handle_msg_ver_download_result_ind) == -1)
    {
        goto add_rru_table_error;
    }

    if (add_rru_state(STATE_RRU_NORMAL, EVENT_VER_ACTIVATE_IND, 
            (IrMsgHandleFn*)handle_msg_ver_activate_ind) == -1)
    {
        goto add_rru_table_error;
    }

    if (add_rru_state(STATE_WAIT_VER_ACTIVATE_RSP, EVENT_VER_ACTIVATE_RSP, 
            (IrMsgHandleFn*)handle_msg_ver_activate_rsp) == -1)
    {
        goto add_rru_table_error;
    }

//    if (add_rru_state(STATE_RRU_NORMAL, EVENT_STATUS_QUERY_REQ,
//            (IrMsgHandleFn*)handle_msg_status_query_req) == -1)
//    {
//        goto add_rru_table_error;
//    }

//    if (add_rru_state(STATE_WAIT_STATUS_QUERY_RSP, EVENT_STATUS_QUERY_RSP,
//            (IrMsgHandleFn*)handle_msg_status_query_rsp) == -1)
//    {
//        goto add_rru_table_error;
//    }

//    if (add_rru_state(STATE_RRU_NORMAL, EVENT_PARAM_QUERY_REQ,
//            (IrMsgHandleFn*)handle_msg_param_query_req) == -1)
//    {
//        goto add_rru_table_error;
//    }

//    if (add_rru_state(STATE_WAIT_PARAM_QUERY_RSP, EVENT_PARAM_QUERY_RSP,
//            (IrMsgHandleFn*)handle_msg_param_query_rsp) == -1)
//    {
//        goto add_rru_table_error;
//    }

//    if (add_rru_state(STATE_RRU_NORMAL, EVENT_PARAM_CFG_REQ,
//            (IrMsgHandleFn*)handle_msg_param_cfg_req) == -1)
//    {
//        goto add_rru_table_error;
//    }

    if (add_rru_state(STATE_WAIT_PARAM_CFG_RSP, EVENT_PARAM_CFG_RSP, 
            (IrMsgHandleFn*)handle_msg_param_cfg_rsp) == -1)
    {
        goto add_rru_table_error;
    }

//    if (add_rru_state(STATE_RRU_NORMAL, EVENT_INIT_CAL_RESULT_REPORT,
//            (IrMsgHandleFn*)handle_msg_init_cal_result_report) == -1)
//    {
//        goto add_rru_table_error;
//    }

//    if (add_rru_state(STATE_RRU_NORMAL, EVENT_TIME_DELAY_MEASURE_REQ,
//            (IrMsgHandleFn*)handle_msg_time_delay_measure_req) == -1)
//    {
//        goto add_rru_table_error;
//    }

    if (add_rru_state(STATE_WAIT_TIME_DELAY_MEASURE_RSP, EVENT_TIME_DELAY_MEASURE_RSP, 
            (IrMsgHandleFn*)handle_msg_time_delay_measure_rsp) == -1)
    {
        goto add_rru_table_error;
    }

//    if (add_rru_state(STATE_RRU_NORMAL, EVENT_TIME_DELAY_CFG_REQ,
//            (IrMsgHandleFn*)handle_msg_time_delay_cfg_req) == -1)
//    {
//        goto add_rru_table_error;
//    }

    if (add_rru_state(STATE_WAIT_TIME_DELAY_CFG_RSP, EVENT_TIME_DELAY_CFG_RSP, 
            (IrMsgHandleFn*)handle_msg_time_delay_cfg_rsp) == -1)
    {
        goto add_rru_table_error;
    }
//
//    if (add_rru_state(STATE_RRU_NORMAL, EVENT_ALARM_REPORT,
//            (IrMsgHandleFn*)handle_msg_alarm_report) == -1)
//    {
//        goto add_rru_table_error;
//    }

//    if (add_rru_state(STATE_RRU_NORMAL, EVENT_ALARM_QUERY_REQ,
//            (IrMsgHandleFn*)handle_msg_alarm_query_req) == -1)
//    {
//        goto add_rru_table_error;
//    }

    if (add_rru_state(STATE_WAIT_ALARM_QUERY_RSP, EVENT_ALARM_QUERY_RSP, 
            (IrMsgHandleFn*)handle_msg_alarm_query_rsp) == -1)
    {
        goto add_rru_table_error;
    }

//    if (add_rru_state(STATE_RRU_NORMAL, EVENT_RESET_IND,
//            (IrMsgHandleFn*)handle_msg_reset_ind) == -1)
//    {
//        goto add_rru_table_error;
//    }

//    if (add_rru_state(STATE_RRU_NORMAL, EVENT_RING_TEST_REQ,
//            (IrMsgHandleFn*)handle_msg_ring_test_req) == -1)
//    {
//        goto add_rru_table_error;
//    }

    if (add_rru_state(STATE_WAIT_RING_TEST_RSP, EVENT_RING_TEST_RSP, 
            (IrMsgHandleFn*)handle_msg_ring_test_rsp) == -1)
    {
        goto add_rru_table_error;
    }

//    if (add_rru_state(STATE_RRU_NORMAL, EVENT_CELL_CFG_REQ,
//            (IrMsgHandleFn*)handle_msg_cell_cfg_req) == -1)
//    {
//        goto add_rru_table_error;
//    }

    if (add_rru_state(STATE_WAIT_CELL_CFG_RSP, EVENT_CELL_CFG_RSP, 
            (IrMsgHandleFn*)handle_msg_cell_cfg_rsp) == -1)
    {
        goto add_rru_table_error;
    }

	return FUNC_OK;

    add_rru_table_error:
		log_msg(LOG_ERR, RRUS_MGMT, "add state table entry err!\n");
        return FUNC_ERR;
}


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
INT32 exec_rru_sm(UINT32 rru_id, RruEventType event, void *msg)
{
    /* set variable for fn */
	INT32 rru_state = 0;
	IrMsgHandleFn p_handle_fn = NULL;
    INT32 index = 0;

	/* Check for null pointer.*/
	if (NULL == msg) 
	{
		log_msg(LOG_ERR, RRUS_MGMT, "message pointer is NULL.\n");

		return FUNC_ERR;
	}

    for (index = 0; index < MAX_RRU_NUM; index++)
    {
        if (g_rru_entity_table[index] == NULL)
            continue;

        if (g_rru_entity_table[index]->logic_info.rru_id == rru_id)
            break;
    }
    if (index == MAX_RRU_NUM)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Cannot find rru_id = %d in rru_entity_table.\n", rru_id);
        return FUNC_ERR;
    }
	
    rru_state = g_rru_entity_table[index]->logic_info.rru_cur_state;

	if (rru_state < 1 || rru_state > RRU_STATE_MAX) 
	{
		log_msg(LOG_ERR, RRUS_MGMT, "rru state is not in range!\n");

		return FUNC_ERR;
	}

	p_handle_fn = g_rru_state_table[rru_state][event].p_handle_fn;

	/* judge whether handle fn exist */
	if (p_handle_fn) 
	{
        #ifdef DEBUG
		log_msg(LOG_INFO, RRUS_MGMT, "Receive RRU Event! (rru_id = %d, state = %s, event "
		   			"= %s)\n", rru_id, rru_state_name[rru_state], rru_event_name[event]);
        #endif
	} 
	else 
	{
		log_msg(LOG_ERR, RRUS_MGMT, "p_handle_fn is NULL pointer! (rru_id = %d, state = %s, event "
		   			"= %s), state= %d, event = %d.\n", rru_id, rru_state_name[rru_state], 
		   			rru_event_name[event], rru_state, event);

		return FUNC_ERR;
	}
	/*exec in handle fn*/
	if (0 != p_handle_fn(rru_id, (void*)msg)) 
	{
		log_msg(LOG_ERR, MAIN, "exec p_handle_fn error! (rru_id = %d, state = %s, event "
					"= %s)\n", rru_id, rru_state_name[rru_state], rru_event_name[event]);


		return FUNC_ERR;
	}
	//show_system_time();

	return FUNC_OK;
}
