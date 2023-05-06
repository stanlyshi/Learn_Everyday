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
** File name: cell_mgmt_rrum.c
** Description: deal with rru register (add and delete)
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
#include <unistd.h>

#include "sbs_type.h"
#include "sbs_log.h"

#include "util_printer.h"

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
 * Description: handle rru register
 *
 * Input: msg_p: MsgRrumRruRegisterReq
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT8 handle_rru_reg(void *msg_p)
{
    debug();

    if (msg_p == NULL)
    {
        error("%s:%d NULL pointer.\n", __func__, __LINE__);
    	return FUNC_ERR;
    }
    
    MsgRrumRruRegisterReq *msg = (MsgRrumRruRegisterReq *)msg_p;
    // throw msg not complete error
    if (msg->msghead.msg_len != sizeof(MsgRrumRruRegisterReq))
    {
        error("%s:%d msg not complete, msg_type = %d, msg_code = %d\n",
            __func__, __LINE__, msg->msghead.msg_type, msg->msghead.msg_code);
        return FUNC_ERR;
    }
    
    int ret;

    /* RruReg rru_reg = RRU_ADD, RRU_DEL */
    switch(msg->rru_reg)
    {
        case RRU_ADD:
            info("%s:%d RRU register: RRU_ADD\n", __func__, __LINE__);
            ret = handle_rru_reg_add(msg);
            
            break;

        case RRU_DEL:
            info("%s:%d RRU register: RRU_DEL\n", __func__, __LINE__);
            ret = handle_rru_reg_del(msg);

            break;

        default:
            error("%s:%d unexpected msg code\n", __func__, __LINE__);
        	return FUNC_ERR;
    }
    return ret;
}

/******************************************************************************
 * Description: handle rru register - add
 *
 * Input: msg_p: MsgRrumRruRegisterReq
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT8 handle_rru_reg_add(MsgRrumRruRegisterReq *msg)
{

	int irru;
	int icell;
	int added = 0;
	int ret;

	for (irru = 0; irru < MAX_RRU_NUM; irru ++)
	{    // if this rru is already an entry in rru_info_table
		if (rru_info_table[irru].rru_status != RRU_STATUS_MIN)
		{
			info("%s:%d irru=%d, msg->rru_sn == %s\n",
					__func__, __LINE__, irru, msg->rru_sn);
			// make sure the length of compare is equal (= larger size)
			// it must has rru_sn, compare rru_sn is sufficient
			if (memcmp(rru_info_table[irru].rru_sn, msg->rru_sn,
				strlen(msg->rru_sn)>strlen(rru_info_table[irru].rru_sn)?
				strlen(msg->rru_sn):strlen(rru_info_table[irru].rru_sn)) == 0)
			{
				warning("%s:%d same rru_sn: %s vs %s\n",
						__func__, __LINE__,
						msg->rru_sn, rru_info_table[irru].rru_sn);

				// update its state into RRU_ADDED
				rru_info_table[irru].rru_status = RRU_ADDED;
				// fill rru_id if necessary in case it does not have it
				if (rru_info_table[irru].rru_id == 0)
				{
					rru_info_table[irru].rru_id = msg->rru_id;
				}
				added = 1;
				break;
			}
		}
	}
	/*added == 1*/
	if (added != 1)
	{   // if this rru has not had an entry in rru_info_table,
		// then add an entry in the first entry with no rru status
		for (irru = 0; irru < MAX_RRU_NUM; irru ++)
		{
			if (rru_info_table[irru].rru_status == RRU_STATUS_MIN)
			{
				success("%s:%d add rru: rru_info_table[%d], %s\n",
						__func__, __LINE__, irru, msg->rru_sn);
				rru_info_table[irru].rru_id = msg->rru_id;

				memcpy(rru_info_table[irru].rru_sn, msg->rru_sn,
					   strlen(msg->rru_sn));

				rru_info_table[irru].rru_status = RRU_ADDED;

				added = 1;

				break;
			}
		}
	}

	if (added != 1)
	{
		error("%s:%d there is no place for new RRU: %s\n",
				__func__, __LINE__, msg->rru_sn);
		// if there is no place for new RRU
		return FUNC_ERR;
	}

	// trigger cell setup process
	for (icell = 0; icell < MAX_CELL_NUM; icell ++)
	{
		if (memcmp(vc_entity_table[icell].cell_rru.rru_sn, msg->rru_sn,
					strlen(msg->rru_sn)>strlen(vc_entity_table[icell].cell_rru.rru_sn)?
					strlen(msg->rru_sn):strlen(vc_entity_table[icell].cell_rru.rru_sn)
					) == 0)
		{
			info("%s:%d vc_entity_table[%d].cell_rru.rru_sn = %s vs %s\n",
				  __func__, __LINE__,
				  icell, vc_entity_table[icell].cell_rru.rru_sn, msg->rru_sn);

			if (vc_entity_table[icell].cell_state == CELL_NULL)
			{
				//TODO close rru wait timer
				vc_entity_table[icell].cell_rru.rru_status  = RRU_ADDED;
				vc_entity_table[icell].cell_rru.rru_id      = msg->rru_id;

				success("%s:%d RRU regeister successful!\n");
				success("%s:%d vc_entity_table[%d].cell_rru.rru_id = %d\n",
						__func__, __LINE__, icell, msg->rru_id);
				success("%s:%d vc_entity_table[%d].cell_rru.rru_sn = %s\n",
						__func__, __LINE__, icell, msg->rru_sn);

				//TODO if timer is synchronized

				info("%s:%d generate MsgRrumCpriAxcCfgReq\n",
					  __func__, __LINE__);
				// set_cpri_axc(icell);
				// invoke send_msg_to_om_self_test to config cpri
				MsgRrumCpriAxcCfgReq cpri_cfg_msg;

				cpri_cfg_msg.msghead.msg_type  = MODULE_RRUS_MGMT;
				cpri_cfg_msg.msghead.msg_code  = RRUS_MGMT_MSG_CPRI_AxC_CFG_REQ;
				cpri_cfg_msg.msghead.msg_len   = sizeof(MsgRrumCpriAxcCfgReq);

				cpri_cfg_msg.rru_id    = vc_entity_table[icell].cell_rru.rru_id;
				cpri_cfg_msg.axc_speed = vc_entity_table[icell].cell_bandwidth;
				cpri_cfg_msg.cfg_ind   = 1;

				info("%s:%d send MsgRrumCpriAxcCfgReq to RRUS_MGMT \n",
						__func__, __LINE__);

                if (g_ne_sync_state != NE_SYNC_OK)
                {
                    error("%s:%d ne sync state is not OK, "\
                          "cannot set cpri-axc\n", __func__, __LINE__);
                    return FUNC_ERR;
                }
                                                        
				if((ret = send_msg_to_om(MODULE_RRUS_MGMT, &cpri_cfg_msg,
						sizeof(MsgRrumCpriAxcCfgReq))) == FUNC_OK)
				{
					success("%s:%d send cpri cfg msg to RRUM Success\n", 
                            __func__, __LINE__);
				}
                else
                {
                    error("%s:%d send cpri cfg msg to RRUM Error\n", 
                            __func__, __LINE__);
                }

				info("%s:%d vc_entity_table[%d].state=WAIT_SETUP_CPRI_CFG \n",
					  __func__, __LINE__, icell);
				/* change cell state into WAIT_SETUP_CPRI_CFG */
				vc_entity_table[icell].cell_state = WAIT_SETUP_CPRI_CFG;

				break; // break loop
				// }
			}
		}
	}
	return FUNC_OK;
}

