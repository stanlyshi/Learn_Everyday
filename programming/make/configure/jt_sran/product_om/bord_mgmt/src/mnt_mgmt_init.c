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
/*
 * mnt_mgmt_init.c
 *
 *  Created on: Aug 21, 2018
 *      Author: root
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "sbs_log.h"
#include "sbs_msgq.h"
#include "sbs_task.h"
#include "sbs_timer.h"
#include "inet_mgmt.h"
#include "om_common_h.h"
#include "mnt_mgmt_api.h"
#include "mnt_mgmt_init.h"
#include "mnt_mgmt_msg_handle.h"
#include "msg_receive_fn.h"
#include "fpga_msg_receive_fn.h"//test

UINT8 om_ip[16];
UINT32 raw_sock_fd;

UINT8 cpu_data_mac1[6];
UINT8 cpu_data_mac2[6];
UINT8 cpu_sre_mac[6];
UINT8 cpu_om_mac[6];

pthread_rwlock_t brd_mnt_rwlock;

Sesson sesson;
struct sockaddr clientaddr;
SaveFpgaServerAssetInfo savefpgaserverassetinfo;
SaveFpgaAssetInfo savefpgaassetinfo[FpgaNum];
SaveCtrlFpgaMac savectrlfpgamac;
SaveDataFpgaMac savedatafpgamac[FpgaNum-1];
SaveFpgaAddr savefpgaaddr;
SaveFpgaServerHealThresInfo   savefpgaserverhealthresinfo;
SaveFpgaHealThresInfo     savefpgahealthresinfo[FpgaNum];
SaveFpgaServerHealInfo  savefpgaserverhealinfo;
SaveFpgaHealInfo   savefpgahealinfo[FpgaNum];
SaveFpgaServerPowerState savefpgaserverpowerstate;
SaveFpgaPowerState	savefpgapowerstate[FpgaNum];
SaveTimeSyncState	savetimesyncstate ;
SaveFpgaId savefpgaid[FpgaNum];
SaveFpgaServerId savefpgaserverid;
SaveCpuId savecpuid[CpuNum];
SaveCpuServerId savecpuserverid;
SaveCpuAddr savecpuaddr;
SaveCpuMac  savecpumac;
SaveCpuAssetInfo  savecpuassetinfo[CpuNum];
SaveBmcAssetInfo  savebmcassetinfo;
SaveCpuServerHealInfo savecpuserverhealinfo;
SaveCpuHealInfo  savecpuhealinfo[CpuNum];
SaveCpuServerComputeSourceInfo savecpuservercomputesourceinfo;
/**********************************************************************************************

 Description :initialization board monitor management module
 input:  NULL
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 **********************************************************************************************/
INT32 bord_mgmt_init()
{
	TaskType flag_thread_bord_mgmt = 0;
#ifdef Test
	memcpy(g_ne_ip_root,"10.21.0.0",16);
	g_ne_id = 0x01010000;
#endif
//initialization massage queue
	MSGQ_ID_BORD_MGMT = open_msgq(BORD_MGMT_MSGQ_NAME,MAX_MSGQ_MSG_NUM,MAX_MSGQ_MSG_LEN);
	if (-1 == MSGQ_ID_BORD_MGMT)
	{
		log_msg(LOG_ERR, BORD_MGMT, "create massage queue for board management module failed!\n");
		return FUNC_ERR;
	}
	printf("MSGQ_ID_BORD_MGMT = %d\n", MSGQ_ID_BORD_MGMT);

	flag_thread_bord_mgmt = create_pthread(MSG_RECEIVE_PRIOR, SCHED_RR, TASK_JOIN, bord_msg_receive_fn, NULL);
	if (-1 == flag_thread_bord_mgmt)
	{
		log_msg(LOG_ERR, BORD_MGMT, "create  board management thread failed!\n");
		close_msgq(BORD_MGMT_MSGQ_NAME,MSGQ_ID_BORD_MGMT);
		return FUNC_ERR;
	}
//register UDP socket and raw socket
	g_udp_sock_fd = socket_udp_register(MSGQ_ID_BORD_MGMT,NULL,0,LOCAL_UDP_PORT);
	if(g_udp_sock_fd < 0)
	{
		 log_msg(LOG_ERR, BORD_MGMT, "register UDP socket error!\n");
	}

	raw_sock_fd = raw_socket_register(MSGQ_ID_BORD_MGMT,NULL,0,PROTOCOL_TYPE);
    if(raw_sock_fd < 0)
    {
    	log_msg(LOG_ERR, BORD_MGMT, "register RAW socket error!\n");
    }

//	memset(cpu_sre_mac,0,6);
//	memcpy(cpu_sre_mac,cpu_om_mac,6);
//initialization struct for save data
	memset(savefpgaassetinfo,0,sizeof(SaveFpgaAssetInfo)*FpgaNum);
	memset(savefpgahealthresinfo,0,sizeof(SaveFpgaHealThresInfo)*FpgaNum);
	memset(savefpgahealinfo,0,sizeof(SaveFpgaHealInfo)*FpgaNum);
	memset(&savefpgaserverassetinfo,0,sizeof(savefpgaserverassetinfo));
    memset(&savefpgaserverhealthresinfo,0,sizeof(SaveFpgaServerHealThresInfo));
    memset(&savefpgaserverhealinfo,0,sizeof(SaveFpgaServerHealInfo));
    memset(&savetimesyncstate,0,sizeof(SaveTimeSyncState));
	memset(&savecpuservercomputesourceinfo,0,sizeof(SaveCpuServerComputeSourceInfo));//cpu
	memset(&savecpuhealinfo,0,sizeof(savecpuhealinfo));
	memset(&savecpuassetinfo,0,sizeof(savecpuassetinfo));
	memset(&savebmcassetinfo,0,sizeof(savebmcassetinfo));
	memset(&savecpuserverhealinfo,0,sizeof(savecpuserverhealinfo));
	memset(&savefpgaid,0,sizeof(savefpgaid)*FpgaNum);
	memset(&savefpgaserverid,0,sizeof(savefpgaserverid));
	memset(&savecpuid,0,sizeof(savecpuid)*CpuNum);
	memset(&savecpuserverid,0,sizeof(savecpuserverid));
	memset(&savefpgaaddr,0,sizeof(savefpgaaddr));
	memset(&savecpuaddr,0,sizeof(savecpuaddr));
	memset(&savecpumac,0,sizeof(savecpumac));

	pthread_rwlock_init(&brd_mnt_rwlock,NULL);//initialization read && write lock

	heart_beat_record_init();//heart beat record initialization
	device_id_init();//device ID initialization



#ifdef Test
	//test:create a thread for receive FPGA massage
	thread_id_recv_fpga_msg = create_pthread(MSG_RECEIVE_PRIOR, SCHED_RR, TASK_JOIN,fpga_msg_receive_fn,NULL);
	if (-1 == thread_id_recv_fpga_msg)
	{
		log_msg(LOG_ERR, BORD_MGMT, "create FPGA massage receive thread failed!\n");
		close_msgq(BORD_MGMT_MSGQ_NAME,MSGQ_ID_BORD_MGMT);
		return FUNC_ERR;
	}
#endif
	if(get_cpu_server_asset_info() == FUNC_ERR) //get CPU server asset information
	{
		log_msg(LOG_ERR, BORD_MGMT, "get CPU asset info failed!\n");
	}
	conver_macaddr_in_string_to_byte();

	return FUNC_OK;
}

