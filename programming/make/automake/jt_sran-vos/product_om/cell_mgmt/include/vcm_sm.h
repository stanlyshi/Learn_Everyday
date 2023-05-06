/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:28 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 19日 星期二 14:31:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 19日 星期二 14:31:24 CST. 
*/
/**********************************************************************************************************************
** 
** Copyright (c) 2008-2018 ICT/CAS.
** All rights reserved.
**
** File name: vcm_sm.h
** Description: 
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2019.01 - 
**
***********************************************************************************************************************/
#ifndef _VCM_SM_H
#define _VCM_SM_H 1
 
#if !defined _VCM_H
 #error "Never include vcm_sm.h."
#endif


/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include <stdio.h>

#include "vcm_types.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/

/**
 *  cell state machine state
 */
typedef enum {
    VC_STATE_MIN_NUM,               //0
    
    CELL_NULL,                      //
    
    WAIT_SETUP_CPRI_CFG,            //
    WAIT_SETUP_PHY_CFG,             //
    WAIT_SETUP_MAC_INIT,            //
    WAIT_SETUP_CELL_START,          //
    
    CELL_NORMAL,                    //
    
    WAIT_DEL_CELL_CLOSE,            //
    WAIT_DEL_MAC_DEL,               //
    WAIT_DEL_PHY_DEL,               //
    WAIT_DEL_CPRI_RESET,            //
    
    CELL_STOP,                      //for sync, rru reg, phy state
    
    WAIT_SETUP_SIMU_PHY_INIT,       //for simu cell setup process
    WAIT_DEL_SIMU_PHY_DEL,          //for simu cell delete process

    VC_STATE_MAX_NUM,               //
}vcm_sm_state_t; 

/**
 *  cell state machine event
 */
typedef enum {
    VC_EVENT_MIN_NUM,
    
    /*cell setup*/
    MSG_VC_SETUP_REQ,
    MSG_VC_SETUP_CPRI_CFG_RSP,
    MSG_VC_SETUP_PHY_CFG_RSP,
    MSG_VC_SETUP_MAC_INIT_RSP,
    MSG_VC_SETUP_CELL_START_RSP, 
    
    /*cell delete*/
    MSG_VC_DEL_REQ, 
    MSG_VC_DEL_CELL_CLOSE_RSP,
    MSG_VC_DEL_MAC_DEL_RSP,
    MSG_VC_DEL_PHY_DEL_RSP,
    MSG_VC_DEL_CPRI_RESET_RSP, 
    
    /*cell stop for a while*/
    MSG_VC_STOP_REQ,                /*vcm 2 vcm*/
    MSG_VC_STOP_RECOVER,            /*vcm 2 vcm*/
    
    /* simu cell setup */
    MSG_SC_SETUP_REQ,
    MSG_SC_SETUP_MAC_INIT_RSP,
    MSG_SC_SETUP_SIMU_PHY_INIT_RSP,
    MSG_SC_SETUP_CELL_START_RSP,

    /* simu cell delete */
    MSG_SC_DEL_REQ,
    MSG_SC_DEL_CELL_CLOSE_RSP,
    MSG_SC_DEL_SIMU_PHY_DEL_RSP,
    MSG_SC_DEL_MAC_DEL_RSP,
    
    VC_EVENT_MAX_NUM
}vcm_sm_event_t; 


/* Types -------------------------------------------------------------------------------------------------------------*/
typedef INT32 (*vcm_sm_handler_f)(UINT32, VOID*);

typedef struct{
	CHAR            *state_name;
	CHAR            *event_name;
	CHAR            *func_name;
	vcm_sm_handler_f    handler;
}__attribute__((packed))vcm_sm_node_t; /* main state machine: ctrl cells */

