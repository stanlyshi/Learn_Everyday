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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:03 CST. */
/*
 * sbsconfig_common.h
 *
 *  Created on: Jul 10, 2018
 *      Author: root
 */
#ifndef H_SBSCONFIG_COMMON_H_
#define H_SBSCONFIG_COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>
#include <string.h>
#include "om_common_h.h"



#define ERROR           (-1)
#define SUCCESS         0
#define STR_LEN_PROTOCOL 	8
#define SRT_LEN_MAX     128
#define BUF_LEN_MAX 	409600
#define STR_LEN_256     256
#define STR_LEN_16      16
#define STR_LEN_12      12
#define STR_LEN_24      24
#define STR_LEN_48      48


#define STR_LEN_PROTOCOL_16  16
#define STR_LEN_PROTOCOL_13  13
#define STR_LEN_PROTOCOL_28  28

#define RS_ACTIVE           1
#define RS_NOTINSERVICE     2
#define RS_NOTREADY         3
#define RS_CREATEANDGO      4
#define RS_CREATEANDWAIT    5
#define RS_DESTROY          6

#define MAX_COMM_SUM        20
#define PHY_CFG_DEFAULT      SYS_PRODUCT_DIR"/phy/phycfg.xml"     /*"/home/SBSAgent/phy_cfg/default/phycfg.xml"*/
#define PROTOCOL_CFG_DEFAULT    SYS_PRODUCT_DIR"/l2_l3/default/" /*"/home/SBSAgent/protocol_cfg/default"*/
#define PROTOCOL_TO_PHY_CFG_DEFAULT  SYS_PRODUCT_DIR"/l2_l3/default/cfgfile/PHYConfig.xer"		/*"/home/SBSAgent/protocol_phy_cfg/default/PHYConfig.xer"*/
#define PROTOCOL_PATH 	SYS_PRODUCT_DIR"/l2_l3/cells"
#define FILE_TYPE_SICFG     0
#define FILE_TYPE_CFG       1
#define OPERATE_MODIFY_VALUE    0
#define OPERATE_MODIFY_NAME     1
#define OPERATE_MODIFY_NAME_NEXT     2
#define OPERATE_MODIFY_VALUE_NEXT    3

//vcm
#define MAX_VCM_CELL_SUM    256

//contrl
#define MAX_CONTRL_DEVICE_SUM    5
#define MAX_CONTRL_DEVICE_CPU_SUM 2



static const char *SICfg_DllBandWidth = "/SIConfig/mib/dl-Bandwidth";
static const char *SICfg_UlBandWidth = "/SIConfig/si1/criticalExtensions/systemInformation-r8/sib-TypeAndInfo/sib2/freqInfo/ul-Bandwidth";
static const char *SICfg_freqBandIndicator = "/SIConfig/sib1/freqBandIndicator";
static const char *SICfg_subframeAssign = "/SIConfig/sib1/tdd-Config/subframeAssignment";
static const char *SICfg_speSubPatterns = "/SIConfig/sib1/tdd-Config/specialSubframePatterns";

static const char *SICfg_MCC_MNC_Digit ="/SIConfig/sib1/cellAccessRelatedInfo/plmn-IdentityList/PLMN-IdentityInfo/plmn-Identity/mcc/MCC-MNC-Digit";
static const char *SICfg_trackingAreaCode = "/SIConfig/sib1/cellAccessRelatedInfo/trackingAreaCode";
static const char *SICfg_cellIdentity = "/SIConfig/sib1/cellAccessRelatedInfo/cellIdentity";
static const char *SICfg_numberOfRA_Preambles ="/SIConfig/si1/criticalExtensions/systemInformation-r8/sib-TypeAndInfo/sib2/radioResourceConfigCommon/rach-ConfigCommon/preambleInfo/numberOfRA-Preambles";

static const char *SICfg_preambleTransMax = "/SIConfig/si1/criticalExtensions/systemInformation-r8/sib-TypeAndInfo/sib2/radioResourceConfigCommon/rach-ConfigCommon/ra-SupervisionInfo/preambleTransMax";
static const char *SICfg_maxHARQ_Msg3Tx = "/SIConfig/si1/criticalExtensions/systemInformation-r8/sib-TypeAndInfo/sib2/radioResourceConfigCommon/rach-ConfigCommon/maxHARQ-Msg3Tx";

