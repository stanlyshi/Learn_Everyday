/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:32 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:45 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:28 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:03 CST. */
/******************************************************************************
** 
** Copyright (c) 2008-2018 ICT/CAS.
** All rights reserved.
**
** File name: cell_mgmt_sm.c
** Description: generate State Machine function
**
** Current Version: 
** $Revision$
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2018.10
**
******************************************************************************/

/* Dependencies --------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "sbs_log.h"
#include "sbs_type.h"

#include "util_printer.h"
#include "util_systemer.h"

#include "cell_mgmt_cell_entity.h"
#include "cell_mgmt_comm_func.h"
#include "cell_mgmt_define.h"
#include "cell_mgmt_msg.h"
#include "cell_mgmt_sm.h"

#include "om_common_h.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
VcSmEntry   g_vc_state_table[VC_STATE_MAX_NUM][VC_EVENT_MAX_NUM];
char        *vc_event_str[VC_EVENT_MAX_NUM]; /*store the name of event.*/
char        *vc_state_str[VC_STATE_MAX_NUM]; /*store the name of state.*/

/* Functions -----------------------------------------------------------------*/
/******************************************************************************
 * Description: add to state machine 
 *
 * Input:   state: see in cell_mgmt_cell_entity.h
 *          event: same as state
 *          p_handle_fn: handle function
 *          state_name: state name string
 *          event_name: event name string
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 add_vc_state_table_entry(
                        VcSmState state, 
                        VcSmEventType event,
                        VcSmHandleFn p_handle_fn, 
                        char *state_name, 
                        char *event_name)
{
    info("add to state machine: %s: %s\n", state_name, event_name);
    /* judge wether input param is correct.*/
    if (state < 0 || state >= VC_STATE_MAX_NUM) 
    {
        log_msg(LOG_WARNING, CELL_MGMT, 
                "add_vc_state_table_entry vc state invalid\n");
        error("add_vc_state_table_entry vc state invalid\n");
        return FUNC_ERR;
    }
    
    /* judge wether input event is correct. */
    if (event < 0 || event >= VC_EVENT_MAX_NUM) 
    {
        log_msg(LOG_WARNING, CELL_MGMT, 
                "add_vc_state_table_entry event invalid. \n");
        error("add_vc_state_table_entry vc event invalid. \n");
        return FUNC_ERR;
    }
    
    /* Check for null pointer.*/
    if (NULL == p_handle_fn) 
    {
        log_msg(LOG_ERR, CELL_MGMT, 
                "the return pointer of node_search is"\
                " NULL in add_vc_state_table_entry().\n");
        error("add_vc_state_table_entry vc func pointer invalid\n");
        return FUNC_ERR;
    }
    
    /*update the sm struct.*/
    g_vc_state_table[state][event].p_handle_fn  = p_handle_fn;
    g_vc_state_table[state][event].state_name   = state_name;
    g_vc_state_table[state][event].event_name   = event_name;
    vc_event_str[event] = event_name;
    vc_state_str[state] = state_name;
    
    log_msg(LOG_INFO, CELL_MGMT, "this is a add_vc_state_table_entry test. %s\n",
            get_system_time());

    /* success return */
    return FUNC_OK;
}