/* Macros ------------------------------------------------------------------------------------------------------------*/
#define add_state_event(state, event, func) \
    vcm_sm_add_state_event(state, event, func, #state, #event, #func)

/* Globals -----------------------------------------------------------------------------------------------------------*/
extern vcm_sm_node_t vcm_sm[VC_STATE_MAX_NUM][VC_EVENT_MAX_NUM];
extern CHAR*    vcm_sm_state_str[VC_STATE_MAX_NUM];
extern CHAR*    vcm_sm_event_str[VC_EVENT_MAX_NUM];


/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_init(VOID);
INT32 vcm_sm_add_state_event(vcm_sm_state_t state, vcm_sm_event_t event, vcm_sm_handler_f handlefunc, 
                             CHAR *Sstate, CHAR *Sevent, CHAR *Sfunction);



/***********************************************************************************************************************
 * Description: cell setup handle functions
 *
 * Input:   cell_virt_id: cell array index+1 (Cells[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 **********************************************************************************************************************/
INT32 vcm_handle_cell_setup(VOID *p_msg_info);
INT32 vcm_sm_handle_cell_setup_req(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_setup_cpri_cfg_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_setup_cpri_cfg_succ_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_setup_cpri_cfg_fail_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_setup_phy_cfg_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_setup_phy_cfg_succ_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_setup_phy_cfg_fail_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_setup_mac_init_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_setup_mac_init_succ_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_setup_mac_init_fail_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_setup_cell_start_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_setup_cell_start_succ_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_setup_cell_start_fail_rsp(UINT32 id, VOID *p_msg_info);


/***********************************************************************************************************************
 * Description: cell delete handle functions
 *
 * Input:   cell_virt_id: cell array index+1 (Cells[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_del_req(UINT32 id,VOID *p_msg_info);
INT32 vcm_sm_handle_cell_del_cell_close_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_del_cell_close_succ_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_del_cell_close_fail_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_del_mac_del_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_del_mac_del_succ_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_del_mac_del_fail_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_del_phy_del_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_del_phy_del_succ_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_del_phy_del_fail_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_del_cpri_reset_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_del_cpri_reset_succ_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_cell_del_cpri_reset_fail_rsp(UINT32 id, VOID *p_msg_info);


/***********************************************************************************************************************
 * Description: cell stop handle functions
 *
 * Input:   cell_virt_id: cell array index+1 (Cells[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_state_stop_req(UINT32 id,VOID *p_msg_info);
INT32 vcm_sm_handle_cell_state_stop_recover(UINT32 id,VOID *p_msg_info);
INT32 vcm_sm_handle_cell_null_stopped_case(UINT32 id,VOID *p_msg_info);


/**********************************************************************************************************************
 * Description: simulated cell setup handle functions
 *
 * Input:   cell_virt_id: cell array index+1 (Cells[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 **********************************************************************************************************************/
INT32 vcm_sm_handle_simu_cell_setup(void *p_msg_info);
INT32 vcm_sm_handle_simu_cell_setup_req(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_simu_cell_setup_mac_init_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_simu_cell_setup_mac_init_succ_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_simu_cell_setup_mac_init_fail_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_simu_cell_setup_simu_phy_init_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_simu_cell_setup_simu_phy_init_succ_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_simu_cell_setup_simu_phy_init_fail_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_simu_cell_setup_cell_start_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_simu_cell_setup_cell_start_succ_rsp(UINT32 id, VOID *p_msg_info);
INT32 vcm_sm_handle_simu_cell_setup_cell_start_fail_rsp(UINT32 id, VOID *p_msg_info);



/**********************************************************************************************************************
 * Description: simulated cell delete handle functions
 *
 * Input:   cell_virt_id: cell array index+1 (Cells[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 **********************************************************************************************************************/
INT32 vcm_sm_handle_simu_cell_del_req(UINT32 id, void *p_msg_info);
INT32 vcm_sm_handle_simu_cell_del_cell_close_rsp(UINT32 id, void *p_msg_info);
INT32 vcm_sm_handle_simu_cell_del_cell_close_succ_rsp(UINT32 id, void *p_msg_info);
INT32 vcm_sm_handle_simu_cell_del_cell_close_fail_rsp(UINT32 id, void *p_msg_info);
INT32 vcm_sm_handle_simu_cell_del_simu_phy_del_rsp(UINT32 id, void *p_msg_info);
INT32 vcm_sm_handle_simu_cell_del_mac_del_rsp(UINT32 id, void *p_msg_info);



#endif /*<_VCM_SM_H>*/
