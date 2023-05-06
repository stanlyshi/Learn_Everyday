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
** File name: vcm_alarm.h
** Description:
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao
** Date: 2019.03 - 
**
***********************************************************************************************************************/
#ifndef _VCM_ALARM_H
#define _VCM_ALARM_H 1

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "vcm_config.h"
#include "vcm_types.h"



/* Constants ---------------------------------------------------------------------------------------------------------*/
/****************************************************************
 **  MAC Alarm message code, 2019.02.14
 **  By Liu Shuzheng(#), Wei Xinglong(#) and Rong Tao(*)
 **      (#)provide message code 
 **      (*)coding
 ****************************************************************/
typedef enum {
 MAC_ALR_CommonCtrl_RRU_CONFIG_RSP_NULL    = 0x1001 ,/* Rru config rsp input is null ����RRU��ӦΪ��*/
 MAC_ALR_CommonCtrl_RRU_CONFIG_RSLT_ERR    = 0x1002 ,/* Config RRU Result is Error ����RRUʧ��*/

 MAC_ALR_RRC_CONN_PROC_ERR                 = 0x2001 ,/* Error in RRC_CONN_PROC RRC���ӹ��̳��� */
 MAC_ALR_RRC_RECFG_PROC_ERR                = 0x2002 ,/* Error in RRC_RECFG_PROC RRC������̳��� */
 MAC_ALR_RRC_CORE_PROC_ERR                 = 0x2003 ,/* Error in RRC_CORE_PROC RRCʵ��ά���쳣 */
 MAC_ALR_RRC_CTX_RECFG_PROC_ERR            = 0x2004 ,/* Error in CTX_RECFG_PROC  RRC�������������*/
 MAC_ALR_RRC_CONN_RELEASE_PROC_ERR         = 0x2005 ,/* Error in RRC_CONN_RELEASE_PROC RRC���ӽ������̳��� */
 MAC_ALR_RRC_AS_SEC_PROC_ERR               = 0x2006 ,/* Error in AS_SEC_PROC RRC���ð�ȫģʽ���� */
 MAC_ALR_RRC_NO_UE_INFO_ERR                = 0x2007 ,/* No UE Info  RRCû���ҵ�UE��Ϣ*/

 MAC_ALR_S1_E_RAB_MAINTENANCE_ABNORMAL     = 0x3001 ,/* Error in E_RAB System S1ά���쳣 */
 MAC_ALR_S1_E_RAB_Param_ERR                = 0x3002 ,/* Error in E_RAB Param  ���ز������� */
 MAC_ALR_S1_NO_Memery_for_E_RAB            = 0x3003 ,/* NO Memery for E_RAB  �����ڴ���Դ����*/
 MAC_ALR_S1_Timeout_on_S1_Setup            = 0x3004 ,/* Timeout on S1 Setup  ��������������ӳ�ʱ*/

 MAC_ALR_GTPU_GTP_Init_Failed              = 0x30f1 ,/* GTP Init Failed  ��ʼ��GTP����*/
 MAC_ALR_GTPU_GTP_Send_Failed              = 0x30f2 ,/* Unkown TEID, GTP Send Failed GTP����ʧ�ܣ�TEID���� */
 MAC_ALR_GTPU_Received_Error_Indication    = 0x30f3 ,/* Received Error Indication �յ��Զ˷��͵Ĵ���ָʾ*/

 MAC_ALR_IDT_Select_Error                  = 0x3f01 ,/* IDT Select Error IDT��·���ô��� */
 MAC_ALR_IDT_S1_Recv_Data_Process_Fail     = 0x3f02 ,/* S1 Recv Data Process Fail ��ȡS1��Ϣ���̳��� */
 MAC_ALR_IDT_GTPU_Recv_Data_Process_Fail   = 0x3f03 ,/* GTPU Recv Data Process Fail ��ȡGTPU��Ϣ���̳��� */

 MAC_ALR_PDCP_Integrity_Verify_Fail        = 0x4001 ,/* Integrity Verify Fail ������У�����*/
 MAC_ALR_PDCP_MAC_I_Error                  = 0x4002 ,/* AS SCR Inte Verify Generate  MAC-I Error 
                                                                �����Ա�������������֤�����У�����mac-i���ִ��� */
 MAC_ALR_PDCP_SN_Invalid                   = 0x4003 ,/* SN Invalid ����DATA PDCP PDU SN���ִ��� */

 MAC_ALR_RLC_Buffer_Full                   = 0x4f01 ,/* Buffer is Full,Cannot Store DL Data RLC���ڻ������ݵ��ڴ�����*/
 MAC_ALR_RLC_SegArray_FULL                 = 0x4f02 ,/* Seg Array is FULL  �ֶ������������޷���������ֶ�*/

 MAC_ALR_TTI_BCH_Schedule_Error            = 0x5001 ,/* Do BCH Schedule Error BCH���ȳ��� */
 MAC_ALR_TTI_SIB_Schedule_Error            = 0x5002 ,/* Do Bcch2bch Schedule Error  SIB���ȳ��� */
 MAC_ALR_TTI_UE_DL_Schedule_Error          = 0x5003 ,/* Do UE DL Schedule Error �û����е��ȳ��� */
 MAC_ALR_TTI_UE_UL_Schedule_Error          = 0x5004 ,/* Do UE UL Schedule Error �û����е��ȳ��� */

 MAC_ALR_MAC_Recv_Error                    = 0x6001 ,/* Recv Error Phy Txstart Indication  ��ʱ��Ϣ����*/
 MAC_ALR_MAC_Err_Chnnel_ID                 = 0x6002 ,/* Get Wrong CRNTI by Channel ID 
                                                                �յ�����Chnnel ID���޷��ҵ���ӦCRNTI */
 MAC_ALR_MAC_Unpack_MAC_Pdu_Error          = 0x6003 ,/* Unpack MAC Pdu Error MAC���ݽ��ʧ�� */

 MAC_ALR_MACPHY_RX_CRC_Error               = 0x7001 ,/* RX CRC Error CRCУ��ʧ�� */
 MAC_ALR_MACPHY_Invalid_TX_VECTOR_Format   = 0x7002 ,/* Invalid TX VECTOR Format TX VECTOR ��ʽ���� */
 MAC_ALR_MACPHY_Timeout_on_TXSTART_req     = 0x7003 ,/* Timeout on TXSTART req û���յ�TXSTART.req */
} MAC_ALR_ENUM;


