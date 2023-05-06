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
** File name: cell_mgmt_sm.h
** Description: State Machine of CELL_MGMT head file
**
** Current Version: 
** $Revision$
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2018.10
**
******************************************************************************/
#ifndef _MODULE_CELL_MGMT_SM_H
#define _MODULE_CELL_MGMT_SM_H

/* Dependencies --------------------------------------------------------------*/
#include "sbs_type.h"

#include "cell_mgmt_cell_entity.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
typedef INT32 (*VcSmHandleFn)(UINT32, void*);

typedef struct{
	char            *state_name;
	char            *event_name;
	VcSmHandleFn    p_handle_fn;
}__attribute__((packed))VcSmEntry;

/* Macros --------------------------------------------------------------------*/
#define add_vc_table_entry(state, event, p_handle_fn) \
        add_vc_state_table_entry(state, event, p_handle_fn, #state, #event)

/* Globals -------------------------------------------------------------------*/
extern VcSmEntry   g_vc_state_table[VC_STATE_MAX_NUM][VC_EVENT_MAX_NUM];
extern char        *vc_event_str[VC_EVENT_MAX_NUM]; /*store the name of event.*/
extern char        *vc_state_str[VC_STATE_MAX_NUM]; /*store the name of state.*/

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
INT32 add_vc_state_table_entry(VcSmState state, VcSmEventType event,
                               VcSmHandleFn p_handle_fn, 
                               char *state_name, char *event_name);

/******************************************************************************
 * Description: state machine initialized
 *
 * Input: void
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 cell_mgmt_sm_init(void);

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
INT32 do_cell_mgmt_sm(UINT16 cell_virt_id, VcSmEventType event, void *p_msg_info);


/******************************************************************************
 * Description: cell setup handle functions
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/

INT32 vcm_handle_cell_setup(void *p_msg_info);
INT32 vcm_handle_cell_setup_req(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_setup_cpri_cfg_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_setup_cpri_cfg_succ_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_setup_cpri_cfg_fail_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_setup_phy_cfg_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_setup_phy_cfg_succ_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_setup_phy_cfg_fail_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_setup_mac_init_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_setup_mac_init_succ_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_setup_mac_init_fail_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_setup_cell_start_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_setup_cell_start_succ_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_setup_cell_start_fail_rsp(UINT32 cell_virt_id, void *p_msg_info);


/******************************************************************************
 * Description: cell delete handle functions
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_del_req(UINT32 cell_virt_id,void *p_msg_info);
INT32 vcm_handle_cell_del_cell_close_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_del_cell_close_succ_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_del_cell_close_fail_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_del_mac_del_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_del_mac_del_succ_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_del_mac_del_fail_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_del_phy_del_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_del_phy_del_succ_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_del_phy_del_fail_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_del_cpri_reset_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_del_cpri_reset_succ_rsp(UINT32 cell_virt_id, void *p_msg_info);
INT32 vcm_handle_cell_del_cpri_reset_fail_rsp(UINT32 cell_virt_id, void *p_msg_info);


/******************************************************************************
 * Description: cell stop handle functions
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_state_stop_req(UINT32 cell_virt_id,void *p_msg_info);
INT32 vcm_handle_cell_state_stop_recover(UINT32 cell_virt_id,void *p_msg_info);

#endif /*<_MODULE_CELL_MGMT_SM_H>*/