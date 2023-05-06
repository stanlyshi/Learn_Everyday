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
** File name: vcm_sm.c
** Description: 
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2019.01 - 
**
***********************************************************************************************************************/
/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm.h"


/* Constants ---------------------------------------------------------------------------------------------------------*/
/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
vcm_sm_node_t    vcm_sm[VC_STATE_MAX_NUM][VC_EVENT_MAX_NUM];
CHAR*       vcm_sm_state_str[VC_STATE_MAX_NUM];
CHAR*       vcm_sm_event_str[VC_EVENT_MAX_NUM];


/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_init(VOID)
{
    log_vcm_warn("%s state machine initializing...\n", CELL_MGMT_NAME);

    /**
     *  Memset zero
     */
    Memset(vcm_sm, 0, sizeof(vcm_sm));
    Memset(vcm_sm_state_str, 0, sizeof(vcm_sm_state_str));
    Memset(vcm_sm_event_str, 0, sizeof(vcm_sm_event_str));

    /**
     *  Add state\ event \handle function into State Machine
     */    
#define __e_add_state_event(state, event, func)         \
    if(VCM_OK != add_state_event(state, event, func)){  \
        log_vcm_err("add state error\n");               \
        goto add_state_event_err;                       \
    }

    __e_add_state_event(CELL_NULL, MSG_VC_SETUP_REQ,vcm_sm_handle_cell_setup_req);
    __e_add_state_event(WAIT_SETUP_CPRI_CFG, MSG_VC_SETUP_CPRI_CFG_RSP,vcm_sm_handle_cell_setup_cpri_cfg_rsp);
    __e_add_state_event(WAIT_SETUP_PHY_CFG, MSG_VC_SETUP_PHY_CFG_RSP,vcm_sm_handle_cell_setup_phy_cfg_rsp);
    __e_add_state_event(WAIT_SETUP_MAC_INIT, MSG_VC_SETUP_MAC_INIT_RSP,vcm_sm_handle_cell_setup_mac_init_rsp);
    __e_add_state_event(WAIT_SETUP_CELL_START, MSG_VC_SETUP_CELL_START_RSP,vcm_sm_handle_cell_setup_cell_start_rsp);
    __e_add_state_event(CELL_NORMAL, MSG_VC_DEL_REQ,vcm_sm_handle_cell_del_req);
    __e_add_state_event(WAIT_DEL_CELL_CLOSE, MSG_VC_DEL_CELL_CLOSE_RSP,vcm_sm_handle_cell_del_cell_close_rsp);
    __e_add_state_event(WAIT_DEL_MAC_DEL, MSG_VC_DEL_MAC_DEL_RSP,vcm_sm_handle_cell_del_mac_del_rsp);
    __e_add_state_event(WAIT_DEL_PHY_DEL, MSG_VC_DEL_PHY_DEL_RSP,vcm_sm_handle_cell_del_phy_del_rsp);
    __e_add_state_event(WAIT_DEL_CPRI_RESET, MSG_VC_DEL_CPRI_RESET_RSP,vcm_sm_handle_cell_del_cpri_reset_rsp);
    __e_add_state_event(CELL_NULL, MSG_VC_STOP_REQ,vcm_sm_handle_cell_state_stop_req);
    __e_add_state_event(WAIT_SETUP_CPRI_CFG, MSG_VC_STOP_REQ,vcm_sm_handle_cell_state_stop_req);
    __e_add_state_event(WAIT_SETUP_PHY_CFG, MSG_VC_STOP_REQ,vcm_sm_handle_cell_state_stop_req);
    __e_add_state_event(WAIT_SETUP_MAC_INIT, MSG_VC_STOP_REQ,vcm_sm_handle_cell_state_stop_req);
    __e_add_state_event(WAIT_SETUP_CELL_START, MSG_VC_STOP_REQ,vcm_sm_handle_cell_state_stop_req);
    __e_add_state_event(CELL_NORMAL, MSG_VC_STOP_REQ,vcm_sm_handle_cell_state_stop_req);
    __e_add_state_event(WAIT_DEL_CELL_CLOSE, MSG_VC_STOP_REQ,vcm_sm_handle_cell_state_stop_req);
    __e_add_state_event(WAIT_DEL_MAC_DEL, MSG_VC_STOP_REQ,vcm_sm_handle_cell_state_stop_req);
    __e_add_state_event(WAIT_DEL_PHY_DEL, MSG_VC_STOP_REQ,vcm_sm_handle_cell_state_stop_req);
    __e_add_state_event(WAIT_DEL_CPRI_RESET, MSG_VC_STOP_REQ,vcm_sm_handle_cell_state_stop_req);
    __e_add_state_event(CELL_STOP, MSG_VC_STOP_REQ,vcm_sm_handle_cell_state_stop_req);
    __e_add_state_event(CELL_STOP, MSG_VC_STOP_RECOVER,vcm_sm_handle_cell_state_stop_recover);
    __e_add_state_event(CELL_NULL, MSG_VC_STOP_RECOVER,vcm_sm_handle_cell_state_stop_recover);
    __e_add_state_event(CELL_STOP, MSG_VC_SETUP_REQ,vcm_sm_handle_cell_null_stopped_case);
    __e_add_state_event(CELL_NULL, MSG_SC_SETUP_REQ,vcm_sm_handle_simu_cell_setup_req);
    __e_add_state_event(WAIT_SETUP_MAC_INIT, MSG_SC_SETUP_MAC_INIT_RSP,vcm_sm_handle_simu_cell_setup_mac_init_rsp);
    __e_add_state_event(WAIT_SETUP_SIMU_PHY_INIT, MSG_SC_SETUP_SIMU_PHY_INIT_RSP,vcm_sm_handle_simu_cell_setup_simu_phy_init_rsp);
    __e_add_state_event(WAIT_SETUP_CELL_START, MSG_SC_SETUP_CELL_START_RSP,vcm_sm_handle_simu_cell_setup_cell_start_rsp);
    __e_add_state_event(CELL_NORMAL, MSG_SC_DEL_REQ,vcm_sm_handle_simu_cell_del_req);
    __e_add_state_event(WAIT_DEL_CELL_CLOSE, MSG_SC_DEL_CELL_CLOSE_RSP,vcm_sm_handle_simu_cell_del_cell_close_rsp);
    __e_add_state_event(WAIT_DEL_SIMU_PHY_DEL, MSG_SC_DEL_SIMU_PHY_DEL_RSP,vcm_sm_handle_simu_cell_del_simu_phy_del_rsp);
    __e_add_state_event(WAIT_DEL_MAC_DEL, MSG_SC_DEL_MAC_DEL_RSP,vcm_sm_handle_simu_cell_del_mac_del_rsp);

    vcm_sm_display(STDOUT);
    
    return VCM_OK;
        
add_state_event_err:
    return VCM_ERR;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_add_state_event(vcm_sm_state_t state, vcm_sm_event_t event, vcm_sm_handler_f handlefunc, 
                             CHAR *Sstate, CHAR *Sevent, CHAR *Sfunction)
{
    /**
     *  Check state is valiable or not
     */
    if (state <= VC_STATE_MIN_NUM || state >= VC_STATE_MAX_NUM) 
    {
        log_vcm_err("state invalid\n");
        return VCM_ERR;
    }

    /**
     *  Check event is valiable or not
     */
    if (event <= VC_EVENT_MIN_NUM || event >= VC_EVENT_MAX_NUM) 
    {
        log_vcm_err("event invalid. \n");
        return VCM_ERR;
    }

    /**
     *  Check handle function is valiable or not
     */
    if (NULL == handlefunc) 
    {
        log_vcm_err("handle function invalid.\n");
        return VCM_ERR;
    }

    /**
     *  add to state machine into global vcm_sm array
     */
    vcm_sm[state][event].state_name  = Sstate;
    vcm_sm[state][event].event_name  = Sevent;
    vcm_sm[state][event].func_name   = Sfunction;
    vcm_sm[state][event].handler = handlefunc;
    vcm_sm_state_str[state] = Sstate;
    vcm_sm_event_str[event] = Sevent;

    return VCM_OK;
}

/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_display(FILE *fp)
{
#ifdef _VCM_DISPLAY

    INT32 ievent, istate;
    
    Fprintf(fp, "+%sState Machine%s------------------------------------\n",VCM_ANSI_RED,VCM_ANSI_RESET);
    Fprintf(fp, "| \n");
    Fprintf(fp, "| State -> Event -> Function \n");
    Fprintf(fp, "| \n");

    for(istate=0; istate<VC_STATE_MAX_NUM; istate++)
    {
        for(ievent=0; ievent<VC_EVENT_MAX_NUM; ievent++)
        {
            if(vcm_sm[istate][ievent].handler != NULL)
            {
                /**
                 *  Show the state machine in screen(stdout)
                 */
                Fprintf(STDOUT, "| %s -> %s -> %s\n", 
                    vcm_sm[istate][ievent].state_name, 
                    vcm_sm[istate][ievent].event_name,
                    vcm_sm[istate][ievent].func_name);
            }
        }
    }
    Fprintf(fp, "| \n");
    Fprintf(fp, "+-------------------------------------------------\n");

    Fflush(fp);
#endif /*<_VCM_DISPLAY>*/

    return VCM_OK;
}

