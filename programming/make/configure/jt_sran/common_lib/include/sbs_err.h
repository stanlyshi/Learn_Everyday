/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:40 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:24 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:00 CST. */
#ifndef ERR_H_
#define ERR_H_

/* This file defined the return value of super eNB  */

#define	Return_FAIL	-1


typedef enum returnValue
{
	Return_ALL_RIGHT = 0,	//MAC_ALL_RIGHT, RA_ALL_RIGHT, PDCP_ALL_RIGHT, AM_ALL_RIGHT, RLC_ALL_RIGHT, TM_ALL_RIGHT, UM_ALL_RIGHT
	/* Common module return error define header */
	COMMON_ERR 		 = 1, 	// Common module return error define head,
	COMMON_ERR_MSGQ_STATUS_DELETED,	//from lte_msgq.c #define MSGQ_STATUS_DELETED -2
	COMMON_ERR_NO_MEM,		//from lte_type.h  #define	ERR_NO_MEM -2000	
	COMMON_ERR_FUNC_PARAM,	//from lte_type.h  #define	ERR_FUNC_PARAM	-1999
	COMMON_ERR_SYS_PARAM,	//from lte_type.h  #define	ERR_SYS_PARAM	-1998
	COMMON_ERR_BIT_PACK	, 	//from lte_type.h  #define	ERR_BIT_PACK	        -1997
	/*Common module return error define end*/
	/*RRC module return error define header  */
	RRC_ERR			= 30,	// RRC module return error define header,	
	/*RRC module return error define end*/
	/*RRM module return error define header  */
	RRM_ERR			= 60,
	/*RRM module return error define end*/
	/*PDCP module return error define header  */
	PDCP_ERR		= 90,
	/*PDCP module return error define end*/
	/*RLC module return error define header  */
	RLC_ERR			= 120,	// from rlc_am.h  #define AM_FUNC_ERROR  1
	RLC_ERR_AM_OUT_WIN, 	// from rlc_am.h  #define #define	AM_OUT_WIN	1
	RLC_ERR_AM_RETX_ERR,	// from rlc_am.h  #define AM_FUNC_ERROR  1	
	RLC_ERR_AM_EMPTY_LIST,	// from rlc_am.h  #define	ERR_AM_EMPTY_LIST	-5
	RLC_ERR_AM_RX_PDU,		// from rlc_am.h	#define ERR_AM_RX_PDU	-6
	RLC_ERR_TM_EMPTY_SDU,	// from rlc_tm.h	#define	TM_EMPTY_SDU		1
	RLC_ERR_SDU_DELETE_FAIL,// from rlc_tx.h		#define	SDU_DELETE_FAIL	1
	RLC_ERR_UM_OUT_WIN,		// from rlc_um.h	#define     UM_OUT_WIN     1
	RLC_ERR_UM_DUP_PDU, 	// from rlc_um.h	#define 	UM_DUP_PDU	 2
	RLC_ERR_UM_BIG_SDU, 	// from rlc_um.h	#define 	UM_BIG_SDU	 1
	RLC_ERR_UM_BUFF_EMPTY, 	// from rlc_um.h	#define 	UM_BUFF_EMPTY 2
	RLC_ERR_UM_EMPTY_LIST, 	// from rlc_um.h	#define 	UM_EMPTY_LIST -5
	RLC_ERR_UM_RX_PDU,		// from rlc_um.h	#define 	UM_RX_PDU -6
	/*RLC module return error define end*/
	/*MAC module return error define header  */
	MAC_ERR			= 150	
	/*MAC module return error define end*/
}returnValue_e;

#ifdef CHECK0810

/*lte_hash.c*/
#define true     1
#define false    0


/*lte_msgq.c*/
#define MSGQ_STATUS_DELETED -2

/*lte_type.h */
#define FALSE 0
#define TRUE 1

#define	ERR_NO_MEM		-2000
#define	ERR_FUNC_PARAM	-1999
#define	ERR_SYS_PARAM	-1998
#define	ERR_BIT_PACK	-1997

		
//zrt

/* harq.h ------------------------------------------------------------- */

/* harq.c ------------------------------------------------------------- */

/* henb_dl_sched.h ------------------------------------------------------------- */
#define		MAC_ALL_RIGHT	 0
/* henb_dl_sched.c ------------------------------------------------------------- */

/* henb_ul_sched.h ------------------------------------------------------------- */

/* henb_ul_sched.c ------------------------------------------------------------- */

/* mac_pdu_construct.h ------------------------------------------------------------- */

/* mac_pdu_construct.c ------------------------------------------------------------- */

/* mac_pdu_rx.h ------------------------------------------------------------- */

/* mac_pdu_rx.c ------------------------------------------------------------- */

/* mac_pdu_tx.h ------------------------------------------------------------- */

/* mac_pdu_tx.c ------------------------------------------------------------- */

/* ra.h ------------------------------------------------------------- */
#define RA_ALL_RIGHT 0
#define CONTEN_PREAMBLE 0
/* ra.c ------------------------------------------------------------- */
#define WRONG -2
/* rrm_dra.h ------------------------------------------------------------- */

/* rrm_dra.c ------------------------------------------------------------- */

/* fs_lowmac_ctrl.h ------------------------------------------------------------- */

/* fs_lowmac_ctrl.c ------------------------------------------------------------- */

/* lowmac_cfg.h ------------------------------------------------------------- */

/* lowmac_cfg.c ------------------------------------------------------------- */

/* packet_mac_lte.h ------------------------------------------------------------- */

/* sched_fs_lowmac_ctrl.c ------------------------------------------------------------- */

/* mac_lte_logger.c --------------------*/

//mayang
/*from pdcp.h*****************************************************************/
#define	PDCP_ALL_RIGHT	0  /*my20150807 all right return value*/

/*from rlc_am.h****************************************************/
	
#define	AM_ALL_RIGHT		0 /* return value: all right in AM   my20180803*/
#define AM_FUNC_ERROR		1 /* return value: function error in AM   my20180803*/
#define	AM_OUT_WIN			1	/* no caller?   my20180803*/
#define	AM_RETX_ERR			1	/*retransaction error in AM no caller?  my20180803*/
#define	AM_DUP_PDU			2   /*no caller   my20180803*/
#define	ERR_AM_EMPTY_LIST	-5  /*when tx_sdu_list is empty, error in AM my20180803*/
#define	ERR_AM_RX_PDU		-6  /* return invalid value when the*/

/*from rlc_mgmt.h****************************************************************/

#define	RLC_ALL_RIGHT	0  /*   my20150807 return value*/
/*from  rlc_tm.h******************************************************************/
#define	TM_ALL_RIGHT		0
#define	TM_EMPTY_SDU		1


/*from  rlc_tx.h*******************************************************************/

#define	NUM_ZERO			0
#define	NUM_ONE				1
#define	SDU_DELETE_FAIL		1 /*my20150807 return value for failing to delete SDU*/
/*from  rlc_um.h*******************************************************************/

#define	UM_ALL_RIGHT		0
#define	UM_OUT_WIN			1	
#define	UM_DUP_PDU			2
#define	UM_BIG_SDU			1
#define	UM_BUFF_EMPTY		2
#define	ERR_UM_EMPTY_LIST	-5
#define	ERR_UM_RX_PDU		-6

#endif

#endif






























