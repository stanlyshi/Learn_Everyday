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
 * mnt_mgmt_msg_handle.c
 *
 *  Created on: Aug 21, 2018
 *      Author: root
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "sbs_timer.h"
#include "sbs_msgq.h"
#include "sbs_task.h"
#include "sbs_log.h"
#include "mnt_mgmt_init.h"
#include "mnt_mgmt_msg_handle.h"
#include "mnt_mgmt_api.h"
#include "om_common_h.h"
#include "om_main_ctrl.h"
#include "fpga_msg_receive_fn.h"
#include "msg_receive_fn.h"
#include "inet_mgmt.h"

MsgHead msghead;
FrameInfoReportCfgInfo frameinforeportcfginfo;
FrameInfoReportCfgRsp frameinforeportcfgrsp;
FpgaStartStateReport fpgastartstatereport;
ResCtrlFpga resctrlfpga;
FpgaChanMoniTrig fpgachanmonitrig;
TempThresCfgReq tempthrescfgreq;
VolFanThresCfgReq fanthrescfgreq;
VolFanThresCfgReq volthrescfgreq;
LocCfgReq loccfgreq;
CloSouCfgReq closourcfgreq;
FpgaServerPowerCtlReq fpgaserverpowerctlreq;
FpgaChipPowerCtlReq fpgachippowerctlreq;

FpgaHeartBeatRcd fpgaheartbeatrcd[FpgaNum][PortNum];
MsgBordSyn msgbordsyn;
/***********************************************************************************************

 Description :send query FPGA healthy information to MODULE_INET_MGMT
 input : NULL
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ***********************************************************************************************/
INT32 query_fpga_heal_info()
{
	memset(&msghead,0,sizeof(MsgHead));
	msghead.msg_type = htons(BORD_MGMT_CTRL_FPGA);
	msghead.msg_code = htons(BORD_MGMT_OM_RX_HEAL_INFO_QUERY_REQ);
	msghead.fpga_id = htonl(savefpgaid[0].fpga_id);
	msghead.msg_len = htonl(sizeof(MsgHead));

   /// use api send to fpga
#ifdef Test
	if(sendto(sesson.udp_fd,(char *)&msghead,sizeof(MsgHead),0,(struct sockaddr *)&sesson.udp_clientaddr,sizeof(sesson.udp_clientaddr)) < 0)
	{
		printf("sendto() error in function query_fpga_heal_info !\n");
    }
#else
	memcpy(&clientaddr, (struct sockaddr*)&sesson.udp_clientaddr, sizeof(sesson.udp_clientaddr));
	if(send_msg_to_inet(MODULE_BORD_MGMT,0,g_udp_sock_fd,0,&clientaddr,(char *)&msghead,sizeof(MsgHead))<0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send query FPGA health info request error!\n");
		return FUNC_ERR;
	}
#endif
	return FUNC_OK;
}

/************************************************************************************************

 Description :handle function of configuring frame report to data FPGA
 input : NULL
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 *************************************************************************************************/
INT32 handle_fn_frame_info_report_cfg_req()
{
	memset(cpu_sre_mac,0,6);
	memcpy(cpu_sre_mac,cpu_data_mac1,6);
	memset(&frameinforeportcfginfo,0,sizeof(FrameInfoReportCfgInfo));
	memcpy(frameinforeportcfginfo.mac_head.des_mac, savedatafpgamac[3].fpga_mac6, 6);
	memcpy(frameinforeportcfginfo.mac_head.sre_mac, cpu_sre_mac, 6);
	frameinforeportcfginfo.mac_head.pro_type = htons(PROTOCOL_TYPE);
	frameinforeportcfginfo.msg_head.msg_type = htons(MSG_TYPE_DATA_FPGA_BORD_MGMT);
	frameinforeportcfginfo.msg_head.msg_code = htons(BORD_MGMT_DATA_FPGA_FRNUM_REQ);
	frameinforeportcfginfo.msg_head.fpga_id = htonl(savefpgaid[1].fpga_id);
	frameinforeportcfginfo.msg_head.msg_len = htonl(sizeof(FrameInfoReportCfgInfo));
	memcpy(frameinforeportcfginfo.des_mac, cpu_data_mac1, 18);

	memcpy(sesson.raw_serveraddr.sll_addr,savedatafpgamac[3].fpga_mac6,6);
	sesson.raw_serveraddr.sll_halen = 6;
	memcpy(&clientaddr, (struct sockaddr*)&sesson.raw_serveraddr, sizeof(sesson.raw_serveraddr));

	printf("sizeof(FrameInfoReportCfgInfo) = %d.\n", sizeof(FrameInfoReportCfgInfo));
	if(send_msg_to_inet(MODULE_BORD_MGMT,0,raw_sock_fd,0,&clientaddr,(char *)&frameinforeportcfginfo, sizeof(FrameInfoReportCfgInfo)) < 0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send the massage of frame information report configuration to data FPGA failed!\n");
		return FUNC_ERR;
	}
	else
		log_msg(LOG_INFO, BORD_MGMT, "send the massage of frame information report configuration to data FPGA success.\n");
	return FUNC_OK;
}

