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
#include <timerCfgMsg.h>
#include "mnt_mgmt_api.h"

//#include <bord_mgmt_api.h>
//#include <bord_mgmt_set_api.h>
#include <om_common_h.h>
/*
 * timerCfgMsg_variables_oid:
 *   this is the top level oid that we want to register under.  This
 *   is essentially a prefix, with the suffix appearing in the
 *   variable below.
 */

oid             timerCfgMsg_variables_oid[] =
    { 1, 3, 6, 1, 4, 1, 12001, 1, 2, 3, 2, 3 };

/*
 * variable4 timerCfgMsg_variables:
 *   this variable defines function callbacks and type return information 
 *   for the timerCfgMsg mib section 
 */

struct variable4 timerCfgMsg_variables[] = {
    /*
     * magic number        , variable type , ro/rw , callback fn  , L, oidsuffix 
     */
#define TIMERCFGMSGPOSMODE		1
    {TIMERCFGMSGPOSMODE, ASN_UNSIGNED, NETSNMP_OLDAPI_RWRITE,
     var_timerCfgMsg, 1, {1}},
#define TIMERCFGMSGCLOSRECFG		2
    {TIMERCFGMSGCLOSRECFG, ASN_UNSIGNED, NETSNMP_OLDAPI_RWRITE,
     var_timerCfgMsg, 1, {2}},

};

/*
 * (L = length of the oidsuffix) 
 */

/** Initializes the timerCfgMsg module */
void
init_timerCfgMsg(void)
{

    DEBUGMSGTL(("timerCfgMsg", "Initializing\n"));

    /*
     * register ourselves with the agent to handle our mib tree 
     */
    REGISTER_MIB("timerCfgMsg", timerCfgMsg_variables, variable4,
                 timerCfgMsg_variables_oid);

    /*
     * place any other initialization junk you need here 
     */
}

/*
 * var_timerCfgMsg():
 *   This function is called every time the agent gets a request for
 *   a scalar variable that might be found within your mib section
 *   registered above.  It is up to you to do the right thing and
 *   return the correct value.
 *     You should also correct the value of "var_len" if necessary.
 *
 *   Please see the documentation for more information about writing
 *   module extensions, and check out the examples in the examples
 *   and mibII directories.
 */
unsigned char  *
var_timerCfgMsg(struct variable *vp,
                oid * name,
                size_t *length,
                int exact, size_t *var_len, WriteMethod ** write_method)
{
    /*
     * variables we may use later 
     */
    static long     long_ret;
    static u_long   ulong_ret;
    static unsigned char string[SPRINT_MAX_LEN];
    static oid      objid[MAX_OID_LEN];
    static struct counter64 c64;

	static unsigned char ucMode;
	static unsigned char ucCloSreCfg;


    if (header_generic(vp, name, length, exact, var_len, write_method)
        == MATCH_FAILED)
        return NULL;

    /*
     * this is where we do the value assignments for the mib results.
     */
    switch (vp->magic) {
    case TIMERCFGMSGPOSMODE:
        *write_method = write_timerCfgMsgPosMode;
	    long_ret=CTRL_FPGA_Clock_Get_PosModeCfg(&ucMode);
	    *var_len = sizeof(ucMode);
	    return (u_char *) & ucMode;

    case TIMERCFGMSGCLOSRECFG:
        *write_method = write_timerCfgMsgCloSreCfg;
	    long_ret=CTRL_FPGA_Clock_Get_CloSreCfg(&ucCloSreCfg);
	    *var_len = sizeof(ucCloSreCfg);
	    return (u_char *) & ucCloSreCfg;

    default:
        ERROR_MSG("");
    }
    return NULL;
}





int
write_timerCfgMsgPosMode(int action,
                         u_char * var_val,
                         u_char var_val_type,
                         size_t var_val_len,
                         u_char * statP, oid * name, size_t name_len)
{
    unsigned char   value;
    int             size;
    PosModeConf     posModeConf;
    switch (action) {
    case RESERVE1:
        if (var_val_type != ASN_UNSIGNED) {
            fprintf(stderr, "write to timerCfgMsg not ASN_UNSIGNED\n");
            return SNMP_ERR_WRONGTYPE;
        }
        if (var_val_len > sizeof(u_long)) {
            fprintf(stderr, "write to timerCfgMsg: bad length\n");
            return SNMP_ERR_WRONGLENGTH;
        }
        break;

    case RESERVE2:
        size = var_val_len;
        value = *(unsigned char *) var_val;

        break;

    case FREE:
        /*
         * Release any resources that have been allocated 
         */
        break;

    case ACTION:
    	posModeConf.msghead.msg_type = MODULE_CFIG_MGMT;
    	posModeConf.msghead.msg_code = CFG_MGMT_BORD_MGMT_FPGA_GPS_CFG_REQ;
    	posModeConf.msghead.msg_len = sizeof(PosModeConf);
    	posModeConf.pos_mode_cfg =  *(unsigned char *) var_val;
        if (send_msg_to_om(MODULE_BORD_MGMT,(char*)&posModeConf,sizeof(PosModeConf)) != 0)
        {
        	return SNMP_ERR_COMMITFAILED;
        }
//        send_msg_to_om(MODULE_BORD_MGMT,(char*)&posModeConf,sizeof(PosModeConf));
        break;

    case UNDO:
        /*
         * Back out any changes made in the ACTION case 
         */
        break;

    case COMMIT:
        /*
         * Things are working well, so it's now safe to make the change
         * permanently.  Make sure that anything done here can't fail!
         */
        break;
    }
    return SNMP_ERR_NOERROR;
}


int
write_timerCfgMsgCloSreCfg(int action,
                           u_char * var_val,
                           u_char var_val_type,
                           size_t var_val_len,
                           u_char * statP, oid * name, size_t name_len)
{
    unsigned char   value;
    int             size;
    CloSreConf      cloSreConf;
    switch (action) {
    case RESERVE1:
        if (var_val_type != ASN_UNSIGNED) {
            fprintf(stderr, "write to timerCfgMsg not ASN_UNSIGNED\n");
            return SNMP_ERR_WRONGTYPE;
        }
        if (var_val_len > sizeof(u_long)) {
            fprintf(stderr, "write to timerCfgMsg: bad length\n");
            return SNMP_ERR_WRONGLENGTH;
        }
        break;

    case RESERVE2:
        size = var_val_len;
        value = *(unsigned char *) var_val;

        break;

    case FREE:
        /*
         * Release any resources that have been allocated 
         */
        break;

    case ACTION:

		cloSreConf.msghead.msg_type = MODULE_CFIG_MGMT;
		cloSreConf.msghead.msg_code = CFG_MGMT_BORD_MGMT_FPGA_CLOCK_SOURCE_CFG_REQ;
		cloSreConf.msghead.msg_len = sizeof(CloSreConf);
		cloSreConf.clo_sre_cfg = *(unsigned char *) var_val;
        if (send_msg_to_om(MODULE_BORD_MGMT,(char*)&cloSreConf,sizeof(CloSreConf)) != 0)
        {
        	return SNMP_ERR_COMMITFAILED;
        }
//        send_msg_to_om(MODULE_BORD_MGMT,(char*)&cloSreConf,sizeof(CloSreConf));
        break;

    case UNDO:
        /*
         * Back out any changes made in the ACTION case 
         */
        break;

    case COMMIT:
        /*
         * Things are working well, so it's now safe to make the change
         * permanently.  Make sure that anything done here can't fail!
         */
        break;
    }
    return SNMP_ERR_NOERROR;
}
