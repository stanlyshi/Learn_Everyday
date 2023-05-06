/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:34 CST. */
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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:03 CST. */
/**********************************************
** Copyright (c) .
** All rights reserved.
**
** File name:om_main.c
** Description:source file of om_main.c
**
** Current Version: 1.0
** Author: 
** Date: 201808
***********************************************/

/* Dependencies ------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <sbs_msgq.h>
#include <getopt.h>
#include <mnt_mgmt_msg_handle.h>
#include <signal.h>
#include "sbs_timer.h"
#include "sbs_type.h"
//#include "sbs_defs.h"
#include "sbs_log.h"
#include "sbs_task.h"

#include "om_common_h.h"
#include "om_main_ctrl.h"
#include "inet_mgmt.h"
//#include "test.h"

/* Global variables ------------------------------------------------------------- */

/* FTP server information*/
UINT8	g_ftp_server_ip[16];		/* FTP server IP*/
UINT32	g_ftp_server_port;			/* FTP server IP port*/
UINT8	g_ftp_server_user_name[32];	/* FTP user name*/
UINT8	g_ftp_server_pwd[32];		/* FTP user password*/


/* OM config files paths on the FTP server*/
UINT8	g_ftp_server_om_files_path[FILE_PATH_MAX_LEN];	/* OM config files path*/
UINT8	g_ftp_server_local_path[FILE_PATH_MAX_LEN];

/* Software Paths on the FTP server*/
UINT8	g_ftp_server_om_sw_path[FILE_PATH_MAX_LEN];	/* OM SW path*/
UINT8	g_ftp_server_mac_sw_path[FILE_PATH_MAX_LEN];	/* MAC SW path*/
UINT8	g_ftp_server_phy_sw_path[FILE_PATH_MAX_LEN];	/* PHY SW path*/
UINT8	g_ftp_server_fpga_fw_path[FILE_PATH_MAX_LEN];	/* FPGA firmware path*/
UINT8	g_ftp_server_rru_fw_path[FILE_PATH_MAX_LEN];	/* RRU firmware path*/
UINT8	g_ftp_server_rru_sw_path[FILE_PATH_MAX_LEN];	/* RRU software path*/
UINT8	g_om_config_file_path[FILE_PATH_MAX_LEN];
UINT8	g_phy_sw_path[FILE_PATH_MAX_LEN];
UINT8	g_phy_execufile_path[FILE_PATH_MAX_LEN];
UINT8	g_mac_sw_path[FILE_PATH_MAX_LEN];
UINT8 	g_mac_cells_path[FILE_PATH_MAX_LEN];
UINT8 	g_mac_default_path[FILE_PATH_MAX_LEN];
UINT8	g_mac_execufile_path[FILE_PATH_MAX_LEN];
UINT8	g_phy_un_socket_path[FILE_PATH_MAX_LEN];
UINT8	g_mac_un_socket_path[FILE_PATH_MAX_LEN];
UINT8 	g_fpga_sw_path[FILE_PATH_MAX_LEN];
UINT8 	g_rru_sw_path[FILE_PATH_MAX_LEN];

/* OMC information*/
UINT8	g_omc_ip[16];			/* OMC server IP*/
UINT32	g_snmp_port;			/* SNMP port*/
UINT32	g_snmp_trap_port;			/* SNMP trap port*/

/* OM information*/
UINT32 	g_ne_id;					/* Network Element ID, Ox*/	
UINT8 	g_ne_ip_root[16];			/* NE ip root*/
UINT8 	g_om_ip_netmask[16];			/*NE ip netmask*/

/* Version of softwares and firmwares*/