/************************************************************************************************

 Description :handle function of frame report configuration respond from data FPGA
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 *************************************************************************************************/
INT32 handle_fn_frame_info_report_cfg_rsp(INT8 *buf)
{
	FrameInfoReportCfgRsp *pmsg_frame_info_report_cfg_rsp = (FrameInfoReportCfgRsp*)buf;
	if(FUNC_OK == pmsg_frame_info_report_cfg_rsp->result)
	{
		log_msg(LOG_INFO, BORD_MGMT, "frame information report configuration success!\n");
		fpgastartstatereport.fpga_start_state = FPGA_OK;
	}
	else
	{
		log_msg(LOG_ERR, BORD_MGMT, "frame information report configuration failed!\n");
		fpgastartstatereport.fpga_start_state = FPGA_ERR;
	}

	fpgastartstatereport.msg_head.msg_type = MODULE_BORD_MGMT;
	fpgastartstatereport.msg_head.msg_code = FPGA_START_STATE_REPORT;
	fpgastartstatereport.msg_head.msg_len = sizeof(fpgastartstatereport);

	if(send_msg_to_om(MODULE_MAIN_CTRL,(UINT8*)&fpgastartstatereport,sizeof(fpgastartstatereport)) < 0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "FPGA start state send to MAIN_CTRL failed.\n");
		return FUNC_ERR;
	}
	log_msg(LOG_INFO, BORD_MGMT, "FPGA start state send to MAIN_CTRL success.\n");
	return FUNC_OK;
}
/************************************************************************************************

 Description :handle MAC broadcast message from control FPGA
 1.save FPGA asset information
 2.allocation IP for control FPGA
 3.update fpga1/2/3/4/5 firmware
 4.trigger FPGA send heart beat massage
 5.send health threshold query request
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 *************************************************************************************************/

