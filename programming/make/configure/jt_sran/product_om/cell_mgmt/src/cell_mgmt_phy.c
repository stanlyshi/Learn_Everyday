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
** File name: cell_mgmt_phy.c
** Description: some of interface for PHY process
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
#include <sys/socket.h>

#include "sbs_type.h"

#include "util_dir.h"
#include "util_printer.h"
#include "util_exec.h"

#include "cell_mgmt_api.h"
#include "cell_mgmt_cell_entity.h"
#include "cell_mgmt_comm_func.h"
#include "cell_mgmt_define.h"
#include "cell_mgmt_msg.h"

#include "om_common_h.h"

/* Constants -----------------------------------------------------------------*/
/* Types ---------------------------------------------------------------------*/
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/
/******************************************************************************
 * Description: handle phy state change
 *
 * Input: msg: MsgPhyState
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT8 handle_phy_state_change(void *msg)
{
    debug();

    if (msg == NULL)
    {
        error("%s:%d NULL pointer.\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    MsgPhyState *phy_state_msg = (MsgPhyState *)msg;

    // 0921 modify: throw msg not complete error
    if (phy_state_msg->msghead.msg_len != sizeof(MsgPhyState))
    {
        error("%s:%d msg not complete, msg_type = %d, msg_code = %d\n",
                __func__, __LINE__, 
                phy_state_msg->msghead.msg_type, 
                phy_state_msg->msghead.msg_code);
        return FUNC_ERR;
    }

    switch(phy_state_msg->new_phy_state)
    {
        case PHY_OK:

            if (g_ne_sync_state != NE_SYNC_OK)
            {
                // TODO open sync waiting timer
                return FUNC_OK;
            }

            if (g_phy_state != PHY_OK)
            {
                error("%s:%d Conflict! phy state is not as expected!\n", 
                        __func__, __LINE__);
                return FUNC_ERR;
            }

            // scan vc_entity_table and trigger cell setup or cell delete process, etc.
            int ivc;

            for (ivc = 0; ivc < MAX_CELL_NUM; ivc ++)
            {
                if (vc_entity_table[ivc].cell_rru.rru_status == RRU_ADDED)
                {
                    switch(vc_entity_table[ivc].cell_state)
                    {
                        case CELL_NULL:
                            if (g_ne_sync_state != NE_SYNC_OK || g_phy_state != PHY_OK)
                            {
                                error("%s:%d unexpected phy or sys timer state\n", 
                                        __func__, __LINE__);
                                return FUNC_ERR;
                            }

                            MsgRrumCpriAxcCfgReq cpri_cfg_msg;
                                    
                            cpri_cfg_msg.msghead.msg_type  = MODULE_CELL_MGMT;
                            cpri_cfg_msg.msghead.msg_code  = RRUS_MGMT_MSG_CPRI_AxC_CFG_REQ;
                            cpri_cfg_msg.msghead.msg_len   = sizeof(MsgRrumCpriAxcCfgReq);
                            
                            cpri_cfg_msg.rru_id    = vc_entity_table[ivc].cell_rru.rru_id;
                            cpri_cfg_msg.axc_speed = vc_entity_table[ivc].cell_bandwidth;
                            cpri_cfg_msg.cfg_ind   = 1; // request for axcind assignment

                            send_msg_to_om(MODULE_RRUS_MGMT, (UINT8 *)&cpri_cfg_msg, 
                                           sizeof(MsgRrumCpriAxcCfgReq));

                            vc_entity_table[ivc].cell_state = WAIT_SETUP_CPRI_CFG;

                            break;

                        default:

                            break;

                    }
                }
            }

            break;

        case PHY_ERR:
            // TODO
            break;

        default:
            error("%s:%d unexpected msg code\n", __func__, __LINE__);
            return FUNC_ERR;
    }

    return FUNC_OK;
}

/******************************************************************************
 * Description: PHY unsocket callback function for inet_mgmt module
 *
 * Input:   sock_fd     : sockfd accept() generate
 *          clientaddr  : 
 *          msg_p       :
 *          msg_len     :
 *
 * Output: void
 ******************************************************************************/
void callback_phy_inet(INT32 sock_fd, struct sockaddr *clientaddr, char *msg_p, UINT32 msg_len)
{
    debug();
    
    if(msg_p == NULL || msg_len <= 0)
    {
        error("%s:%d msg is NULL pointer. msg_len = %d\n", 
                __func__, __LINE__, msg_len);
        return;
    }
    if(clientaddr == NULL)
    {
        warning("%s:%d clientaddr is NULL pointer\n", __func__, __LINE__);
    }
    
    UINT32 MODULE_ID;
    
    MsgPhyHead msg_phy_head;
    memcpy(&msg_phy_head, msg_p, sizeof(MsgPhyHead));

    UINT32 msg_type = msg_phy_head.msghead.msg_type;
    
    switch(msg_type)
    {
        case MSG_TYPE_PHY_CELL_MGMT:
            MODULE_ID = MODULE_CELL_MGMT;
            UINT32 cell_virt_id = msg_phy_head.cell_virt_id;
            vc_entity_table[cell_virt_id - 1].cell_phy.unsockfd = sock_fd;
            info("%s:%d Got PHY cell setup/del rsp.\n", __func__, __LINE__);
            break;
            
        /* case MSG_TYPE_PHY_ALAR_MGMT:
            MODULE_ID = MODULE_ALAR_MGMT;
            break; */
            
        case MSG_TYPE_PHY_MAIN_CTRL:
            MODULE_ID = MODULE_MAIN_CTRL;
            info("%s:%d Got PHY load rsp.\n", __func__, __LINE__);
            break;
            
        default:
        	warning("%s:%d Got unknown msg from PHY.\n", __func__, __LINE__);
            break;
    }
    
    if (send_msg_to_om(MODULE_ID, (char*)msg_p, msg_len) == FUNC_ERR)
    {
        error("%s:%d send msg to MSGQ error!\n", __func__, __LINE__);
    }
    else
    {
        success("%s:%d send msg to MSGQ success!\n", __func__, __LINE__);
    }
    
    return;
}
