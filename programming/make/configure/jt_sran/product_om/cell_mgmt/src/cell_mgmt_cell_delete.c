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
** File name: cell_mgmt_cell_delete.c
** Description: cell delete process handle functions
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

#include "om_common_h.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/
/******************************************************************************
 * Description: handle cell delete request msg recv from cfig_mgmt module
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_del_req(UINT32 cell_virt_id,void *p_msg_info)
{
    debug();
    
    if (g_phy_state != PHY_OK || g_ne_sync_state != NE_SYNC_OK)
    {
        error("%s:%d PHY or SYNC state error\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    if (cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id out of range\n", __func__, __LINE__);
    	return FUNC_ERR;
    }

    if (p_msg_info == NULL)
    {
        error("%s:%d p_msg_info is null\n", __func__, __LINE__);
    	return FUNC_ERR;
    }
    
    // check cell state
    if(vc_entity_table[cell_virt_id-1].cell_state != CELL_NORMAL)
    {
        error("%s:%d cell state is not expected CELL_NORMAL\n", 
                __func__, __LINE__);
    	return FUNC_ERR;
    }
    /* send cell close request to MAC */
    /* change cell state into WAIT_DEL_CELL_CLOSE */
    int icell;
    MsgCfigCellDelReq *msg_cfig_cell_del_req = (MsgCfigCellDelReq *)p_msg_info;

    for (icell = 0; icell < MAX_CELL_NUM; icell ++)
    {
    	if (vc_entity_table[icell].cell_snmp_index 
                == msg_cfig_cell_del_req->cell_snmp_index)
    	{
            warning("find cell to delete: cell_virt_id[%d],cell_snmp_index[%d]\n",
                    icell+1, msg_cfig_cell_del_req->cell_snmp_index);
            
    		/* send cell close request to MAC */
    		MsgMacCellDelReq msg_mac_cell_del_req = {
                .msghead.msg_type   = MODULE_CELL_MGMT,
                .msghead.msg_code   = VCM_MAC_CELL_DELET_REQ,
                .msghead.msg_len    = sizeof(MsgMacCellDelReq),
                .cell_virt_id       = cell_virt_id,
            };
            
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
                
                /*  Send cell delete requset to MAC, start a timer for receive 
                 *  response message timeout.
                 */
                vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                        .cell_timer.timer_cell_del_cell_close = 
                        (struct sbs_timer_list *)malloc(sizeof(struct sbs_timer_list));
                vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                        .cell_timer.timer_cell_del_cell_close->data = 
                            (void *)&msg_mac_cell_del_req;
                vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                        .cell_timer.timer_cell_del_cell_close->cb = 
                            callback_handle_timer_cell_del_cell_close_timeout;
                
                /* Add the timer to timer list */
                sbs_timer_schedule(vc_entity_table[cell_virt_id-MIN_CELL_NUM]
                        .cell_timer.timer_cell_del_cell_close, 
                            Timeout_Wait_Seconds, 
                            0);
            }
            else
            {
                error("%s:%d send mac cell del req ERROR, cell_virt_id = %d\n", 
                        __func__, __LINE__, cell_virt_id);
                return FUNC_ERR;
            }

    		/* change cell state */
            vc_entity_table[icell].cell_state = WAIT_DEL_CELL_CLOSE;
            
    		break;
    	}
    }
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle cell delete response msg recv from MAC
 *  
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_del_cell_close_rsp(UINT32 cell_virt_id, void *p_msg_info)
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
    if (vc_entity_table[cell_virt_id-1].cell_state != WAIT_DEL_CELL_CLOSE)
    {
        error("%s:%d cell state of cell %d is not expected WAIT_DEL_CELL_CLOSE\n",
        		__func__, __LINE__,cell_virt_id);
        return FUNC_ERR;
    }
    
    MsgMacCellDelRsp *msg_mac_cell_del_rsp = (MsgMacCellDelRsp *)p_msg_info;
    
    if(msg_mac_cell_del_rsp->rslt == 0)
    {
        success("%s:%d mac cell del success rsp.\n", __func__, __LINE__);
        info("%s:%d now handle mac cell del success info.\n",
                __func__, __LINE__);
        vcm_handle_cell_del_cell_close_succ_rsp(cell_virt_id, p_msg_info);
    }
    else if(msg_mac_cell_del_rsp->rslt == -1)
    {
        error("%s:%d mac cell del  failed.\n", __func__, __LINE__);
        vcm_handle_cell_del_cell_close_fail_rsp(cell_virt_id, p_msg_info);
    }
    
    
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle cell delete response succ msg recv from MAC
 *  
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_del_cell_close_succ_rsp(UINT32 cell_virt_id, void *p_msg_info)
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
    
    MsgMacCellDelRsp *msg_mac_cell_del_rsp = (MsgMacCellDelRsp *)p_msg_info;

    if(FUNC_OK == set_mac_kill(cell_virt_id))
    {
        success("%s:%d set_mac_kill successful.\n", __func__, __LINE__);
    }
    else
    {
        error("%s:%d set_mac_kill failed.\n", __func__, __LINE__);
    }
    
    info("%s:%d change vc_entity info\n", __func__, __LINE__);

    // clear mac info in cell entity
    memset(&vc_entity_table[cell_virt_id-1].cell_mac, 0,
    		sizeof(vc_entity_table[cell_virt_id-1].cell_mac));
            
    if(vc_entity_table[cell_virt_id-1].cell_state == WAIT_DEL_CELL_CLOSE)
    {
        // TODO send PHY del req to PHY
    	// change state into wait del phy del
        
        vc_entity_table[cell_virt_id-1].cell_state = WAIT_DEL_MAC_DEL;
        success("%s:%d set cell entiry state to WAIT_DEL_MAC_DEL.\n", 
                __func__, __LINE__);
                
        MsgMacKillRsp msg_mac_kill_rsp = {
            .msghead.msg_type   = MSG_TYPE_MAC_CELL_MGMT,
            .msghead.msg_code   = MAC_KILL_RSP,
            .msghead.msg_len    = sizeof(MsgMacKillRsp),
            .cell_virt_id       = cell_virt_id,
        };
        
        if(0 == send_msg_to_om(MODULE_CELL_MGMT, 
                               &msg_mac_kill_rsp, 
                               sizeof(MsgMacKillRsp)))
        {
            success("%s:%d send mac kill rsp Success.\n", __func__, __LINE__);
        }
        else
        {
            error("%s:%d send mac kill rsp Error.\n", __func__, __LINE__);
            return FUNC_ERR;
        }
    }
    else
    {
        error("%s:%d vc_entiry[%d].cell_state Error.\n", 
                __func__, __LINE__, cell_virt_id-1);
        return FUNC_ERR;
    }
    
    success("%s:%d successful\n", __func__, __LINE__);

    return FUNC_OK;
}