INT32 handle_fn_fpga_broadcast_info_req(INT8 *buf)
{
	log_msg(LOG_INFO, BORD_MGMT,"FPGA broadcast massage require\n");
	UINT32 index;
	FpgaServerAssetInfo *pmsg_fpga_server_asset = (FpgaServerAssetInfo*)buf;
	CtrlFpgaAssetInfo *pmsg_ctrlfpga_asset = (CtrlFpgaAssetInfo*)(buf + MsgHeadBufLen + MacHeadBufLen + FpgaServerInfoBufLen);
	DataFpgaAssetInfo *pmsg_datafpga_asset = (DataFpgaAssetInfo*)(buf + MsgHeadBufLen + MacHeadBufLen + FpgaServerInfoBufLen + CtrlFpgaInfoBufLen);

	savefpgaserverassetinfo.product_type = ntohs(pmsg_fpga_server_asset->product_type);
	savefpgaserverassetinfo.board_type = ntohs(pmsg_fpga_server_asset->board_type);
	memcpy(savefpgaserverassetinfo.fpga_server_name,pmsg_fpga_server_asset->fpga_server_name,16);
	memcpy(savefpgaserverassetinfo.fpga_server_sn,pmsg_fpga_server_asset->fpga_server_sn,32);
	memcpy(savefpgaserverassetinfo.fpga_server_version,pmsg_fpga_server_asset->fpga_server_version,16);
	memcpy(savefpgaserverassetinfo.fpga_server_manuf_data,pmsg_fpga_server_asset->fpga_server_manuf_data,12);

	savefpgaassetinfo[0].fpga_slot = pmsg_ctrlfpga_asset->fpga_slot;
	memcpy(savefpgaassetinfo[0].fpga_name,pmsg_ctrlfpga_asset->fpga_name,16);
	memcpy(savefpgaassetinfo[0].fpga_version,pmsg_ctrlfpga_asset->fpga_version,24);
	memcpy(savefpgaassetinfo[0].fpga_sn,pmsg_ctrlfpga_asset->fpga_sn,32);
	memcpy(savefpgaassetinfo[0].fpga_fw_version,pmsg_ctrlfpga_asset->fpga_fw_version,16);
	memcpy(savectrlfpgamac.ctrl_fpga_mac,pmsg_ctrlfpga_asset->fpga_mac,6);

	for(index = 1;index < FpgaNum;index++)
	{
		savefpgaassetinfo[index].fpga_slot = (pmsg_datafpga_asset+index-1)->fpga_slot;
		memcpy(savefpgaassetinfo[index].fpga_name,(pmsg_datafpga_asset+index-1)->fpga_name,16);
		memcpy(savefpgaassetinfo[index].fpga_version,(pmsg_datafpga_asset+index-1)->fpga_version,24);
		memcpy(savefpgaassetinfo[index].fpga_sn,(pmsg_datafpga_asset+index-1)->fpga_sn,32);
		memcpy(savefpgaassetinfo[index].fpga_fw_version,(pmsg_datafpga_asset+index-1)->fpga_fw_version,16);
		memcpy(savedatafpgamac[index-1].fpga_mac1,(pmsg_datafpga_asset+index-1)->fpga_mac1,6);
		memcpy(savedatafpgamac[index-1].fpga_mac2,(pmsg_datafpga_asset+index-1)->fpga_mac2,6);
		memcpy(savedatafpgamac[index-1].fpga_mac3,(pmsg_datafpga_asset+index-1)->fpga_mac3,6);
		memcpy(savedatafpgamac[index-1].fpga_mac4,(pmsg_datafpga_asset+index-1)->fpga_mac4,6);
		memcpy(savedatafpgamac[index-1].fpga_mac5,(pmsg_datafpga_asset+index-1)->fpga_mac5,6);
		memcpy(savedatafpgamac[index-1].fpga_mac6,(pmsg_datafpga_asset+index-1)->fpga_mac6,6);
	}

#ifdef Debug
	log_msg(LOG_INFO, BORD_MGMT, "receive FPGA broadcast information success!\n");
	printf("MODULE_BORD_MGMT:fpga1_mac  %02x %02x %02x %02x %02x %02x\n",savectrlfpgamac.ctrl_fpga_mac[0],savectrlfpgamac.ctrl_fpga_mac[1],savectrlfpgamac.ctrl_fpga_mac[2],savectrlfpgamac.ctrl_fpga_mac[3],savectrlfpgamac.ctrl_fpga_mac[4],savectrlfpgamac.ctrl_fpga_mac[5]);
	log_msg(LOG_INFO, BORD_MGMT, "product_type=%04x\n",savefpgaserverassetinfo.product_type);
	log_msg(LOG_INFO, BORD_MGMT, "fpga3_solt=%02x\n",savefpgaassetinfo[2].fpga_slot);
	log_msg(LOG_INFO, BORD_MGMT, "fpga4_fw_ver=%s\n",savefpgaassetinfo[3].fpga_fw_version);
	INT8 fpga5_Mac6[18] = "0";
	INT8 fpga1_Mac[18] = "0";
	INT8 fpga2_Mac1[18] = "0";
	sprintf(fpga5_Mac6,"%02x:%02x:%02x:%02x:%02x:%02x",savedatafpgamac[3].fpga_mac6[0],savedatafpgamac[3].fpga_mac6[1],savedatafpgamac[3].fpga_mac6[2],savedatafpgamac[3].fpga_mac6[3],savedatafpgamac[3].fpga_mac6[4],savedatafpgamac[3].fpga_mac6[5]);
	sprintf(fpga1_Mac,"%02x:%02x:%02x:%02x:%02x:%02x",savectrlfpgamac.ctrl_fpga_mac[0],savectrlfpgamac.ctrl_fpga_mac[1],savectrlfpgamac.ctrl_fpga_mac[2],savectrlfpgamac.ctrl_fpga_mac[3],savectrlfpgamac.ctrl_fpga_mac[4],savectrlfpgamac.ctrl_fpga_mac[5]);
	sprintf(fpga2_Mac1,"%02x:%02x:%02x:%02x:%02x:%02x",savedatafpgamac[0].fpga_mac1[0],savedatafpgamac[0].fpga_mac1[1],savedatafpgamac[0].fpga_mac1[2],savedatafpgamac[0].fpga_mac1[3],savedatafpgamac[0].fpga_mac1[4],savedatafpgamac[0].fpga_mac1[5]);
	log_msg(LOG_INFO, BORD_MGMT, "fpga5_Mac6 = %s\n",fpga5_Mac6);
	log_msg(LOG_INFO, BORD_MGMT, "fpga1_Mac = %s\n",fpga1_Mac);
	log_msg(LOG_INFO, BORD_MGMT,"fpga2_Mac1 = %s\n",fpga2_Mac1);
#endif
	if(FUNC_OK != handle_fn_fpga_broadcast_rsp())
	{
		return FUNC_ERR;
	}
	return FUNC_OK;
}

/************************************************************************************************

 Description :handle function of control FPGA broadcast rspond

 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 *************************************************************************************************/
INT32 handle_fn_fpga_broadcast_rsp()
{
//allocation IP address for control FPGA

	UINT32  ip_root1,ip_root2,ip_root3,ip_root4;
	UINT8 str[12] = "0";
	UINT32 re1 = 0;
	UINT32 re2 = 0;
	UINT32 ne_id;
	sprintf(str, "%08x", g_ne_id);
	if ((re1 = char_trans_int(str[2])) < 0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "char_trans_int() error1!\n");
	}
	if ((re2 = char_trans_int(str[3])) < 0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "char_trans_int() error2!\n");
	}
	ne_id = re1 * 16 + re2;
#ifdef Debug
	log_msg(LOG_INFO, BORD_MGMT, "g_ne_ip_root=%s!\n",g_ne_ip_root);
	log_msg(LOG_INFO, BORD_MGMT, "g_ftp_server_ip=%s!\n",g_ftp_server_ip);
