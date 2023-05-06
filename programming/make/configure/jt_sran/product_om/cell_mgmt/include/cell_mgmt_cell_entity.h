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
** File name: cell_mgmt_cell_entity.h
** Description: cell_mgmt module cell entity struct
**
** Current Version: 
** $Revision$
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2018.10
**
******************************************************************************/
#ifndef _MODULE_CELL_MGMT_CELL_ENTITY_H
#define _MODULE_CELL_MGMT_CELL_ENTITY_H

/* Dependencies --------------------------------------------------------------*/
#include "sbs_type.h"
#include "cell_mgmt_define.h"

#include "rrus_mgmt_rru_entity.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
typedef enum {
    RRU_STATUS_MIN,
    RRU_ADDED,
    RRU_DELETED,
    //more
    RRU_STATUS_MAX,
}RRU_STATUS; 

typedef struct {
    UINT16          rru_id;         // 0000 0000      0000       0000
                                    //  fpga_id     cpri-port  rru jilian
    UINT8           rru_sn[16];     // RRU sn
    RRU_STATUS      rru_status;     // 0: added;  1: deleted 
    UINT8           rru_ip[16];     // RRU IP
    UINT16          rru_port;       // RRU port
    UINT32          axc_speed;      // 1.4M, 3M, 5M, 10M, 15M, 20M
    UINT32          axc_index;      // AxC 0100010100101000010
    //more
}__attribute__((packed))RruInfo; 

typedef struct{
    UINT8           cell_phy_index;     /**/
    UINT8           FpgaMac[6];         /**/
    INT32           unsockfd;           /**/
    //more
}__attribute__((packed))PhyInfo; 

typedef enum{
    MAC_NULL,
    MAC_OK,
    MAC_Err,
    //more
}MAC_STATUS; 

typedef struct{
    UINT32          cell_snmp_index;            //
    pid_t           mac_pid;                    //
    MAC_STATUS      mac_status;                 //
    char            mac_cfg_pth[MAX_STR_LEN];  //
    UINT8           enb_ipaddr[16];             //eNodeB ip
    UINT8           gtpu_recv_addr[16];         //mac_ip
    char            vbs_eth[256];       //
    INT32           unsockfd;                   //
    //more
}__attribute__((packed))MacInfo;   


typedef enum{
    VC_MODE_MIN,
    GSM,
    GMR,
    WCDMA,
    TD_SCDMA,
    TDD_LTE,
    FDD_LTE,  
    // more
    VC_MODE_MAX
}VcMode; 


typedef enum {
    VC_STATE_MIN_NUM,               //0
    
    CELL_NULL,                      //
    
    WAIT_SETUP_CPRI_CFG,            //
    WAIT_SETUP_PHY_CFG,             //
    WAIT_SETUP_MAC_INIT,            //
    WAIT_SETUP_CELL_START,          //
    
    CELL_NORMAL,                    //
    
    WAIT_DEL_CELL_CLOSE,            //
    WAIT_DEL_MAC_DEL,               //
    WAIT_DEL_PHY_DEL,               //
    WAIT_DEL_CPRI_RESET,            //
    
    CELL_STOP,                      //for sync, rru reg, phy state
    
    VC_STATE_MAX_NUM,               //
}VcSmState; 


typedef enum {
    VC_EVENT_MIN_NUM,
    
    /*cell setup*/
    MSG_VC_SETUP_REQ,
    MSG_VC_SETUP_CPRI_CFG_RSP,
    MSG_VC_SETUP_PHY_CFG_RSP,
    MSG_VC_SETUP_MAC_INIT_RSP,
    MSG_VC_SETUP_CELL_START_RSP, 
    
    /*cell delete*/
    MSG_VC_DEL_REQ, 
    MSG_VC_DEL_CELL_CLOSE_RSP,
    MSG_VC_DEL_MAC_DEL_RSP,
    MSG_VC_DEL_PHY_DEL_RSP,
    MSG_VC_DEL_CPRI_RESET_RSP, 
    
    /*cell stop for a while*/
    MSG_VC_STOP_REQ,                /*vcm 2 vcm*/
    MSG_VC_STOP_RECOVER,            /*vcm 2 vcm*/
    
    VC_EVENT_MAX_NUM
}VcSmEventType; 


typedef struct{
    struct sbs_timer_list   *timer_cell_rru_wait;
    struct sbs_timer_list   *timer_cell_setup_phy_cfg;
    struct sbs_timer_list   *timer_cell_setup_mac_init;
    struct sbs_timer_list   *timer_cell_setup_cell_start;
    struct sbs_timer_list   *timer_cell_del_cell_close;
}__attribute__((packed))VcTimer;


typedef struct {
    UINT32          cell_virt_id;           //0-7, mac cfg file 'Cfg/vbs-id'
    UINT32          cell_snmp_index;        //index
    UINT32          cell_g_id;              //global id
    UINT8           cell_is_uni;            //unique
    
    UINT8           cell_power;
    UINT8           cell_usernum;
    UINT8           cell_bandwidth;
    
    VcMode          cell_mode;              //TDD, FDD, .etc
    VcSmState       cell_state;             //cell's state
    VcSmState       cell_state_tmp;         //save state before change to STOP
    VcTimer         cell_timer;             //timer
    
    RruInfo         cell_rru;               // cell's rru 
    MacInfo         cell_mac;               // cell's mac info
    PhyInfo         cell_phy;               // cell's phy info
}__attribute__((packed))VcEntityNode; 

typedef struct {
    VcSmState   memery;
}VcStateStore;

/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
extern VcEntityNode vc_entity_table[MAX_CELL_NUM];  /*cell entity */
extern RruInfo      rru_info_table[MAX_RRU_NUM];    /*RRU*/
extern INT8         phy_state_table[MAX_PHY_INDEX]; /*PHY:0-no use;1-use*/


extern char        *vc_mode_str[VC_MODE_MAX];       /*store the name of mode.*/

#define vc_mode_to_str(mode) add_vc_mode_to_str(mode, #mode)

/******************************************************************************
 * Description: add to vc_mode_str
 *
 * Input:   vcmode: cell mode (int)
 *      vcmode_str: cell mode (string)
 *
 * Output: return:  success: FUNC_OK
 *                  error:   FUNC_ERR
 ******************************************************************************/
INT32 add_vc_mode_to_str(VcMode vcmode, char *vcmode_str);

#endif /*<_MODULE_CELL_MGMT_CELL_ENTITY_H>*/
