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
** File name: vcm_cell_setup.c
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
INT32 vcm_handle_cell_setup(VOID *p_msg_info)
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
INT32 vcm_sm_handle_cell_setup_req(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    /* set the VCM_SET */
    VCM_SET(id, &all_cell_set);
    VCM_SET(id, &virt_cell_set);
    

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_setup_cpri_cfg_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_cell_setup_cpri_cfg_succ_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();


    /**
     *  Get the Real phy_index: example:(0-7)
     */
    log_vcm_info("Get phy_index.\n");
    UINT8 iphy;
    for (iphy = REAL_PHY_INDEX_START; iphy <= REAL_PHY_INDEX_END; iphy ++)
    {
        if(1 != VCM_ISSET(iphy, &real_phy_index_set))
        {
            VCM_SET(iphy, &real_phy_index_set);
            VCM_SET(iphy, &all_phy_index_set);
            
            set_phy_index(id, iphy);

            Fprintf(STDOUT, "Real phy index bits: \n");
            vcm_set_display(STDOUT, all_phy_index_set, 0, PHY_INDEX_NUM, '1');
            Fprintf(STDOUT, "\n");
            vcm_set_display(STDOUT, real_phy_index_set, 0, PHY_INDEX_NUM, '1');
            Fprintf(STDOUT, "\n");
            
            break;
        }
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
INT32 vcm_sm_handle_cell_setup_cpri_cfg_fail_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_cell_setup_phy_cfg_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_cell_setup_phy_cfg_succ_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_cell_setup_phy_cfg_fail_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_cell_setup_mac_init_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_cell_setup_mac_init_succ_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_cell_setup_mac_init_fail_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_cell_setup_cell_start_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_cell_setup_cell_start_succ_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    /* VCM_SET is normal set bits */
    VCM_SET(id, &normal_cell_set);

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_cell_setup_cell_start_fail_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_simu_cell_setup(void *p_msg_info)
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
INT32 vcm_sm_handle_simu_cell_setup_req(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    /* set the VCM_SET */
    VCM_SET(id, &all_cell_set);
    VCM_SET(id, &simu_cell_set);

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_simu_cell_setup_mac_init_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_simu_cell_setup_mac_init_succ_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_simu_cell_setup_mac_init_fail_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_simu_cell_setup_simu_phy_init_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_simu_cell_setup_simu_phy_init_succ_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_simu_cell_setup_simu_phy_init_fail_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_simu_cell_setup_cell_start_rsp(UINT32 id, VOID *p_msg_info)
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
INT32 vcm_sm_handle_simu_cell_setup_cell_start_succ_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    /* VCM_SET is normal set bits */
    VCM_SET(id, &normal_cell_set);

    return VCM_OK;
}


/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/
INT32 vcm_sm_handle_simu_cell_setup_cell_start_fail_rsp(UINT32 id, VOID *p_msg_info)
{
    log_vcm_debug();

    return VCM_OK;
}


