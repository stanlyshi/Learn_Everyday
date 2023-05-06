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
** File name: vcm_msgs.h
** Description: 
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2019.01 - 
**
***********************************************************************************************************************/
#ifndef _VCM_MSGS_H
#define _VCM_MSGS_H 1

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm_types.h"
#include "vcm_config.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/
typedef enum VCM_MSG_CODE_SET_e {
    VCM_INNER_MSG_CODE_MIN = 0x1000,
        
    VCM_INNER_MSG_CODE_CELL_SET_REQ,        //cell setup request
    VCM_INNER_MSG_CODE_CELL_SET_RSP,        //cell setup response
    VCM_INNER_MSG_CODE_CELL_DEL_REQ,        //cell delete request
    VCM_INNER_MSG_CODE_CELL_DEL_RSP,        //cell delete response
    VCM_INNER_MSG_CODE_L1_INIT_RSP,         //phy initial message, no response
    VCM_INNER_MSG_CODE_L2L3_INIT_RSP,       //mac initial message, no response

    VCM_INNER_MSG_CODE_MAX,
}VCM_MSG_CODE_SET;


/* Types -------------------------------------------------------------------------------------------------------------*/
typedef struct {
	UINT16	type;
	UINT16	code;
	UINT32	len;
}_packed msghdr_t;

/**
 *  MAC&PHY's message type
 */
#define L2L3_MSG_TYPE_MAC_CELL_MGMT     0x5000  
#define L2L3_MSG_TYPE_MAC_RRUS_MGMT     0x5001 
#define L2L3_MSG_TYPE_MAC_CFIG_MGMT     0x5002  
#define L2L3_MSG_TYPE_MAC_ALAR_MGMT     0x5003  
#define L2L3_MSG_TYPE_MAC_PRFM_MGMT     0x5004 
#define L1_MSG_TYPE_PHY_CELL_MGMT       0x5100  
#define L1_MSG_TYPE_PHY_ALAR_MGMT       0x5101  
#define L1_MSG_TYPE_PHY_MAIN_CTRL       0x5102  
#define L1_MSG_TYPE_PHY_PRFM_MGMT       0x5103
#define L1_MSG_TYPE_SIMU_PHY_CELL_MGMT  0X5200
#define L1_MSG_TYPE_SIMU_PHY_ALAR_MGMT  0X5201
#define L1_MSG_TYPE_SIMU_PHY_PRFM_MGMT  0X5202


/**
 *  MAC&PHY's message code
 */
#define L2L3_MSG_CODE_MAC_VCM_MAC_INIT_RSP      001
#define L2L3_MSG_CODE_VCM_MAC_CELL_START_REQ    002
#define L2L3_MSG_CODE_MAC_VCM_CELL_START_RSP    003
#define L2L3_MSG_CODE_VCM_MAC_CELL_DELETE_REQ   004
#define L2L3_MSG_CODE_MAC_VCM_CELL_DELETE_RSP   005
#define L2L3_MSG_CODE_MAC_TERMINATE_RSP         006 

#define L1_MSG_CODE_VCM_PHY_CELL_SETUP_REQ      001
#define L1_MSG_CODE_PHY_VCM_CELL_SETUP_RSP      002
#define L1_MSG_CODE_VCM_PHY_CELL_DELETE_REQ     003
#define L1_MSG_CODE_PHY_VCM_CELL_DELETE_RSP     004

#define L1_MSG_CODE_SIMU_PHY_INIT_RSP           001
#define L1_MSG_CODE_SIMU_PHY_TERMINATE_RSP      002

/**
 *  RRUM's message code
 */
#define RRUS_MGMT_MSG_CODE_CPRI_AxC_CFG_REQ     001/*cpri configure request*/
#define RRUS_MGMT_MSG_CODE_CPRI_AxC_CFG_RSP     002/*cpri configure response*/
#define RRUS_MGMT_MSG_CODE_RRU_REGISTER_REQ     003/*rru regeister request*/



/***********************************************************************************************************************
 *  CELL_MGMT inner messages
 **********************************************************************************************************************/
typedef struct {
    UINT32  cell_id;
    ULONG   event_mask;
}_packed vcm_inner_msg_pipe_t;


/***********************************************************************************************************************
 *  L1: PHY
 **********************************************************************************************************************/
typedef struct {
    msghdr_t  hdr;
    UINT32  cell_id;
}_packed vcm_outer_msg_l1_hdr;  /* phy/l1 message header */