/**
 *  String of MAC Alarm 
 */
#define s_Alr_(enum_mac_alr) s_Alr_##enum_mac_alr
#define s_Clr_(enum_mac_alr) s_Clr_##enum_mac_alr

/* TODO: 2019.02.14 */
#define s_Alr_MAC_ALR_CommonCtrl_RRU_CONFIG_RSP_NULL      " "  
#define s_Clr_MAC_ALR_CommonCtrl_RRU_CONFIG_RSP_NULL      " "  
#define s_Alr_MAC_ALR_CommonCtrl_RRU_CONFIG_RSLT_ERR      " "  
#define s_Clr_MAC_ALR_CommonCtrl_RRU_CONFIG_RSLT_ERR      " "  

#define s_Alr_MAC_ALR_RRC_CONN_PROC_ERR                   " "  
#define s_Clr_MAC_ALR_RRC_CONN_PROC_ERR                   " "  
#define s_Alr_MAC_ALR_RRC_RECFG_PROC_ERR                  " "  
#define s_Clr_MAC_ALR_RRC_RECFG_PROC_ERR                  " "  
#define s_Alr_MAC_ALR_RRC_CORE_PROC_ERR                   " "  
#define s_Clr_MAC_ALR_RRC_CORE_PROC_ERR                   " "  
#define s_Alr_MAC_ALR_RRC_CTX_RECFG_PROC_ERR              " "  
#define s_Clr_MAC_ALR_RRC_CTX_RECFG_PROC_ERR              " "  
#define s_Alr_MAC_ALR_RRC_CONN_RELEASE_PROC_ERR           " "  
#define s_Clr_MAC_ALR_RRC_CONN_RELEASE_PROC_ERR           " "  
#define s_Alr_MAC_ALR_RRC_AS_SEC_PROC_ERR                 " "  
#define s_Clr_MAC_ALR_RRC_AS_SEC_PROC_ERR                 " "  
#define s_Alr_MAC_ALR_RRC_NO_UE_INFO_ERR                  " "  
#define s_Clr_MAC_ALR_RRC_NO_UE_INFO_ERR                  " "  