#endif

	sscanf(g_ne_ip_root,"%d.%d.%d.%d",&ip_root1,&ip_root2,&ip_root3,&ip_root4);
	memset(&resctrlfpga, 0, sizeof(ResCtrlFpga));
	sprintf(resctrlfpga.fpga1_addr, "%d.%d.%d.%d",ip_root1,ip_root2, ne_id, CtrlFpgaIpNode);

	struct in_addr ip_temp;

	memset(&ip_temp, 0, sizeof(ip_temp));
	inet_aton(resctrlfpga.fpga1_addr, &ip_temp);
	memset(resctrlfpga.fpga1_addr, 0, sizeof(resctrlfpga.fpga1_addr));
	memcpy(resctrlfpga.fpga1_addr, &ip_temp.s_addr,4);
	memcpy(savefpgaaddr.ctrl_fpga_addr,resctrlfpga.fpga1_addr,4);

#ifdef Debug
	log_msg(LOG_INFO, BORD_MGMT, "control FPGA IP address is:%08x!\n",resctrlfpga.fpga1_addr);
#endif
	resctrlfpga.fpga_server_id = htonl(savefpgaserverid.fpga_server_id);
	resctrlfpga.iptype = htons(IpType);
	resctrlfpga.fpga1_id = htonl(savefpgaid[0].fpga_id);
	resctrlfpga.fpga2_id = htonl(savefpgaid[1].fpga_id);
	resctrlfpga.fpga3_id = htonl(savefpgaid[2].fpga_id);
	resctrlfpga.fpga4_id = htonl(savefpgaid[3].fpga_id);
	resctrlfpga.fpga5_id = htonl(savefpgaid[4].fpga_id);

	memset(&ip_temp, 0, sizeof(ip_temp));
	inet_aton(g_ftp_server_ip, &ip_temp);
	memset(resctrlfpga.ftp_server_addr, 0, sizeof(resctrlfpga.ftp_server_addr));
	memcpy(resctrlfpga.ftp_server_addr, &ip_temp.s_addr,4);


	memset(&ip_temp, 0, sizeof(ip_temp));
	inet_aton(g_om_port_ip_addr, &ip_temp);
	memset(resctrlfpga.cpu_om_addr, 0, sizeof(resctrlfpga.cpu_om_addr));
	memcpy(resctrlfpga.cpu_om_addr,&ip_temp.s_addr,4);

	memcpy(resctrlfpga.mac_head.des_mac, savectrlfpgamac.ctrl_fpga_mac, 6);
	memcpy(resctrlfpga.mac_head.sre_mac, cpu_om_mac, 6);
	resctrlfpga.mac_head.pro_type = htons(PROTOCOL_TYPE);
	resctrlfpga.msg_head.msg_type = htons(BORD_MGMT_CTRL_FPGA);
	resctrlfpga.msg_head.msg_code = htons(BORD_MGMT_OM_RX_BROADCAST_INFO_RSP);
	resctrlfpga.msg_head.fpga_id = htonl(savefpgaid[0].fpga_id);
	resctrlfpga.msg_head.msg_len = htonl(sizeof(ResCtrlFpga));


/**********************use int_api send broadcast reply massage to MODULE_INET_MGMT**************/
#ifdef Test
     memcpy(sesson.raw_serveraddr.sll_addr,fpgamacaddrinfo.fpga1_mac,6);
     if(sendto(sesson.raw_fd,(INT8 *)&resctrlfpga,sizeof(ResCtrlFpga),0,(struct sockaddr  *)&sesson.raw_serveraddr,sizeof(sesson.raw_serveraddr)) < 0)
         {
    	    log_msg(LOG_ERR, BORD_MGMT, "send respond FPGA MAC broadcast error!\n");
    	    return FUNC_ERR;
         }
#else
	  memcpy(sesson.raw_serveraddr.sll_addr,savectrlfpgamac.ctrl_fpga_mac,6);
	  sesson.raw_serveraddr.sll_halen = 6;
	  memcpy(&clientaddr, (struct sockaddr*)&sesson.raw_serveraddr, sizeof(sesson.raw_serveraddr));

	  printf("sizeof(ResCtrlFpga) = %d.\n", sizeof(ResCtrlFpga));
     if(send_msg_to_inet(MODULE_BORD_MGMT,0,raw_sock_fd,0,&clientaddr,(char *)&resctrlfpga, sizeof(ResCtrlFpga)) < 0)
      {
    	  log_msg(LOG_ERR, BORD_MGMT, "send broadcast response to control FPGA failed!\n");
    	  return FUNC_ERR;
      }
     else
    	 log_msg(LOG_INFO, BORD_MGMT, "send broadcast response to control FPGA success!\n");
#endif

     memset(&sesson.udp_clientaddr,0,sizeof(sesson.udp_clientaddr));
     sesson.udp_clientaddr.sin_family = AF_INET;
     memcpy(&sesson.udp_clientaddr.sin_addr.s_addr,savefpgaaddr.ctrl_fpga_addr,4);
     sesson.udp_clientaddr.sin_port = htons(CLIENT_UDP_PORT);

//add update fpga1/2/3/4/5 firmware
     return FUNC_OK;
}

