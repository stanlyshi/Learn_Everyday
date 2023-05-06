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
** File name: cell_mgmt_cell_entity.c
** Description: some cell entity struct parameters set & get interface
**
** Current Version: 
** $Revision$
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2018.10
**
******************************************************************************/

/* Dependencies --------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>

#include "sbs_type.h"
#include "sbs_log.h"

#include "util_systemer.h"
#include "util_biter.h"
#include "util_printer.h"

#include "cell_mgmt_api.h"
#include "cell_mgmt_cell_entity.h"
#include "cell_mgmt_comm_func.h"
#include "cell_mgmt_define.h"
#include "cell_mgmt_msg.h"
#include "cell_mgmt_sm.h"

#include "rrus_mgmt_rru_entity.h"

#include "om_common_h.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
char    *vc_mode_str[VC_MODE_MAX];       /*store the name of mode.*/


/* Functions -----------------------------------------------------------------*/

/******************************************************************************
 * Description: add to vc_mode_str
 *
 * Input:   vcmode: cell mode (int)
 *      vcmode_str: cell mode (string)
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 add_vc_mode_to_str(VcMode vcmode, char *vcmode_str)
{
    debug();
    if(vcmode < 0 || vcmode < VC_MODE_MIN || vcmode > VC_MODE_MAX)
    {
        error("%s:%d vcmode(%d) invalid\n", vcmode);
        return FUNC_ERR;
    }
    if(vcmode_str == NULL)
    {
        error("%s:%d NULL pointer.\n");
        return FUNC_ERR;
    }

    vc_mode_str[vcmode] = vcmode_str;       /*store the name of mode.*/
    return FUNC_OK;
}


