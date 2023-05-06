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
** File name: vcm_cell_entity.h
** Description: 
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2019.01 - 
**
***********************************************************************************************************************/
#ifndef _VCM_CELL_ENTITY_H
#define _VCM_CELL_ENTITY_H  1

#if !defined _VCM_H
 #error "Never include vcm_cell_entity.h."
#endif


/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm_config.h"
#include "vcm_types.h"
#include "vcm_errno.h"
#include "vcm_sys_wrapper.h"
#include "vcm_sm.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/
typedef enum {
    BORD_SYNC_NULL, //Must is 0
    BORD_SYNC_OK,   //Must is 1
}vcm_inner_bord_sync;

/**
 *  simulate PHY status: null, ok and error
 */
typedef enum {
    SIMU_PHY_STATUS_MIN = -1,
    SIMU_PHY_STATUS_NULL,
    SIMU_PHY_STATUS_OK,
    SIMU_PHY_STATUS_ERR,
    SIMU_PHY_STATUS_MAX,
}SIMU_PHY_STATUS;

/**
 *  simulate MAC status: ok and error
 */
typedef enum{
    MAC_STATUS_MIN = -1,
    MAC_STATUS_NULL,
    MAC_STATUS_OK,
    MAC_STATUS_ERR,
    //more
    MAC_STATUS_MAX,
}MAC_STATUS; 

/**
 *  RRU status: add and delete
 */
typedef enum {
    RRU_STATUS_MIN,
    RRU_STATUS_ADDED,
    RRU_STATUS_DELETED,
    RRU_STATUS_MAX,
}RRU_STATUS; 



/**
 * mac information
 */
typedef struct MacInfo_t{
    UINT32          snmp_index;            //
    UINT32          pid;                    //
    MAC_STATUS      status;                 //
    UINT8           cfg_pth[VCM_STR_LEN];   //
    UINT8           ether[VCM_IP_LEN];              //MAC address: 525400c94d80
    UINT8           enb_ipaddr[VCM_IP_LEN];             //eNodeB ip
    UINT8           gtpu_recv_addr[VCM_IP_LEN];         //mac_ip
    UINT8           vbs_eth[VCM_STR_LEN];               //
    INT32           sockfd;                   //
    //more
}_packed mac_info_t;  
    
/**
 *  PHY information
 */
typedef struct RealPhyInfo_t{
    UINT8           index;     /*range in 0-7, but stored as 1-8, minus 1 before send it to mac*/
    UINT8           FpgaMac[6];         /**/
    INT32           sockfd;           /**/
    //more
}_packed real_phy_info_t; 

/**
 *  simulate phy information
 */
typedef struct SimuPhyInfo_t{
    UINT8           index;     // range in 8-14, stored as 8-14
    UINT32          pid;
    UINT64          tcpServerPort;
    SIMU_PHY_STATUS status;
    INT32           sockfd;       
}_packed simu_phy_info_t;


/**
 *  RRU information
 */
typedef struct RruInfo_t{
    UINT16          id;         // 0000 0000      0000       0000
                                    //  fpga_id     cpri-port  rru jilian
    UINT8           sn[VCM_RRU_SN_LEN];     // RRU sn
    RRU_STATUS      status;     // 0: added;  1: deleted 
    UINT8           ip[VCM_IP_LEN];     // RRU IP
    UINT16          port;       // RRU port
    UINT32          axc_speed;      // 1.4M, 3M, 5M, 10M, 15M, 20M
    UINT32          axc_index;      // AxC 0100010100101000010
    UINT32          power_factor;      //2^16 = 1
    
    //more
}_packed rru_info_t; 


/**
 *  cell timer: deal with message(rsp/req) timeout
 */
typedef struct VcVosTimer_t{
    Timer_t rru_wait_timer_id;
    Timer_t setup_phy_cfg_timer_id;
    Timer_t setup_mac_init_timer_id;
    Timer_t setup_cell_start_timer_id;
    Timer_t del_cell_close_timer_id;
    Timer_t setup_simu_phy_init_timer_id;
}_packed VcVosTimer;


/**
 *  temp state, and argument
 */
typedef struct VcRecover_t{
    vcm_sm_state_t  state;          //save state before change to STOP
    UINT8           *arg;        //for cell recover from Stop State
    UINT32          len;        //message length
}_packed cell_recover_t;

