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
/************************************************************************
 *Copyright (c) 2018 Sylincom.
 *All rights reserved.
 *
 *File name: rrus_mgmt_api_data_fpga.h
 *Description: the api of rrus_mgmt with data_fpga.
 *
 *Current Version: 0.l
 *Author: zhanghenghao (zhanghenghao@sylincom.com)
 *Date: 2018.09.04
 *
 ************************************************************************/

#ifndef RRUS_MGMT_API_DATA_FPGA_H
#define RRUS_MGMT_API_DATA_FPGA_H

/* Dependencies---------------------------------------------------------*/
#include "sbs_type.h"


/* Functions------------------------------------------------------------*/
extern INT32 handle_data_fpga_rru_mac_rsp(UINT16 cpri_index, void* buf);


extern INT32 handle_data_fpga_frnum_rsp(void*, UINT32);


extern INT32 handle_data_fpga_cpri_init_cfg_rsp(UINT16 cpri_index, void* buf);

extern INT32 handle_data_fpga_axc_channel_cfg_rsp(UINT16 cpri_index, void* buf);

extern INT32 handle_data_fpga_cpri_state_query_rsp(UINT16 cpri_index, void* buf);

extern INT32 handle_data_fpga_time_delay_measure_rsp(UINT16 cpri_index, void* buf);

extern INT32 handle_data_fpga_time_delay_cfg_rsp(UINT16 cpri_index, void* buf);

extern INT32 handle_data_fpga_ring_test_rsp(UINT16 cpri_index, void* buf);

extern INT32 handle_data_fpga_ser_test_rsp(UINT16 cpri_index, void* buf);



extern INT32 handle_data_fpga_work_mode_cfg_rsp(void*, UINT32);

extern INT32 handle_data_fpga_single_send_cfg_rsp(UINT16 cpri_index, void* buf);

extern INT32 handle_data_fpga_iq_out_cfg_rsp(void*, UINT32);


#endif
