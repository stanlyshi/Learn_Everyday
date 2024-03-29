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
 * Note: this file originally auto-generated by mib2c using
 *        $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "phyCommConfigInfoScalar.h"

/** Initializes the phyCommConfigInfo module */
sbs_phyCommConfigInfo_Scalar_t phycomm_scalar_packet;
char phy_info_int_value[12];
#define SCALAR_SET_ACTION_INTEGER(node,leaf)    \
  node.old_##leaf = node.leaf;                  \
  node.leaf = *requests->requestvb->val.integer

#define SCALAR_SET_UNDO_INTEGER(node,leaf)      \
  node.leaf = node.old_##leaf;                  \
  node.old_##leaf = 0

void
init_phyCommConfigInfo_scalar(void)
{
    const oid       phyRadioEnable_oid[] =
        { 1, 3, 6, 1, 4, 1, 12001, 1, 3, 2, 1 };
    const oid       phyFerryBridgeMode_oid[] =
        { 1, 3, 6, 1, 4, 1, 12001, 1, 3, 2, 2 };
    const oid       phyFerryBridgeEthPort_oid[] =
        { 1, 3, 6, 1, 4, 1, 12001, 1, 3, 2, 3 };

    DEBUGMSGTL(("phyCommConfigInfo", "Initializing\n"));
    memset(&phycomm_scalar_packet, 0, sizeof(phycomm_scalar_packet));
    netsnmp_register_scalar(netsnmp_create_handler_registration
                            ("phyRadioEnable", handle_phyRadioEnable,
                             phyRadioEnable_oid,
                             OID_LENGTH(phyRadioEnable_oid),
                             HANDLER_CAN_RWRITE));
    netsnmp_register_scalar(netsnmp_create_handler_registration
                            ("phyFerryBridgeMode",
                             handle_phyFerryBridgeMode,
                             phyFerryBridgeMode_oid,
                             OID_LENGTH(phyFerryBridgeMode_oid),
                             HANDLER_CAN_RWRITE));
    netsnmp_register_scalar(netsnmp_create_handler_registration
                            ("phyFerryBridgeEthPort",
                             handle_phyFerryBridgeEthPort,
                             phyFerryBridgeEthPort_oid,
                             OID_LENGTH(phyFerryBridgeEthPort_oid),
                             HANDLER_CAN_RWRITE));
}

int
handle_phyRadioEnable(netsnmp_mib_handler *handler,
                      netsnmp_handler_registration *reginfo,
                      netsnmp_agent_request_info *reqinfo,
                      netsnmp_request_info *requests)
{
    int             ret;


    switch (reqinfo->mode) {

    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED,
                                 /*
                                  * XXX: a pointer to the scalar's data 
                                  */ (void*)&(phycomm_scalar_packet.phyRadioEnable),
                                 /*
                                  * XXX: the length of the data in bytes 
                                  */ sizeof(phycomm_scalar_packet.phyRadioEnable));
        break;

    case MODE_SET_RESERVE1:
        /*
         * or you could use netsnmp_check_vb_type_and_size instead 
         */
        ret = netsnmp_check_vb_type(requests->requestvb, ASN_UNSIGNED);
        if (ret != SNMP_ERR_NOERROR) {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        break;

    case MODE_SET_RESERVE2:

        break;

    case MODE_SET_FREE:

        break;

    case MODE_SET_ACTION:

    	SCALAR_SET_ACTION_INTEGER(phycomm_scalar_packet, phyRadioEnable);
        //write xml file
    	memset(phy_info_int_value,0,sizeof(phy_info_int_value));
    	sprintf(phy_info_int_value, "%d",*requests->requestvb->val.integer);
    	ret = modify_phycfg_file( PHY_CFG_DEFAULT, PhyCfg_radioEnable, phy_info_int_value);

        break;

    case MODE_SET_COMMIT:
        break;

    case MODE_SET_UNDO:
    	 SCALAR_SET_UNDO_INTEGER(phycomm_scalar_packet, phyRadioEnable);
        break;

    default:
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_phyRadioEnable\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int
handle_phyFerryBridgeMode(netsnmp_mib_handler *handler,
                          netsnmp_handler_registration *reginfo,
                          netsnmp_agent_request_info *reqinfo,
                          netsnmp_request_info *requests)
{
    int             ret;

    switch (reqinfo->mode) {

    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED,
                                 /*
                                  * XXX: a pointer to the scalar's data 
                                  */
        		                 (void*)&(phycomm_scalar_packet.phyFerryBridgeMode),
                                 /*
                                  * XXX: the length of the data in bytes 
                                  */ sizeof(phycomm_scalar_packet.phyFerryBridgeMode));
        break;

    case MODE_SET_RESERVE1:
        ret = netsnmp_check_vb_type(requests->requestvb, ASN_UNSIGNED);
        if (ret != SNMP_ERR_NOERROR) {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        break;

    case MODE_SET_RESERVE2:
        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:
    	SCALAR_SET_ACTION_INTEGER(phycomm_scalar_packet, phyFerryBridgeMode);
    	//write xml
    	memset(phy_info_int_value,0,sizeof(phy_info_int_value));
    	sprintf(phy_info_int_value, "%d",*requests->requestvb->val.integer);
    	ret = modify_phycfg_file( PHY_CFG_DEFAULT, PhyCfg_ferryBridgeMode, phy_info_int_value);

        break;

    case MODE_SET_COMMIT:

        break;

    case MODE_SET_UNDO:

        SCALAR_SET_UNDO_INTEGER(phycomm_scalar_packet, phyFerryBridgeMode);
        break;

    default:
        /*
         * we should never get here, so this is a really bad error 
         */
        snmp_log(LOG_ERR,
                 "unknown mode (%d) in handle_phyFerryBridgeMode\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

int
handle_phyFerryBridgeEthPort(netsnmp_mib_handler *handler,
                             netsnmp_handler_registration *reginfo,
                             netsnmp_agent_request_info *reqinfo,
                             netsnmp_request_info *requests)
{
    int             ret;

    switch (reqinfo->mode) {

    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_UNSIGNED,
                                 /*
                                  * XXX: a pointer to the scalar's data 
                                  */
        		                  (void*)&(phycomm_scalar_packet.phyFerryBridgeEthPort),
                                 /*
                                  * XXX: the length of the data in bytes 
                                  */ sizeof(phycomm_scalar_packet.phyFerryBridgeEthPort));
        break;

    case MODE_SET_RESERVE1:
        ret = netsnmp_check_vb_type(requests->requestvb, ASN_UNSIGNED);
        if (ret != SNMP_ERR_NOERROR) {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        break;

    case MODE_SET_RESERVE2:

        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:
    	SCALAR_SET_ACTION_INTEGER(phycomm_scalar_packet, phyFerryBridgeEthPort);
    	//write xml
    	memset(phy_info_int_value,0,sizeof(phy_info_int_value));
    	sprintf(phy_info_int_value, "%d",*requests->requestvb->val.integer);
    	ret = modify_phycfg_file( PHY_CFG_DEFAULT, PhyCfg_ferryBridgeEthPort, phy_info_int_value);
        break;

    case MODE_SET_COMMIT:


        break;

    case MODE_SET_UNDO:

        break;

    default:
        snmp_log(LOG_ERR,
                 "unknown mode (%d) in handle_phyFerryBridgeEthPort\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