/**
 *  cell entity node
 */
typedef struct cell_node {
    UINT32          id;                 //0-7 ==> 1-8, mac cfg file 'Cfg/vbs-id'
    UINT32          snmp_index;        //index
    UINT32          g_id;              //global id
    cell_type_t      type;              //simulated cell or not, 0-not simu; 1-simu
    cell_role_t     is_unify;          //unified cell or not, 0-not unified; 1-unified
    
    UINT8           power;
    
    UINT8           usernum;
    UINT8           bandwidth;  
    
    cell_mode_t          mode;              //TDD, FDD, .etc
    vcm_sm_state_t       state;             //cell's state
    
#ifdef _VCM_TURN_ON_TIMER
    VcVosTimer      timer;             //VOS timer
#endif /*<_VCM_TURN_ON_TIMER>*/

    rru_info_t         rru;               // cell's rru 
    mac_info_t         mac;               // cell's mac info
    
    union PhyInfo{
    real_phy_info_t     real;               // cell's phy info
    simu_phy_info_t     simu;               // simulated phy info
    }phy;
    
    cell_recover_t       recover;           //recover from STOP state

    struct cell_node   *next;       // For further extend
    struct cell_node   *prev;       //
    
}_packed cell_node_t; 


#if 0
/**
 *  Like file_operations structure
 */
typedef struct cell_operations {
	INT32 (*init) (cell_node_t *cell, SIZE_T nmemb, SIZE_T size);
    INT32 (*setup) (cell_node_t *cell, UINT32 cell_id);
    INT32 (*delete) (cell_node_t *cell, UINT32 cell_id);
    INT32 (*migration) (cell_node_t *cell, UINT32 cell_id);
    INT32 (*expand) (cell_node_t *cell, UINT32 cell_id);
    INT32 (*reduce) (cell_node_t *cell, UINT32 cell_id);
	INT32 (*clean) (cell_node_t *cell, UINT32 cell_id);
    INT32 (*ctrl) (cell_node_t *cell, UINT32 cell_id);
}_packed cell_operations_t;
#endif


/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
#define add_mode(mode)          vcm_cell_add_mode(mode, #mode)
#define add_rru_status(status)  vcm_cell_add_rru_status(status, #status)
#define add_mac_status(status)  vcm_cell_add_mac_status(status, #status)


#define MEM_ADDR_OF_CELL(id) (VOID *)(&Cells[id-VCM_MIN_CELL_NUM])


#if VCM_MIN_CELL_NUM>VCM_MAX_CELL_NUM
 #error "VCM_MIN_CELL_NUM Must lower than VCM_MAX_CELL_NUM."
#endif



/* Globals -----------------------------------------------------------------------------------------------------------*/
extern cell_node_t Cells[VCM_CELL_NUM];  /*cell entity */

extern CHAR*    vcm_mode_str[VC_MODE_MAX];  /* cell mode string */

extern vcm_set  all_cell_set, virt_cell_set, simu_cell_set; /* for cell recording */


extern INT8     Real_phy_states[REAL_PHY_INDEX_NUM]; /*PHY:0-no use;1-use*/
extern INT8     Simu_phy_states[SIMU_PHY_INDEX_NUM]; /*  */
extern vcm_set  all_phy_index_set, real_phy_index_set, simu_phy_index_set; /* for phy index recording */

extern CHAR*    vcm_mac_status_str[MAC_STATUS_MAX];  /* cell mode string */
extern INT8     Real_phy_states[REAL_PHY_INDEX_NUM]; /*PHY:0-no use;1-use*/
extern INT8     Simu_phy_states[SIMU_PHY_INDEX_NUM]; /*  */

extern INT      L1_listen_sockfd, L2L3_listen_sockfd;

extern rru_info_t  Rrus[VCM_RRU_NUM];  /* rrus */
extern vcm_set  rrus_set; /* for rru register recording */
extern CHAR*    vcm_rru_status_str[RRU_STATUS_MAX];  /* cell mode string */



/* Functions ---------------------------------------------------------------------------------------------------------*/
INT32 vcm_cell_entity_init();