UINT8	g_om_sw_version[32];			/* OM SW version*/
UINT8	g_ne_cfg_file_version[32];		/* NE config file version*/
UINT8	g_ne_mac_sw_version[32];		/* NE mac software  version*/
UINT8	g_ne_phy_sw_version[32];		/* NE phy software  version*/
UINT8	g_ne_fpga1_fw_version[32];		/* NE FPGA1 firmware  version*/
UINT8	g_ne_fpga2_fw_version[32];		/* NE FPGA2 firmware  version*/
UINT8	g_ne_fpga3_fw_version[32];		/* NE FPGA3 firmware  version*/
UINT8	g_ne_fpga4_fw_version[32];		/* NE FPGA4 firmware  version*/
UINT8	g_ne_fpga5_fw_version[32];		/* NE FPGA5 firmware  version*/
UINT8	g_ne_rru_fw_version[40];		/* NE RRU firmware  version*/
UINT8	g_ne_rru_sw_version[32];		/* NE RRU software  version*/


/* MAX CELL Number */

UINT8 	max_cell_num;			/*cell number supported by a NE*/

/*dpdk port number supported by a NE, max_dpdk_num = max_cell_num/2 */
UINT8 	max_dpdk_num; 		

/* Msg ID in OM*/
MsgqType 	MSGQ_ID_RRUS_MGMT;
MsgqType	MSGQ_ID_RRUS_MGMT_INET_SUP;
MsgqType 	MSGQ_ID_MAIN_CTRL;
MsgqType 	MSGQ_ID_CFIG_MGMT;
MsgqType 	MSGQ_ID_CELL_MGMT;
MsgqType 	MSGQ_ID_BORD_MGMT;
MsgqType 	MSGQ_ID_INET_MGMT;
MsgqType 	MSGQ_ID_TIMER_MGMT;


/* Task defined in OM*/
TaskType THREAD_RRUS_MGMT;
TaskType THREAD_RRUS_MGMT_INET_SUP;
TaskType THREAD_MAIN_CTRL;
TaskType THREAD_CFIG_MGMT;
TaskType THREAD_CELL_MGMT;
TaskType THREAD_BORD_MGMT;
TaskType THREAD_INET_MGMT;
TaskType THREAD_TIMER;


/* Task flag defined in OM*/
UINT8 flag_thread_rrus_mgmt;
UINT8 flag_thread_rrus_mgmt_inet_sup;
UINT8 flag_thread_main_ctrl;
UINT8 flag_thread_cfig_mgmt;
UINT8 flag_thread_bord_mgmt;
UINT8 flag_thread_cell_mgmt;
UINT8 flag_thread_inet_mgmt;
UINT8 flag_thread_timer;

/* Network Element SYNC state*/
UINT8	g_ne_sync_state = NE_SYNC_OK;

/* OM State*/
OmState g_om_state; 

/* Phy protocol software state*/
UINT8	g_phy_state;
UINT32  phy_pid;

/* FPGA initial state*/
UINT8	g_fpga_state;

/*Dpdk port MAC Addr*/
UINT8	g_dpdk_mac_addr[MAX_DPDK_PORT_NUM][18];

UINT8   g_om_port_ip_addr[16];
UINT8   g_om_port_mac_addr[18];
UINT8   g_om_port_name[16];

UINT32 g_udp_sock_fd;

/* Declarations ---------------------------------------------------------- */
extern INT32 rrus_mgmt_clean_up();

extern INT32 cfig_mgmt_clean_up();

extern INT32 cell_mgmt_clean_up();

extern INT32 bord_mgmt_clean_up();
/* Functions --------------------------------------------------------------- */







void sig_handler(int signo)
{

	if (signo == SIGINT){
		mq_close(MSGQ_ID_MAIN_CTRL);
		mq_close(MSGQ_ID_RRUS_MGMT);
		mq_close(MSGQ_ID_RRUS_MGMT_INET_SUP);
		mq_close(MSGQ_ID_CFIG_MGMT);
		mq_close(MSGQ_ID_CELL_MGMT);
		mq_close(MSGQ_ID_BORD_MGMT);
		mq_close(MSGQ_ID_INET_MGMT);

		if(rrus_mgmt_clean_up() == -1)
		{
			log_msg(LOG_ERR,MAIN,"clean failed of RRU MGMT.\n");
		}
		if(cfig_mgmt_clean_up() == -1)
		{
			log_msg(LOG_ERR,MAIN,"clean failed of CFIG MGMT.\n");
		}
		if(cell_mgmt_clean_up() == -1)
		{
			log_msg(LOG_ERR,MAIN,"clean failed of CFIG MGMT.\n");
		}
		if(bord_mgmt_clean_up() == -1)
		{
			log_msg(LOG_ERR,MAIN,"clean failed of CFIG MGMT.\n");
		}


		log_msg(LOG_ERR,MAIN,"capture SIGINT.\n");
	}
	exit(-1);

}


