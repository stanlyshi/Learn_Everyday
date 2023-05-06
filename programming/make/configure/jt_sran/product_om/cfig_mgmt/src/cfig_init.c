/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:33 CST. */
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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:04 CST. */
/*
 * cfig_init.c
 *
 *  Created on: Sep 3, 2018
 *      Author: root
 */

#include <signal.h>
#include <unistd.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "sbs_timer.h"
#include "sbs_type.h"
#include "sbs_msgq.h"
#include "sbs_log.h"
#include "sbs_task.h"
#include "om_common_h.h"
#include "cfig_common.h"
#include "cm_api.h"
#include "sbsconfig_common.h"
#include "rruInitInfo.h"
#include "cpriConfigInfo.h"

extern void
init_protocolCommConfigInfo (void);
extern void
init_vcmCommConfigInfo(void);
extern void
init_phyCommConfigInfo_scalar(void);
extern void
init_cpuManageMsg (void);
extern void
init_fpgaAssetMsg(void);
extern void
init_fpgaHealthMsg(void);
extern void
init_fpgaPowerMsg(void);
extern void
init_timerCfgMsg(void);
//#include "cm_api.h"
//#include "sbs_common.h"

static int keep_running;
char *app_name = "sbsSubAgent";

//RETSIGTYPE
//stop_server (int a)
//{
//  keep_running = 0;
//}

INT32 cfig_mgmt_clean_up()
{
	 keep_running = 0;

	return FUNC_OK;
}


INT32 cfig_msgq_recv_fn(void){
	char buffer[MAX_MSGQ_MSG_LEN] = {0};
    CommMsgHead *pstMsg = NULL;
    CONFIG_VCMAddState_t *pstVcmAddState = NULL;
    CONFIG_VCMDelState_t *pstVcmDelState = NULL;
    CONFIG_NESetup_t *pstNeSetup = NULL;
	CONFIG_TimeSyncState_t *pstTimeSync = NULL;
	INT32 len_receive_msgq = 0;
	long lRet = 0;
	while(1)
	{
			len_receive_msgq = receive_msgq(MSGQ_ID_CFIG_MGMT, buffer, sizeof(buffer), WAIT_FOREVER);
			if(len_receive_msgq < 0){
				log_msg(LOG_ERR,CFIG_MGMT,"receive messages from message queue error in cfig_msgq_recv_fn\n");
				return FUNC_ERR;
			}

			if(len_receive_msgq < MSG_HEAD_LEN){
				log_msg(LOG_ERR, CFIG_MGMT, "cfig_msgq_recv_fn message's length incorrect!\n");
				return FUNC_ERR;
			}
		    pstMsg = (CommMsgHead *)buffer;
			switch(*(UINT16*)buffer){

				case MODULE_CFIG_MGMT:

					break;
				case MODULE_MAIN_CTRL:
					break;
				case MODULE_BORD_MGMT:
					switch(pstMsg->msg_code){
						case CONFIG_CONTRL_REPLY_NESETUP:
							pstNeSetup = (CONFIG_NESetup_t*)buffer;
							//send trap
							send_omCtlNEStatus_trap(pstNeSetup);
							break;
						case CONFIG_CONTRL_REPLY_TIMESYNC:
							pstTimeSync = (CONFIG_TimeSyncState_t*)buffer;
							//send trap
							send_omCtlTimeSync_trap(pstTimeSync);
						break;
					}
					break;
				case MODULE_CELL_MGMT:
					switch(pstMsg->msg_code){
					    case CONFIG_VCM_REPLY_ADDCELL:
			            	pstVcmAddState = (CONFIG_VCMAddState_t*)buffer;
			            	//send trap
		                    lRet = send_omVcmAddResponse_trap(pstVcmAddState);
						    if ( lRet == ERROR)
						    {
						       continue;
						    }
					    break;
						case CONFIG_VCM_REPLY_DELCELL:
			            	pstVcmDelState = (CONFIG_VCMAddState_t*)buffer;
			            	//send trap
						    lRet = send_omVcmAddResponse_trap(pstVcmDelState);
						    if ( lRet == ERROR)
						    {
						       continue;
						    }
						break;
					}
					break;
				case MODULE_RRUS_MGMT:
					if(handle_fn_rru_trap_msg((void*)buffer) != 0){
						log_msg(LOG_INFO,CFIG_MGMT,"transmit status response handle fn error in cfig_msgq_recv_fn()\n");
					}
					break;

				case MODULE_FTPS_MGMT:
					break;
				case MODULE_ALAR_MGMT:
					break;
				case MODULE_INET_MGMT:
					break;

				default:
						log_msg(LOG_ERR,CFIG_MGMT, "wrong msg_type in cfig_msgq_recv_fn()!\n");
			}
	}


	return FUNC_OK;
}