typedef struct {
    msghdr_t  hdr;
    UINT32  cell_id;
    UINT32  axc_ind;
    UINT8   cell_phy_index;
    UINT8   fpga_mac[6];  
    UINT32  fiber_num; 
}_packed vcm_outer_msg_l1_cell_setup_req; /* cell setup requare */

typedef struct {
    msghdr_t  hdr;
    UINT32  cell_id;
    INT32   rslt;
}_packed vcm_outer_msg_l1_cell_setup_rsp; /* cell setup response */

typedef struct {
    msghdr_t  hdr;
    UINT32  cell_id;
    UINT8   PhyCellIndex;
}_packed vcm_outer_msg_l1_cell_delete_req; /* cell delete requare */


typedef struct {
    msghdr_t  hdr;
    UINT32  cell_id;
    INT32   rslt;
}_packed vcm_outer_msg_l1_cell_delete_rsp; /* cell delete response */


/***********************************************************************************************************************
 *  simulate L1: simulate PHY
 **********************************************************************************************************************/
typedef struct{
    msghdr_t  hdr;
    UINT32  cell_id;
    UINT32  pid;
    INT32   rslt;
}_packed vcm_outer_msg_simu_l1_init_rsp;

typedef struct{
    msghdr_t  hdr;
    UINT32  cell_id;
}_packed vcm_inner_msg_simu_l1_terminate_rsp;



/***********************************************************************************************************************
 *  L2L3: MAC
 **********************************************************************************************************************/
typedef struct {
    msghdr_t  hdr;
    UINT32  cell_id;
}_packed vcm_outer_msg_l2l3_hdr;  /* mac/l2l3 message header */

typedef struct{
    msghdr_t  hdr;
    UINT32  cell_id;
    UINT32  pid;
    INT32   rslt;
} _packed vcm_outer_msg_l2l3_init_rsp;

typedef struct{
    msghdr_t  hdr;
    UINT32  cell_id;
    UINT8   PhyCellIndex;
    UINT8   UniCellInd;
    UINT8   RruPort;    /*31000*/   //not use
    UINT8   RruIp[16];              //not use
}_packed vcm_outer_msg_l2l3_cell_start_req;

typedef struct{
    msghdr_t  hdr;
    UINT32  cell_id;
    INT32   rslt;
}_packed vcm_outer_msg_l2l3_cell_start_rsp;

typedef struct{
    msghdr_t  hdr;
    UINT32  cell_id;
}_packed vcm_outer_msg_l2l3_cell_delete_req;

typedef struct{
    msghdr_t  hdr;
    UINT32  cell_id;
    INT32   rslt;
}_packed vcm_outer_msg_l2l3_cell_delete_rsp;

typedef struct{
    msghdr_t  hdr;
    UINT32  cell_id;
    INT32   rslt;
}_packed vcm_inner_msg_l2l3_terminate_rsp;

/**
 *  Mac Alarm
 */
typedef struct {
    msghdr_t    hdr;
    UINT32      cell_id;
    UINT8       level; /* alarm level: [1-4:important-regular] [5:clear] */
    /*... other paras */
}_packed vcm_inner_msg_l2l3_alr;


/***********************************************************************************************************************
 *  RRUM_MGMT's message
 **********************************************************************************************************************/
typedef enum{
    RRU_REGISTER,
    RRU_UNREGISTER,
}RRU_REG_TYPE;

typedef struct { 
    msghdr_t    hdr;
    UINT32      cell_id;
    UINT32      cell_power_factor;      //2^16 = 1
    UINT32      rru_id;
    UINT32      axc_speed;  /* bandwidth */   
    UINT32      cfg_ind;    /* axc valide: 0-valide, 1-invalide*/
}_packed vcm_outer_msg_rrum_cpriaxc_cfg_req;

typedef struct {
    msghdr_t    hdr; 
    UINT32      cell_id;
    UINT32      rru_id;     
    UINT32      axc_ind;
    INT32       cfg_rslt; 
    INT32       err_type;
}_packed vcm_outer_msg_rrum_cpriaxc_cfg_rsp;


typedef struct {
    msghdr_t        hdr;
    UINT8           rru_sn[VCM_RRU_SN_LEN];
    UINT32          rru_id;
    RRU_REG_TYPE    rru_reg;
}_packed vcm_outer_msg_rrum_rru_reg_req;



/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/


#endif /*<_VCM_MSGS_H>*/

