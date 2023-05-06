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
** File name: cell_mgmt_timer.c
** Description: some of timer for msg send/recv, system timer sync. etc.
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
#include <sys/time.h>

#include "util_printer.h"

#include "cell_mgmt_api.h"
#include "cell_mgmt_cell_entity.h"
#include "cell_mgmt_comm_func.h"
#include "cell_mgmt_define.h"
#include "cell_mgmt_msg.h"

#include "mnt_mgmt_api.h"

#include "om_common_h.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/


/******************************************************************************
 * Description: 
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
INT32 cell_mgmt_module_timer_callback(void *data)
{
    debug();
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle timer syn msg form bord_mgmt
 *
 * Input: msg pointer
 *
 * Output: FUNC_OK / FUNC_ERR
 ******************************************************************************/
INT8 handle_sys_timer_syn(void *msg)
{
    debug();

    if (msg == NULL)
    {
        error("%s:%d NULL pointer.\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    MsgBordSyn *syn_msg = (MsgBordSyn *)msg;

    if (syn_msg->msghead.msg_type != MODULE_BORD_MGMT || 
        syn_msg->msghead.msg_code != BORD_MGMT_MSG_NE_SYNC_STATE)
    {
        return FUNC_ERR;
    }

    switch (syn_msg->bord_syn)
    {
        case NE_SYNC_OK:
            // 1. check whether PHY is ready
            // 2. if PHY is ready, scan cell entity table and trigger cell
                //setup/delete process
            if (g_phy_state != PHY_OK)
            {
                // TODO open phy waiting timer
                return FUNC_OK;
            }

            if (g_ne_sync_state != NE_SYNC_OK)
            {
                error("%s:%d Conflict! sys timer is not synchronized!\n", 
                        __func__, __LINE__);
                return FUNC_ERR;
            }

            // scan vc_entity_table and trigger cell setup process
            int ivc;

            for (ivc = 0; ivc < MAX_CELL_NUM; ivc ++)
            {
                if (vc_entity_table[ivc].cell_rru.rru_status == RRU_ADDED)
                {
                    switch (vc_entity_table[ivc].cell_state)
                    {
                        case CELL_NULL:
                            // trigger cell setup process
                            // send cpri-axc cfg msg to RRUM

                            if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
                            {
                                error("%s:%d unexpected phy or sys timer state\n",
                                        __func__, __LINE__);
                                return FUNC_ERR;
                            }

                            MsgRrumCpriAxcCfgReq cpri_cfg_msg;
                            
                            cpri_cfg_msg.msghead.msg_type   = MODULE_CELL_MGMT;
                            cpri_cfg_msg.msghead.msg_code   = RRUS_MGMT_MSG_CPRI_AxC_CFG_REQ;
                            cpri_cfg_msg.msghead.msg_len    = sizeof(MsgRrumCpriAxcCfgReq);
                            cpri_cfg_msg.rru_id     = vc_entity_table[ivc].cell_rru.rru_id;
                            cpri_cfg_msg.axc_speed  = vc_entity_table[ivc].cell_bandwidth;
                            cpri_cfg_msg.cfg_ind    = 1; // request for axcind assignment

                            send_msg_to_om(MODULE_RRUS_MGMT, 
                                           (UINT8 *)&cpri_cfg_msg, 
                                           sizeof(MsgRrumCpriAxcCfgReq));

                            vc_entity_table[ivc].cell_state = WAIT_SETUP_CPRI_CFG;

                            break;

                        default:
                            // remain unused TODO deal with other cell state
                            break;
                    }
                }
            }

            break;

        case NE_SYNC_ERR:
            // TODO
            break;
            
        default:
            error("%s:%d unexpected msg code\n", __func__, __LINE__);
            return FUNC_ERR;
    }
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle timeout event: rru reg wait timeout
 *
 * Input: arg: 
 *
 * Output: void
 ******************************************************************************/
void callback_handle_timer_cell_rru_wait_timeout(void *arg)
{
    debug();
    
    warning("%s:%d wait rru reg timeout\n", __func__, __LINE__);
}

/******************************************************************************
 * Description: handle timeout event: phy cfg timeout
 *
 * Input: arg: 
 *
 * Output: void
 ******************************************************************************/
void callback_handle_timer_cell_setup_phy_cfg_timeout(void *arg)
{
    debug();
    warning("%s:%d receive cell setup PHY config timeout\n", __func__, __LINE__);
    
    if(arg == NULL)
    {
        error("%s:%d NULL pointer Error\n", __func__, __LINE__);
        return;
    }
    
    UINT32              cell_virt_id;
    MsgPhyCellSetupReq  msg_phy_cell_setup_req;
    
    memcpy(&msg_phy_cell_setup_req, arg, sizeof(MsgPhyCellSetupReq));
    
    cell_virt_id = msg_phy_cell_setup_req.cell_virt_id;
    
    /*  Timeout Handle process
     *   If cell state still is WAIT_SETUP_PHY_CFG means PHY response message
     * not received, then send PHY Cell setup request message again.
     */
    if(vc_entity_table[cell_virt_id-1].cell_state == WAIT_SETUP_PHY_CFG)
    {
        if(0 == send_msg_to_inet(MODULE_CELL_MGMT, 
                                 SOCK_STREAM_VALID, 
                                 vc_entity_table[cell_virt_id-1].cell_phy.unsockfd, 
                                 IP_INVALID,
                                 SOCKADDR_NULL, 
                                 &msg_phy_cell_setup_req, 
                                 sizeof(MsgPhyCellSetupReq)))
        {
            success("%s:%d send phy cfg req success, cell_virt_id = %d\n", 
                    __func__, __LINE__, cell_virt_id);
        }
    }
    else
    {
        success("%s:%d Receive rsp Not Timeout\n", __func__, __LINE__);
        
        free(vc_entity_table[cell_virt_id-1].cell_timer.timer_cell_setup_phy_cfg);
        
        /*  If Cell State change means already receive cellsetup PHY cfg rsp 
         *  Then return, not add timer to timer list again.
         */
        return;
    }
    
    /*  Add cell setup PHY configure timer to timer list again, 
     *  to deal with not receive PHY configure success response again.
     *  if not receive this message still, then callback this function, and 
     *  send this message again and again. 
     *  Maybe can change it to send this msg some times, but send forever.
     */
    sbs_timer_schedule(vc_entity_table[cell_virt_id-1]
                .cell_timer.timer_cell_setup_phy_cfg, 
                    Timeout_Wait_Seconds, 
                    0);
}

/******************************************************************************
 * Description: handle timeout event: mac initialized timeout
 *
 * Input: arg: 
 *
 * Output: void
 ******************************************************************************/
void callback_handle_timer_cell_setup_mac_init_timeout(void *arg)
{
    debug();
    warning("%s:%d receive mac initial rsp timeout\n", __func__, __LINE__);
    
    if(arg == NULL)
    {
        error("%s:%d NULL pointer Error\n", __func__, __LINE__);
        return;
    }
    
    UINT32              cell_virt_id;
    MsgPhyCellSetupRsp  msg_phy_cell_setup_rsp;
    
    memcpy(&msg_phy_cell_setup_rsp, arg, sizeof(MsgPhyCellSetupRsp));
    
    cell_virt_id = msg_phy_cell_setup_rsp.cell_virt_id;
    
    /*  If cell state still is WAIT_SETUP_MAC_INIT means don't receive MAC 
     *  initial success response, then call the "set_mac_init()" function try
     *  to execute MAC process again.
     */
    if(vc_entity_table[cell_virt_id-1].cell_state == WAIT_SETUP_MAC_INIT)
    {
        if(FUNC_OK == set_mac_init(cell_virt_id))
        {
            success("%s:%d set_mac_init successful.\n", __func__, __LINE__);
        }
    }
    else
    {
        success("%s:%d Receive rsp Not Timeout\n", __func__, __LINE__);
        
        free(vc_entity_table[cell_virt_id-1].cell_timer.timer_cell_setup_mac_init);
        
        /*  Cell State is change means already receive MAC initial success rsp 
         *  Then return, not add timer to timer list again.
         */
        return;
    }
    
    /*  Add this timer to timer list again, if not receive MAC initial response
     *  or MAC initial error that we not receive MAC initial response, then call
     *  the "set_mac_init()" again and again until the Cell have a MAC process 
     *  running(receive MAC initial success response).
     */
    sbs_timer_schedule(vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                .cell_timer.timer_cell_setup_mac_init, 
                    Timeout_Wait_Seconds, 
                    0);
}

/******************************************************************************
 * Description: handle timeout event: cell_start timeout
 *
 * Input: arg: 
 *
 * Output: void
 ******************************************************************************/
void callback_handle_timer_cell_setup_cell_start_timeout(void *arg)
{
    debug();
    warning("%s:%d receive cell setup rsp from MAC timeout\n", __func__, __LINE__);
    
    if(arg == NULL)
    {
        error("%s:%d NULL pointer Error\n", __func__, __LINE__);
        return;
    }
    
    UINT32              cell_virt_id;
    MsgMacCellSetupReq  msg_mac_cell_setup_req;
    
    memcpy(&msg_mac_cell_setup_req, arg, sizeof(MsgMacCellSetupReq));
    
    cell_virt_id = msg_mac_cell_setup_req.cell_virt_id;
    
    /*  If cell state still WAIT_SETUP_CELL_START means not receive message of
     *  cell setup response from MAC, so we need send cell setup request again.
     */
    if(vc_entity_table[cell_virt_id-1].cell_state == WAIT_SETUP_CELL_START)
    {
        if(0 == send_msg_to_inet(MODULE_CELL_MGMT, 
                                 SOCK_STREAM_VALID, 
                                 vc_entity_table[cell_virt_id-1].cell_mac.unsockfd, 
                                 IP_INVALID,
                                 SOCKADDR_NULL, 
                                 &msg_mac_cell_setup_req, 
                                 sizeof(MsgMacCellSetupReq)))
        {
            success("%s:%d send mac cell setup req success, cell_virt_id = %d\n", 
                    __func__, __LINE__, cell_virt_id);
        }
    }
    else
    {
        success("%s:%d Receive rsp Not Timeout\n", __func__, __LINE__);
        
        free(vc_entity_table[cell_virt_id-1].cell_timer.timer_cell_setup_cell_start);
        
        /*  cell state change means receive cell setup response from MAC, then
         *  return, not add the timer to timer list again.
         */
        return;
    }

    /* Add the timer to timer list again */
    sbs_timer_schedule(vc_entity_table[cell_virt_id-MIN_CELL_NUM]
            .cell_timer.timer_cell_setup_cell_start, 
                Timeout_Wait_Seconds, 
                0);
}

/******************************************************************************
 * Description: handle timeout event: cell_delete timeout
 *
 * Input: arg: 
 *
 * Output: void
 ******************************************************************************/
void callback_handle_timer_cell_del_cell_close_timeout(void *arg)
{
    debug();
    warning("%s:%d cell delete cell close timeout\n", __func__, __LINE__);
    
    if(arg == NULL)
    {
        error("%s:%d NULL pointer Error\n", __func__, __LINE__);
        return;
    }
    
    UINT32              cell_virt_id;
    MsgMacCellDelReq    msg_mac_cell_del_req;
    
    memcpy(&msg_mac_cell_del_req, arg, sizeof(MsgMacCellDelReq));
    
    cell_virt_id = msg_mac_cell_del_req.cell_virt_id;
    
    /*  If cell state is not WAIT_DEL_CELL_CLOSE means not receive cell delete
     *  response from MAC, then send cell delete request to MAC again.
     */
    if(vc_entity_table[cell_virt_id-1].cell_state == WAIT_DEL_CELL_CLOSE)
    {
        if(0 == send_msg_to_inet(MODULE_CELL_MGMT, 
                                 SOCK_STREAM_VALID, 
                                 vc_entity_table[cell_virt_id-1].cell_mac.unsockfd, 
                                 IP_INVALID,
                                 SOCKADDR_NULL,
                                 &msg_mac_cell_del_req, 
                                 sizeof(MsgMacCellDelReq)))
        {
            success("%s:%d send mac cell del req success, cell_virt_id = %d\n", 
                    __func__, __LINE__, cell_virt_id);
        }
    }
    else
    {
        success("%s:%d Receive rsp Not Timeout\n", __func__, __LINE__);
        
        free(vc_entity_table[cell_virt_id-1].cell_timer.timer_cell_del_cell_close);
        
        /*  cell state change means receive cell delete response from MAC, then
         *  return, not add the timer to timer list again.
         */
        return;
    }
    
    /* Add the timer to timer list */
    sbs_timer_schedule(vc_entity_table[cell_virt_id-MIN_CELL_NUM]
            .cell_timer.timer_cell_del_cell_close, 
                Timeout_Wait_Seconds, 
                0);
}

