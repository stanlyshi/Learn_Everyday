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
** File name: cell_mgmt_comm_func.h 
** Description: CELL_MGMT module's common functions
**
** Current Version: 
** $Revision$
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2018.10
**
******************************************************************************/
#ifndef _MODULE_CELL_MGMT_COMM_FUNC_H
#define _MODULE_CELL_MGMT_COMM_FUNC_H

/* Dependencies --------------------------------------------------------------*/
#include "sbs_type.h"
#include "cell_mgmt_define.h"
#include "cell_mgmt_cell_entity.h"
#include "cell_mgmt_msg.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/
/******************************************************************************
 * CELL_MGMT
 ******************************************************************************/
INT32   cell_mgmt_cell_entity_init();
INT32   cell_mgmt_unsocket_init();
void    *cell_mgmt_module_main_ctrl(void  *arg);

/******************************************************************************
 * PHY
 ******************************************************************************/
INT8    handle_phy_state_change(void *msg);
void    callback_phy_inet(INT32 sock_fd, struct sockaddr *clientaddr, 
                            char *msg_p, UINT32 msg_len);

/******************************************************************************
 * MAC
 ******************************************************************************/
INT32   set_mac_init(INT32 cell_virt_id);
INT32   set_mac_kill(INT32 cell_virt_id);
INT32   cfg_Macconfig_paras(const UINT32 cell_virt_id);
INT32   delete_cell_folder(const UINT32 cell_virt_id);
void    callback_mac_inet(INT32 sock_fd, struct sockaddr *clientaddr, 
                            char *msg_p, UINT32 msg_len);
INT32   create_virtual_ipv4(const UINT32 id, UINT8 *ip_str);
INT32   create_virtual_network_card(const UINT32 cell_virt_id, const UINT8 *ipv4,
                                    const UINT8 *ethname);
INT32   delete_virtual_network_card(const UINT32 cell_virt_id, const UINT8 *ethname);

/******************************************************************************
 * RRUM
 ******************************************************************************/
INT8    handle_rru_reg(void *msg_p);
INT8    handle_rru_reg_del(MsgRrumRruRegisterReq *msg);
INT8    handle_rru_reg_add(MsgRrumRruRegisterReq *msg);

/******************************************************************************
 * Timer Syn
 ******************************************************************************/
INT8    handle_sys_timer_syn(void *msg);

/******************************************************************************
 * Show Functions 
 ******************************************************************************/
INT32   show_sm_on_screen(void);
INT32   show_cell_entity_on_screen(const INT32 cell_virt_id);
INT32   show_rru_info_table_on_screen();
INT32   show_phy_state_table_on_screen();

/******************************************************************************
 * Cell entity api Functions for cell_mgmt module
 ******************************************************************************/
INT32 set_cell_virt_id(const UINT32 cell_virt_id);
INT32 set_cell_snmp_index(const UINT32 cell_virt_id, UINT32 cell_snmp_index);
INT32 set_cell_g_id(const UINT32 cell_virt_id, UINT32 cell_g_id);
INT32 set_cell_is_uni(const UINT32 cell_virt_id, UINT8 cell_is_uni);
INT32 set_cell_power(const UINT32 cell_virt_id, UINT8 cell_power);
INT32 set_cell_usernum(const UINT32 cell_virt_id, UINT8 cell_usernum);
INT32 set_cell_bandwidth(const UINT32 cell_virt_id, UINT8 cell_bandwidth);
INT32 set_cell_mode(const UINT32 cell_virt_id, VcMode cell_mode);
INT32 set_cell_state(const UINT32 cell_virt_id, VcSmState cell_state);

// INT32 set_cell_rru_id(const UINT32 cell_virt_id, UINT16 rru_id);
// INT32 set_cell_rru_sn(const UINT32 cell_virt_id, UINT8 *rru_sn);
// INT32 set_cell_rru_status(const UINT32 cell_virt_id, RRU_STATUS rru_status);
// INT32 set_cell_rru_ip(const UINT32 cell_virt_id, UINT8 *rru_ip);
// INT32 set_cell_rru_port(const UINT32 cell_virt_id, UINT16 rru_port);
// INT32 set_cell_rru_axc_speed(const UINT32 cell_virt_id, UINT32 axc_speed);
// INT32 set_cell_rru_axc_index(const UINT32 cell_virt_id, UINT32 axc_index);

/******************************************************************************
 * Cell entity message timeout handle
 *      check in cell_mgmt_cell_entity.h file cell entity info
 ******************************************************************************/
void callback_handle_timer_cell_rru_wait_timeout(void *arg);
void callback_handle_timer_cell_setup_phy_cfg_timeout(void *arg);
void callback_handle_timer_cell_setup_mac_init_timeout(void *arg);
void callback_handle_timer_cell_setup_cell_start_timeout(void *arg);
void callback_handle_timer_cell_del_cell_close_timeout(void *arg);


 #endif /*<_MODULE_CELL_MGMT_COMM_FUNC_H>*/