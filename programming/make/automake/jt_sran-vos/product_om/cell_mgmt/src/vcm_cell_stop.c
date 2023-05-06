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
** File name: vcm_cell_stop.c
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
extern vcm_set all_cell_set, virt_cell_set, simu_cell_set, normal_cell_set;
extern vcm_set all_phy_index_set, real_phy_index_set, simu_phy_index_set; /* for phy index recording */

/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_state_stop_req(UINT32 id,VOID *p_msg_info)
{
    log_vcm_debug();

    /**
     *  Check for null pointer.
     */
    if (NULL == p_msg_info) 
    {
        log_vcm_err("the pointer of p_msg_info is NULL\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }

    vcm_sm_state_t   cell_state = get_cell_state(id);
    
    if(cell_state <= VC_STATE_MIN_NUM || cell_state >= VC_STATE_MAX_NUM)
    {
        log_vcm_err("cell state error, %d\n", id);
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }

    msghdr_t msghdr _unused;
    //TODO: deal with: 1.rru register; 2.board sync; 3.real phy statusc change
    

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_state_stop_recover(UINT32 id,VOID *p_msg_info)
{
    log_vcm_debug();

    /**
     *  Check for null pointer.
     */
    if (NULL == p_msg_info) 
    {
        log_vcm_err("the pointer of p_msg_info is NULL\n");
        __vcm_set_errno(_VCM_ENULLPOINTER);
        return VCM_ERR;
    }

    vcm_sm_state_t   cell_state = get_cell_state(id);
    
    if(cell_state <= VC_STATE_MIN_NUM || cell_state >= VC_STATE_MAX_NUM)
    {
        log_vcm_err("cell state error, %d\n", id);
        __vcm_set_errno(_VCM_EICID);
        return VCM_ERR;
    }


    

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_null_stopped_case(UINT32 id,VOID *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