/******************************************************************************
 * Description: show cell entity table on screen
 *
 * Input: cell_virt_id: cell array index+1 (vc_entity_table[cell_virt_id-1])
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 show_cell_entity_on_screen(const INT32 cell_virt_id)
{
    debug();

    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell id error, %d\n", __func__, __LINE__, cell_virt_id);
        return FUNC_ERR;
    }
    
    int i = cell_virt_id - 1;
    
    info("##################################################################\n");
    info("## \033[1;31mVirtual Cell Info.\033[m  %s\n", get_system_time());
    info("## \n");
    info("## [%d].array index             = %d\n", i, i);
    info("## [%d].cell_virt_id            = %d\n", i, vc_entity_table[i].cell_virt_id);
    info("## [%d].cell_snmp_index         = %d\n", i, vc_entity_table[i].cell_snmp_index);
    info("## [%d].cell_g_id               = %d\n", i, vc_entity_table[i].cell_g_id);
    info("## [%d].cell_is_uni             = %d (unique/union 0:yes 1:no)\n", i, vc_entity_table[i].cell_is_uni);
    info("## [%d].cell_power              = %d\n", i, vc_entity_table[i].cell_power);
    info("## [%d].cell_usernum            = %d\n", i, vc_entity_table[i].cell_usernum);
    info("## [%d].cell_bandwidth          = %d\n", i, vc_entity_table[i].cell_bandwidth);
    info("## [%d].cell_mode               = %s\n", i, vc_mode_str[vc_entity_table[i].cell_mode]);
    info("## [%d].cell_state              = %s\n", i, vc_state_str[vc_entity_table[i].cell_state]);
    info("## [%d].cell_rru.rru_id         = %d\n", i, vc_entity_table[i].cell_rru.rru_id);
    info("## [%d].cell_rru.rru_sn         = %s\n", i, vc_entity_table[i].cell_rru.rru_sn);
    info("## [%d].cell_rru.rru_status     = %d\n", i, vc_entity_table[i].cell_rru.rru_status);
    info("## [%d].cell_rru.rru_ip         = %s\n", i, vc_entity_table[i].cell_rru.rru_ip);
    info("## [%d].cell_rru.rru_port       = %d\n", i, vc_entity_table[i].cell_rru.rru_port);
    info("## [%d].cell_rru.axc_speed      = %d\n", i, vc_entity_table[i].cell_rru.axc_speed);
    info("## [%d].cell_rru.axc_index      = %d\n", i, vc_entity_table[i].cell_rru.axc_index);
    info("## [%d].cell_mac.cell_snmp_index= %d\n", i, vc_entity_table[i].cell_mac.cell_snmp_index);
    info("## [%d].cell_mac.mac_pid        = %d\n", i, vc_entity_table[i].cell_mac.mac_pid);
    info("## [%d].cell_mac.mac_status     = %d\n", i, vc_entity_table[i].cell_mac.mac_status);
    info("## [%d].cell_mac.mac_cfg_pth    = %s\n", i, vc_entity_table[i].cell_mac.mac_cfg_pth);
    info("## [%d].cell_mac.enb_ipaddr     = %s\n", i, vc_entity_table[i].cell_mac.enb_ipaddr);
    info("## [%d].cell_mac.gtpu_recv_addr = %s\n", i, vc_entity_table[i].cell_mac.gtpu_recv_addr);
    info("## [%d].cell_mac.vbs_eth        = %s\n", i, vc_entity_table[i].cell_mac.vbs_eth);
    info("## [%d].cell_phy.cell_phy_index = %d\n", i, vc_entity_table[i].cell_phy.cell_phy_index);
    info("## [%d].cell_phy.FpgaMac        = %s\n", i, vc_entity_table[i].cell_phy.FpgaMac);
    info("## \n");
    info("##################################################################\n");
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: show rru info table on screen
 *
 * Input: void
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 show_rru_info_table_on_screen()
{
    debug();

    info("################################################################\n");
    info("# \033[1;31mRRU Register Status Table.\033[m %s\n", get_system_time());
    info("# \n");
    info("#  id: status: sn: ip: port: axc_speed: axc_index\n");

    INT32 id;
    for(id=0; id<MAX_RRU_NUM; id++)
    {
        if(rru_info_table[id].rru_id != 0)
        {
            info("# %d: %d: %s: %s: %d: %d: %d \n",
                  rru_info_table[id].rru_id,
                  rru_info_table[id].rru_status,
                  rru_info_table[id].rru_sn,
                  rru_info_table[id].rru_ip,
                  rru_info_table[id].rru_port,
                  rru_info_table[id].axc_speed,
                  rru_info_table[id].axc_index);
        }
        if(id == MAX_RRU_NUM)
        {
    info("# \n");
    info("################################################################\n");
            return FUNC_ERR;
        }
    }
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: show phy state table on screen 
 *
 * Input: void
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 show_phy_state_table_on_screen()
{
    debug();

    UINT32 iphy;
    char phy_state[MAX_PHY_INDEX+1];
    for(iphy=0; iphy<MAX_PHY_INDEX; iphy++)
    {
        phy_state[iphy] = phy_state_table[iphy]>0?'1':'0';
    }
    phy_state[iphy] = '\0';

    info("################################################################\n");
    info("# \033[1;31mPHY Status Table. \033[m   %s\n", get_system_time());
    info("#                      %s\n", phy_state);
    info("################################################################\n");
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_cell_fd_by_cell_id(UINT32 cell_virt_id, INT32 cell_fd)
{
    debug();
    if(cell_virt_id<MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id error\n ", __func__, __LINE__);
        return FUNC_ERR;
    }
    set_mac_unsockfd_by_cell_virt_id(cell_virt_id, cell_fd);
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: unsockfd: mac's socket
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 get_cell_fd_by_cell_id(UINT32 cell_virt_id)
{
    debug();

    if(cell_virt_id<MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id error\n ", __func__, __LINE__);
        return FUNC_ERR;
    }
    return get_mac_unsockfd_by_cell_virt_id(cell_virt_id);
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: mac's socket
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 get_mac_unsockfd_by_cell_virt_id(UINT32 cell_virt_id)
{
    debug();
    if(cell_virt_id<MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id error\n ", __func__, __LINE__);
        return FUNC_ERR;
    }
    return vc_entity_table[cell_virt_id-1].cell_mac.unsockfd;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_mac_unsockfd_by_cell_virt_id(UINT32 cell_virt_id, int unsockfd)
{
    debug();
    if(cell_virt_id<MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id error\n ", __func__, __LINE__);
        return FUNC_ERR;
    }
    vc_entity_table[cell_virt_id-1].cell_mac.unsockfd = unsockfd;
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: phy's socket
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 get_phy_unsockfd_by_cell_virt_id(UINT32 cell_virt_id)
{
    debug();
    if(cell_virt_id<MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id error\n ", __func__, __LINE__);
        return FUNC_ERR;
    }
    return vc_entity_table[cell_virt_id-1].cell_phy.unsockfd;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_phy_unsockfd_by_cell_virt_id(UINT32 cell_virt_id, int unsockfd)
{
    debug();

    if(cell_virt_id<MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id error\n ", __func__, __LINE__);
        return FUNC_ERR;
    }
    vc_entity_table[cell_virt_id-1].cell_phy.unsockfd = unsockfd;
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  the number of running cells, those state is "CELL_NORMAL"
 ******************************************************************************/
