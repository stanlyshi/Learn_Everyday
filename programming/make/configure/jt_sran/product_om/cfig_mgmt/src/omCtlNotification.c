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
#include "omCtlNotification.h"

extern const oid snmptrap_oid[];
extern const size_t snmptrap_oid_len;

int
send_omCtlNEStatus_trap(void* pPacket)
{
	CONFIG_NESetup_t * pState = (CONFIG_NESetup_t*)pPacket;
	unsigned char status= pState->ucNESetupStatus;
    netsnmp_variable_list *var_list = NULL;
    const oid       omCtlNEStatus_oid[] =
        { 1, 3, 6, 1, 4, 1, 12001, 4, 2, 1 };
    const oid       omCtlAlarmNEStartUpStatus_oid[] =
        { 1, 3, 6, 1, 4, 1, 12001, 2, 1, 1, 0 };

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
                              snmptrap_oid, snmptrap_oid_len,
                              ASN_OBJECT_ID,
                              omCtlNEStatus_oid,
                              sizeof(omCtlNEStatus_oid));

    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
                              omCtlAlarmNEStartUpStatus_oid,
                              OID_LENGTH(omCtlAlarmNEStartUpStatus_oid),
                              ASN_UNSIGNED,
                              /*
                               * Set an appropriate value for omCtlAlarmNEStartUpStatus 
                               */
                              (u_char)&status, sizeof(status));

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap(var_list);
    snmp_free_varbind(var_list);

    return SNMP_ERR_NOERROR;
}

int
send_omCtlTimeSync_trap(void* pPacket)
{
	CONFIG_TimeSyncState_t * pState = (CONFIG_TimeSyncState_t*)pPacket;
	unsigned char ucTimeSync;
	unsigned char ucErrCode;
	unsigned char ucPosMode;
	unsigned char ucCloSource;
	ucTimeSync = pState->ucTimeSync;
	ucErrCode = pState->ucErrCode;
	ucPosMode = pState->ucPosMode;
	ucCloSource = pState->ucClosource;
    netsnmp_variable_list *var_list = NULL;
    const oid       omCtlTimeSync_oid[] =
        { 1, 3, 6, 1, 4, 1, 12001, 4, 2, 2 };
    const oid       omCtlAlarmTimeSyncStatus_oid[] =
        { 1, 3, 6, 1, 4, 1, 12001, 2, 1, 2, 1, 2, 1 
        };
    const oid       omCtlAlarmTimeSyncErrCode_oid[] =
        { 1, 3, 6, 1, 4, 1, 12001, 2, 1, 2, 1, 3, 1 
        };
    const oid       omCtlAlarmTimeSyncPosMode_oid[] =
        { 1, 3, 6, 1, 4, 1, 12001, 2, 1, 2, 1, 4, 1 
        };
    const oid       omCtlAlarmTimeSyncCloSource_oid[] =
        { 1, 3, 6, 1, 4, 1, 12001, 2, 1, 2, 1, 5, 1 
        };

    /*
     * Set the snmpTrapOid.0 value
     */
    snmp_varlist_add_variable(&var_list,
                              snmptrap_oid, snmptrap_oid_len,
                              ASN_OBJECT_ID,
                              omCtlTimeSync_oid,
                              sizeof(omCtlTimeSync_oid));

    /*
     * Add any objects from the trap definition
     */
    snmp_varlist_add_variable(&var_list,
                              omCtlAlarmTimeSyncStatus_oid,
                              OID_LENGTH(omCtlAlarmTimeSyncStatus_oid),
                              ASN_OCTET_STR,
                              /*
                               * Set an appropriate value for omCtlAlarmTimeSyncStatus 
                               */
                              (u_char *)&ucTimeSync, sizeof(ucTimeSync));
    snmp_varlist_add_variable(&var_list,
                              omCtlAlarmTimeSyncErrCode_oid,
                              OID_LENGTH(omCtlAlarmTimeSyncErrCode_oid),
                              ASN_OCTET_STR,
                              /*
                               * Set an appropriate value for omCtlAlarmTimeSyncErrCode 
                               */
                              (u_char *)&ucErrCode, sizeof(ucErrCode));
    snmp_varlist_add_variable(&var_list,
                              omCtlAlarmTimeSyncPosMode_oid,
                              OID_LENGTH(omCtlAlarmTimeSyncPosMode_oid),
                              ASN_OCTET_STR,
                              /*
                               * Set an appropriate value for omCtlAlarmTimeSyncPosMode 
                               */
                              (u_char *)&ucPosMode, sizeof(ucPosMode));
    snmp_varlist_add_variable(&var_list,
                              omCtlAlarmTimeSyncCloSource_oid,
                              OID_LENGTH(omCtlAlarmTimeSyncCloSource_oid),
                              ASN_OCTET_STR,
                              /*
                               * Set an appropriate value for omCtlAlarmTimeSyncCloSource 
                               */
                              (u_char *)&ucCloSource, sizeof(ucCloSource));

    /*
     * Add any extra (optional) objects here
     */

    /*
     * Send the trap to the list of configured destinations
     *  and clean up
     */
    send_v2trap(var_list);
    snmp_free_varbind(var_list);

    return SNMP_ERR_NOERROR;
}
