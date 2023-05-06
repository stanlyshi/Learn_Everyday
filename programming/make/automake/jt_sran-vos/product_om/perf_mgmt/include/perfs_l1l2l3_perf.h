/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:29 CST. */
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
/***********************************************************************************************************************
** 
** Copyright (c) 2008-2019 ICT/CAS.
** All rights reserved.
**
** File name: perfs_l1l2l3_perf.h
** Description:  performance statistics module
**
** Current Version: 
** $Revision$
** Author: Rong Tao
** Date: 2019.02 - 
**
***********************************************************************************************************************/
#ifndef _PERFS_L1L2L3_PERF_H
#define _PERFS_L1L2L3_PERF_H

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include "perfs_types.h"


/* Constants ---------------------------------------------------------------------------------------------------------*/
/* Types -------------------------------------------------------------------------------------------------------------*/
typedef struct {
    struct {
        UINT8 i     /*integer*/;
        UINT8 d;    /*decimal*/
    }__attribute__((packed)) ebr;   /* error block rate */
    
    struct {
        UINT64  pdcp_ul; /*PDCP uplink rate*/
        UINT64  pdcp_dl; /*PDCP downlink rate*/
        UINT64  rlc_ul; /*RLC uplink rate*/
        UINT64  rlc_dl; /*RLC downlink rate*/
        UINT64  mac_ul; /*MAC uplink rate*/
        UINT64  mac_dl; /*MAC downlink rate*/
    }__attribute__((packed)) lr;    /* layers rate */

    UINT32 rlc_buf;
    
    struct {
        UINT8 i     /*integer*/;
        UINT8 d;    /*decimal*/
    }__attribute__((packed)) bw_occupy; /*bandwidth occupancy*/

    UINT8 ue_offs;  /*UE����ʣ����*/
    UINT8 ue_ons;   /*UE��ǰ������*/

    struct {
        UINT8 i     /*integer*/;
        UINT8 d;    /*decimal*/
    }__attribute__((packed)) ue_asr;   /*UE access success rate*/
    
    /*maybe some more*/
}__attribute__((packed)) perf_mac_t;


typedef struct {
    /**
     *  dl_through: ������������С��Ϊ��λ��kbps
     *      dl_through[0]: ����0������
     *      dl_through[1]: ����1������
     *      dl_through[2]: ��������
     *
     *  ul_through: ��������������С��Ϊ��λ��kbps
     */
    struct {
        UINT32 i     /*integer*/;
        UINT8 d;    /*decimal*/
    }__attribute__((packed)) dl_through[3], ul_through; 

    /**
     *  sn: ��������ȣ���Χ��-100~+100��
     *      sn[0]:  ��Χ��ֵ
     *      sn[1]:  ��Χ��ֵ
     */
    struct {
        UINT8 i     /*integer*/;
        UINT8 d;    /*decimal*/
    }__attribute__((packed)) sn[2]; 

    /**
     *  ���С��������ݴ��������ֽڣ�
     *  downlink��uplink data transmission volume
     */
    UINT64 dl_vol;  
    UINT64 ul_vol; 

    /*maybe some more*/
}__attribute__((packed)) perf_realphy_t;
    
typedef struct {
    
    //...etc
}__attribute__((packed)) perf_simuphy_t;



/* Macros ------------------------------------------------------------------------------------------------------------*/
/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/
/***********************************************************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 **********************************************************************************************************************/


#endif /*<_PERFS_L1L2L3_PERF_H>*/
