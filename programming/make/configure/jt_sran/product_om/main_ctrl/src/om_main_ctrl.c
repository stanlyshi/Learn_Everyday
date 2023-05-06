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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:04 CST. */
/**********************************************
** Copyright (c) .
** All rights reserved.
**
** File name:om_main_ctrl.c
** Description:source file of om_main_ctrl.
**
** Current Version: 1.0
** Author: wangyuanyuan@ict.ac.cn
** Date: 201809
***********************************************/

#include "sbs_type.h"
#include "sbs_xml.h"
#include "sbs_log.h"
#include "om_common_h.h"
#include "om_main_ctrl.h"
#include "cell_mgmt_msg.h"
#include <string.h>
#include "utils_base.h"

extern INT32 send_msg_to_om(UINT16 module_id, UINT8 *msg_p, UINT32 msg_len);

INT32 parse_om_config_xml()
{

#define MEMSET_BUF_8	memset(buf, 0, 20)
	char buf[20];

	xmlDocPtr	pdoc;
	pdoc = utils_xml_open(OM_MAIN_CFIG_XML);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ftp_server_ip", g_ftp_server_ip);


	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ftp_server_port", buf);
	g_ftp_server_port = atoi(buf);
	MEMSET_BUF_8;

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ftp_server_om_sw_path", g_ftp_server_om_sw_path);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ftp_server_mac_sw_path", g_ftp_server_mac_sw_path);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ftp_server_phy_sw_path", g_ftp_server_phy_sw_path);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ftp_server_fpga_fw_path", g_ftp_server_fpga_fw_path);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ftp_server_rru_fw_sw_path", g_ftp_server_rru_fw_path);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ftp_server_rru_fw_sw_path", g_ftp_server_rru_sw_path);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ftp_local_path", g_ftp_server_local_path);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ftp_server_user_name", g_ftp_server_user_name);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ftp_server_pwd", g_ftp_server_pwd);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/omc_ip", g_omc_ip);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/snmp_nms_port", buf);
	g_snmp_port = atoi(buf);
	MEMSET_BUF_8;

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/snmp_trap_port", buf);
	g_snmp_trap_port = atoi(buf);
	MEMSET_BUF_8;

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ne_id", buf);
	g_ne_id = atoi(buf);
	MEMSET_BUF_8;

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ne_ip", g_ne_ip_root);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/communication/ne_net_mask", g_om_ip_netmask);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/version/om_sw_version", g_om_sw_version);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/version/ne_cfg_version", g_ne_cfg_file_version);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/version/mac_sw_version", g_ne_mac_sw_version);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/version/phy_sw_version", g_ne_phy_sw_version);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/version/fpga1_fw_version", g_ne_fpga1_fw_version);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/version/fpga2_fw_version", g_ne_fpga2_fw_version);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/version/fpga3_fw_version", g_ne_fpga3_fw_version);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/version/fpga4_fw_version", g_ne_fpga4_fw_version);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/version/fpga5_fw_version", g_ne_fpga5_fw_version);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/version/rru_fw_version", g_ne_rru_fw_version);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/version/rru_sw_version", g_ne_rru_sw_version);

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/max_cell_num", buf);
	max_cell_num = atoi(buf);
	MEMSET_BUF_8;

	utils_xml_get_node_value_by_xpath(pdoc, "/MainCtrlLocal/max_dpdk_num", buf);
	max_dpdk_num = atoi(buf);
	MEMSET_BUF_8;

	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/node_addr_info/om_port_name",g_om_port_name);

	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/node_addr_info/om_port_mac",g_om_port_mac_addr);

	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/node_addr_info/om_port_ip",g_om_port_ip_addr);
	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/node_addr_info/om_data_mac1",g_dpdk_mac_addr[0]);
	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/node_addr_info/om_data_mac2",g_dpdk_mac_addr[1]);
	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/file_path/om_path/om_config_path",g_om_config_file_path);
	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/file_path/phy_path/phy_sw_path",g_phy_sw_path);

	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/file_path/phy_path/phy_execufile_path",g_phy_execufile_path);

	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/file_path/mac_path/mac_sw_path",g_mac_sw_path);

	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/file_path/mac_path/mac_default_path",g_mac_default_path);

	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/file_path/mac_path/mac_cells_path",g_mac_cells_path);

	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/file_path/mac_path/mac_execufile_path",g_mac_execufile_path);

	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/file_path/un_socket_path/phy_unsocket_path",g_phy_un_socket_path);

	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/file_path/un_socket_path/mac_unsocket_path",g_mac_un_socket_path);

	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/file_path/fpga_fw_path",g_fpga_sw_path);

	utils_xml_get_node_value_by_xpath(pdoc,"/MainCtrlLocal/file_path/rru_fw_path",g_rru_sw_path);

	utils_xml_clean(pdoc);

	return FUNC_OK;
}