/******************************************************************************
 * Description: handle cell delete response failed msg recv from MAC
 *  
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 
vcm_handle_cell_del_cell_close_fail_rsp(UINT32 cell_virt_id, void *p_msg_info)
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
    
    MsgMacCellDelRsp *msg_mac_cell_del_rsp = (MsgMacCellDelRsp *)p_msg_info;
    
    info("%s:%d now handle mac cell del failed info\n", __func__, __LINE__);
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle MAC killed response msg recv from
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_del_mac_del_rsp(UINT32 cell_virt_id, void *p_msg_info)
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
    
    info("%s:%d change vc_entity info\n", __func__, __LINE__);

    if(vc_entity_table[cell_virt_id-1].cell_state == WAIT_DEL_MAC_DEL)
    {
        vc_entity_table[cell_virt_id-1].cell_state = WAIT_DEL_PHY_DEL;
        
        MsgPhyCellDelReq msg_phy_cell_del_req = {
            .msghead.msg_type   = MSG_TYPE_PHY_CELL_MGMT,
            .msghead.msg_code   = VCM_PHY_CELL_DELETE_REQ,
            .msghead.msg_len    = sizeof(MsgPhyCellDelReq),
            .cell_virt_id       = cell_virt_id,
            .PhyCellIndex       = vc_entity_table[cell_virt_id-1].cell_phy.cell_phy_index,
        };
        
        if(0 == send_msg_to_inet(MODULE_CELL_MGMT, 
                                 SOCK_STREAM_VALID, 
                                 vc_entity_table[cell_virt_id-1].cell_phy.unsockfd, 
                                 IP_INVALID, 
                                 SOCKADDR_NULL, 
                                 &msg_phy_cell_del_req, 
                                 sizeof(MsgPhyCellDelReq)))
        {
            success("%s:%d send phy cell del req Success.\n", __func__, __LINE__);
        }
        else
        {
            error("%s:%d send phy cell del req Success.\n", __func__, __LINE__);
            return FUNC_ERR;
        }
    }
    else
    {
        error("%s:%d vc_entiry[%d].cell_state Error.\n", 
                __func__, __LINE__, cell_virt_id-1);
        return FUNC_ERR;
    }
    success("set cell entiry state to WAIT_DEL_PHY_DEL.\n");
    success("%s:%d change vc_entity info successful\n", __func__, __LINE__);
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: TODO
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 
vcm_handle_cell_del_mac_del_succ_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    //TODO
    return FUNC_OK;
}

/******************************************************************************
 * Description: TODO
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR 
 ******************************************************************************/