/**************************************************************************************************

 Description :handle function of control FPGA IP configuration result from control FPGA:
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 **************************************************************************************************/
INT32 handle_fn_om_udp_msg(INT8 *buf)
{
	FpgaAddrCfgReault *pmsg_fpga_addr_cfg_result = (FpgaAddrCfgReault*)buf;
	if(FUNC_OK!=pmsg_fpga_addr_cfg_result->fpga_addr_cfg_result)
	{
		log_msg(LOG_ERR, BORD_MGMT, "CTRL FPGA IP address configuration error!\n");
		return -1;
	}
	log_msg(LOG_INFO, BORD_MGMT, "CTRL FPGA IP address configuration success!\n");

	handle_fn_frame_info_report_cfg_req();//send the massage to configure data fpga

	memset(&fpgachanmonitrig, 0, sizeof(FpgaChanMoniTrig));
	fpgachanmonitrig.msg_head.msg_type = htons(BORD_MGMT_CTRL_FPGA);
    fpgachanmonitrig.msg_head.msg_code = htons(BORD_MGMT_OM_RX_CHAN_MONI_TRIGGER);
    fpgachanmonitrig.msg_head.fpga_id = htonl(savefpgaid[0].fpga_id);
    fpgachanmonitrig.msg_head.msg_len = htonl(sizeof(FpgaChanMoniTrig));
    memcpy(fpgachanmonitrig.cpu_data_mac1,cpu_data_mac1,6);

#ifdef Test
      if(sendto(sesson.udp_fd,(char *)&fpgachanmonitrig,sizeof(fpgachanmonitrig),0,(struct sockaddr *)&sesson.udp_clientaddr,sizeof(sesson.udp_clientaddr)) < 0)
      {
	    log_msg(LOG_ERR, BORD_MGMT, "send the massage of heartbear monitor trigger success!\n");
      }
#else
     memcpy(&clientaddr, (struct sockaddr*)&sesson.udp_clientaddr, sizeof(sesson.udp_clientaddr));
     if(send_msg_to_inet(MODULE_BORD_MGMT,0,g_udp_sock_fd,0,&clientaddr,(char *)&fpgachanmonitrig,sizeof(fpgachanmonitrig)) < 0)
      {
    	  log_msg(LOG_ERR, BORD_MGMT, "send the massage of heartbear monitor trigger failed!\n");
      }
#endif


//send heal threshold query request to MODULE_INET_MGMT
	memset(&msghead, 0, sizeof(MsgHead));
	msghead.msg_type = htons(BORD_MGMT_CTRL_FPGA);
	msghead.msg_code = htons(BORD_MGMT_OM_RX_HEAL_THRES_QUERY_REQ);
	msghead.fpga_id = htonl(savefpgaid[0].fpga_id);
	msghead.msg_len = htonl(sizeof(MsgHead));

//use inet_api send the message to ctrl_fpga

#ifdef Test
	if(sendto(sesson.udp_fd,(char *)&msghead,sizeof(MsgHead),0,(struct sockaddr *)&sesson.udp_clientaddr,sizeof(sesson.udp_clientaddr)) < 0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send heal threshold query request  error!\n");
	}
	log_msg(LOG_INFO, BORD_MGMT, "send heal threshold query request  ok!\n");
#else
	memcpy(&clientaddr, (struct sockaddr*)&sesson.udp_clientaddr, sizeof(sesson.udp_clientaddr));
	if(send_msg_to_inet(MODULE_BORD_MGMT,0,g_udp_sock_fd,0,&clientaddr,(char *)&msghead,sizeof(MsgHead)) < 0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send the massage to query FPGA health threshold information failed!\n");
		return FUNC_ERR;
	}

#endif
//add heartbeat timer and  fpga_hea_info inquire timer
	add_handle_heartb_msg_timer(HANDLE_HB_TIME);
	add_query_fpga_timer(QUERY_FPGA_TIME);
	return FUNC_OK;
}
/**************************************************************************************************

 Description :handle function of clock synchronization report message from control FPGA:
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 **************************************************************************************************/
INT32 handle_fn_time_sync_report_req(INT8 *buf)
{
	CloSyncReport *pmsg_clo_sync = (CloSyncReport*)buf;

#ifdef Debug
	printf("receive the massage of FPGA clock synchronization information.\n");
#endif
	memset(&msgbordsyn,0,sizeof(MsgBordSyn));
	if(0x00 == pmsg_clo_sync->timesync)
	{
		log_msg(LOG_INFO, BORD_MGMT, "clock synchronization success!\n");
		savetimesyncstate.curr_pos_mode = pmsg_clo_sync->posmode;
		savetimesyncstate.curr_clo_sre = pmsg_clo_sync->closource;

		msgbordsyn.bord_syn = NE_SYNC_OK;
	}

	else
	{
		log_msg(LOG_ERR, BORD_MGMT, "clock synchronization failed!\n");

		msgbordsyn.bord_syn = NE_SYNC_ERR;
	}
//respond FPGA clock ACK massage
	memset(&msghead,0,sizeof(MsgHead));
	msghead.msg_type = htonl(BORD_MGMT_CTRL_FPGA);
	msghead.msg_code = htons(BORD_MGMT_OM_RX_TIME_SYNC_ACK);
	msghead.fpga_id = htonl(savefpgaid[0].fpga_id);//
	msghead.msg_len = htonl(sizeof(MsgHead));

	memcpy(&clientaddr, (struct sockaddr*)&sesson.udp_clientaddr, sizeof(sesson.udp_clientaddr));
	if(send_msg_to_inet(MODULE_BORD_MGMT,0,g_udp_sock_fd,0,&clientaddr,(char *)&msghead,sizeof(MsgHead)) < 0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send the massage of clock synchronization  ACK to control FPGA failed!\n");
		return FUNC_ERR;
	}
//report to MODULE_CELL_MGMT
	msgbordsyn.msghead.msg_type = MODULE_BORD_MGMT;
	msgbordsyn.msghead.msg_code = BORD_MGMT_MSG_NE_SYNC_STATE;
	msgbordsyn.msghead.msg_len = sizeof(MsgBordSyn);
	if(send_msg_to_om(MODULE_CELL_MGMT,(UINT8*)&msgbordsyn,sizeof(MsgBordSyn)) < 0)
	{
		log_msg(LOG_ERR, BORD_MGMT, "send the massage of clock synchronization state to cell management failed!\n");
		return FUNC_ERR;
	}
	log_msg(LOG_INFO, BORD_MGMT, "send the massage of clock synchronization state to cell management success!\n");
//trap to MODULE_CFG_MGMT/
    return FUNC_OK;
}
/***************************************************************************************************

 Description :handle function of FPGA health information threshold query respond from control FPGA
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 **************************************************************************************************/
INT32 handle_fpga_heal_thres_query_rsp(INT8 *buf)
{
	FpgaServerHealThresInfo *pmsg_fpga_server_heal_thres = (FpgaServerHealThresInfo*)buf;
	FpgaHealThresInfo *pmsg_fpga_heal_thres = (FpgaHealThresInfo*)(buf + MsgHeadBufLen + FpgaServerHealLen);
	UINT32 index;

	savefpgaserverhealthresinfo.fpga_server_id = ntohl(pmsg_fpga_server_heal_thres->fpga_server_id);
	savefpgaserverhealthresinfo.curr_fpga_server_fan_thres = ntohs(pmsg_fpga_server_heal_thres->fpga_server_fan_thres);

	for(index = 0;index < FpgaNum;index++)
	{
		savefpgahealthresinfo[index].fpga_id = ntohl((pmsg_fpga_heal_thres+index)->fpga_id);
		savefpgahealthresinfo[index].curr_fpga_temp_thres =ntohl((pmsg_fpga_heal_thres+index)->fpga_temp_thres);
		savefpgahealthresinfo[index].curr_fpga_vol_thres = ntohs((pmsg_fpga_heal_thres+index)->fpga_vol_thres);
	}
#ifdef Debug
	log_msg(LOG_INFO, BORD_MGMT, "receive the massage of fpga_heal_thres\n");
	log_msg(LOG_INFO, BORD_MGMT, "fpga1_thres_temp=%08x\n",savefpgahealthresinfo[0].curr_fpga_temp_thres);
	log_msg(LOG_INFO, BORD_MGMT, "fpga3_thres_temp=%08x\n",savefpgahealthresinfo[2].curr_fpga_temp_thres);
	log_msg(LOG_INFO, BORD_MGMT, "fpga5_thres_vol=%04x\n",savefpgahealthresinfo[4].curr_fpga_vol_thres);
#endif
	return FUNC_OK;
}
/***************************************************************************************************

 Description :handle function of FPGA health information query respond from control FPGA
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 **************************************************************************************************/
INT32 handle_fn_fpga_heal_info_query_rsp (INT8 *buf)
{
	FpgaServerHealInfo *pmsg_fpga_server_heal = (FpgaServerHealInfo*)buf;
	FpgaHealInfo *pmsg_fpga_heal = (FpgaHealInfo*)(buf + MsgHeadBufLen + FpgaServerHealLen);
	UINT32 index;

	savefpgaserverhealinfo.fpga_server_id = ntohl(pmsg_fpga_server_heal->fpga_server_id);
	savefpgaserverhealinfo.fpga_server_fan = ntohs(pmsg_fpga_server_heal->fpga_server_fan);

	for(index = 0;index < FpgaNum;index++)
	{
		savefpgahealinfo[index].fpga_id = ntohl((pmsg_fpga_heal+index)->fpga_id);
		savefpgahealinfo[index].fpga_temp =ntohl((pmsg_fpga_heal+index)->fpga_temp);
		savefpgahealinfo[index].fpga_vol = ntohs((pmsg_fpga_heal+index)->fpga_vol);
	}
#ifdef Debug
	log_msg(LOG_INFO, BORD_MGMT, "receive the massage of fpga_heal_info\n");
	log_msg(LOG_INFO, BORD_MGMT, "fpga1_id=%08x\n",savefpgahealinfo[0].fpga_id);
    log_msg(LOG_INFO, BORD_MGMT, "fpga1_heal_temp=%08x\n",savefpgahealinfo[0].fpga_temp);
    log_msg(LOG_INFO, BORD_MGMT, "fpga2_heal_vol=%04x\n",savefpgahealinfo[1].fpga_vol);
    log_msg(LOG_INFO, BORD_MGMT, "fpga5_id=%08x\n",savefpgahealinfo[4].fpga_id);
    log_msg(LOG_INFO, BORD_MGMT, "fpga5_heal_temp=%08x\n",savefpgahealinfo[4].fpga_temp);
#endif
	return FUNC_OK;
}
/***************************************************************************************************

 Description :handle function of FPGA temperature threshold configuration result from control FPGA.
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ***************************************************************************************************/
INT32 handle_fn_fpga_temp_thres_cfg_rsp (INT8 *buf)
{
	HealThresCfgRsp *pmsg_cfg_rsp = (HealThresCfgRsp*)buf;
	UINT8 result = 0;
	UINT32 index;
	index = (pmsg_cfg_rsp->fpga_id) & 0x00000f00 >> 2;

	if (result == pmsg_cfg_rsp->result)
	{
		log_msg(LOG_INFO, BORD_MGMT, "FPGA temperature threshold configuration success!\n");
	}
	else
	{
		log_msg(LOG_ERR, BORD_MGMT, "FPGA temperature threshold configuration failed!\n");
		savefpgahealthresinfo[index-1].curr_fpga_temp_thres = savefpgahealthresinfo[index-1].old_fpga_temp_thres;
//send trap top massage
	}

	return FUNC_OK;
}
/*************************************************************************************************

 Description :handle function of FPGA voltage threshold configuration result from control FPGA.
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 *************************************************************************************************/
INT32 handle_fn_fpga_vol_thres_cfg_rsp (INT8 *buf)
{
	HealThresCfgRsp *pmsg_cfg_rsp = (HealThresCfgRsp*)buf;
	UINT8 result = 0;
	UINT32 index;
	index = (pmsg_cfg_rsp->fpga_id) & 0x00000f00 >> 2;

	if(result == pmsg_cfg_rsp->result)
	{
		log_msg(LOG_INFO, BORD_MGMT, "FPGA voltage threshold configuration success!\n");
	 }
	else
	{
		log_msg(LOG_ERR, BORD_MGMT, "FPGA voltage threshold configuration failed!\n");
		savefpgahealthresinfo[index-1].curr_fpga_vol_thres = savefpgahealthresinfo[index-1].old_fpga_vol_thres;
//send trap massage
	}

	return FUNC_OK;
}
/*************************************************************************************************

 Description :handle function of FPGA voltage threshold configuration result from control FPGA
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 *************************************************************************************************/
INT32 handle_fn_fpga_fan_thres_cfg_rsp (INT8 *buf)
{
	HealThresCfgRsp *pmsg_cfg_rsp = (HealThresCfgRsp*)buf;
	UINT8 result = 0;

	if(result == pmsg_cfg_rsp->result)
	 {
		log_msg(LOG_INFO, BORD_MGMT, "FPGA fan threshold configuration success!\n");
	 }
	else
	{
		log_msg(LOG_ERR, BORD_MGMT, "FPGA fan threshold configuration failed!\n");
		savefpgaserverhealthresinfo.curr_fpga_server_fan_thres = savefpgaserverhealthresinfo.old_fpga_server_fan_thres;
//send trap massage
	}
	return FUNC_OK;
}