/******************************************************************************
 * Description: state machine initialized
 *
 * Input: void
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 cell_mgmt_sm_init(void)
{
    debug();
    warning("cell_mgmt state machine initialize...\n");
    
    /* init vc table */
    memset(g_vc_state_table, 0, sizeof(g_vc_state_table));

    /*init pointer array.*/
    memset(vc_event_str, 0, sizeof(vc_event_str));
    memset(vc_state_str, 0, sizeof(vc_state_str));
    
    
    /* add handle fn in sm */
    if (add_vc_table_entry(CELL_NULL, MSG_VC_SETUP_REQ,
        (VcSmHandleFn )vcm_handle_cell_setup_req)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_SETUP_CPRI_CFG, MSG_VC_SETUP_CPRI_CFG_RSP,
        (VcSmHandleFn )vcm_handle_cell_setup_cpri_cfg_rsp)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_SETUP_PHY_CFG, MSG_VC_SETUP_PHY_CFG_RSP,
        (VcSmHandleFn )vcm_handle_cell_setup_phy_cfg_rsp)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_SETUP_MAC_INIT, MSG_VC_SETUP_MAC_INIT_RSP,
        (VcSmHandleFn )vcm_handle_cell_setup_mac_init_rsp)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_SETUP_CELL_START, MSG_VC_SETUP_CELL_START_RSP,
        (VcSmHandleFn )vcm_handle_cell_setup_cell_start_rsp)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(CELL_NORMAL, MSG_VC_DEL_REQ,
        (VcSmHandleFn )vcm_handle_cell_del_req)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_DEL_CELL_CLOSE, MSG_VC_DEL_CELL_CLOSE_RSP,
        (VcSmHandleFn )vcm_handle_cell_del_cell_close_rsp)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_DEL_MAC_DEL, MSG_VC_DEL_MAC_DEL_RSP,
        (VcSmHandleFn )vcm_handle_cell_del_mac_del_rsp)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_DEL_PHY_DEL, MSG_VC_DEL_PHY_DEL_RSP,
        (VcSmHandleFn )vcm_handle_cell_del_phy_del_rsp)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_DEL_CPRI_RESET, MSG_VC_DEL_CPRI_RESET_RSP,
        (VcSmHandleFn )vcm_handle_cell_del_cpri_reset_rsp)) 
    {
        goto add_sm_table_error;
    }

    /*for cell stop state handle*/
    if (add_vc_table_entry(CELL_NULL, MSG_VC_STOP_REQ,
        (VcSmHandleFn )vcm_handle_cell_state_stop_req)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_SETUP_CPRI_CFG, MSG_VC_STOP_REQ,
        (VcSmHandleFn )vcm_handle_cell_state_stop_req)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_SETUP_PHY_CFG, MSG_VC_STOP_REQ,
        (VcSmHandleFn )vcm_handle_cell_state_stop_req)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_SETUP_MAC_INIT, MSG_VC_STOP_REQ,
        (VcSmHandleFn )vcm_handle_cell_state_stop_req)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_SETUP_CELL_START, MSG_VC_STOP_REQ,
        (VcSmHandleFn )vcm_handle_cell_state_stop_req)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(CELL_NORMAL, MSG_VC_STOP_REQ,
        (VcSmHandleFn )vcm_handle_cell_state_stop_req)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_DEL_CELL_CLOSE, MSG_VC_STOP_REQ,
        (VcSmHandleFn )vcm_handle_cell_state_stop_req)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_DEL_MAC_DEL, MSG_VC_STOP_REQ,
        (VcSmHandleFn )vcm_handle_cell_state_stop_req)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_DEL_PHY_DEL, MSG_VC_STOP_REQ,
        (VcSmHandleFn )vcm_handle_cell_state_stop_req)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(WAIT_DEL_CPRI_RESET, MSG_VC_STOP_REQ,
        (VcSmHandleFn )vcm_handle_cell_state_stop_req)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(CELL_STOP, MSG_VC_STOP_REQ,
        (VcSmHandleFn )vcm_handle_cell_state_stop_req)) 
    {
        goto add_sm_table_error;
    }
    if (add_vc_table_entry(CELL_STOP, MSG_VC_STOP_RECOVER,
        (VcSmHandleFn )vcm_handle_cell_state_stop_recover)) 
    {
        goto add_sm_table_error;
    }
    
    if (add_vc_table_entry(CELL_NULL, MSG_VC_STOP_RECOVER,
		(VcSmHandleFn )vcm_handle_cell_state_stop_recover))
	{
		goto add_sm_table_error;
	}

    log_msg(LOG_INFO, CELL_MGMT, "cell_mgmt state machine initialize successful\n");

    success("cell_mgmt state machine initialize successful\n");
    show_sm_on_screen();
    return FUNC_OK;
    
add_sm_table_error:

    log_msg(LOG_ERR, CELL_MGMT, "add state table entry err!\n");
    error("cell_mgmt state machine initialize failed\n");
    return  FUNC_ERR;
}