/******************************************************************************
 * Description: handle rru register - delete
 *
 * Input: msg_p: MsgRrumRruRegisterReq
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT8 handle_rru_reg_del(MsgRrumRruRegisterReq *msg)
{
    int irru;
    int icell;
    int deleted = 0;

	// 0921 add handle for RRU del msg
	// STEP 1: update rru_info_table
	for (irru = 0; irru < MAX_RRU_NUM; irru ++)
	{
		// rru id must has been stored in registered rru
		// compare rru_id is sufficient
		// if no match (deleted != 1), return FUNC_ERR OR ignore it and return FUNC_OK
		if (rru_info_table[irru].rru_id == msg->rru_id)
		{
			// throw sn error
			if (memcmp(rru_info_table[irru].rru_sn, msg->rru_sn,
				strlen(msg->rru_sn)>strlen(rru_info_table[irru].rru_sn)?
				strlen(msg->rru_sn):strlen(rru_info_table[irru].rru_sn)) != 0)
			{
                error("%s:%d unexpected rru sn = %s, failed delete rru_id = %d\n",
                        __func__, __LINE__, msg->rru_sn, msg->rru_id);

				return FUNC_ERR;
			}

			// throw state error
			if (rru_info_table[irru].rru_status == RRU_STATUS_MIN)
			{
                error("%s:%d unexpected rru state = %d, "\
                        "failed delete rru_id = %d\n",
                        __func__, __LINE__, 
                        rru_info_table[irru].rru_status, msg->rru_id);
				return FUNC_ERR;
			}

			// deal with duplicated request
			if (rru_info_table[irru].rru_status == RRU_DELETED)
			{
                info("%s:%d duplicated RRU delete request, rru_id = %d\n",
                        __func__, __LINE__, msg->rru_id);
				return FUNC_OK;

			}
			// change rru_status into RRU_DELETED
			rru_info_table[irru].rru_status = RRU_DELETED;
			deleted = 1;
			break;
		}
	}

	// deal with no match
	if (deleted != 1)
	{
		warning("%s:%d no matching rru: rru_id = %d, rru_sn = %s\n",
				__func__, __LINE__, msg->rru_id, msg->rru_sn);
		return FUNC_ERR;
	}


	// STEP 2: change rru info in vc_entity_table, trigger rru_err_del process
	for (icell = 0; icell < MAX_CELL_NUM; icell ++)
	{
		// compare rru_sn
		if (memcmp(vc_entity_table[icell].cell_rru.rru_sn, msg->rru_sn,
					strlen(msg->rru_sn)>strlen(vc_entity_table[icell].cell_rru.rru_sn)?
					strlen(msg->rru_sn):strlen(vc_entity_table[icell].cell_rru.rru_sn)
					) == 0)
		{
			switch (vc_entity_table[icell].cell_state)
			{
				case CELL_NULL:
					// change rru status
					vc_entity_table[icell].cell_rru.rru_status = RRU_DELETED;
					// todo open wait rru timer
					break;

				default:
					// invoke handle_rru_reg_err_del method
					break;
			}
			break; // break loop
		}
	}
	return FUNC_OK;
}