/*************************************************************************************************

 Description :handle function of  GPS position mode configuration result respond from control FPGA
 input : massage pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ************************************************************************************************/
INT32 handle_fn_gps_cfg_rsp(INT8 *buf)
{
	LocCfgRsp *pmsg_pos_cfg = (LocCfgRsp*)buf;

	UINT8 cfgresult = 0;
	if(cfgresult == pmsg_pos_cfg->cfgresult)
	{
		log_msg(LOG_INFO, BORD_MGMT, "GPS position mode configuration success!\n");
	}
	else
	{
		log_msg(LOG_ERR, BORD_MGMT, "GPS position mode configuration failed!\n");
		savetimesyncstate.curr_pos_mode = savetimesyncstate.old_pos_mode;
//send trap massage
	}
   return FUNC_OK;
}
/***********************************************************************************************

 Description :handle function of FPGA clock source configuration result respond from control FPGA
 input : message pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ***********************************************************************************************/
INT32 handle_fn_clock_source_cfg_rsp( INT8 *buf)
{
	CloSouCfgRsp *pmsg_clo_sre = (CloSouCfgRsp*)buf;
	UINT8 cfgresult = 0;
	if(cfgresult == pmsg_clo_sre->cfgresult)
	{
		log_msg(LOG_INFO, BORD_MGMT, "clock source configuration success!\n");
	}
	else
	{
		log_msg(LOG_ERR, BORD_MGMT, "clock source configuration failed!\n");
		savetimesyncstate.curr_clo_sre = savetimesyncstate.old_clo_sre;

//send trap massage
	}
	return FUNC_OK;
}