/******************************************************************************
 * Description: execute state machine function with "event"
 *              call in "cell_mgmt_module_main_ctrl()"
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 do_cell_mgmt_sm(UINT16 cell_virt_id, VcSmEventType event, void *p_msg_info)
{
    debug();
    log_msg(LOG_INFO, CELL_MGMT, "this is a do_cell_mgmt_sm test.\n");

    INT32 cell_state = 0;
    VcSmHandleFn p_handle_fn = NULL;
    
    if ((cell_virt_id < MIN_CELL_NUM) || (cell_virt_id > MAX_CELL_NUM)) 
	{
 		log_msg(LOG_ERR, CELL_MGMT, "cell_virt_id invalid. %d\n", cell_virt_id);

 		return FUNC_ERR;
 	}
    
    if (NULL == p_msg_info) 
	{
		log_msg(LOG_ERR, CELL_MGMT, "msg is NULL\n");
		return FUNC_ERR;
	}
    
    cell_state = vc_entity_table[cell_virt_id-1].cell_state;
    
    if (cell_state < VC_STATE_MIN_NUM || cell_state > VC_STATE_MAX_NUM) 
	{
		log_msg(LOG_ERR, CELL_MGMT, "cell state incorrect!\n");

		return FUNC_ERR;
	}
    
    p_handle_fn = g_vc_state_table[cell_state][event].p_handle_fn;
    
    if (p_handle_fn)
    {
        info("%s:%d Receive VC MSG!(vc = %d, state = %s, event = %s)\n",
             __func__, __LINE__, 
             cell_virt_id, vc_state_str[cell_state], vc_event_str[event]);
		log_msg(LOG_INFO, CELL_MGMT, 
				"Receive VC Event! (vc = %d, state = %s, event = %s)\n", 
				cell_virt_id, vc_state_str[cell_state], vc_event_str[event]);
    }
    else 
	{
        error("%s:%d VC State Error! (vc = %d, state = %s, "\
             "event = %s),vc state=%d,event=%d\n", 
             __func__, __LINE__, 
             cell_virt_id, vc_state_str[cell_state], 
             vc_event_str[event], cell_state, event);

		log_msg(LOG_ERR, CELL_MGMT, 
                "VC State Error! (vc = %d, state = %s, "\
                "event = %s),vc state=%d,event=%d\n", 
                cell_virt_id, vc_state_str[cell_state], 
		   		vc_event_str[event], cell_state, event);

		return FUNC_ERR;
	}
    
	if (0 != p_handle_fn(cell_virt_id, p_msg_info)) 
	{
		log_msg(LOG_ERR, CELL_MGMT, 
                "Function use error! (vbs = %d, state = %s, event = %s)\n",
                cell_virt_id, vc_state_str[cell_state], vc_event_str[event]);

		return FUNC_ERR;
	}
	info("do_cell_mgmt_sm done\n");
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: show state machine on screen
 *          for example:
    [Info][CELL_MGMT] ##########################################################
    [Info][CELL_MGMT] # State Machine.   Tue Oct  9 21:37:41 2018
    [Info][CELL_MGMT] # 
    [Info][CELL_MGMT] #   State         Event
    [Info][CELL_MGMT] #  CELL_NULL -> MSG_VC_SETUP_REQ
    [Info][CELL_MGMT] #  CELL_NULL -> MSG_VC_STOP_REQ
    [Info][CELL_MGMT] ##########################################################
 *
 * Input: void 
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 show_sm_on_screen(void)
{
    warning("show_sm_on_screen ...\n");
    int ievent, istate;
    info("##########################################################\n");
    info("# \033[1;31mState Machine.\033[m   %s\n", get_system_time());
    info("# \n");
    info("#   State         Event\n");
    for(istate=0; istate<VC_STATE_MAX_NUM; istate++)
    {
        for(ievent=0; ievent<VC_EVENT_MAX_NUM; ievent++)
        {
            if(g_vc_state_table[istate][ievent].p_handle_fn != NULL)
            {
                info("#  %s -> %s\n", 
                    g_vc_state_table[istate][ievent].state_name,
                    g_vc_state_table[istate][ievent].event_name);
            }
        }
    }
    info("# \n");
    info("##########################################################\n");
    
    log_msg(LOG_INFO, CELL_MGMT, "show_sm_on_screen\n");
    return FUNC_OK;
}
