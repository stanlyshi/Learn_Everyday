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
** File name: vcm_cell_delete.c
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
INT32 vcm_sm_handle_cell_del_req(UINT32 id,VOID *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_del_cell_close_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_del_cell_close_succ_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    /* set the VCM_SET */
    VCM_CLR(id, &normal_cell_set);

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_del_cell_close_fail_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_del_mac_del_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_del_mac_del_succ_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_del_mac_del_fail_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_del_phy_del_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_del_phy_del_succ_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    /**
     *  Set the Real phy_index to 0: example:(0-7)
     */
    log_vcm_info("Set phy_index to zero.\n");

    if(!VCM_ISSET(get_phy_index(id), &all_phy_index_set))
        VCM_CLR(get_phy_index(id), &all_phy_index_set);
    if(!VCM_ISSET(get_phy_index(id), &real_phy_index_set))
        VCM_CLR(get_phy_index(id), &real_phy_index_set);
    
    set_phy_index(id, 0);

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_del_phy_del_fail_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_del_cpri_reset_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_del_cpri_reset_succ_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    /* set the VCM_SET */
    VCM_CLR(id, &all_cell_set);
    VCM_CLR(id, &virt_cell_set);

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_del_cpri_reset_fail_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_simu_cell_del_req(UINT32 id, void *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_simu_cell_del_cell_close_rsp(UINT32 id, void *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_simu_cell_del_cell_close_succ_rsp(UINT32 id, void *p_msg_info)
{
    log_vcm_debug();

    /* set the VCM_SET */
    VCM_CLR(id, &normal_cell_set);

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_simu_cell_del_cell_close_fail_rsp(UINT32 id, void *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_simu_cell_del_simu_phy_del_rsp(UINT32 id, void *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_simu_cell_del_mac_del_rsp(UINT32 id, void *p_msg_info)
{
    log_vcm_debug();

    /* set the VCM_SET */
    VCM_CLR(id, &all_cell_set);
    VCM_CLR(id, &simu_cell_set);

    

    return VCM_OK;
}