INT32 main(int argc, char *argv[])
{
	INT32 		rc;
	INT32		rslt = 0;
    /*define var*/
    INT32 		opt = 0;				//main para opt(eg:-h -t)
    UINT8 		p_opt_string[] = "s:f:l:";	//opt value string ("s:f:l:")

    /*define the log model parameters*/
    UINT8 		log_style = WRITE2STDOUT;
    //UINT8 		log_style = WRITE2FILE;

    //UINT8 		log_format = LOG_SIMPLE;
    UINT8 		log_format =LOG_COMPLETE;
    UINT8 		log_level = LOG_INFO;

    /*--------test para-------------*/
   // UINT32 fd;


	/*get the input options.*/
	while ((opt = getopt(argc, argv, p_opt_string)) != -1)		/*parse the main arguments to set the log parameters*/
	{ 
		switch (opt)
		{
			case 's':
				if((atoi(optarg) <= WRITE2FILE) && (atoi(optarg) >= WRITE2STDOUT))
				{
					log_style = atoi(optarg);
				}
				else
				{
					printf("log_style option is extend and will use the default option.\n");
				}
				break;
			case 'f':
				if((atoi(optarg) <= LOG_COMPLETE) && (atoi(optarg) >= LOG_SIMPLE))
				{
					log_format = atoi(optarg);
				}
				else
				{
					printf("log_format option is extend and will use the default option.\n");
				}
				break;
			case 'l':
				if((atoi(optarg) <= LOG_INFO ) && (atoi(optarg) >= LOG_ERR))
				{
					log_level = atoi(optarg);
				}
				else
				{
					printf("log_level option is extend and will use the default option.\n");
				}
				break;
			default :
				printf("input option of -%c was invalid.\n", opt);
				break;
		}
	}
	if(signal(SIGINT,sig_handler)==SIG_ERR)
	{
		log_msg(LOG_ERR,MAIN,"register signal error:%s\n",strerror(errno));
		exit(-1);
	}


	/*open the log file*/
	rc = log_init(log_style, log_format, log_level);
	if (rc != 0)
	{
		printf("MAIN_CTRL, create log error.Exiting......\n");

		return FUNC_ERR;
	}


	/*open the message queue for the MAIN CTRL module.*/

	MSGQ_ID_MAIN_CTRL = open_msgq(MAIN_CTRL_MSGQ_NAME, MAX_MSGQ_MSG_NUM, MAX_MSGQ_MSG_LEN);
	if (-1 == MSGQ_ID_MAIN_CTRL)
	{
		log_msg(LOG_ERR, MAIN, "MAIN CTRL open msgq error.\n");
		log_cleanup();

		return FUNC_ERR;
	}
	else
	{
	   log_msg(LOG_INFO,MAIN,"MAIN CTRL open msgq success,msgq=%d\n",MSGQ_ID_MAIN_CTRL);
	}


	/*--------parse OM control local config xml--------------*/
	
	rslt = parse_om_config_xml();	
	if (!rslt)
	{
		log_msg(LOG_INFO, MAIN, "Parse local config xml OK\n");
	}
	else
	{
		log_msg(LOG_ERR, MAIN, "Parse local config xml ERR\n");
	}
		

	/* ----------Update local config files and softwares------- */
	rslt = update_om_by_ftp();	
	if (!rslt)
	{
		log_msg(LOG_INFO, MAIN, "Update_om_by_ftp OK\n");
	}
	else
	{
		log_msg(LOG_ERR, MAIN, "Update_om_by_ftp ERR\n");
	}


	/* -------------Init all OM modules-----------*/

	/*----------------------MODULE_TIMER-----------------*/
	if (timer_module_init() == FUNC_ERR)
	{
		log_msg(LOG_ERR, MAIN, "init timer module fail\n");
	}
	else
	{
		log_msg(LOG_INFO, MAIN, "init timer module ok\n");
	}
	
	/*----------------------init MODULE_INET_MGMT-----------------*/

	if (inet_mgmt_module_init() != FUNC_OK)
	{
		log_msg(LOG_ERR,MAIN,"init inet mgmt fail\n");
	}
	else
	{
		log_msg(LOG_INFO,MAIN,"init inet mgmt Ok\n");
	}


   /*-----------------init RRU_MGMT---------------------------*/
	rslt = rrus_mgmt_module_init();	
	if (!rslt) 
	{
		log_msg(LOG_INFO, MAIN, "init RRU_MGMT OK\n");
	} 
	else 
	{
		log_msg(LOG_ERR, MAIN, "init RRU_MGMT ERR\n");
	}

    
    /*--------Init CELL_MGMT MODULE--------------*/
    
    rslt = cell_mgmt_module_init();
	if (!rslt) {
		log_msg(LOG_SUMMARY, MAIN, "Init CELL_MGMT OK\n");
	} else {
		log_msg(LOG_ERR, MAIN, "Init CELL_MGMT ERR\n");
	}

	/*----------------------MODULE_CFIG_MGMT-----------------*/
	if(cfig_mgmt_module_init()!=0)
	{
		log_msg(LOG_ERR,MAIN,"init cfig mgmt fail\n");
	}
	else
	{
		log_msg(LOG_INFO,MAIN,"init cfig mgmt Ok\n");
	}

	
	rslt=bord_mgmt_init();
	if(rslt==FUNC_ERR)
	{
		log_msg(LOG_ERR, MAIN, " MODULE_BORD_MGMT: bord_mgmt_init() error.\n");
		return FUNC_ERR;
	}
	

	//test_module_init();
	/* ----------Start OM configuration recovering-------------*/

	cfig_mgmt_cfig_recover();
	/* Change om state to SYS_RUNNING */
	g_om_state = SYS_RUNNING;


	/* -----------------Start PHY -----------------------------*/
	start_phy();

	char buf[MAX_MSGQ_MSG_LEN];
	int ret; // receive_msgq does not return msg len !
	while(1)
	{
		printf("sys_running.\n");

		memset(buf, 0, MAX_MSGQ_MSG_LEN);
		if ((ret = receive_msgq(MSGQ_ID_MAIN_CTRL, buf, MAX_MSGQ_MSG_LEN, WAIT_FOREVER)) != FUNC_ERR)
		{
			CommMsgHead	*tmp;
			tmp = (CommMsgHead*)buf;
			switch (tmp->msg_type)
			{
				case MSG_TYPE_PHY_MAIN_CTRL:
				{
					if (handle_module_phy_msg((void*)buf, sizeof(PHY_MAIN_CTRL_PHY_LOAD_RSP)) == FUNC_ERR)
					{
						log_msg(LOG_ERR, MAIN_CTRL, "Function handle_module_phy_msg return error.\n");
					}
					break;
				}

				case MODULE_BORD_MGMT:
				{
					if (handle_module_bord_mgmt_msg((void*)buf, ret) == FUNC_ERR)
					{
						log_msg(LOG_ERR, MAIN_CTRL, "Function handle_module_bord_mgmt return error.\n");
					}
					break;
				}

				default:
					log_msg(LOG_ERR, MAIN_CTRL, "main_ctrl_module receive error message.\n");
					break;
			}
		}
		
		sleep(2);
	}
	
	return 0;
}