INT32 
vcm_handle_cell_del_mac_del_fail_rsp(UINT32 cell_virt_id, void *p_msg_info)
{
    debug();
    //TODO
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle PHY del response msg
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_del_phy_del_rsp(UINT32 cell_virt_id, void *p_msg_info)
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
		log_msg(LOG_ERR, CELL_MGMT, "the pointer of p_msg_info is"\
			     	" NULL in vcm_handle_cell_del_phy_del_rsp().\n");
		return FUNC_ERR;
	}

	/* Check for cell_virt_id.*/
	if ((cell_virt_id < MIN_CELL_NUM) || (cell_virt_id > MAX_CELL_NUM)) 
    {
		log_msg(LOG_ERR, CELL_MGMT, "the cell_virt_id is exceed in "
			             	"vcm_handle_cell_del_phy_del_rsp().\n");
		return FUNC_ERR;
	}

    // check cell state
    if (vc_entity_table[cell_virt_id-1].cell_state != WAIT_DEL_PHY_DEL)
    {
        error("%s:%d cell state of cell %d is not expected WAIT_DEL_PHY_DEL\n",
        		__func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
	MsgPhyCellDelRsp *msg_phy_cell_del_rsp = NULL;
	msg_phy_cell_del_rsp = (MsgPhyCellDelRsp *)p_msg_info;

	if(msg_phy_cell_del_rsp->DeleteRslt == 0)
    {
        info("%s:%d phy delete successful.\n", __func__, __LINE__);
		vcm_handle_cell_del_phy_del_succ_rsp(cell_virt_id, p_msg_info);

	}
    else
    {
        info("%s:%d phy delete failed.\n", __func__, __LINE__);
		vcm_handle_cell_del_phy_del_fail_rsp(cell_virt_id, p_msg_info);

	}
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle phy del response msg
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 
vcm_handle_cell_del_phy_del_succ_rsp(UINT32 cell_virt_id, void *p_msg_info)
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
		log_msg(LOG_ERR, CELL_MGMT, "the pointer of p_msg_info is"\
			     	" NULL in vcm_handle_cell_del_phy_del_succ_rsp().\n");
		return FUNC_ERR;
	}

	/* Check for cell_virt_id.*/
	if ((cell_virt_id < MIN_CELL_NUM) || (cell_virt_id > MAX_CELL_NUM)) 
    {
		log_msg(LOG_ERR, CELL_MGMT, "the cell_virt_id is exceed in "
			             	"vcm_handle_cell_del_phy_del_succ_rsp().\n");
		return FUNC_ERR;
	}

    // free phy index
	if (phy_state_table[vc_entity_table[cell_virt_id-1]
            .cell_phy.cell_phy_index-1] == 1)
	{
		phy_state_table[vc_entity_table[cell_virt_id-1]
            .cell_phy.cell_phy_index-1] = 0;
            
        show_phy_state_table_on_screen();
	}
	else 
    {
        show_phy_state_table_on_screen();
        
		error("%s:%d phy state is wrong at index %d, expected 1 but is 0\n",
				__func__, __LINE__,
				vc_entity_table[cell_virt_id-1].cell_phy.cell_phy_index-1);
		return FUNC_ERR;
	}

	// clear phy info in cell_entity_table
	memset(&vc_entity_table[cell_virt_id-1].cell_phy, 0,
			sizeof(vc_entity_table[cell_virt_id-1].cell_phy));
    
    
	MsgRrumCpriAxcCfgReq msg_rrum_cpri_cfg_req;
    
	msg_rrum_cpri_cfg_req.msghead.msg_type  = MODULE_CELL_MGMT;
	msg_rrum_cpri_cfg_req.msghead.msg_code  = RRUS_MGMT_MSG_CPRI_AxC_CFG_REQ;
	msg_rrum_cpri_cfg_req.msghead.msg_len   = sizeof(MsgRrumCpriAxcCfgReq);
    
	msg_rrum_cpri_cfg_req.rru_id       = 
            vc_entity_table[cell_virt_id - 1].cell_rru.rru_id;
	msg_rrum_cpri_cfg_req.axc_speed    = 
            vc_entity_table[cell_virt_id - 1].cell_bandwidth;
	msg_rrum_cpri_cfg_req.cfg_ind      = 0;		//reset
	// TODO start send cpri cfg timer
    
    //TODO comment this when RRUM has been initialized
	if(0 == send_msg_to_om(MODULE_RRUS_MGMT, (INT8 *)&msg_rrum_cpri_cfg_req, 
                           sizeof(MsgRrumCpriAxcCfgReq)))
    {
		success("%s:%d send rrum cpri cfg req Success!\n", __func__, __LINE__);
    }
    else
    {
		error("%s:%d send rrum cpri cfg req Error!\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    
    if(vc_entity_table[cell_virt_id-1].cell_state == WAIT_DEL_PHY_DEL)
    {
        vc_entity_table[cell_virt_id-1].cell_state = WAIT_DEL_CPRI_RESET;
    }
    else
    {
        error("%s:%d vc_entiry[%d].cell_state Error.\n", 
                __func__, __LINE__, cell_virt_id-1);
        return FUNC_ERR;
    }
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle phy del response msg
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_del_phy_del_fail_rsp(UINT32 cell_virt_id, void *p_msg_info)
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
		log_msg(LOG_ERR, CELL_MGMT, "the pointer of p_msg_info is"\
					" NULL in vcm_handle_cell_del_phy_del_succ_rsp().\n");
		return FUNC_ERR;
	}

	/* Check for cell_virt_id.*/
	if ((cell_virt_id < MIN_CELL_NUM) || (cell_virt_id > MAX_CELL_NUM)) 
    {
		log_msg(LOG_ERR, CELL_MGMT, "the cell_virt_id is exceed in "
							"vcm_handle_cell_del_phy_del_succ_rsp().\n");
		return FUNC_ERR;
	}

	MsgPhyCellDelRsp *msg_phy_cell_del_rsp = NULL;
	msg_phy_cell_del_rsp = (MsgPhyCellDelRsp *)p_msg_info;


	//TODO
    
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle cpri reset response msg 
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 vcm_handle_cell_del_cpri_reset_rsp(UINT32 cell_virt_id, void *p_msg_info)
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
    if (vc_entity_table[cell_virt_id-1].cell_state != WAIT_DEL_CPRI_RESET)
    {
        error("%s:%d cell state of cell %d is not expected WAIT_DEL_CPRI_RESET\n",
        		__func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    MsgRrumCpriAxcCfgRsp *msg_cpri_cfg_rsp = (MsgRrumCpriAxcCfgRsp *)p_msg_info;
    
    if(msg_cpri_cfg_rsp->cfg_rslt == 1)
    {
        info("%s:%d cpri reset successful.\n", __func__, __LINE__);
        vcm_handle_cell_del_cpri_reset_succ_rsp(cell_virt_id, p_msg_info);
    }
    else
    {
        info("%s:%d cpri reset failed.\n",__func__, __LINE__);
        vcm_handle_cell_del_cpri_reset_fail_rsp(cell_virt_id, p_msg_info);
    }
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle cpri reset response msg 
 *  
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 
vcm_handle_cell_del_cpri_reset_succ_rsp(UINT32 cell_virt_id, void *p_msg_info)
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
    
    MsgRrumCpriAxcCfgRsp *msg_cpri_cfg_rsp = (MsgRrumCpriAxcCfgRsp *)p_msg_info;
    
    info("%s:%d change vc_entity info\n", __func__, __LINE__);

    if(vc_entity_table[cell_virt_id-1].cell_state == WAIT_DEL_CPRI_RESET)
    {
        info("%s:%d change vc_entity state to VC_STATE_MIN_NUM\n", 
                __func__, __LINE__);
        
        CONFIG_VCMDelState_t del_rsp;
            
        del_rsp.stSysMsg.msg_type    = MODULE_CELL_MGMT; 
        del_rsp.stSysMsg.msg_code    = CFIG_MGMT_MSG_VC_DEL_RSP;
        del_rsp.stSysMsg.msg_len     = sizeof(CONFIG_VCMDelState_t);
        del_rsp.ulIndex              = vc_entity_table[cell_virt_id - 1].cell_snmp_index;
        del_rsp.ucRes                = 0;
        del_rsp.ulError_type         = 0;
        
        if(0 == send_msg_to_om(MODULE_CFIG_MGMT, (INT8 *)&del_rsp, 
                               sizeof(CONFIG_VCMDelState_t)))
        {
            success("%s:%d send CFIG cell del rsq Success!\n", __func__, __LINE__);
        }
        else
        {
            error("%s:%d send CFIG cell del rsq Error!\n", __func__, __LINE__);
            return FUNC_ERR;
        }
        
        success("%s:%d send cell del succ rsp.\n", __func__, __LINE__);
        vc_entity_table[cell_virt_id-1].cell_state = VC_STATE_MIN_NUM;
        memset(&vc_entity_table[cell_virt_id-1], 0, sizeof(VcEntityNode));
    }
    else
    {
        error("%s:%d vc_entiry[%d].cell_state Error.\n", 
                __func__, __LINE__, cell_virt_id-1);
        return FUNC_ERR;
    }
    
    success("set cell entiry memory to ZERO.\n");
    success("%s:%d change vc_entity info successful\n", __func__, __LINE__);
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: handle cpri reset response msg 
 *  
 *
 * Input:   cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *          p_msg_info: message
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 
vcm_handle_cell_del_cpri_reset_fail_rsp(UINT32 cell_virt_id, void *p_msg_info)
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
    
    MsgRrumCpriAxcCfgRsp *msg_cpri_cfg_rsp = (MsgRrumCpriAxcCfgRsp *)p_msg_info;
    
    info("%s:%d now handle cpri reset failed info\n", __func__, __LINE__);
    
    return FUNC_OK;
}
