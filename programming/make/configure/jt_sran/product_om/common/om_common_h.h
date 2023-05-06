/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:33 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:46 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:04 CST. */
/**********************************************
** Copyright (c) .
** All rights reserved.
**
** File name:om_common_h.h
** Description:header file of JT OM common.
**
** Current Version: 1.0
** Author: wangyuanyuan@ict.ac.cn
** Date: 20180803
***********************************************/

#ifndef OM_COMMON_H_
#define OM_COMMON_H_

/* Dependencies ------------------------------------------------------------- */


#include "sbs_timer.h"
#include "sbs_type.h"
#include "sbs_msgq.h"
#include "sbs_task.h"





/* Constants -----------------------------------------------------------------*/

#define FILE_PATH_MAX_LEN 	128
/* function return */
#define FUNC_ERR        -1
#define FUNC_OK         0
#define MSG_LEN_ZERO    0
#define STRING_EQUAL    0
#define THREAD_VALID	1


/* MODULEs defined in OM*/
#define MAX_MSGQ_MSG_NUM    256
#define MAX_MSGQ_MSG_LEN    2048


#define 	MODULE_CFIG_MGMT  0x0100        //OM Configuration management module
#define     MODULE_MAIN_CTRL  0x0200        //OM main control module   
#define     MODULE_BORD_MGMT  0x0300        //OM board management module  
#define     MODULE_CELL_MGMT  0x0400        //OM virtual cell management module       
#define     MODULE_RRUS_MGMT  0x0500        //OM RRU management module
#define     MODULE_FTPS_MGMT  0x0600        //OM FTP module  
#define	   	MODULE_ALAR_MGMT  0x0700        //OM ALarm management module
#define		MODULE_INET_MGMT  0x0800        //OM netport msgs management module
#define 	MODULE_TIMER	  0x0900

/* Modules defined in JT*/
#define     MODULE_MAC          0x1100      //MAC protocol entity  
#define     MODULE_PHY          0x1200      //PHY protocol entity 
#define     MODULE_CTRL_FPGA    0x1300      //Control FPGA module    
#define     MODULE_DATA_FPGA    0x1400      //Data FPGA module 1
#define     MODULE_DATA_FPGA2   0x1500      //Data FPGA module 2
#define     MODULE_DATA_FPGA3   0x1600      //Data FPGA module 3
#define     MODULE_DATA_FPGA4   0x1700      //Data FPGA module 4
#define	    MODULE_RRU          0x1800      //RRU module

/* MAC & PHY's message type */
#define     MSG_TYPE_MAC_CELL_MGMT  0x5000  
#define     MSG_TYPE_MAC_RRUS_MGMT  0x5001 
#define     MSG_TYPE_MAC_CFIG_MGMT  0x5002  
#define     MSG_TYPE_MAC_ALAR_MGMT  0x5003  

#define     MSG_TYPE_PHY_CELL_MGMT  0x5100  
#define     MSG_TYPE_PHY_ALAR_MGMT  0x5101  
#define     MSG_TYPE_PHY_MAIN_CTRL  0x5102  

/* MAC & PHY's message code */
#define     VCM_PHY_CELL_SETUP_REQ          001
#define     PHY_VCM_CELL_SETUP_RSP          002
#define     VCM_PHY_CELL_DELETE_REQ         003
#define     PHY_VCM_CELL_DELETE_RSP         004

#define     MAC_VCM_MAC_INIT_RSP            001
#define     VCM_MAC_CELL_START_REQ          002
#define     MAC_VCM_CELL_START_RSP          003
#define     VCM_MAC_CELL_DELET_REQ          004
#define     MAC_VCM_CELL_DELET_RSP          005

/*Msgq name of modules defined in OM*/