INT32 vcm_cell_add_mode(cell_mode_t mode, CHAR *Smode);
INT32 vcm_cell_add_rru_status(RRU_STATUS status, CHAR *Sstatus);
INT32 vcm_cell_add_mac_status(MAC_STATUS status, CHAR *Sstatus);


/**
 *  cell's get and set api, you should not call the global "CellNode Cells[VCM_CELL_NUM]"
 */
/**
 *  Check Cell id with return 
 */
#define CHECK_ONE_CELL_ID(cell_id)                                              \
    if(VCM_OK != is_cell_id_available(cell_id) || cell_id == VCM_ALL_CELLS) {   \
        log_vcm_err("Cell id error. %d\n", cell_id);                            \
        __vcm_set_errno(_VCM_EICID);                                            \
        return VCM_ERR;                                                         \
    }

INT32 is_cell_id_available(CONST UINT32 cell_id);
INT32 set_cell_id(CONST UINT32 cell_id);
INT32 get_cell_id(CONST UINT32 cell_id);

INT32 is_snmp_index_available(CONST UINT32 snmp_index);
INT32 set_snmp_index(CONST UINT32 cell_id, CONST UINT32 snmp_index);
INT32 get_snmp_index(CONST UINT32 cell_id);

INT32 set_g_id(CONST UINT32 cell_id, CONST UINT32 g_id);
INT32 get_g_id(CONST UINT32 cell_id);

INT32 is_cell_type_available(cell_type_t type);
INT32 set_cell_type(CONST UINT32 cell_id, cell_type_t type);
INT32 get_cell_type(CONST UINT32 cell_id);

INT32 is_cell_unify(CONST UINT32 cell_id);
INT32 set_is_unify(CONST UINT32 cell_id, cell_role_t is_unify);
INT32 get_is_unify(CONST UINT32 cell_id);

INT32 set_cell_power(CONST UINT32 cell_id, CONST UINT8 power);
INT8 get_cell_power(CONST UINT32 cell_id);

INT32 set_cell_usernum(CONST UINT32 cell_id, CONST UINT8 usernum);
INT8 get_cell_usernum(CONST UINT32 cell_id);

INT32 set_cell_bandwidth(CONST UINT32 cell_id, CONST UINT8 bandwidth);
INT8 get_cell_bandwidth(CONST UINT32 cell_id);

INT32 is_cell_mode_available(cell_mode_t mode);
INT32 set_cell_mode(CONST UINT32 cell_id, CONST cell_mode_t mode);
INT32 get_cell_mode(CONST UINT32 cell_id);

INT32 is_cell_state_available(vcm_sm_state_t state);
INT32 set_cell_state(CONST UINT32 cell_id, CONST vcm_sm_state_t state);
INT32 get_cell_state(CONST UINT32 cell_id);

/* RRU information */
INT32 set_rru_id(CONST UINT32 cell_id, CONST UINT16 rru_id);
INT16 get_rru_id(CONST UINT32 cell_id);

INT32 set_rru_sn(CONST UINT32 cell_id, CONST UINT8 rru_sn[VCM_RRU_SN_LEN]);
INT32 get_rru_sn(CONST UINT32 cell_id, UINT8 rru_sn[VCM_RRU_SN_LEN]);

INT32 set_rru_status(CONST UINT32 cell_id, RRU_STATUS status);
INT32 get_rru_status(CONST UINT32 cell_id);

INT32 set_rru_ip(CONST UINT32 cell_id, CONST UINT8 ip[VCM_IP_LEN]);
INT32 get_rru_ip(CONST UINT32 cell_id, UINT8 ip[VCM_IP_LEN]);

INT32 set_rru_port(CONST UINT32 cell_id, CONST UINT16 port);
INT16 get_rru_port(CONST UINT32 cell_id);

INT32 set_rru_axc_speed(CONST UINT32 cell_id, CONST UINT32 axc_speed);
INT32 get_rru_axc_speed(CONST UINT32 cell_id);

INT32 set_rru_axc_index(CONST UINT32 cell_id, CONST UINT32 axc_index);
INT32 get_rru_axc_index(CONST UINT32 cell_id);

/* MAC information */
INT32 set_mac_snmp_index(CONST UINT32 cell_id, CONST UINT32 snmp_index);
INT32 get_mac_snmp_index(CONST UINT32 cell_id);

