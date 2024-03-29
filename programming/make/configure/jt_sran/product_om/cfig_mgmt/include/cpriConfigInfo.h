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
 * Note: this file originally auto-generated by mib2c using
 *        $
 */
#ifndef CPRICONFIGINFO_H
#define CPRICONFIGINFO_H

#include "sbs_type.h"
#define CPRICONFIGINFOTABLES_INDEX_POS 14

typedef struct  {
   
    long            cpriDealyCfgIndex;
	
    u_long          cpriDealyCfgAxcInd;
    u_long          cpriDealyCfgTimeDelayUl;
    u_long          cpriDealyCfgTimeDelayDl;
	long			cpriDealyCfgRowStatus;

	UINT8 			fin_flag;
}cpriDealyCfgTable;

typedef struct  {
   
    long            cpriSingleCfgIndex;
	
    u_long          cpriSingleCfgAxcInd;
    u_long          cpriSingleCfgOnOrOff;
    u_long          cpriSingleCfgSingleMode;
	long			cpriSingleCfgRowStatus;

	UINT8 			fin_flag;
}cpriSingleCfgTable;


/*
 * function declarations 
 */
void            init_cpriConfigInfo(void);
FindVarMethod   var_cpriConfigInfo;
FindVarMethod   var_cpriConfigTable;
FindVarMethod   var_cpriDelayCfgTable;
FindVarMethod   var_cpriSingleCfgTable;
WriteMethod     write_cpriConfigCpriSpeed;
WriteMethod     write_cpriConfigChannelState;
WriteMethod     write_cpriConfigRingTestReq;
WriteMethod     write_cpriConfigCtlWord;
WriteMethod     write_cpriConfigTotalHigh;
WriteMethod     write_cpriConfigTotalLow;
WriteMethod     write_cpriConfigErrHigh;
WriteMethod     write_cpriConfigErrLow;
WriteMethod     write_cpriConfigRowStatus;
WriteMethod     write_cpriDelayCfgAxCInd;
WriteMethod		write_cpriDelayCfgTimeDelayUL;
WriteMethod     write_cpriDelayCfgTimeDelayDL;
WriteMethod     write_cpriDelayCfgT14;
WriteMethod     write_cpriDelayCfgRowStatus;
WriteMethod     write_cpriSingleCfgAxCInd;
WriteMethod     write_cpriSingleCfgOnOrOff;
WriteMethod     write_cpriSingleCfgSingleMode;
WriteMethod     write_cpriSingleCfgRowStatus;

#endif                          /* CPRICONFIGINFO_H */