#define 	RRUS_MGMT_MSGQ_NAME		"/RRUS_MGMT_MSGQ"
#define     RRUS_MGMT_INET_SUP_MSGQ_NAME   "/RRUS_MGMT_SUP_MSGQ"
#define 	CELL_MGMT_MSGQ_NAME		"/CELL_MGMT_MSGQ"
#define 	MAIN_CTRL_MSGQ_NAME		"/MAIN_CTRL_MSGQ"
#define 	CFIG_MGMT_MSGQ_NAME		"/CFIG_MGMT_MSGQ"
#define	 	BORD_MGMT_MSGQ_NAME		"/BORD_MGMT_MSGQ"
#define 	INET_MGMT_MSGQ_NAME		"/INET_MGMT_MSGQ"
#define 	TIMER_MGMT_MSGQ_NAME		"/TIMER_MGMT_MSGQ"


#define		MAX_DPDK_PORT_NUM	4


#define		OM_MAIN_CFIG_XML	"./initCfg/MainCtrlLocal.xml"

#define		SYS_PRODUCT_DIR		"/etc/jt_product/jt_sran"

/* OM states */
typedef enum{
	SYS_INITING,
	SYS_RUNNING	
}OmState;	

/* NE SYNC state*/
typedef enum {
	NE_SYNC_NULL,
	NE_SYNC_OK,
	NE_SYNC_ERR
}NeSyncState;


/* PHY states */
typedef enum{
	PHY_NULL,
	PHY_OK,
	PHY_ERR	
}PhySwState;	


/* FPGA states */
typedef enum{
	FPGA_NULL,
	FPGA_UPDATING,
	FPGA_OK,
	FPGA_ERR
}FpgaState;	





/* Types ---------------------------------------------------------------------*/


/* Common Msg Head defined in OM*/
typedef struct {
	UINT16	msg_type;
	UINT16	msg_code;
	UINT32	msg_len;
}CommMsgHead;



/* Globals ------------------------------------------------------------------ */


/* FTP server information*/
extern UINT8	g_ftp_server_ip[16];		/* FTP server IP*/
extern UINT32	g_ftp_server_port;			/* FTP server IP port*/
extern UINT8	g_ftp_server_user_name[32];	/* FTP user name*/
extern UINT8	g_ftp_server_pwd[32];		/* FTP user password*/


/* OM config files paths on the FTP server*/
extern UINT8	g_ftp_server_om_files_path[FILE_PATH_MAX_LEN];	/* OM config files path*/
extern UINT8	g_ftp_server_local_path[FILE_PATH_MAX_LEN];

/* Software Paths on the FTP server*/
extern UINT8	g_ftp_server_om_sw_path[FILE_PATH_MAX_LEN];	/* OM SW path*/
extern UINT8	g_ftp_server_mac_sw_path[FILE_PATH_MAX_LEN];	/* MAC SW path*/
extern UINT8	g_ftp_server_phy_sw_path[FILE_PATH_MAX_LEN];	/* PHY SW path*/
extern UINT8	g_ftp_server_fpga_fw_path[FILE_PATH_MAX_LEN];	/* FPGA firmware path*/
extern UINT8	g_ftp_server_rru_fw_path[FILE_PATH_MAX_LEN];	/* RRU firmware path*/
extern UINT8	g_ftp_server_rru_sw_path[FILE_PATH_MAX_LEN];	/* RRU software path*/
extern UINT8	g_om_config_file_path[FILE_PATH_MAX_LEN];
extern UINT8	g_phy_sw_path[FILE_PATH_MAX_LEN];
extern UINT8	g_phy_execufile_path[FILE_PATH_MAX_LEN];
extern UINT8	g_mac_sw_path[FILE_PATH_MAX_LEN];
extern UINT8 	g_mac_cells_path[FILE_PATH_MAX_LEN];
extern UINT8 	g_mac_default_path[FILE_PATH_MAX_LEN];
extern UINT8	g_mac_execufile_path[FILE_PATH_MAX_LEN];
extern UINT8	g_phy_un_socket_path[FILE_PATH_MAX_LEN];
extern UINT8	g_mac_un_socket_path[FILE_PATH_MAX_LEN];
extern UINT8 	g_fpga_sw_path[FILE_PATH_MAX_LEN];
extern UINT8 	g_rru_sw_path[FILE_PATH_MAX_LEN];

/* OMC information*/
extern UINT8	g_omc_ip[16];			/* OMC server IP*/
extern UINT32	g_snmp_port;			/* SNMP port*/
extern UINT32	g_snmp_trap_port;			/* SNMP trap port*/