INT32 set_mac_pid(CONST UINT32 cell_id, CONST UINT32 pid);
INT32 get_mac_pid(CONST UINT32 cell_id);

INT32 set_mac_status(CONST UINT32 cell_id, MAC_STATUS status);
INT32 get_mac_status(CONST UINT32 cell_id);

INT32 set_mac_cfg_pth(CONST UINT32 cell_id, CONST CHAR cfg_pth[VCM_STR_LEN]);
INT32 get_mac_cfg_pth(CONST UINT32 cell_id, CHAR cfg_pth[VCM_STR_LEN]);

INT32 set_mac_ether(CONST UINT32 cell_id, CONST CHAR ether[VCM_IP_LEN]);
INT32 get_mac_ether(CONST UINT32 cell_id, CHAR ether[VCM_IP_LEN]);

INT32 set_mac_enb_ipaddr(CONST UINT32 cell_id, CONST CHAR enb_ipaddr[VCM_IP_LEN]);
INT32 get_mac_enb_ipaddr(CONST UINT32 cell_id, CHAR enb_ipaddr[VCM_IP_LEN]);

INT32 set_mac_gtpu_recv_addr(CONST UINT32 cell_id, CONST CHAR gtpu_recv_addr[VCM_IP_LEN]);
INT32 get_mac_gtpu_recv_addr(CONST UINT32 cell_id, CHAR gtpu_recv_addr[VCM_IP_LEN]);

INT32 set_mac_vbs_eth(CONST UINT32 cell_id, CONST CHAR vbs_eth[VCM_STR_LEN]);
INT32 get_mac_vbs_eth(CONST UINT32 cell_id, CHAR vbs_eth[VCM_STR_LEN]);

INT32 set_mac_sockfd(CONST UINT32 cell_id, CONST INT32 sockfd);
INT32 get_mac_sockfd(CONST UINT32 cell_id);

/* PHY information */
INT32 set_phy_index(CONST UINT32 cell_id, CONST UINT8 index);
INT8 get_phy_index(CONST UINT32 cell_id);

INT32 set_phy_FpgaMac(CONST UINT32 cell_id, CONST UINT8 FpgaMac[6]);
INT32 get_phy_FpgaMac(CONST UINT32 cell_id, UINT8 FpgaMac[6]);

INT32 set_phy_sockfd(CONST UINT32 cell_id, CONST INT32 sockfd);
INT32 get_phy_sockfd(CONST UINT32 cell_id);

/* simulate PHY information */
INT32 set_simu_phy_index(CONST UINT32 cell_id, CONST UINT8 index);
INT8 get_simu_phy_index(CONST UINT32 cell_id);

INT32 set_simu_phy_pid(CONST UINT32 cell_id, CONST UINT32 pid);
INT32 get_simu_phy_pid(CONST UINT32 cell_id);

INT32 set_simu_phy_tcpServerPort(CONST UINT32 cell_id, UINT64 tcpServerPort);
INT64 get_simu_phy_tcpServerPort(CONST UINT32 cell_id);

INT32 set_simu_phy_status(CONST UINT32 cell_id, SIMU_PHY_STATUS status);
INT32 get_simu_phy_status(CONST UINT32 cell_id);

INT32 set_simu_phy_sockfd(CONST UINT32 cell_id, CONST INT32 sockfd);
INT32 get_simu_phy_sockfd(CONST UINT32 cell_id);

/* summary information check */
INT32 get_cellnum_of_celltype(CONST cell_type_t type);
INT32 get_cellnum_of_cellstate(CONST vcm_sm_state_t state);
INT32 get_cellnum_of_cellmode(CONST cell_mode_t mode);

/* "is" */
INT32 is_mode(CONST UINT32 cell_id, CONST cell_mode_t mode);
INT32 is_state(CONST UINT32 cell_id, CONST vcm_sm_state_t state);
INT32 is_type(CONST UINT32 cell_id, CONST cell_type_t type);



/* cell recover from STOP */
INT32 cell_recover(CONST UINT32 cell_id);

/* cell stop */
INT32 cell_stop(CONST UINT32 cell_id);

/* display function */
INT32 cell_info_display(FILE *fp, CONST UINT32 cell_id);
INT32 vcm_rrus_display(FILE *fp);

#endif /*<_VCM_CELL_ENTITY_H>*/
