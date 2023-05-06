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
** File name: cell_mgmt_api.h
** Description: CELL_MGMT's api for other module
**
** Current Version: 
** $Revision$
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2018.10
**
******************************************************************************/
#ifndef _MODULE_CELL_MGMT_API_H
#define _MODULE_CELL_MGMT_API_H

/* Dependencies --------------------------------------------------------------*/
#include "sbs_type.h"
#include "cell_mgmt_msg.h"
#include "om_common_h.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
extern INT32   unsockfd_mac_server, unsockfd_phy_server;

/* Functions -----------------------------------------------------------------*/
/******************************************************************************
 *  cell mgmt init function for main ctrl module
 ******************************************************************************/
INT32   cell_mgmt_module_init();
INT32   cell_mgmt_module_timer_callback(void *data);
INT32   cell_mgmt_clean_up();

/******************************************************************************
 *  cell mgmt api for self and other modules 
 ******************************************************************************/
INT32   set_cell_fd_by_cell_id(UINT32 cell_virt_id, INT32 cell_fd);
INT32   get_cell_fd_by_cell_id(UINT32 cell_virt_id);
INT32   get_mac_unsockfd_by_cell_virt_id(UINT32 cell_virt_id);
INT32   set_mac_unsockfd_by_cell_virt_id(UINT32 cell_virt_id, int unsockfd);
INT32   get_phy_unsockfd_by_cell_virt_id(UINT32 cell_virt_id);
INT32   set_phy_unsockfd_by_cell_virt_id(UINT32 cell_virt_id, int unsockfd);

INT32   get_virtual_cell_num();
INT32   get_virtual_cell_info(INT32 cell_snmp_index, MsgCfigCellSetupReq *msg);
INT32   cell_index_exist(INT32 *cell_snmp_index);
INT32   get_next_cell_index(INT32 *cell_snmp_index);
INT32   get_first_cell_index(INT32 *cell_snmp_index);

INT32   get_cell_virt_id_use_cell_snmp_index(INT32 cell_snmp_index);
INT32   get_cell_virt_id_use_cell_phy_index(INT32 cell_phy_index);
UINT32  get_rru_id(UINT32 cell_virt_id);
UINT32  get_cell_virt_id_use_rru_id(UINT32 rru_id);

INT32   get_available_cell_virt_id();
INT32   is_cell_virt_id_exist(const UINT16 cell_virt_id);

#endif /*<_MODULE_CELL_MGMT_API_H>*/