/*********************************************************************************************

 Description :handle function of control FPGA heart beat message:
 input :massage pointer
 output:NULL
 return: success FUNC_OK
         false FUNC_ERR

 ********************************************************************************************/
INT32 handle_fn_fpga1_heartbeat_msg( INT8 *buf)
{

	fpgaheartbeatrcd[0][0].heartb_count++;
//	fpgaheartbeatrcd[0][0].fpga_state=1;
	fpgaheartbeatrcd[0][0].fpga_port_state = 1;

	memset(&msghead, 0, sizeof(MsgHead));
	msghead.msg_type = htons(BORD_MGMT_CTRL_FPGA);
	msghead.msg_code = htons(BORD_MGMT_OM_RX_OM_CHANNEL_MONITOR);
	msghead.fpga_id = htonl(savefpgaid[0].fpga_id);
	msghead.msg_len = htonl(sizeof(MsgHead));
	memcpy(&clientaddr, (struct sockaddr*)&sesson.udp_clientaddr, sizeof(sesson.udp_clientaddr));
    if(send_msg_to_inet(MODULE_BORD_MGMT,0,g_udp_sock_fd,0,&clientaddr,(char *)&msghead,sizeof(MsgHead)) < 0)
    {
	    log_msg(LOG_ERR, BORD_MGMT, "send the massage of CPU OM channel monitor to control FPGA failed!\n");
	    return FUNC_ERR;
    }
    return FUNC_OK;
}
/********************************************************************************************

 Description :handle function of FPGA board power control result from control FPGA:
 input : massage pointer
 output: null
 return: success FUNC_OK
         false FUNC_ERR

 ********************************************************************************************/