INT32 cfig_snmp_agent_fn(void){

	  /* change this if you want to be a SNMP master agent */
	  int agentx_subagent = 1;
	  /* change this if you want to run in the background */
	  int background = 0;
	  /* change this if you want to use syslog */
	  int syslog_enable = 0;
	  int snmp_pdu_max_size = -1;
	  struct sigaction act;
	  if (syslog_enable)
		{
		  snmp_enable_calllog ();
		}
	  else
		{
		  snmp_enable_stderrlog ();
		}
	  /* Being an agentX subAgent */
	  if (agentx_subagent)
		{
		  /* make us a agentX client. */
		  netsnmp_ds_set_boolean (NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_ROLE,1);
		}

	  /* run in background, if requested */
	  if (background && netsnmp_daemonize (1, !syslog_enable))
		exit (1);

	  /* Initialize TCP-IP, if NEEDED */
	  SOCK_STARTUP;

	  /* Master agent address */
	  //netsnmp_ds_set_string (NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_X_SOCKET,
	  //			 "localhost:705");/*localhost:19003 or 705*/

	  /* Initialize the agent library */
	  init_agent (app_name);

	  /* Initialize our mib code here */
	  init_protocolCommConfigInfo ();
	  init_vcmCommConfigInfo ();
	  init_phyCommConfigInfo_scalar();
	  init_cpuManageMsg();
	  init_fpgaAssetMsg();
	  init_fpgaHealthMsg();
	  init_fpgaPowerMsg();
	  init_timerCfgMsg();
	  init_protocolToPhyConfigInfo();
	  init_rruInitInfo();
	  init_cpriConfigInfo();

	#if 0
	  /*VACM/USM access control  */
	  if (!agentx_subagent)
		{
		  (void)init_vacm_vars();
		  (void)init_usmUser();
		}
	#endif

	  /* gsc_sub_test will be used to read gsc_sub.conf files. */
	  init_snmp (app_name);

	  /* If we're going to be a snmp master agent, initial the ports */
	  if (!agentx_subagent)
		/* open the port to listen on (defaults to udp:161) */
		init_master_agent ();

	  /* In case we recevie a request to stop (kill -TERM or kill -INT) */
	  keep_running = TRUE;
//	  signal (SIGTERM, stop_server);
//	  signal (SIGINT, stop_server);
	  act.sa_handler = SIG_IGN;
	  sigaction (SIGPIPE, &act, NULL);

	  snmp_log (LOG_INFO,
			"[STAT] sbsSubAgent Initialize [\033[49;32mSUCESS\033[0m]\n");
	#if 0
	  snmp_log (LOG_INFO, "\033[49;30m30\n\033[0m");
	  snmp_log (LOG_INFO, "\033[49;31m31\n\033[0m");
	  snmp_log (LOG_INFO, "\033[49;32m32\n\033[0m");
	  snmp_log (LOG_INFO, "\033[49;33m33\n\033[0m");
	  snmp_log (LOG_INFO, "\033[49;34m34\n\033[0m");
	  snmp_log (LOG_INFO, "\033[49;35m35\n\033[0m");
	  snmp_log (LOG_INFO, "\033[49;36m36\n\033[0m");
	  snmp_log (LOG_INFO, "\033[49;37m37\n\033[0m");
	  snmp_log (LOG_INFO, "\033[49;38m38\n\033[0m");
	#endif
	  /* call other functions here */
	  /***************************************************/
	  init_sbs_mibs();
	  //init_grm_subagent ();
	  /***************************************************/

	  /* Main loop for potential snmp quest*/
	  while (keep_running)
		{
		  /* if you use select(), see snmp_select_info() in snmp_api(3) */
		  /*	 --- OR ---  */
		  agent_check_and_process (1); /* 0 == don't block */
		  usleep (20000);
		}

//	  snmp_log (LOG_INFO,
//			"\n[STAT] \033[49;31msbs subagent is shutting down...\033[0m\n");
	  log_msg(LOG_INFO, CFIG_MGMT,"subagent is shuting down...\n");
	  //printf ("\n[STAT] \033[49;31msbs subagent is shutting down...\033[0m\n");
	  /* at shutdown time */
	  snmp_shutdown (app_name);

	  SOCK_CLEANUP;
	  log_msg(LOG_INFO, CFIG_MGMT,"sbs subagent off\n");
//	  snmp_log (LOG_INFO, "[STAT] \033[49;31mSbsSubAgent OFF\033[0m\n");


	return FUNC_OK;
}
INT32 cfig_mgmt_module_init(){
	TaskType cfig_msgq_task_id = 0;
	TaskType cfig_snmpagent_task_id = 0;
	MSGQ_ID_CFIG_MGMT = open_msgq(CFIG_MGMT_MSGQ_NAME, MAX_MSGQ_MSG_NUM, MAX_MSGQ_MSG_LEN);
	if (-1 == MSGQ_ID_CFIG_MGMT)
	{
		log_msg(LOG_ERR, CFIG_MGMT, "cfig modal open msgq error.\n");
		log_cleanup();

		return FUNC_ERR;
	}

	cfig_msgq_task_id = create_pthread(1,SCHED_RR,TASK_DETACH,cfig_msgq_recv_fn, (void*)NULL);
	if(-1 == cfig_msgq_task_id){
		log_msg(LOG_ERR,CFIG_MGMT, "create cfig msgq thread error\n");
		return FUNC_ERR;
	}

	cfig_snmpagent_task_id = create_pthread(1,SCHED_RR,TASK_DETACH,cfig_snmp_agent_fn, (void*)NULL);
	if(-1 == cfig_snmpagent_task_id){
		log_msg(LOG_ERR,CFIG_MGMT, "create cfig snmpagent thread error\n");
		return FUNC_ERR;
	}

	return FUNC_OK;
}
