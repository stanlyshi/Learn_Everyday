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
** File name: cell_mgmt_cell_stop.c
** Description: alter cell state to CELL_STOP and recovery handle functions
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

#include "sbs_msgq.h"
#include "sbs_type.h"
#include "sbs_log.h"

#include "util_systemer.h"
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
 * Description: alter cell state to CELL_STOP for rru register/phy OK/Sync
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_state_stop_req(UINT32 cell_virt_id,void *p_msg_info)
{
    debug();
    
    /*figure NULL pointer*/
    if (p_msg_info == NULL)
    {
        error("%s:%d NULL pointer.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    UINT32      id;
    VcSmState   cell_state = vc_entity_table[cell_virt_id-1].cell_state;
    
    if(cell_state <= VC_STATE_MIN_NUM || cell_state >= VC_STATE_MAX_NUM)
    {
        error("%s:%d cell state error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    CommMsgHead                 msghead;
    MsgBordSyn                  msg_bord_time_sync;
    MsgRrumRruRegisterReq       msg_rru_reg;
    MsgPhyState                 msg_phy_state;
    
    if(NULL == memcpy(&msghead, p_msg_info, sizeof(CommMsgHead)))
    {
        error("%s:%d memcpy error\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    switch(msghead.msg_type)
    {
        /*main ctrl module phy state msg*/
        case MODULE_MAIN_CTRL:
            memcpy(&msg_phy_state, p_msg_info, sizeof(MsgPhyState));
            
            if(msg_phy_state.new_phy_state != PHY_OK)
            {
                info("%s:%d set all cells state to STOP\n", __func__, __LINE__);
                
                /*set all cells state to CELL_STOP state */
                for(id = MIN_CELL_NUM; id<=MAX_CELL_NUM; id++)
                {
                    cell_state = vc_entity_table[id-1].cell_state;
                    vc_entity_table[id-1].cell_state        = CELL_STOP;
                    vc_entity_table[id-1].cell_state_tmp    = cell_state;
                }
            }
            
            break;
            
        /*bord mgmt module timer sync msg*/
        case MODULE_BORD_MGMT:
            memcpy(&msg_bord_time_sync, p_msg_info, sizeof(MsgBordSyn));
            
            /* system timer not sync */
            if(msg_bord_time_sync.bord_syn != NE_SYNC_OK)
            {
                info("%s:%d set all cells state to STOP\n", __func__, __LINE__);
                
                /*set all cells state to CELL_STOP state */
                for(id = MIN_CELL_NUM; id<=MAX_CELL_NUM; id++)
                {
                    cell_state = vc_entity_table[id-1].cell_state;
                    vc_entity_table[id-1].cell_state        = CELL_STOP;
                    vc_entity_table[id-1].cell_state_tmp    = cell_state;
                }
            }
            break;
            
        /*rru mgmt module rru delete msg*/
        case MODULE_RRUS_MGMT:
            memcpy(&msg_rru_reg, p_msg_info, sizeof(MsgRrumRruRegisterReq));
            
            if(msghead.msg_code == RRUS_MGMT_MSG_RRU_REG_REQ && 
                msg_rru_reg.rru_reg == RRU_DEL)
            {
                vc_entity_table[cell_virt_id-1].cell_state      = CELL_STOP;
                vc_entity_table[cell_virt_id-1].cell_state_tmp  = cell_state;
            }
            break;
            
        /*default*/
        default:
            break;
    
    }/*end of "switch(msghead.msg_type)"*/
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: recover cell state from CELL_STOP for rru register/phy OK/Sync
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_state_stop_recover(UINT32 cell_virt_id,void *p_msg_info)
{
    debug();
    
    /*figure NULL pointer*/
    if (p_msg_info == NULL)
    {
        error("%s:%d NULL pointer.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    /*if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }*/
    
    UINT32    id;
    VcSmState cell_state_tmp;
    
    CommMsgHead                 msghead;
    MsgBordSyn                  msg_bord_time_sync;
    MsgRrumRruRegisterReq       msg_rru_reg;
    MsgPhyState                 msg_phy_state;
    
    if(NULL == memcpy(&msghead, p_msg_info, sizeof(CommMsgHead)))
    {
        error("%s:%d memcpy error\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    switch(msghead.msg_type)
    {
        /*main ctrl module phy state msg*/
        case MODULE_MAIN_CTRL:
            memcpy(&msg_phy_state, p_msg_info, sizeof(MsgPhyState));
            
            if(msg_phy_state.new_phy_state == PHY_OK)
            {
                info("%s:%d recover all cells state from STOP\n", __func__, __LINE__);
                
                /*set all cells state to CELL_STOP state */
                for(id = MIN_CELL_NUM; id<=MAX_CELL_NUM; id++)
                {
                    cell_state_tmp = vc_entity_table[id-1].cell_state_tmp;
                    vc_entity_table[id-1].cell_state    = cell_state_tmp;
                }
                handle_phy_state_change(p_msg_info);
            }
            
            break;
            
        /*bord mgmt module timer sync msg*/
        case MODULE_BORD_MGMT:
            memcpy(&msg_bord_time_sync, p_msg_info, sizeof(MsgBordSyn));
            
            /* system timer not sync */
            if(msg_bord_time_sync.bord_syn == NE_SYNC_OK)
            {
                info("%s:%d set all cells state to STOP\n", __func__, __LINE__);
                
                /*set all cells state to CELL_STOP state */
                for(id = MIN_CELL_NUM; id<=MAX_CELL_NUM; id++)
                {
                    cell_state_tmp = vc_entity_table[id-1].cell_state_tmp;
                    vc_entity_table[id-1].cell_state    = cell_state_tmp;
                }
                handle_sys_timer_syn(p_msg_info);
            }
            break;
            
        /*rru mgmt module rru delete msg*/
        case MODULE_RRUS_MGMT:
            memcpy(&msg_rru_reg, p_msg_info, sizeof(MsgRrumRruRegisterReq));
            
            if(msghead.msg_code == RRUS_MGMT_MSG_RRU_REG_REQ && 
                msg_rru_reg.rru_reg == RRU_ADD)
            {

                cell_state_tmp = vc_entity_table[cell_virt_id-1].cell_state_tmp;
                vc_entity_table[cell_virt_id-1].cell_state  = cell_state_tmp;
                handle_rru_reg(p_msg_info);
                
            }
            break;
            
        /*default*/
        default:
            break;
    
    }/*end of "switch(msghead.msg_type)"*/
    
    return FUNC_OK;
}