INT32 update_om_by_ftp()
{
	return FUNC_OK;
}

INT32 cfig_mgmt_cfig_recover()
{
	return system("./initCfg/cfg_init.sh");
}

INT32 start_phy()
{
    system("./simu_phy &");
	return FUNC_OK;
}




/*
 * MAIN_CTRL message handler
 */
INT32 handle_module_phy_msg(void* msg, UINT32 len)
{
	UINT16 msg_code;
	msg_code = *((UINT16*)((char*)msg + 2));

	switch (msg_code)
	{
		case CODE_PHY_MAIN_CTRL_PHY_LOAD_RSP:
		{
			if (handle_msg_phy_load_rsp((void*)msg, len) == FUNC_ERR)
			{
				log_msg(LOG_ERR, MAIN_CTRL, "Handle msg phy load rsp error.\n");
			}
			break;
		}

		default:
			log_msg(LOG_ERR, MAIN_CTRL, "Message type is module phy, but code out of range, code is %d.\n", msg_code);
			break;
	}

	return FUNC_OK;
}

INT32 handle_msg_phy_load_rsp(void* msg, UINT32 len)
{
	if (msg == NULL)
	{
		log_msg(LOG_ERR, MAIN_CTRL, "Message is NULL.\n");
		return FUNC_ERR;
	}

	PHY_MAIN_CTRL_PHY_LOAD_RSP phy_load_rsp;
	memcpy(&phy_load_rsp, msg, len);
	if (phy_load_rsp.init_rslt != 0)
	{
		log_msg(LOG_ERR, MAIN_CTRL, "Phy load error, error code is %d.\n", phy_load_rsp.init_rslt);
		return FUNC_ERR;
	}

	log_msg(LOG_WARNING, MAIN_CTRL, "init rslt= %d\n", phy_load_rsp.init_rslt);
	log_msg(LOG_WARNING, MAIN_CTRL, "PHY pid = %d\n", phy_load_rsp.phy_pid);

	switch (phy_load_rsp.port_num)
	{
		case 0:
		{
			log_msg(LOG_WARNING, MAIN_CTRL, "DPDK port num is %d\n", phy_load_rsp.port_num);
			return FUNC_OK;
		}

		case 1:
		{
			log_msg(LOG_INFO, MAIN_CTRL, "DPDK port num is %d\n", phy_load_rsp.port_num);
			utils_mac6_to_mac18(phy_load_rsp.mac_addr1, g_dpdk_mac_addr[0]);
			memset(g_dpdk_mac_addr[1], 0, 18);
			memset(g_dpdk_mac_addr[2], 0, 18);
			memset(g_dpdk_mac_addr[3], 0, 18);
			break;
		}

		case 2:
		{
			log_msg(LOG_INFO, MAIN_CTRL, "DPDK port num is %d\n", phy_load_rsp.port_num);
			utils_mac6_to_mac18(phy_load_rsp.mac_addr1, g_dpdk_mac_addr[0]);
			utils_mac6_to_mac18(phy_load_rsp.mac_addr2, g_dpdk_mac_addr[1]);
			memset(g_dpdk_mac_addr[2], 0, 18);
			memset(g_dpdk_mac_addr[3], 0, 18);
			break;
		}

		case 3:
		{
			log_msg(LOG_INFO, MAIN_CTRL, "DPDK port num is %d\n", phy_load_rsp.port_num);
			utils_mac6_to_mac18(phy_load_rsp.mac_addr1, g_dpdk_mac_addr[0]);
			utils_mac6_to_mac18(phy_load_rsp.mac_addr2, g_dpdk_mac_addr[1]);
			utils_mac6_to_mac18(phy_load_rsp.mac_addr2, g_dpdk_mac_addr[2]);
			memset(g_dpdk_mac_addr[3], 0, 18);
			break;
		}

		case 4:
		{
			log_msg(LOG_INFO, MAIN_CTRL, "DPDK port num is %d\n", phy_load_rsp.port_num);
			utils_mac6_to_mac18(phy_load_rsp.mac_addr1, g_dpdk_mac_addr[0]);
			utils_mac6_to_mac18(phy_load_rsp.mac_addr2, g_dpdk_mac_addr[1]);
			utils_mac6_to_mac18(phy_load_rsp.mac_addr2, g_dpdk_mac_addr[2]);
			utils_mac6_to_mac18(phy_load_rsp.mac_addr2, g_dpdk_mac_addr[3]);
			break;
		}

		default:
		{
			log_msg(LOG_ERR, MAIN_CTRL, "DPDK port num error: %d\n", phy_load_rsp.port_num);
			return FUNC_ERR;
		}

	}
	g_phy_state = PHY_OK;
	// send MsgPhyState msg to cell_mgmt
	MsgPhyState phy_state_msg = {
			.msghead.msg_type = MODULE_MAIN_CTRL,
			.msghead.msg_code = 1,
			.msghead.msg_len = sizeof(MsgPhyState),
			.new_phy_state = PHY_OK,
	};
	if (send_msg_to_om(MODULE_CELL_MGMT, (UINT8 *)&phy_state_msg, sizeof(phy_state_msg)) == FUNC_ERR) {
		log_msg(LOG_ERR, MAIN_CTRL, "send phy state change msg to CELL_MGMT failed\n");
	}
	log_msg(LOG_INFO, MAIN_CTRL, "send phy state change msg to CELL_MGMT succeed.\n");


	return FUNC_OK;
}