static const char *SICfg_q_RxLevMin = "/SIConfig/si2/criticalExtensions/systemInformation-r8/sib-TypeAndInfo/sib3/intraFreqCellReselectionInfo/q-RxLevMin";
static const char *SICfg_p_Max =  "/SIConfig/sib1/p-Max";
static const char *SICfg_preambleInitialReceivedTargetPower = "/SIConfig/si1/criticalExtensions/systemInformation-r8/sib-TypeAndInfo/sib2/radioResourceConfigCommon/rach-ConfigCommon/powerRampingParameters/preambleInitialReceivedTargetPower";
static const char *SICfg_p0_NominalPUSCH = "/SIConfig/si1/criticalExtensions/systemInformation-r8/sib-TypeAndInfo/sib2/radioResourceConfigCommon/uplinkPowerControlCommon/p0-NominalPUSCH";
static const char *SICfg_alpha = "/SIConfig/si1/criticalExtensions/systemInformation-r8/sib-TypeAndInfo/sib2/radioResourceConfigCommon/uplinkPowerControlCommon/alpha";
static const char *SICfg_p0_NominalPUCCH = "/SIConfig/si1/criticalExtensions/systemInformation-r8/sib-TypeAndInfo/sib2/radioResourceConfigCommon/uplinkPowerControlCommon/p0-NominalPUCCH";
static const char *SICfg_dl_CarrierFreq = "/SIConfig/si3/criticalExtensions/systemInformation-r8/sib-TypeAndInfo/sib5/interFreqCarrierFreqList/InterFreqCarrierFreqInfo/dl-CarrierFreq";
static const char *SICfg_additionalSpectrumEmission = "/SIConfig/si1/criticalExtensions/systemInformation-r8/sib-TypeAndInfo/sib2/freqInfo/additionalSpectrumEmission";
static const char *SICfg_ul_CarrierFreq = "/SIConfig/si1/criticalExtensions/systemInformation-r8/sib-TypeAndInfo/sib2/freqInfo/ul-CarrierFreq";
static const char *SICfg_plmnid = "/SIConfig/sib1/cellAccessRelatedInfo/plmn-IdentityList/PLMN-IdentityInfo/plmn-Identity/mcc/MCC-MNC-Digit";
static const char *SICfg_plmnid_more = "/SIConfig/sib1/cellAccessRelatedInfo/plmn-IdentityList/PLMN-IdentityInfo/plmn-Identity/mnc/MCC-MNC-Digit";
//cfg
static const char *Cfg_enb_ipaddr = "/Cfg/enb-ipaddr";
static const char *Cfg_gtpu_recv_addr = "/Cfg/gtpu-recv-addr";
static const char *Cfg_log_ip = "/Cfg/log-dest-addr/ipaddr";
static const char *Cfg_log_macaddr = "/Cfg/log-dest-addr/macaddr";
static const char *Cfg_log_port = "/Cfg/log-dest-addr/port";
static const char *Cfg_phy_cell_id = "/Cfg/cell-info/phy-cell-id";
static const char *Cfg_earfcn_dl = "/Cfg/cell-info/earfcn-dl";
static const char *Cfg_earfcn_ul = "/Cfg/cell-info/earfcn-ul";
static const char *Cfg_tx_ant_port = "/Cfg/cell-info/tx-ant-port";
static const char *Cfg_rx_ant_port = "/Cfg/cell-info/rx-ant-port";
static const char *Cfg_plmn_id = "/Cfg/plmn-id";
static const char *Cfg_cellid = "/Cfg/cellid";
static const char *Cfg_vbs_eth = "/Cfg/vbs-eth";
static const char *Cfg_vbs_mode = "/Cfg/vbs-mode";
static const char *Cfg_bbu_dl_mac = "/Cfg/bbu-dl-mac";
static const char *Cfg_bbu_ul_mac = "/Cfg/bbu-ul-mac";
static const char *Cfg_ppu_mac = "/Cfg/ppu-mac";
static const char *Cfg_mme_ipaddr = "/Cfg/mme-ipaddr/IP-ADDR/ipaddr";
static const char *Cfg_sgw_ipaddr = "/Cfg/sgw-ipaddr";
static const char *Cfg_vbs_id = "/Cfg/vbs-id";

//phycfg
static const char *PhyCfg_radioEnable = "/PhyConfig/Radio/radioEnable";
static const char *PhyCfg_ferryBridgeMode = "/PhyConfig/Radio/ferryBridgeMode";
static const char *PhyCfg_ferryBridgeEthPort = "/PhyConfig/Radio/ferryBridgeEthPort";

//proTophycfg
static const char *proTophyCfg_phyCfg = "/PHYConfig/phy-cfg";
static const char *proTophyCfg_puschDtxThresh = "/PHYConfig/pusch-dtx-thresh";
static const char *proTophyCfg_muxPuschAckNackDetThresh = "/PHYConfig/mux-pusch-ack-nack-det-thresh";
static const char *proTophyCfg_muxPuschCqiDetThresh = "/PHYConfig/mux-pusch-cqi-det-thresh";
static const char *proTophyCfg_muxPuschRiDetThresh = "/PHYConfig/mux-pusch-ri-det-thresh";
static const char *proTophyCfg_pucchRbStart = "/PHYConfig/pucch-rb-start";

static const char *proTophyCfg_pucchF1DetThresh = "/PHYConfig/pucch-F1-det-thresh";
static const char *proTophyCfg_pucchF2sDetThresh = "/PHYConfig/pucch-F2s-det-thresh";
static const char *proTophyCfg_prachThresh = "/PHYConfig/prach-thresh";
static const char *proTophyCfg_radioAccessMode = "/PHYConfig/radio-access-mode";

static const char *proTophyCfg_psyncPower= "/PHYConfig/psyncPower";
static const char *proTophyCfg_ssyncPower = "/PHYConfig/ssyncPower";
static const char *proTophyCfg_imcs = "/PHYConfig/imcs";
static const char *proTophyCfg_mu_mimo = "/PHYConfig/mu-mimo";

static const char *proTophyCfg_sync_ant_selection= "/PHYConfig/sync-ant-selection";
static const char *proTophyCfg_carrier_aggregation_leve = "/PHYConfig/carrier-aggregation-level";
static const char *proTophyCfg_operational_mode = "/PHYConfig/operational-mode";

typedef struct
{
	int commId;
	char pro_cfg_dic[STR_LEN_256];
}ProCfgDicItem;

ProCfgDicItem g_proCfgDicTable[MAX_COMM_SUM];




pid_t remove_protocol_docu(char *source_path);
pid_t generate_protocol_docu(char *source_path, char *det_path);
char *generate_protocol_dic(char *des, int index);
int mofify_cfg_file(const char *pathname, const char *nodename, const char *nodevalue, const int operate_type,const int file_type, const int index);
int modify_proTophycfg_file( const char *pathname, const char *nodename, const char *nodevalue, const int operate_type);
int modify_phycfg_file( const char *pathname, const char *nodename, const char *nodevalue);
char* itobs(int n, char *ps);

#endif /* H_SBS_COMMON_H_ */