/* OM information*/
extern UINT32 	g_ne_id;					/* Network Element ID, Ox*/	
extern UINT8 	g_ne_ip_root[16];			/* NE ip root*/
extern UINT8 	g_om_ip_netmask[16];			/*NE ip netmask*/

/* Version of softwares and firmwares*/

extern UINT8	g_om_sw_version[32];			/* OM SW version*/
extern UINT8	g_ne_cfg_file_version[32];		/* NE config file version*/
extern UINT8	g_ne_mac_sw_version[32];		/* NE mac software  version*/
extern UINT8	g_ne_phy_sw_version[32];		/* NE phy software  version*/
extern UINT8	g_ne_fpga1_fw_version[32];		/* NE FPGA1 firmware  version*/
extern UINT8	g_ne_fpga2_fw_version[32];		/* NE FPGA2 firmware  version*/
extern UINT8	g_ne_fpga3_fw_version[32];		/* NE FPGA3 firmware  version*/
extern UINT8	g_ne_fpga4_fw_version[32];		/* NE FPGA4 firmware  version*/
extern UINT8	g_ne_fpga5_fw_version[32];		/* NE FPGA5 firmware  version*/
extern UINT8	g_ne_rru_fw_version[40];		/* NE RRU firmware  version*/
extern UINT8	g_ne_rru_sw_version[32];		/* NE RRU software  version*/


/* MAX CELL Number */

extern UINT8 	max_cell_num;			/*cell number supported by a NE*/

/*dpdk port number supported by a NE, max_dpdk_num = max_cell_num/2 */
extern UINT8 	max_dpdk_num; 		

/* Msg ID in OM*/
extern MsgqType 	MSGQ_ID_RRUS_MGMT;
extern MsgqType    MSGQ_ID_RRUS_MGMT_INET_SUP;
extern MsgqType 	MSGQ_ID_MAIN_CTRL;
extern MsgqType 	MSGQ_ID_CFIG_MGMT;
extern MsgqType 	MSGQ_ID_CELL_MGMT;
extern MsgqType 	MSGQ_ID_BORD_MGMT;
extern MsgqType 	MSGQ_ID_INET_MGMT;
extern MsgqType 	MSGQ_ID_TIMER_MGMT;


/* Task defined in OM*/
extern TaskType THREAD_RRUS_MGMT;
extern TaskType THREAD_RRUS_MGMT_INET_SUP;
extern TaskType THREAD_MAIN_CTRL;
extern TaskType THREAD_CFIG_MGMT;
extern TaskType THREAD_CELL_MGMT;
extern TaskType THREAD_BORD_MGMT;
extern TaskType THREAD_INET_MGMT;
extern TaskType THREAD_TIMER;


/* Task flag defined in OM*/
extern UINT8	 flag_thread_rrus_mgmt;
extern UINT8	 flag_thread_rrus_mgmt_inet_sup;
extern UINT8	 flag_thread_main_ctrl;
extern UINT8	 flag_thread_cfig_mgmt;
extern UINT8	 flag_thread_bord_mgmt;
extern UINT8	 flag_thread_cell_mgmt;
extern UINT8	 flag_thread_inet_mgmt;
extern UINT8 flag_thread_timer;

/* Network Element SYNC state*/
extern UINT8	g_ne_sync_state;

/* OM State*/
extern OmState g_om_state; 

/* Phy protocol software state*/
extern UINT8	g_phy_state;

/* FPGA initial state*/
extern UINT8	g_fpga_state;

/*Dpdk port MAC Addr*/
extern UINT8	g_dpdk_mac_addr[MAX_DPDK_PORT_NUM][18];

extern UINT8    g_om_port_ip_addr[16];
extern UINT8 	g_om_port_mac_addr[18];
extern UINT8    g_om_port_name[16];
extern UINT32 g_udp_sock_fd;                /* need check*/

//void send_msg_to_om(UINT16 module_id, UINT8 *msg_p, UINT32 msg_len);

#endif /* OM_COMMON_H_ */