INT32 get_virtual_cell_num()
{
    info("get_virtual_cell_num\n");

    INT32 counter = 0, cell_virt_id;
    VcSmState state;
    
    for(cell_virt_id = MIN_CELL_NUM; cell_virt_id <= MAX_CELL_NUM; cell_virt_id++)
    {
        state = vc_entity_table[cell_virt_id-1].cell_state;
        
        if(state == CELL_NORMAL)
        {
            counter++;
        }
    }
    return counter;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 get_virtual_cell_info(INT32 cell_snmp_index, MsgCfigCellSetupReq *msg)
{
    info("get_virtual_cell_info\n");

    if(msg == NULL)
    {
        error("get_virtual_cell_info() NULL pointer\n");
        return FUNC_ERR;
    }
    
    INT32 cell_virt_id;
    
    for(cell_virt_id = MIN_CELL_NUM; cell_virt_id <= MAX_CELL_NUM; cell_virt_id++)
    {
        if(cell_snmp_index == vc_entity_table[cell_virt_id-1].cell_snmp_index)
        {
            msg->msghead.msg_type   = MODULE_CFIG_MGMT;
            msg->msghead.msg_code   = CFIG_MGMT_MSG_VC_SETUP_REQ;
            msg->msghead.msg_len    = sizeof(MsgCfigCellSetupReq);
            msg->cell_snmp_index    = vc_entity_table[cell_virt_id-1].cell_snmp_index;
            msg->global_cell_id     = vc_entity_table[cell_virt_id-1].cell_g_id;
            msg->cell_mode          = vc_entity_table[cell_virt_id-1].cell_mode;
            msg->cell_bandwidth     = vc_entity_table[cell_virt_id-1].cell_bandwidth;
            
            memcpy(msg->mac_cfg_pth, 
                    vc_entity_table[cell_virt_id-1].cell_mac.mac_cfg_pth,
                    sizeof(char)*MAX_STR_LEN);
                    
            msg->cell_is_uni        = vc_entity_table[cell_virt_id-1].cell_is_uni;
            msg->rru_item.rru_id    = vc_entity_table[cell_virt_id-1].cell_rru.rru_id;
            
            memcpy(msg->rru_item.rru_ip, 
                    vc_entity_table[cell_virt_id-1].cell_rru.rru_ip,
                    sizeof(INT8)*16);
                    
            msg->rru_item.rru_port  = vc_entity_table[cell_virt_id-1].cell_rru.rru_port;
            
            return FUNC_OK;
        }
    }
    
    return FUNC_ERR;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 cell_index_exist(INT32 *cell_snmp_index)
{
    info("cell_index_exist\n");
    
    INT32 cell_virt_id;
    
    for(cell_virt_id = MIN_CELL_NUM; cell_virt_id <= MAX_CELL_NUM; cell_virt_id++)
    {        
        if(*cell_snmp_index == vc_entity_table[cell_virt_id-1].cell_snmp_index)
        {
            return FUNC_OK;
        }
    }
    
    return FUNC_ERR;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 get_next_cell_index(INT32 *cell_snmp_index)
{
    info("get_next_cell_index\n");
    
    INT32 cell_virt_id, flag = 0;
    VcSmState state;
    
    for(cell_virt_id = MIN_CELL_NUM; cell_virt_id <= MAX_CELL_NUM; cell_virt_id++)
    {        
        if(*cell_snmp_index == vc_entity_table[cell_virt_id-1].cell_snmp_index)
        {
            break;
        }
        if(cell_virt_id == MAX_CELL_NUM)
        {
            return FUNC_ERR;
        }
    }
    for(; cell_virt_id <= MAX_CELL_NUM; cell_virt_id++)
    {        
        if(vc_entity_table[cell_virt_id-1].cell_state == CELL_NORMAL)
        {
            *cell_snmp_index = vc_entity_table[cell_virt_id-1].cell_snmp_index;
            return FUNC_OK;
        }
    }
    
    return FUNC_ERR;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output:  cell_snmp_index: index for cfig_mgmt module(SNMP)
 *          return:  success: FUNC_OK
 *                   error:   FUNC_ERR
 ******************************************************************************/
INT32 get_first_cell_index(INT32 *cell_snmp_index)
{
    info("get_first_cell_index\n");
    
    INT32 cell_virt_id;
    
    for(cell_virt_id = MIN_CELL_NUM; cell_virt_id <= MAX_CELL_NUM; cell_virt_id++)
    {        
        if(vc_entity_table[cell_virt_id-1].cell_state == CELL_NORMAL)
        {
            *cell_snmp_index = vc_entity_table[cell_virt_id-1].cell_snmp_index;
            return FUNC_OK;
        }
    }
    
    return FUNC_ERR;
}


/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: cell_virt_id
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 get_cell_virt_id_use_cell_snmp_index(INT32 cell_snmp_index)
{
    info("get_first_cell_index\n");

    INT32 cell_virt_id;
    
    for(cell_virt_id = MIN_CELL_NUM; cell_virt_id <= MAX_CELL_NUM; cell_virt_id++)
    {        
        if(vc_entity_table[cell_virt_id-1].cell_snmp_index == cell_snmp_index)
        {
            return cell_virt_id;
        }
    }
    return FUNC_OK;
}
/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: cell_virt_id
 *                  error:   0
 ******************************************************************************/
INT32 get_available_cell_virt_id()
{
    debug();

    UINT32 cell_virt_id;
    
    for (cell_virt_id=MIN_CELL_NUM; cell_virt_id <= MAX_CELL_NUM; cell_virt_id++) 
    {
        if(vc_entity_table[cell_virt_id - 1].cell_virt_id == 0)
        {
            return cell_virt_id;
        }
    }
    return 0;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 is_cell_virt_id_exist(const UINT16 cell_virt_id)
{
    debug();

    UINT32 id;
    
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id invalid.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    for (id=MIN_CELL_NUM; id <= MAX_CELL_NUM; id++) 
    {
        if (cell_virt_id == vc_entity_table[id - 1].cell_virt_id &&
            cell_virt_id == vc_entity_table[id - 1].cell_snmp_index &&
            cell_virt_id != 0 &&
			vc_entity_table[id - 1].cell_state != 0)
        {
            success("cell_virt_id exist.\n");
            return FUNC_OK;
        }
    }
    return FUNC_ERR;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: cell_virt_id
 *                  error:   0
 ******************************************************************************/
UINT32 get_cell_virt_id_use_rru_id(UINT32 rru_id)
{
    debug();
    
    INT32 cell_virt_id;
    
    for(cell_virt_id=MIN_CELL_NUM; cell_virt_id<=MAX_CELL_NUM; cell_virt_id++)
    {
        if(vc_entity_table[cell_virt_id-1].cell_rru.rru_id == rru_id)
        {
            success("rru_id = %d, cell_virt_id = %d\n", rru_id, cell_virt_id);
            return cell_virt_id;
        }
    }
    
    return 0;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: rru_id
 *                  error:   FUNC_ERR
 ******************************************************************************/
UINT32 get_rru_id(UINT32 cell_virt_id)
{
    debug();
    
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id invalid.\n", __func__, __LINE__);
        return FUNC_ERR;
    }

    if(vc_entity_table[cell_virt_id-1].cell_rru.rru_id == 0)
    {
        error("%s:%d rru_id = %d, cell_virt_id = %d\n", __func__, __LINE__, 
              vc_entity_table[cell_virt_id-1].cell_rru.rru_id, cell_virt_id);
        return FUNC_ERR;
    }
    return vc_entity_table[cell_virt_id-1].cell_rru.rru_id;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: cell_virt_id
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 get_cell_virt_id_use_cell_phy_index(INT32 cell_phy_index)
{
    debug();

    if(cell_phy_index < 1 || cell_phy_index > MAX_PHY_INDEX)
    {
        error("%s:%d cell phy id error, %d\n", __func__, __LINE__, MAX_PHY_INDEX);
        return FUNC_ERR;
    }
    
    UINT32 cell_virt_id;
    
    for(cell_virt_id=MIN_CELL_NUM; cell_virt_id<=MAX_CELL_NUM; cell_virt_id++)
    {
        if(vc_entity_table[cell_virt_id-1].cell_phy.cell_phy_index == cell_phy_index)
        {
            return cell_virt_id;
        }
    }
    
    return FUNC_ERR;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_cell_virt_id(const UINT32 cell_virt_id)
{
    debug();
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id invalid.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    vc_entity_table[cell_virt_id-1].cell_virt_id = cell_virt_id;
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_cell_snmp_index(const UINT32 cell_virt_id, UINT32 cell_snmp_index)
{
    debug();
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id invalid.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    vc_entity_table[cell_virt_id-1].cell_snmp_index = cell_snmp_index;
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_cell_g_id(const UINT32 cell_virt_id, UINT32 cell_g_id)
{
    debug();
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id invalid.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    vc_entity_table[cell_virt_id-1].cell_g_id = cell_g_id;
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_cell_is_uni(const UINT32 cell_virt_id, UINT8 cell_is_uni)
{
    debug();
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id invalid.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    vc_entity_table[cell_virt_id-1].cell_is_uni = cell_is_uni;
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_cell_power(const UINT32 cell_virt_id, UINT8 cell_power)
{
    debug();
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id invalid.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    vc_entity_table[cell_virt_id-1].cell_power = cell_power;
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_cell_usernum(const UINT32 cell_virt_id, UINT8 cell_usernum)
{
    debug();
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id invalid.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    vc_entity_table[cell_virt_id-1].cell_usernum = cell_usernum;
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_cell_bandwidth(const UINT32 cell_virt_id, UINT8 cell_bandwidth)
{
    debug();
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id invalid.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    vc_entity_table[cell_virt_id-1].cell_bandwidth = cell_bandwidth;
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_cell_mode(const UINT32 cell_virt_id, VcMode cell_mode)
{
    debug();
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id invalid.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    vc_entity_table[cell_virt_id-1].cell_mode = cell_mode;
    
    return FUNC_OK;
}

/******************************************************************************
 * Description: cell entity's parameters set/get interface  
 *
 * Input: cell entity's parameters, check in "cell_mgmt_cell_entity.h"
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 set_cell_state(const UINT32 cell_virt_id, VcSmState cell_state)
{
    debug();
    if(cell_virt_id < MIN_CELL_NUM || cell_virt_id > MAX_CELL_NUM)
    {
        error("%s:%d cell_virt_id invalid.\n", __func__, __LINE__);
        return FUNC_ERR;
    }
    vc_entity_table[cell_virt_id-1].cell_state = cell_state;
    
    return FUNC_OK;
}