/**********************************************************************************************

 Description :clean function, unregister socket when kill the thread
 input:  NULL
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 **********************************************************************************************/
INT32 bord_mgmt_clean_up()
{
	if(socket_unregister(g_udp_sock_fd) == -1)
	{
        log_msg(LOG_ERR, BORD_MGMT, "UDP socket unregister error. \n");
		return FUNC_ERR;
	}
	if(socket_unregister(raw_sock_fd) == -1)
	{
		log_msg(LOG_ERR, BORD_MGMT, "RAW socket unregister error. \n");
		return FUNC_ERR;
	}
	log_msg(LOG_INFO, BORD_MGMT, "RAW && UDP socket unregister success. \n");
	return FUNC_OK;
}

/**********************************************************************************************

 Description :heart beat record initialization
 input:  NULL
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 **********************************************************************************************/
INT32 heart_beat_record_init()
{
	int i,j;
	for(i = 0;i < FpgaNum;i++)
	{
		for(j = 0;j < PortNum;j++)
		{
			fpgaheartbeatrcd[i][j].heartb_count = 0;
		    fpgaheartbeatrcd[i][j].fpga_port_state = 0;
		}
	}
	return FUNC_OK;
}

/**********************************************************************************************

 Description :device ID initialization
 input:  NULL
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 **********************************************************************************************/
INT32 device_id_init()
{
	UINT32 index;
	savefpgaserverid.fpga_server_id =  ((g_ne_id << 4) & 0xffff0000) | 0x00001000;
	for(index = 1;index < (FpgaNum+1);index++)
	{
		savefpgaid[index-1].fpga_id = ((g_ne_id << 4) & 0xffff0000) | (0x00001000 | (index << 8));
	}
	savecpuserverid.cpu_server_id = ((g_ne_id << 4) & 0xffff0000) | 0x00000000;
	savecpuserverid.bmc_id = ((g_ne_id << 4) & 0xffff0000) | 0x00000100;
	for(index = 1;index < (CpuNum+1);index++)
	{
		savecpuid[index-1].cpu_id = ((g_ne_id << 4) & 0xffff0000) | (index << 8);
	}
	return FUNC_OK;
}