#define s_Alr_MAC_ALR_S1_E_RAB_MAINTENANCE_ABNORMAL       " "  
#define s_Clr_MAC_ALR_S1_E_RAB_MAINTENANCE_ABNORMAL       " "  
#define s_Alr_MAC_ALR_S1_E_RAB_Param_ERR                  " "  
#define s_Clr_MAC_ALR_S1_E_RAB_Param_ERR                  " "  
#define s_Alr_MAC_ALR_S1_NO_Memery_for_E_RAB              " "  
#define s_Clr_MAC_ALR_S1_NO_Memery_for_E_RAB              " "  
#define s_Alr_MAC_ALR_S1_Timeout_on_S1_Setup              " " 
#define s_Clr_MAC_ALR_S1_Timeout_on_S1_Setup              " "  

#define s_Alr_MAC_ALR_GTPU_GTP_Init_Failed                " "  
#define s_Clr_MAC_ALR_GTPU_GTP_Init_Failed                " "  
#define s_Alr_MAC_ALR_GTPU_GTP_Send_Failed                " "  
#define s_Clr_MAC_ALR_GTPU_GTP_Send_Failed                " "  
#define s_Alr_MAC_ALR_GTPU_Received_Error_Indication      " "  
#define s_Clr_MAC_ALR_GTPU_Received_Error_Indication      " "  

#define s_Alr_MAC_ALR_IDT_Select_Error                    " "  
#define s_Clr_MAC_ALR_IDT_Select_Error                    " "  
#define s_Alr_MAC_ALR_IDT_S1_Recv_Data_Process_Fail       " "  
#define s_Clr_MAC_ALR_IDT_S1_Recv_Data_Process_Fail       " "  
#define s_Alr_MAC_ALR_IDT_GTPU_Recv_Data_Process_Fail     " "  
#define s_Clr_MAC_ALR_IDT_GTPU_Recv_Data_Process_Fail     " "  

#define s_Alr_MAC_ALR_PDCP_Integrity_Verify_Fail          " "  
#define s_Clr_MAC_ALR_PDCP_Integrity_Verify_Fail          " "  
#define s_Alr_MAC_ALR_PDCP_MAC_I_Error                    " " 
#define s_Clr_MAC_ALR_PDCP_MAC_I_Error                    " " 
#define s_Alr_MAC_ALR_PDCP_SN_Invalid                     " "  
#define s_Clr_MAC_ALR_PDCP_SN_Invalid                     " "  

#define s_Alr_MAC_ALR_RLC_Buffer_Full                     " " 
#define s_Clr_MAC_ALR_RLC_Buffer_Full                     " " 
#define s_Alr_MAC_ALR_RLC_SegArray_FULL                   " " 
#define s_Clr_MAC_ALR_RLC_SegArray_FULL                   " "  

#define s_Alr_MAC_ALR_TTI_BCH_Schedule_Error              " " 
#define s_Clr_MAC_ALR_TTI_BCH_Schedule_Error              " "  
#define s_Alr_MAC_ALR_TTI_SIB_Schedule_Error              " "  
#define s_Clr_MAC_ALR_TTI_SIB_Schedule_Error              " "  
#define s_Alr_MAC_ALR_TTI_UE_DL_Schedule_Error            " " 
#define s_Clr_MAC_ALR_TTI_UE_DL_Schedule_Error            " " 
#define s_Alr_MAC_ALR_TTI_UE_UL_Schedule_Error            " " 
#define s_Clr_MAC_ALR_TTI_UE_UL_Schedule_Error            " "  

#define s_Alr_MAC_ALR_MAC_Recv_Error                      " "  
#define s_Clr_MAC_ALR_MAC_Recv_Error                      " "  
#define s_Alr_MAC_ALR_MAC_Err_Chnnel_ID                   " "  
#define s_Clr_MAC_ALR_MAC_Err_Chnnel_ID                   " "  
#define s_Alr_MAC_ALR_MAC_Unpack_MAC_Pdu_Error            " "  
#define s_Clr_MAC_ALR_MAC_Unpack_MAC_Pdu_Error            " "  

#define s_Alr_MAC_ALR_MACPHY_RX_CRC_Error                 " "  
#define s_Clr_MAC_ALR_MACPHY_RX_CRC_Error                 " "  
#define s_Alr_MAC_ALR_MACPHY_Invalid_TX_VECTOR_Format     " "  
#define s_Clr_MAC_ALR_MACPHY_Invalid_TX_VECTOR_Format     " "  
#define s_Alr_MAC_ALR_MACPHY_Timeout_on_TXSTART_req       " "  
#define s_Clr_MAC_ALR_MACPHY_Timeout_on_TXSTART_req       " "  


/****************************************************************
 *  Here some other alarm ...
 ****************************************************************/


/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/

/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/


#endif /*<_VCM_ALARM_H>*/