INT32 handle_module_bord_mgmt_msg(void* msg, UINT32 len)
{
	UINT16 msg_code;
	msg_code = *((UINT16*)((char*)msg + 2));

	switch (msg_code)
	{
		case CODE_BORD_MGMT_MAIN_CTRL_FPGA_INIT_RSLT_IND:
		{
			if (handle_msg_fpga_init_rslt_ind((void*)msg, len) == FUNC_ERR)
			{
				log_msg(LOG_ERR, MAIN_CTRL, "Handle msg fpga init rslt ind error.\n");
			}
			break;
		}

		default:
			log_msg(LOG_ERR, MAIN_CTRL, "Message type is module bord_mgmt, but code out of range, code is %d.\n", msg_code);
			break;
	}

	return FUNC_OK;
}


INT32 handle_msg_fpga_init_rslt_ind(void*msg, UINT32 len)
{
	if (msg == NULL)
	{
		log_msg(LOG_ERR, MAIN_CTRL, "Message is NULL.\n");
		return FUNC_ERR;
	}
	BORD_MGMT_MAIN_CTRL_FPGA_INIT_RSLT_IND fpga_init_rslt;
	//printf("fpga_init_rslt length is %d.\n", sizeof(BORD_MGMT_MAIN_CTRL_FPGA_INIT_RSLT_IND));

	if (fpga_init_rslt.fpga_init_rslt != 0)
	{
		log_msg(LOG_ERR, MAIN_CTRL, "Fpga init error and error code is %d\n", fpga_init_rslt.fpga_init_rslt);
		return FUNC_ERR;
	}
	MAIN_CTRL_RRUS_MGMT_CPRI_INIT_REQ cpri_init_req;
	cpri_init_req.head.msg_type = MODULE_MAIN_CTRL;
	cpri_init_req.head.msg_code = CODE_MAIN_CTRL_RRUS_MGMT_CPRI_INIT_REQ;
	cpri_init_req.head.msg_len = sizeof(MAIN_CTRL_RRUS_MGMT_CPRI_INIT_REQ);

	send_msg_to_om(MODULE_RRUS_MGMT, (UINT8*)&cpri_init_req, sizeof(MAIN_CTRL_RRUS_MGMT_CPRI_INIT_REQ));

	return FUNC_OK;
}