INT32 handle_fn_fpga_board_power_rsp( INT8 *buf)
{
	FpgaBoardPowerCtlRsp *pmsg_bord_power_rsp = (FpgaBoardPowerCtlRsp*)buf;

	if(0x00 == pmsg_bord_power_rsp->result)
	{
		 log_msg(LOG_INFO, BORD_MGMT, "FPGA Server power control success!\n");

	}
	else
	{
		log_msg(LOG_ERR, BORD_MGMT, "FPGA Server power control failed!\n");
		///////
	}


	return FUNC_OK;
}
/********************************************************************************************

 Description :handle function of signal FPGA power control result respond from control fpga:
 input : message pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ********************************************************************************************/
INT32 handle_fn_fpga_chip_power_rsp( INT8 *buf)
{
	FpgaChipPowerCtlRsp *pmsg_fpga_power_rsp = (FpgaChipPowerCtlRsp*)buf;
	UINT32 index;
	index = (pmsg_fpga_power_rsp->fpga_id) & 0x00000f00 >> 2;

	if(0x00 == pmsg_fpga_power_rsp->result)
	{
		log_msg(LOG_INFO, BORD_MGMT, "FPGA chip power control success!\n");
	}
	else
	{
		log_msg(LOG_ERR, BORD_MGMT, "FPGA  chip power control failed!\n");
		savefpgapowerstate[index-1].curr_fpga_power_state = savefpgapowerstate[index-1].old_fpga_power_state;
	}

	return FUNC_OK;
}
/********************************************************************************************

 Description :handle function of data FPGA heart beat message from FPGA2/3/4/5:
 input : message pointer
 output: NULL
 return: success FUNC_OK
         false FUNC_ERR

 ********************************************************************************************/
INT32 handle_fn_data_fpga_heartbeat_msg(INT8 *buf)
{
#ifdef Debug
	log_msg(LOG_INFO, BORD_MGMT, "fpga_id=%08x\n",ntohl(*(UINT32 *)(buf+18)));
#endif
	if(ntohl(*(UINT32 *)(buf+18)) == savefpgaid[1].fpga_id) //fpga2_id is alloc id
	{
		fpgaheartbeatrcd[1][*(buf+26)].heartb_count++;
//		fpgaheartbeatrcd[1][*(buf+26)].fpga_state=1;
		fpgaheartbeatrcd[1][*(buf+26)].fpga_port_state=1;
    }
	if(ntohl(*(UINT32 *)(buf+18)) == savefpgaid[2].fpga_id) //fpga3
	{
		fpgaheartbeatrcd[2][*(buf+26)].heartb_count++;
//		fpgaheartbeatrcd[2][*(buf+26)].fpga_state=1;
		fpgaheartbeatrcd[2][*(buf+26)].fpga_port_state = 1;
	}
	if(ntohl(*(UINT32 *)(buf+18)) == savefpgaid[3].fpga_id) //fpga4
	{
		fpgaheartbeatrcd[3][*(buf+26)].heartb_count++;
//		fpgaheartbeatrcd[3][*(buf+26)].fpga_state = 1;
		fpgaheartbeatrcd[3][*(buf+26)].fpga_port_state = 1;
	}
	if(ntohl(*(UINT32 *)(buf+18)) == savefpgaid[4].fpga_id) //fpga5
	{
		fpgaheartbeatrcd[4][*(buf+26)].heartb_count++;
//		fpgaheartbeatrcd[4][*(buf+26)].fpga_state=1;
		fpgaheartbeatrcd[4][*(buf+26)].fpga_port_state = 1;
	}

	return FUNC_OK;
}

