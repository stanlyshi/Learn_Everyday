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
** File name: cell_mgmt_cell_setup.c
** Description: cell setup process handle functions
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
#include <string.h>

#include "sbs_msgq.h"
#include "sbs_type.h"
#include "sbs_log.h"

#include "util_printer.h"
#include "util_systemer.h"

#include "mnt_mgmt_api.h"

#include "cell_mgmt_api.h"
#include "cell_mgmt_cell_entity.h"
#include "cell_mgmt_comm_func.h"
#include "cell_mgmt_define.h"
#include "cell_mgmt_msg.h"

#include "rrus_mgmt_rru_entity.h"

#include "om_common_h.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/
/******************************************************************************
 * Description: handle MsgCfigCellSetupReq from config module,
 * when this handle is invoked, cell entity info is filled
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 * Author: Liu Yixin
 ******************************************************************************/
INT32 vcm_handle_cell_setup(void *p_msg_info)
{
    debug();

    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    /*figure NULL pointer*/
    if (p_msg_info == NULL)
    {
        error("%s:%d NULL pointer.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    /* store req info into vc_entity_table for future process */
    /* invoke vcm_handle_cell_setup_req */
    int cell_virt_id;
    int irru;
    
    info("%s:%d get MsgCfigCellSetupReq. %s\n", __func__, __LINE__, get_system_time());
    
    MsgCfigCellSetupReq *msg_cfig_cell_setup_req = (MsgCfigCellSetupReq *)p_msg_info;
#if 0
    for (cell_virt_id = MIN_CELL_NUM; cell_virt_id <= MAX_CELL_NUM; cell_virt_id ++)
    {
        if(vc_entity_table[cell_virt_id-1].cell_snmp_index  
                == msg_cfig_cell_setup_req->cell_snmp_index)
        {
            error("%s:%d already exist cell_snmp_index = %d, cell_virt_id = %d\n", 
                  __func__, __LINE__, msg_cfig_cell_setup_req->cell_snmp_index, cell_virt_id);
            return FUNC_ERR;
        }
        if (vc_entity_table[cell_virt_id-1].cell_state == VC_STATE_MIN_NUM)
        {
            info("%s:%d setup a cell: state 0 to NULL\n", 
                  __func__, __LINE__);

            vc_entity_table[cell_virt_id-1].cell_state       = CELL_NULL;
            vc_entity_table[cell_virt_id-1].cell_state_tmp	 = CELL_NULL;
            vc_entity_table[cell_virt_id-1].cell_virt_id     = cell_virt_id;

            /* fill in info from cell_setup_req */
            vc_entity_table[cell_virt_id-1].cell_snmp_index  = msg_cfig_cell_setup_req->cell_snmp_index;
            vc_entity_table[cell_virt_id-1].cell_g_id        = msg_cfig_cell_setup_req->global_cell_id;
            vc_entity_table[cell_virt_id-1].cell_mode        = msg_cfig_cell_setup_req->cell_mode;
            vc_entity_table[cell_virt_id-1].cell_bandwidth   = msg_cfig_cell_setup_req->cell_bandwidth;
            vc_entity_table[cell_virt_id-1].cell_is_uni      = msg_cfig_cell_setup_req->cell_is_uni;
            
            memcpy(vc_entity_table[cell_virt_id-1].cell_mac.mac_cfg_pth, 
                   msg_cfig_cell_setup_req->mac_cfg_pth,
                   sizeof(vc_entity_table[cell_virt_id-1].cell_mac.mac_cfg_pth));

            /* fill in rru info */
            if (msg_cfig_cell_setup_req->rru_item.rru_id != 0)
            {
                vc_entity_table[cell_virt_id-1].cell_rru.rru_id 
                        = msg_cfig_cell_setup_req->rru_item.rru_id;
            }
            
            memcpy(vc_entity_table[cell_virt_id-1].cell_rru.rru_sn, 
                    msg_cfig_cell_setup_req->rru_item.rru_sn,
                    sizeof(vc_entity_table[cell_virt_id-1].cell_rru.rru_sn));
                    
            // by now, axc speed = cell_bandwidth
            vc_entity_table[cell_virt_id-1].cell_rru.axc_speed 
                        = msg_cfig_cell_setup_req->cell_bandwidth;
            
            // when cell is not uni, fill in rru ip and port
            if (msg_cfig_cell_setup_req->cell_is_uni == 0)
            {
                info("%s:%d this cell is cell_is_uni:%d\n", 
                      __func__, __LINE__,msg_cfig_cell_setup_req->cell_is_uni);
                memcpy(vc_entity_table[cell_virt_id-1].cell_rru.rru_ip, 
                       msg_cfig_cell_setup_req->rru_item.rru_ip,
                       sizeof(vc_entity_table[cell_virt_id-1].cell_rru.rru_ip));
                       
                vc_entity_table[cell_virt_id-1].cell_rru.rru_port 
                        = msg_cfig_cell_setup_req->rru_item.rru_port;
                        
                info("%s:%d rru_id = %d, rru_ip = %s, rru_port = %d, "\
                      "sizeof(rru_ip) = %d\n", __func__, __LINE__,                
                      vc_entity_table[cell_virt_id-1].cell_rru.rru_id,
                      vc_entity_table[cell_virt_id-1].cell_rru.rru_ip,
                      vc_entity_table[cell_virt_id-1].cell_rru.rru_port,
                      sizeof(vc_entity_table[cell_virt_id-1].cell_rru.rru_ip));
            }
            break;
        }
    }
#else
	cell_virt_id = msg_cfig_cell_setup_req->cell_snmp_index;
	vc_entity_table[cell_virt_id-1].cell_state       = CELL_NULL;
    vc_entity_table[cell_virt_id-1].cell_state_tmp	 = CELL_NULL;
    vc_entity_table[cell_virt_id-1].cell_virt_id     = msg_cfig_cell_setup_req->cell_snmp_index;

    /* fill in info from cell_setup_req */
    vc_entity_table[cell_virt_id-1].cell_snmp_index  = msg_cfig_cell_setup_req->cell_snmp_index;
    vc_entity_table[cell_virt_id-1].cell_g_id        = msg_cfig_cell_setup_req->global_cell_id;
    vc_entity_table[cell_virt_id-1].cell_mode        = msg_cfig_cell_setup_req->cell_mode;
    vc_entity_table[cell_virt_id-1].cell_bandwidth   = msg_cfig_cell_setup_req->cell_bandwidth;
    vc_entity_table[cell_virt_id-1].cell_is_uni      = msg_cfig_cell_setup_req->cell_is_uni;
    
    memcpy(vc_entity_table[cell_virt_id-1].cell_mac.mac_cfg_pth, 
           msg_cfig_cell_setup_req->mac_cfg_pth,
           sizeof(vc_entity_table[cell_virt_id-1].cell_mac.mac_cfg_pth));

    /* fill in rru info */
    if (msg_cfig_cell_setup_req->rru_item.rru_id != 0)
    {
        vc_entity_table[cell_virt_id-1].cell_rru.rru_id 
                = msg_cfig_cell_setup_req->rru_item.rru_id;
    }
    
    memcpy(vc_entity_table[cell_virt_id-1].cell_rru.rru_sn, 
            msg_cfig_cell_setup_req->rru_item.rru_sn,
            sizeof(vc_entity_table[cell_virt_id-1].cell_rru.rru_sn));
            
    // by now, axc speed = cell_bandwidth
    vc_entity_table[cell_virt_id-1].cell_rru.axc_speed 
                = msg_cfig_cell_setup_req->cell_bandwidth;
    
    // when cell is not uni, fill in rru ip and port
    if (msg_cfig_cell_setup_req->cell_is_uni == 0)
    {
        info("%s:%d this cell is cell_is_uni:%d\n", 
              __func__, __LINE__,msg_cfig_cell_setup_req->cell_is_uni);
        memcpy(vc_entity_table[cell_virt_id-1].cell_rru.rru_ip, 
               msg_cfig_cell_setup_req->rru_item.rru_ip,
               sizeof(vc_entity_table[cell_virt_id-1].cell_rru.rru_ip));
               
        vc_entity_table[cell_virt_id-1].cell_rru.rru_port 
                = msg_cfig_cell_setup_req->rru_item.rru_port;
                
        info("%s:%d rru_id = %d, rru_ip = %s, rru_port = %d, "\
              "sizeof(rru_ip) = %d\n", __func__, __LINE__,                
              vc_entity_table[cell_virt_id-1].cell_rru.rru_id,
              vc_entity_table[cell_virt_id-1].cell_rru.rru_ip,
              vc_entity_table[cell_virt_id-1].cell_rru.rru_port,
              sizeof(vc_entity_table[cell_virt_id-1].cell_rru.rru_ip));
    }
#endif
    return cell_virt_id;
}


/******************************************************************************
 * Description: handle cell setup request msg 
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_setup_req(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    if(p_msg_info == NULL)
    {
        error("%s:%d p_msg_info is NULL\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    /* validate cell state: make sure the current cell state is CELL_NULL */
    if(vc_entity_table[cell_virt_id - 1].cell_state != CELL_NULL)
    {
        error("%s:%d current cell state of cell %d is not expected CELL_NULL\n", 
                __func__, __LINE__, cell_virt_id);
    	return FUNC_ERR;
    }
    /* see if requested rru is available, if added, send cpri-axc cfg msg to RRUM */
    /* change cell state into WAIT_SETUP_CPRI_CFG */
    int irru;
    MsgCfigCellSetupReq *msg_cfig_cell_setup_req = (MsgCfigCellSetupReq *)p_msg_info;

    for (irru = 0; irru < MAX_RRU_NUM; irru ++)
    {
        if (memcmp(rru_info_table[irru].rru_sn, msg_cfig_cell_setup_req->rru_item.rru_sn,
                   strlen(rru_info_table[irru].rru_sn)>
                   strlen(msg_cfig_cell_setup_req->rru_item.rru_sn)?
                   strlen(rru_info_table[irru].rru_sn):
                   strlen(msg_cfig_cell_setup_req->rru_item.rru_sn)) == 0)
        {
            if (rru_info_table[irru].rru_status == RRU_ADDED)
            {
                /* fill rru status in cell entity */
                vc_entity_table[cell_virt_id-1].cell_rru.rru_status = RRU_ADDED;
                // if rru is registered (added), rru_info_table has stored it rru_id

                /* store rru id into corresponding cell entity */
                if (vc_entity_table[cell_virt_id-1].cell_rru.rru_id == 0)
                {
                    vc_entity_table[cell_virt_id-1].cell_rru.rru_id =
                            rru_info_table[irru].rru_id;
                }

                
                MsgRrumCpriAxcCfgReq msg_rrum_cpri_cfg_req;
                
                msg_rrum_cpri_cfg_req.msghead.msg_type  = MODULE_CELL_MGMT;
                msg_rrum_cpri_cfg_req.msghead.msg_code  = RRUS_MGMT_MSG_CPRI_AxC_CFG_REQ;
                msg_rrum_cpri_cfg_req.msghead.msg_len   = sizeof(MsgRrumCpriAxcCfgReq);
                msg_rrum_cpri_cfg_req.rru_id    = vc_entity_table[cell_virt_id-1].cell_rru.rru_id;
                msg_rrum_cpri_cfg_req.axc_speed = vc_entity_table[cell_virt_id-1].cell_bandwidth;
                msg_rrum_cpri_cfg_req.cfg_ind   = 1;
                
                // TODO start send cpri cfg timer
                //comment this when RRUM module has been initialized
                if (g_ne_sync_state != NE_SYNC_OK)
                {
                    error("%s:%d SYNC state error\n", __func__, __LINE__);
                    return FUNC_ERR;
                }
                
                if(0 == send_msg_to_om(MODULE_RRUS_MGMT, 
                                       (INT8 *)&msg_rrum_cpri_cfg_req, 
                                       sizeof(MsgRrumCpriAxcCfgReq)))
                {
                    success("%s:%d send cpri cfg req to RRUS_MGMT Success. "\
                            "cell_virt_id:%d\n", 
                            __func__, __LINE__, cell_virt_id);
                }
                else
                {
                    error("%s:%d send cpri cfg req to RRUS_MGMT Error. "\
                          "cell_virt_id:%d\n", __func__, __LINE__, cell_virt_id);
                    return FUNC_ERR;
                }
                
                info("%s:%d vc_entity_table[%d].state=WAIT_SETUP_CPRI_CFG \n", 
                      __func__, __LINE__, cell_virt_id-1);
                      
                /* change celll state */
                if(vc_entity_table[cell_virt_id - 1].cell_state == CELL_NULL)
                {
                    vc_entity_table[cell_virt_id - 1].cell_state = WAIT_SETUP_CPRI_CFG;
                }
                else
                {
                    return FUNC_ERR;
                }
                
                break;
            }
            else /* if rru status is not added, start rru wait timer in this cell entity */
            {
                // TODO start rru wait timer
                break;
            }
        }
    }
    return FUNC_OK;
}


/******************************************************************************
 * Description: handle cpri configure response msg 
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_setup_cpri_cfg_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    /* Check for null pointer.*/
    if (NULL == p_msg_info) 
    {
        error("%s:%d NULL pointer\n", __func__, __LINE__);
        log_msg(LOG_ERR, CELL_MGMT, "the pointer of p_msg_info is"\
                     " NULL in vcm_handle_cell_setup_cpri_cfg_rsp().\n");
        return FUNC_ERR;
    }

    /* Check for cell_virt_id.*/
    if ((cell_virt_id < MIN_CELL_NUM) || (cell_virt_id > MAX_CELL_NUM)) 
    {
        log_msg(LOG_ERR, CELL_MGMT, "the cell_virt_id is exceed in "
                             "vcm_handle_cell_setup_cpri_cfg_rsp().\n");
        return FUNC_ERR;
    }
    
    //check cell state
    if (vc_entity_table[cell_virt_id-1].cell_state != WAIT_SETUP_CPRI_CFG) 
    {
        error("%s:%d current cell state of cell %d is not expected "\
                "WAIT_SETUP_CPRI_CFG\n",
        		__func__, __LINE__, cell_virt_id);
    	return FUNC_ERR;
    }
    /*declare the variable.*/
    MsgRrumCpriAxcCfgRsp *msg_rrum_cpri_axc_cfg_rsp  = 
                (MsgRrumCpriAxcCfgRsp *)p_msg_info;
    
    INT8 re_value = 0;

    if(msg_rrum_cpri_axc_cfg_rsp->err_type == 0)
    {
        success("cpri cfg success.\n");
        info("now handle cpri cfg success info.\n");
        re_value = vcm_handle_cell_setup_cpri_cfg_succ_rsp(cell_virt_id, p_msg_info);

    }
    else
    {
        error("cpri cfg failed.\n");
        re_value = vcm_handle_cell_setup_cpri_cfg_fail_rsp(cell_virt_id, p_msg_info);
    }
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle cpri configure success response msg 
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_setup_cpri_cfg_succ_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    /* Check for null pointer.*/
    if (NULL == p_msg_info)
    {
        error("%s:%d p_msg_info is NULL\n", __func__, __LINE__);
        log_msg(LOG_ERR, CELL_MGMT, "the pointer of p_msg_info is"\
                     " NULL in vcm_handle_cell_setup_cpri_cfg_succ_rsp().\n");
        return FUNC_ERR;
    }

    /* Check for cell_virt_id.*/
    if ((cell_virt_id < MIN_CELL_NUM) || (cell_virt_id > MAX_CELL_NUM)) 
    {
        log_msg(LOG_ERR, CELL_MGMT, "the cell_virt_id is exceed in "
                             "vcm_handle_cell_setup_cpri_cfg_succ_rsp().\n");
        return FUNC_ERR;
    }
    
    /*the pointer of message between vcm and rrum.*/
    MsgRrumCpriAxcCfgRsp *msg_rrum_cpri_axc_cfg_rsp = 
            (MsgRrumCpriAxcCfgRsp *)p_msg_info;

    info("%s:%d axc_ind = %d\n", __func__, __LINE__, 
            msg_rrum_cpri_axc_cfg_rsp->axc_ind);
    
    //save cpri axcind
    vc_entity_table[cell_virt_id-1].cell_rru.axc_index = 
            msg_rrum_cpri_axc_cfg_rsp->axc_ind;
    
    MsgPhyCellSetupReq msg_phy_cell_setup_req; /*the massage about vcm and phy.*/
    
    /*消息头*/
    msg_phy_cell_setup_req.msghead.msg_type   = MSG_TYPE_PHY_CELL_MGMT;
    msg_phy_cell_setup_req.msghead.msg_code   = VCM_PHY_CELL_SETUP_REQ;
    msg_phy_cell_setup_req.msghead.msg_len    = sizeof(MsgPhyCellSetupReq);

    /*build the massage that vcm send to PHY.*/
    //axc_index
    msg_phy_cell_setup_req.axc_ind        = 
            vc_entity_table[cell_virt_id-1].cell_rru.axc_index;
    msg_phy_cell_setup_req.cell_virt_id   = cell_virt_id;
    
    //fpga_mac
    UINT8 fpga_mac[6];
    memset(fpga_mac, 0, 6);
    
    UINT8 ret;
    
    if((ret = get_data_fpga_mac(fpga_mac)) == -1)
    {
        error("%s:%d get_data_fpga_mac Error\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    memcpy(msg_phy_cell_setup_req.fpga_mac, fpga_mac[0], 6);

    //modify vc before phy rsp, because there is no place to store data
    memcpy(vc_entity_table[cell_virt_id-1].cell_phy.FpgaMac, fpga_mac,
    		sizeof(vc_entity_table[cell_virt_id-1].cell_phy.FpgaMac));
    
    //fibernum
    /* EqIndex index;
    msg_phy_cell_setup_req.fiber_num = (UINT32)index.fpga_id; */

    UINT16 fiber_num = vc_entity_table[cell_virt_id - 1].cell_rru.rru_id & 0xf0;
    fiber_num = fiber_num >> 4;
    
    UINT32 fiber_num_phy = 1;
    fiber_num_phy = fiber_num_phy << fiber_num;
    msg_phy_cell_setup_req.fiber_num = fiber_num_phy;

    info("%s:%d cfg phy table\n", __func__, __LINE__);
    
    //phycellindex
    INT8 iphy;
    for(iphy=0; iphy<MAX_PHY_INDEX; iphy++)
    {
        if(phy_state_table[iphy] == 0)
        {
            phy_state_table[iphy]       =   1;
            msg_phy_cell_setup_req.cell_phy_index =   iphy;
            
            //should be add in succ rsp but there is no place to store data
            //should start from 1
            success("%s:%d cfg vc entity phy index = %d\n", 
                    __func__, __LINE__,iphy + 1);
            vc_entity_table[cell_virt_id-1].cell_phy.cell_phy_index = iphy + 1;
            
            break;
        }
    }
    
    show_phy_state_table_on_screen();
    
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
        
        /*  Send PHY configure request success, then start the timer to 
         *  handle PHY response timeout. 
         */
        vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                .cell_timer.timer_cell_setup_phy_cfg = 
                (struct sbs_timer_list *)malloc(sizeof(struct sbs_timer_list));
        vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                .cell_timer.timer_cell_setup_phy_cfg->data = 
                    (void *)&msg_phy_cell_setup_req;
        vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                .cell_timer.timer_cell_setup_phy_cfg->cb = 
                    callback_handle_timer_cell_setup_phy_cfg_timeout;
                    
        /* Add the timer to timer list */
        sbs_timer_schedule(vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                .cell_timer.timer_cell_setup_phy_cfg, 
                    Timeout_Wait_Seconds, 
                    0);
    }
    else
    {
        error("%s:%d send phy cfg req ERROR, cell_virt_id = %d\n", 
                __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }

    info("%s:%d vc_entity_table[%d].state=WAIT_SETUP_PHY_CFG \n", 
          __func__, __LINE__, cell_virt_id-1);
    if(vc_entity_table[cell_virt_id - 1].cell_state == WAIT_SETUP_CPRI_CFG)
    {
        vc_entity_table[cell_virt_id-1].cell_state = WAIT_SETUP_PHY_CFG;
    }
    else
    {
        return FUNC_ERR;
    }

    log_msg(LOG_INFO, CELL_MGMT, 
            "\tVCM is Setup vbs %d Now.\n", cell_virt_id);
    log_msg(LOG_INFO, CELL_MGMT, 
            "\tvcm_handle_cell_setup_cpri_cfg_succ_rsp  success!\n");

    return FUNC_OK;
}

/******************************************************************************
 * Description: handle cpri configure failed response msg 
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 
vcm_handle_cell_setup_cpri_cfg_fail_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    /* Check for null pointer.*/
    if (NULL == p_msg_info) 
    {
        error("%s:%d p_msg_info is NULL\n", __func__, __LINE__);
        log_msg(LOG_ERR, CELL_MGMT, "the pointer of p_msg_info is"\
                     " NULL in %s.\n", __func__);
        return FUNC_ERR;
    }

    /* Check for cell_virt_id.*/
    if ((cell_virt_id < MIN_CELL_NUM) || (cell_virt_id > MAX_CELL_NUM)) 
    {
        error("%s:%d cell_virt_id error\n", __func__, __LINE__);
        log_msg(LOG_ERR, CELL_MGMT, "the cell_virt_id is exceed in %s.\n",__func__);
        return FUNC_ERR;
    }
    
    /*the pointer of message between vcm and rrum.*/
    MsgRrumCpriAxcCfgRsp *msg_rrum_cpri_axc_cfg_rsp = NULL;     
    msg_rrum_cpri_axc_cfg_rsp = (MsgRrumCpriAxcCfgRsp *)p_msg_info;

    //TODO
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle phy configure response message
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_setup_phy_cfg_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    if(p_msg_info == NULL)
    {
        error("%s:%d p_msg_info is NULL\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    // check cell state
    if (vc_entity_table[cell_virt_id - 1].cell_state != WAIT_SETUP_PHY_CFG)
    {
        error("%s:%d cell state is not expected WAIT_SETUP_PHY_CFG\n", 
                __func__, __LINE__);
    	return FUNC_ERR;
    }
    
    MsgPhyCellSetupRsp *msg_phy_cell_setup_rsp = (MsgPhyCellSetupRsp *)p_msg_info;
    
    if(msg_phy_cell_setup_rsp->SetupRslt == 0)
    {
        success("phy cfg success.\n");
        info("now handle phy cfg success info.\n");
        vcm_handle_cell_setup_phy_cfg_succ_rsp(cell_virt_id, p_msg_info);
    }
    else if(msg_phy_cell_setup_rsp->SetupRslt == -1)
    {
        error("phy cfg failed.\n");
        vcm_handle_cell_setup_phy_cfg_fail_rsp(cell_virt_id, p_msg_info);
    }
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle phy configure success response message
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_setup_phy_cfg_succ_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    if(p_msg_info == NULL)
    {
        error("%s:%d p_msg_info is NULL\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    MsgPhyCellSetupRsp *msg_phy_cell_setup_rsp = (MsgPhyCellSetupRsp *)p_msg_info;
    
    if(FUNC_OK == set_mac_init(cell_virt_id))
    {
        success("%s:%d set_mac_init successful.\n", __func__, __LINE__);
        
        /*  If we execute the MAC(./mac -P txt ... .etc), wait receive MAC init 
         *  response. So start the MAC initialized timer to handle MAC initial 
         *  timeout. 
         */
        vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                .cell_timer.timer_cell_setup_mac_init = 
                (struct sbs_timer_list *)malloc(sizeof(struct sbs_timer_list));
        vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                .cell_timer.timer_cell_setup_mac_init->data = 
                    (void *)msg_phy_cell_setup_rsp;
        vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                .cell_timer.timer_cell_setup_mac_init->cb = 
                    callback_handle_timer_cell_setup_mac_init_timeout;
                    
        /* Add the timer to timer list */
        sbs_timer_schedule(vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                .cell_timer.timer_cell_setup_mac_init, 
                    Timeout_Wait_Seconds, 
                    0);
    }
    else
    {
        error("%s:%d set_mac_init error.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    info("%s:%d change vc_entity info\n", __func__, __LINE__);
    info("%s:%d vc_entity_table[%d].state=WAIT_SETUP_MAC_INIT \n", 
          __func__, __LINE__, cell_virt_id-1);

    if(vc_entity_table[cell_virt_id - 1].cell_state == WAIT_SETUP_PHY_CFG)
    {
        vc_entity_table[cell_virt_id-1].cell_state = WAIT_SETUP_MAC_INIT;
    }
    else
    {
        return FUNC_ERR;
    }
    success("set cell entiry state to WAIT_SETUP_MAC_INIT.\n");
    success("%s:%d change vc_entity info successful\n", __func__, __LINE__);

    return FUNC_OK;
}

/******************************************************************************
 * Description: handle phy configure failed response message
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_setup_phy_cfg_fail_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    if(p_msg_info == NULL)
    {
        error("%s:%d p_msg_info is NULL\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    MsgPhyCellSetupRsp *msg_phy_cell_setup_rsp = (MsgPhyCellSetupRsp *)p_msg_info;
    
    // zero out phy info in vc entity
    vc_entity_table[cell_virt_id-1].cell_phy.cell_phy_index = 0;
    memset(vc_entity_table[cell_virt_id-1].cell_phy.FpgaMac, 0,
    		sizeof(vc_entity_table[cell_virt_id-1].cell_phy.FpgaMac));
    
    info("%s:%d cfg phy again...\n", __func__, __LINE__);

    return FUNC_OK;
}

/******************************************************************************
 * Description: handle mac initialized response message
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_setup_mac_init_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    if(p_msg_info == NULL)
    {
        error("%s:%d p_msg_info is NULL\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    // check cell state
    if(vc_entity_table[cell_virt_id - 1].cell_state != WAIT_SETUP_MAC_INIT)
    {
        error("%s:%d cell state is not expected WAIT_SETUP_MAC_INIT\n", 
                __func__, __LINE__);
    	return FUNC_ERR;
    }
    
    MsgMacInitRsp *msg_mac_init_rsp = (MsgMacInitRsp *)p_msg_info;
    
    if(msg_mac_init_rsp->rslt == 0)
    {
        success("%s:%d recv mac init success msg\n", __func__, __LINE__);

        vc_entity_table[cell_virt_id - 1].cell_mac.mac_pid  = msg_mac_init_rsp->pid;

        vcm_handle_cell_setup_mac_init_succ_rsp(cell_virt_id, p_msg_info);
    }
    else if(msg_mac_init_rsp->rslt == -1)
    {
        warning("%s:%d recv mac init falied msg\n", __func__, __LINE__);
        vcm_handle_cell_setup_mac_init_fail_rsp(cell_virt_id, p_msg_info);
    }
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle mac initialized success response message
 *
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_setup_mac_init_succ_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    if(p_msg_info == NULL)
    {
        error("%s:%d p_msg_info is NULL\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    MsgMacInitRsp *msg_mac_init_rsp = (MsgMacInitRsp *)p_msg_info;
    
    if(vc_entity_table[cell_virt_id - 1].cell_state == WAIT_SETUP_MAC_INIT)
    {
        info("%s:%d change vc_entity info\n", __func__, __LINE__);
        info("%s:%d vc_entity_table[%d].state=WAIT_SETUP_CELL_START \n", 
            __func__, __LINE__, cell_virt_id-1);

        // fill in mac info in vc_entity_table
    	//vc_entity_table[cell_virt_id-1].cell_mac.mac_pid 	= msg_mac_init_rsp->pid;
    	vc_entity_table[cell_virt_id-1].cell_mac.mac_status = MAC_OK;

        MsgMacCellSetupReq msg_mac_cell_setup_req = {
            .msghead.msg_type   = MODULE_MAC,
            .msghead.msg_code   = VCM_MAC_CELL_START_REQ,
            .msghead.msg_len    = sizeof(MsgMacCellSetupReq),
            .cell_virt_id       = cell_virt_id,
            //more...
        };
        
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
            
            /*  Send Cell Setup Request to MAC, and start a timer that if 
             *  receive cell setup response timeout, do something with callback
             *  function(timer->cb function).
             */
            vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                .cell_timer.timer_cell_setup_cell_start = 
                (struct sbs_timer_list *)malloc(sizeof(struct sbs_timer_list));
            vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                    .cell_timer.timer_cell_setup_cell_start->data = 
                        (void *)&msg_mac_cell_setup_req;
            vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                    .cell_timer.timer_cell_setup_cell_start->cb = 
                        callback_handle_timer_cell_setup_cell_start_timeout;
                        
            /* Add the timer to timer list */
            sbs_timer_schedule(vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                    .cell_timer.timer_cell_setup_cell_start, 
                        Timeout_Wait_Seconds, 
                        0);
        }
        else
        {
            error("%s:%d send mac cell setup req ERROR, cell_virt_id = %d\n", 
                    __func__, __LINE__, cell_virt_id);
            return FUNC_ERR;
        }
        
        info("%s:%d send cell start requ to mac\n", __func__, __LINE__);

        vc_entity_table[cell_virt_id-1].cell_state = WAIT_SETUP_CELL_START;
    }
    else
    {
        return FUNC_ERR;
    }
    
    success("set cell entity state to WAIT_SETUP_CELL_START\n");
    success("%s:%d change vc_entity info successful\n", 
                __func__, __LINE__);
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle mac initialized failed response message
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_setup_mac_init_fail_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    if(p_msg_info == NULL)
    {
        error("%s:%d p_msg_info is NULL\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    MsgMacInitRsp *msg_mac_init_rsp = (MsgMacInitRsp *)p_msg_info;
    
    info("%s:%d send cell start msg to mac again...\n", __func__, __LINE__);
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: send CONFIG_VCMAddState_t (cell setup response) to CONFIG module,
 * change cell entity state into CELL_NORMAL.
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 * Author: Liu Yixin
 ******************************************************************************/
INT32 vcm_handle_cell_setup_cell_start_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    if(p_msg_info == NULL)
    {
        error("%s:%d p_msg_info is NULL\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    // check cell state
    if (vc_entity_table[cell_virt_id-1].cell_state != WAIT_SETUP_CELL_START)
    {
        error("%s:%d cell state is not expected WAIT_SETUP_CELL_START\n", 
                __func__, __LINE__);
    	return FUNC_ERR;
    }
    // TODO close timer of cell start rsp

    int res;
    MsgMacCellSetupRsp *cell_start_rsp = (MsgMacCellSetupRsp *)p_msg_info;
    
    info("%s:%d mac cell setup rsp: cell_virt_id:%d, rslt:%d\n", 
          __func__, __LINE__, cell_virt_id, cell_start_rsp->rslt);

    if (cell_start_rsp->rslt == 0)
    {
        res = vcm_handle_cell_setup_cell_start_succ_rsp(cell_virt_id, p_msg_info);
    }
    else
    {
        res = vcm_handle_cell_setup_cell_start_fail_rsp(cell_virt_id, p_msg_info);
    }

    return res;
}

/******************************************************************************
 * Description: invoked when MAC sent a cell boot succ rsp
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 * Author: Liu Yixin
 ******************************************************************************/
INT32 vcm_handle_cell_setup_cell_start_succ_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    if(p_msg_info == NULL)
    {
        error("%s:%d p_msg_info is NULL\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    /* send CONFIG_VCMAddState_t (cell setup response) to CONFIG module */
    CONFIG_VCMAddState_t setup_rsp;
            
    setup_rsp.stSysMsg.msg_type    = MODULE_CELL_MGMT; 
    setup_rsp.stSysMsg.msg_code    = CFIG_MGMT_MSG_VC_SETUP_RSP;
    setup_rsp.stSysMsg.msg_len     = sizeof(CONFIG_VCMAddState_t);
    setup_rsp.ulIndex              = vc_entity_table[cell_virt_id - 1].cell_snmp_index;
    setup_rsp.ucRes                = 0;
    setup_rsp.ulError_type         = 0;
    
    info("%s:%d send CONFIG_VCMAddState_t to CFIG_MGMT \n", __func__, __LINE__);
    
    //TODO comment this when CONFIG module has been initialized
    if(0 == send_msg_to_om(MODULE_CFIG_MGMT, 
                           &setup_rsp, 
                           sizeof(CONFIG_VCMAddState_t)))
    {
        success("%s:%d send cell setup succ rsp to CFIG Success!\n", 
                __func__, __LINE__);
    }
    else
    {
        error("%s:%d send cell setup succ rsp to CFIG Error!\n", 
                __func__, __LINE__);
        return FUNC_ERR;
    }
    
    info("%s:%d vc_entity_table[%d].state=CELL_NORMAL \n", 
          __func__, __LINE__, cell_virt_id-1);
    /* change cell entity state into CELL_NORMAL */
    vc_entity_table[cell_virt_id - 1].cell_state = CELL_NORMAL;
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: invoked when MAC sent a cell boot failed rsp
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 * Author: Liu Yixin
 ******************************************************************************/
INT32 vcm_handle_cell_setup_cell_start_fail_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    if(p_msg_info == NULL)
    {
        error("%s:%d p_msg_info is NULL\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    // TODO CHECK ERR CODE OF RSP
    /* cell start retry if the time of retry does not reach 3 */
    /* failed when this is the thirdth invocation */
    // TODO /* hui gun not considered */

    /* send cell_setup_fail rsp to CONFIG module */
    
    
    return FUNC_OK;
}

