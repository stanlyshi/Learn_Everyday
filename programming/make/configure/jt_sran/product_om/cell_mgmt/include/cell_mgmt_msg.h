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
** File name: cell_mgmt_msg.h 
** Description: CELL_MGMT module's msgs with other module
**
** Current Version: 
** $Revision$
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2018.10
**
******************************************************************************/
#ifndef _MODULE_CELL_MGMT_MSG_H
#define _MODULE_CELL_MGMT_MSG_H

/* Dependencies --------------------------------------------------------------*/
#include "sbs_type.h"

#include "cell_mgmt_define.h"
#include "cell_mgmt_cell_entity.h"

#include "cm_api.h"

#include "om_common_h.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/*********************************
 * BORD Msg
 *********************************/
/* #ifndef MNT_MGMT_API_H_
typedef struct {
	CommMsgHead     msghead;
	NeSyncState 	bord_syn; 					   
}__attribute__((packed))MsgBordSyn;
#endif */

/*********************************
 * MAIN_CTRL Msg
 *********************************/
typedef struct {
	CommMsgHead     msghead;
	PhySwState		new_phy_state;
}__attribute__((packed))MsgPhyState;

/*********************************
 * PHY Msg
 *********************************/
typedef struct {
	CommMsgHead     msghead;
    UINT32          cell_virt_id;
}__attribute__((packed))MsgPhyHead;

typedef struct {
	CommMsgHead     msghead;
    UINT32          cell_virt_id;
    UINT32	        axc_ind;
    UINT8	        cell_phy_index;
    UINT8	        fpga_mac[6];  
    UINT32	        fiber_num; 
}__attribute__((packed))MsgPhyCellSetupReq;

typedef struct {
	CommMsgHead     msghead;
    UINT32          cell_virt_id;
    UINT8           PhyCellIndex;
}__attribute__((packed))MsgPhyCellDelReq;

typedef struct {
	CommMsgHead     msghead;
    UINT32	        cell_virt_id;
    INT32           SetupRslt;
}__attribute__((packed))MsgPhyCellSetupRsp;

typedef struct {
	CommMsgHead     msghead;
    UINT32	        cell_virt_id;
    INT32           DeleteRslt;
}__attribute__((packed))MsgPhyCellDelRsp;

/*********************************
 * MAC Msg
 *********************************/
typedef enum {
    MAC_KILL_RSP = 6,   /*send msg to CELL_MGMT*/
}MsgMacCellMgmtMsgType;

typedef struct{
    CommMsgHead     msghead;
    UINT32          cell_virt_id;
    UINT32          pid;
    INT32           rslt;
} __attribute__((packed)) MsgMacInitRsp;

typedef struct{
    CommMsgHead     msghead;
    UINT32          cell_virt_id;
    UINT8           PhyCellIndex;
    UINT8           UniCellInd;
    UINT8           RruPort;    /*31000*/
    UINT8           RruIp[16];
} __attribute__((packed))MsgMacCellSetupReq;

typedef struct{
    CommMsgHead     msghead;
    UINT32          cell_virt_id;
    INT32           rslt;
} __attribute__((packed))MsgMacCellSetupRsp;

typedef struct{
    CommMsgHead     msghead;
    UINT32          cell_virt_id;
} __attribute__((packed))MsgMacCellDelReq;

typedef struct{
    CommMsgHead     msghead;
    UINT32          cell_virt_id;
    INT32           rslt;
} __attribute__((packed))MsgMacCellDelRsp;

typedef struct{
    CommMsgHead     msghead;
    UINT32          cell_virt_id;
} __attribute__((packed))MsgMacKillRsp;

/*********************************
 * CFIG_MGMT module Msg
 *********************************/
typedef enum{
    CFIG_MGMT_MSG_VC_SETUP_REQ  = 1,    /*cfig 2 vcm: virtual setup*/
    CFIG_MGMT_MSG_VC_DEL_REQ,           /*cfig 2 vcm: virtual delete*/
    CFIG_MGMT_MSG_VC_SETUP_RSP,         /*vcm 2 cfig*/
    CFIG_MGMT_MSG_VC_DEL_RSP,           /*vcm 2 cfig*/
    
    CFIG_MGMT_MSG_MAX_NUM,
}CfigMsgType;

typedef struct {
    UINT32          rru_id;
    UINT8           rru_sn[16]; // must provide
    UINT8           rru_ip[16];
    UINT16          rru_port;
}__attribute__((packed))RruParas;

typedef struct {
    CommMsgHead     msghead;
    UINT32          cell_snmp_index;
    UINT32          global_cell_id;
    VcMode          cell_mode;
    UINT32          cell_bandwidth;
    char            mac_cfg_pth[MAX_STR_LEN];
    UINT8           cell_is_uni;
    RruParas        rru_item;
}__attribute__((packed))MsgCfigCellSetupReq;

typedef struct {
    CommMsgHead     msghead;
    UINT32          cell_snmp_index;
}__attribute__((packed))MsgCfigCellDelReq;

/*********************************
 * RRUM Msg
 *********************************/
typedef enum{
    RRUS_MGMT_MSG_CPRI_AxC_CFG_REQ = 1, /*cpri configure request*/
    RRUS_MGMT_MSG_CPRI_AxC_CFG_RSP,     /*cpri configure response*/
    RRUS_MGMT_MSG_RRU_REG_REQ,          /*rru regeister request*/
    RRUS_MGMT_MSG_MAX_NUM,
}RrumMsgType;

typedef enum{
    RRU_ADD,
    RRU_DEL,
}RruReg;

typedef struct {
    CommMsgHead     msghead;
    UINT8           rru_sn[16];
    UINT32          rru_id;
    RruReg          rru_reg;
} __attribute__((packed))MsgRrumRruRegisterReq;

typedef struct { 
    CommMsgHead     msghead;
    UINT32          rru_id;
    UINT32          axc_speed;  /* bandwidth */   
    UINT32          cfg_ind;    /* axc valide: 0-valide, 1-invalide*/
} __attribute__((packed))MsgRrumCpriAxcCfgReq;

typedef struct {
    CommMsgHead     msghead;
    UINT32          rru_id;     
    UINT32          axc_ind;
    INT32           cfg_rslt; 
    INT32           err_type;
} __attribute__((packed))MsgRrumCpriAxcCfgRsp;


#endif /*<_MODULE_CELL_MGMT_MSG_H>*/