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
** File name: cell_mgmt_cell_ctrl.c
** Description: CELL_MGMT module's main control function
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "sbs_msgq.h"
#include "sbs_timer.h"
#include "sbs_type.h"
#include "sbs_log.h"
#include "sbs_task.h"

#include "util_printer.h"
#include "util_systemer.h"

#include "mnt_mgmt_api.h"

#include "cell_mgmt_api.h"
#include "cell_mgmt_cell_entity.h"
#include "cell_mgmt_msg.h"
#include "cell_mgmt_comm_func.h"
#include "cell_mgmt_define.h"
#include "cell_mgmt_sm.h"

#include "om_common_h.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

/******************************************************************************
 * Description: cell_mgmt module recv msgq main control function
 *
 * Input: void (for instance)
 *
 * Output: void
 ******************************************************************************/
void *cell_mgmt_module_main_ctrl(void *arg)
{
    debug();
    warning("Start cell_mgmt_module_main_ctrl, recording msg...\n");
    
    log_msg(LOG_INFO, CELL_MGMT, "Start cell_mgmt_module_main_ctrl \n");

    /*define variable */
    UINT8   msg_buffer[MAX_MSGQ_MSG_LEN];   /*store msgq msg.*/
    
    INT32   ret;
    
    UINT16  cell_virt_id        = 0;    /*store vbs id*/
    UINT16  rru_id              = 0;    /*store vbs id*/
    UINT32  cell_snmp_index     = 0;    /*cell_snmp_index: CFIG_MGMT*/
    UINT32  len_msgq            = 0;    /*length of massage: massage queue.*/
    
    VcSmEventType   sm_event    = 0;    /*event of state machine*/
    
    CommMsgHead     msghead;
    
    /*Msg of msgq*/
    MsgCfigCellDelReq           *msg_cfig_cell_del_req;     /*cfig 2 vcm*/
    MsgRrumCpriAxcCfgRsp        *msg_rrum_cpri_cfg_rsp;     /*rrum 2 vcm*/
    MsgRrumRruRegisterReq       *msg_rru_reg;               /*rrum 2 vcm*/
    MsgPhyCellSetupRsp          *msg_phy_cell_setup_rsp;    /*phy 2 vcm*/
    MsgPhyCellDelRsp            *msg_phy_cell_del_rsp;      /*phy 2 vcm*/
    MsgMacInitRsp               *msg_mac_init_rsp;          /*mac 2 vcm*/
    MsgMacCellSetupRsp          *msg_mac_cell_setup_rsp;    /*mac 2 vcm*/
    MsgMacCellDelRsp            *msg_mac_cell_del_rsp;      /*mac 2 vcm*/
    MsgMacKillRsp               *msg_mac_kill_rsp;          /*vcm 2 vcm*/
    
    /* recv msg from msgq */
    while(1)
    {
        /*receive the msg of vbs_ctrl_msgq.*/
		if (0>=(len_msgq = receive_msgq(MSGQ_ID_CELL_MGMT, msg_buffer, 
                                        sizeof(UINT8)*MAX_MSGQ_MSG_LEN, 
                                        WAIT_FOREVER)))
		{
            error("%s:%d receive msgq error.\n", __func__, __LINE__);
			log_msg(LOG_WARNING, CELL_MGMT, 
                    "Receive error message in cell_mgmt_module_main_ctrl().\n");
			continue;
		}
        else 
        {
            
            if(NULL == memcpy(&msghead, msg_buffer, sizeof(CommMsgHead)))
            {
                error("%s:%d memcpy error\n", __func__, __LINE__);
                continue;
            }
            
            /*msg_type*/
            switch(msghead.msg_type)
            {
/******************************************************************************
 *  MAIN_CTRL msg
 ******************************************************************************/
case MODULE_MAIN_CTRL:
    warning("%s:%d MAIN msg[type(%x), code(%d), len(%d)].\n", 
        __func__, __LINE__, msghead.msg_type, msghead.msg_code, msghead.msg_len);
    /*msg_code*/
    switch(((MsgPhyState *)msg_buffer)->new_phy_state)
    {
        case PHY_OK:
            warning("%s:%d receive PHY_OK Msg from PHY.\n", __func__, __LINE__);
            sm_event = MSG_VC_STOP_RECOVER;
            
            break;
            
        default:
            warning("%s:%d receive PHY not OK Msg from PHY.\n", __func__, __LINE__);
            sm_event = MSG_VC_STOP_REQ;
            
            break;
    }
    
    /* do state machine func */
    if(FUNC_OK == do_cell_mgmt_sm(cell_virt_id, sm_event, (void *)msg_buffer))
    {
        success("%s:%d PHY state change to make cells state change Success.\n", 
            __func__, __LINE__);
    }
    else
    {
        error("%s:%d do_cell_mgmt_sm error\n", __func__, __LINE__);
    }
    
    break;/*<MAIN_CTRL>*/
    
/******************************************************************************
 *  BORD_MGMT msg
 ******************************************************************************/
case MODULE_BORD_MGMT:
    warning("%s:%d BORD msg[type(%x), code(%d), len(%d)].\n", 
        __func__, __LINE__, msghead.msg_type, msghead.msg_code, msghead.msg_len);
    /*msg_code*/
    switch(msghead.msg_code)
    {
        case BORD_MGMT_MSG_NE_SYNC_STATE:
            warning("%s:%d BORD's timer sync msg\n", __func__, __LINE__);
            //TODO
            switch(((MsgBordSyn *)msg_buffer)->bord_syn)
            {
                case NE_SYNC_OK:
                    warning("%s:%d NE_SYNC_OK msg.\n", __func__, __LINE__);
                    sm_event = MSG_VC_STOP_RECOVER;
                    break;
                    
                default:
                    warning("%s:%d timer not sync msg.\n", __func__, __LINE__);
                    sm_event = MSG_VC_STOP_REQ;
                    break;
            }
            
            break;
            
        default:
            error("%s:%d BORD's no this kind of msg\n", __func__, __LINE__);
            break;
    }
    /* do state machine func */
    if(FUNC_OK == do_cell_mgmt_sm(cell_virt_id, sm_event, (void *)msg_buffer))
    {
        success("%s:%d Timer Sync change to make cells state change Success.\n", 
            __func__, __LINE__);
    }
    else
    {
        error("%s:%d do_cell_mgmt_sm error\n", __func__, __LINE__);
    }
    
    break;/*<BORD_MGMT>*/
    
/******************************************************************************
 *  CFIG_MGMT msg
 ******************************************************************************/
case MODULE_CFIG_MGMT:
    warning("%s:%d CFIG msg[type(%x), code(%d), len(%d)].\n", 
        __func__, __LINE__, msghead.msg_type, msghead.msg_code, msghead.msg_len);
    /*msg_code*/
    switch(msghead.msg_code)
    {
        case CFIG_MGMT_MSG_VC_SETUP_REQ:
            warning("%s:%d CFIG's vc setup msg\n", __func__, __LINE__);
            
            if((ret = vcm_handle_cell_setup((void *)msg_buffer)) == FUNC_ERR)
            {
                error("%s:%d No cell_virt_id available!!\n", __func__, __LINE__);
                continue;
            }
            else
            {
                cell_virt_id = ret;
                
                info("%s:%d do_cell_mgmt_sm: cell_virt_id=%d\n",
                    __func__, __LINE__, cell_virt_id);
                    
                sm_event = MSG_VC_SETUP_REQ;
            }
            log_msg(LOG_INFO, CELL_MGMT, "Receive cell setup message.\n");
            //TODO
            break;
        
        case CFIG_MGMT_MSG_VC_DEL_REQ:
        
            warning("%s:%d CFIG's vc delete msg\n", __func__, __LINE__);
            
            log_msg(LOG_INFO, CELL_MGMT, "Receive cell delete message.\n");
                    
            msg_cfig_cell_del_req = (MsgCfigCellDelReq *)msg_buffer;
            #if 0
            cell_snmp_index = msg_cfig_cell_del_req->cell_snmp_index;
            cell_virt_id = get_cell_virt_id_use_cell_snmp_index(cell_snmp_index);
            #else
            cell_virt_id = msg_cfig_cell_del_req->cell_snmp_index;
            #endif
            
            info("cell_virt_id = %d, cell_snmp_index = %d\n",
                    cell_virt_id, cell_snmp_index);
                    
            sm_event = MSG_VC_DEL_REQ;
            
            //TODO
            break;
        
        /**/
        default:
            error("%s:%d No this kind of msg_code\n", __func__, __LINE__);
            log_msg(LOG_ERR, CELL_MGMT, "No this kind of msg_code\n");
            //TODO
            break;
    }//switch(msghead.msg_code)->cfig
    
    /**/
    if(FUNC_OK == do_cell_mgmt_sm(cell_virt_id, sm_event, (void *)msg_buffer))
    {
        show_cell_entity_on_screen(cell_virt_id);
    }
    else
    {
        error("%s:%d do_cell_mgmt_sm error\n", __func__, __LINE__);
    }
    
    break;/*<CFIG_MGMT>*/
    
/******************************************************************************
 *  RRUS_MGMT msg
 ******************************************************************************/
case MODULE_RRUS_MGMT:
    warning("%s:%d RRUM msg[type(%x), code(%d), len(%d)].\n", 
        __func__, __LINE__, msghead.msg_type, msghead.msg_code, msghead.msg_len);
    /*msg_code*/
    switch(msghead.msg_code)
    {
        /*rru reg*/
        case RRUS_MGMT_MSG_RRU_REG_REQ:
            warning("%s:%d RRU_REG_REQ\n", __func__, __LINE__);
            log_msg(LOG_INFO, CELL_MGMT, "Receive rru register message.\n");
            //TODO
            msg_rru_reg = (MsgRrumRruRegisterReq *)msg_buffer;
            
            handle_rru_reg(msg_rru_reg);
            
            /*if rru deleted, turn cell state to CELL_STOP */
            switch(msg_rru_reg->rru_reg)
            {
                case RRU_DEL:
                    sm_event = MSG_VC_STOP_REQ;
                    break;
                    
                case RRU_ADD:
                    sm_event = MSG_VC_STOP_RECOVER;
                    break;
                    
                default:
                    break;
            }
            
            show_rru_info_table_on_screen();
            
            break;
        
        case RRUS_MGMT_MSG_CPRI_AxC_CFG_RSP:
            warning("%s:%d CPRI_AxC_CFG_RSP\n", __func__, __LINE__);
            
            msg_rrum_cpri_cfg_rsp   = (MsgRrumCpriAxcCfgRsp *)msg_buffer;
            rru_id                  = msg_rrum_cpri_cfg_rsp->rru_id;
            cell_virt_id            = get_cell_virt_id_use_rru_id(rru_id);
            
            if(vc_entity_table[cell_virt_id-1].cell_state == WAIT_DEL_CPRI_RESET)
            {
                sm_event = MSG_VC_DEL_CPRI_RESET_RSP;
            }
            else if(vc_entity_table[cell_virt_id-1].cell_state == WAIT_SETUP_CPRI_CFG)
            {
                sm_event = MSG_VC_SETUP_CPRI_CFG_RSP;
            }
            else
            {
                error("%s:%d CPRI_AxC_CFG_RSP error\n", __func__, __LINE__);
                continue;
            }
            
            log_msg(LOG_INFO, CELL_MGMT, "Receive cpri-axc configure message.\n");
            //TODO
            break;
            
        default:
            error("%s:%d No this kind of msg_code\n", __func__, __LINE__);
            log_msg(LOG_ERR, CELL_MGMT, "No this kind of msg_code\n");
            //TODO
            break;
    }//switch
    
    if(FUNC_OK == do_cell_mgmt_sm(cell_virt_id, sm_event, (void *)msg_buffer))
    {
        show_cell_entity_on_screen(cell_virt_id);
    }
    else
    {
        error("%s:%d do_cell_mgmt_sm error\n", __func__, __LINE__);
    }
            
    break;/*<RRUM_MGMT>*/

/******************************************************************************
 * PHY msg
 ******************************************************************************/
case MSG_TYPE_PHY_CELL_MGMT:
    warning("%s:%d PHY msg[type(%x), code(%d), len(%d)].\n", 
        __func__, __LINE__, msghead.msg_type, msghead.msg_code, msghead.msg_len);
    /*msg_code*/
    switch(msghead.msg_code)
    {
        case PHY_VCM_CELL_SETUP_RSP:
            warning("%s:%d PHY_VCM_CELL_SETUP_RSP\n", __func__, __LINE__);
            msg_phy_cell_setup_rsp  = (MsgPhyCellSetupRsp *)msg_buffer;
            cell_virt_id            = msg_phy_cell_setup_rsp->cell_virt_id;
            sm_event                = MSG_VC_SETUP_PHY_CFG_RSP;
            break;
            
        case PHY_VCM_CELL_DELETE_RSP:
            warning("%s:%d PHY_VCM_CELL_DELETE_RSP\n", __func__, __LINE__);
            msg_phy_cell_del_rsp    = (MsgPhyCellDelRsp *)msg_buffer;
            cell_virt_id            = msg_phy_cell_del_rsp->cell_virt_id;
            sm_event                = MSG_VC_DEL_PHY_DEL_RSP;
            break;
            
        default:
            error("%s:%d no this kind of msg_code Error.\n", __func__, __LINE__);
            //TODO
            break;
    }//switch(msghead.msg_code)->PHY
    
    if(FUNC_OK == do_cell_mgmt_sm(cell_virt_id, sm_event, (void *)msg_buffer))
    {
        show_cell_entity_on_screen(cell_virt_id);
    }
    else
    {
        error("%s:%d do_cell_mgmt_sm error\n", __func__, __LINE__);
    }
    break; /*<PHY>*/

/******************************************************************************
 * MAC msg
 ******************************************************************************/
case MSG_TYPE_MAC_CELL_MGMT:
    warning("%s:%d MAC msg[type(%x), code(%d), len(%d)].\n", 
        __func__, __LINE__, msghead.msg_type, msghead.msg_code, msghead.msg_len);
    /*msg_code*/
    switch(msghead.msg_code)
    {
        case MAC_VCM_MAC_INIT_RSP:
            warning("%s:%d MAC_VCM_MAC_INIT_RSP\n", __func__, __LINE__);
            msg_mac_init_rsp        = (MsgMacInitRsp *)msg_buffer;
            cell_virt_id            = msg_mac_init_rsp->cell_virt_id;
            sm_event                = MSG_VC_SETUP_MAC_INIT_RSP;
            
            break;
            
        case MAC_VCM_CELL_START_RSP:
            warning("%s:%d MAC_VCM_CELL_START_RSP\n", __func__, __LINE__);
            msg_mac_cell_setup_rsp  = (MsgMacCellSetupRsp *)msg_buffer;
            cell_virt_id            = msg_mac_cell_setup_rsp->cell_virt_id;
            sm_event                = MSG_VC_SETUP_CELL_START_RSP;
            
            break;
            
        case MAC_VCM_CELL_DELET_RSP:
            warning("%s:%d MAC_VCM_CELL_DELET_RSP\n", __func__, __LINE__);
            msg_mac_cell_del_rsp    = (MsgMacCellDelRsp *)msg_buffer;
            cell_virt_id            = msg_mac_cell_del_rsp->cell_virt_id;
            sm_event                = MSG_VC_DEL_CELL_CLOSE_RSP;
            break;
            
        case MAC_KILL_RSP:
            warning("%s:%d MAC_KILL_RSP\n", __func__, __LINE__);
            msg_mac_kill_rsp        = (MsgMacKillRsp *)msg_buffer;
            cell_virt_id            = msg_mac_kill_rsp->cell_virt_id;
            sm_event                = MSG_VC_DEL_MAC_DEL_RSP;
            break;
            
        default:
            error("%s:%d no this kind of msg_code Error.\n", __func__, __LINE__);
            //TODO
            break;
    }//switch(msghead.msg_code)->MAC
    
    if(FUNC_OK == do_cell_mgmt_sm(cell_virt_id, sm_event, (void *)msg_buffer))
    {
        show_cell_entity_on_screen(cell_virt_id);
    }
    else
    {
        error("%s:%d do_cell_mgmt_sm error\n", __func__, __LINE__);
    }
    break; /*<MAC>*/
    
/******************************************************************************
 * Other Msgs
 ******************************************************************************/
default:
    //TODO
    error("%s:%d no this kind of msg_type Error. msg[type(%x), code(%d), len(%d)]\n", 
        __func__, __LINE__, msghead.msg_type, msghead.msg_code, msghead.msg_len);
    break;/*<default>*/
    
    
/******************************************************************************
 * End of switch(msghead.msg_type)
 ******************************************************************************/
            }//switch(msghead.msg_type)
        }//else: msg process
    }//while(1)
    
quit_while_1:

    close_msgq(CELL_MGMT_MSGQ_NAME, MSGQ_ID_CELL_MGMT);
    success("cell_mgmt close_msgq success %s\n", CELL_MGMT_MSGQ_NAME);
}/*<cell_mgmt_module_main_ctrl>*/