INT32 conver_macaddr_in_string_to_byte()
{
	memset(cpu_data_mac2,0,6);
	memset(cpu_data_mac1,0,6);
	memset(cpu_om_mac,0,6);
	sscanf(g_dpdk_mac_addr[1],"%02x:%02x:%02x:%02x:%02x:%02x",cpu_data_mac2,cpu_data_mac2+1,cpu_data_mac2+2,cpu_data_mac2+3,cpu_data_mac2+4,cpu_data_mac2+5);
	sscanf(g_dpdk_mac_addr[0],"%02x:%02x:%02x:%02x:%02x:%02x",cpu_data_mac1,cpu_data_mac1+1,cpu_data_mac1+2,cpu_data_mac1+3,cpu_data_mac1+4,cpu_data_mac1+5);
	sscanf(g_om_port_mac_addr,"%02x:%02x:%02x:%02x:%02x:%02x",cpu_om_mac,cpu_om_mac+1,cpu_om_mac+2,cpu_om_mac+3,cpu_om_mac+4,cpu_om_mac+5);
	printf("cpu_om_mac = %02x:%02x:%02x:%02x:%02x:%02x \n",cpu_om_mac[0],cpu_om_mac[1],cpu_om_mac[2],cpu_om_mac[3],cpu_om_mac[4],cpu_om_mac[5]);
	printf("cpu_data_mac1 = %02x:%02x:%02x:%02x:%02x:%02x \n",cpu_data_mac1[0],cpu_data_mac1[1],cpu_data_mac1[2],cpu_data_mac1[3],cpu_data_mac1[4],cpu_data_mac1[5]);
	printf("cpu_data_mac2 = %02x:%02x:%02x:%02x:%02x:%02x \n",cpu_data_mac2[0],cpu_data_mac2[1],cpu_data_mac2[2],cpu_data_mac2[3],cpu_data_mac2[4],cpu_data_mac2[5]);
	return FUNC_OK;
}
