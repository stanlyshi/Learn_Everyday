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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:30 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:04 CST. */
/************************************************************************
 *Copyright (c) 2018 Sylincom.
 *All rights reserved.
 *
 *File name: rrus_mgmt_init.h
 *Description: the entrance of rru_mgmt.
 *
 *Current Version: 0.l
 *Author: wangcheng (wangcheng2017@ict.ac.cn)
 *Date: 2018.08.26
 *
 ************************************************************************/
/* Dependencies -------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

#include "sbs_type.h"
#include "sbs_msgq.h"
#include "sbs_log.h"
#include "om_common_h.h"


/* Declarations--------------------------------------------------------------*/
extern INT32    handle_from_cell_mgmt_msg(char*, int);
extern INT32    handle_from_cfig_mgmt_msg(char*, int);
extern INT32    handle_from_main_ctrl_msg(char*, int);
extern INT32    handle_from_mont_mgmt_msg(char*, int);
//extern INT32    handle_from_inet_mgmt_msg(char*, int);
extern INT32    handle_from_timer_msg(char*, int);


extern INT32    handle_from_mac_msg(char*, int);
extern INT32    handle_from_data_fpga_msg(char*, int);
extern INT32    handle_from_rru_msg(char*, int);

//extern INT32    rrus_mgmt_timer_init();
extern INT32    rrus_mgmt_inet_init();




/* Functions ------------------------------------------------------------- */

void *rrus_mgmt_entrance()
{
    char    buffer[MAX_MSGQ_MSG_LEN];      
    INT32   rl = 0;

    if (rrus_mgmt_inet_init() == FUNC_ERR)
    {
        log_msg(LOG_ERR, RRUS_MGMT, "Function rrus_mgmt_inet_init return error.\n");
		return NULL;
    }


    log_msg(LOG_INFO, RRUS_MGMT, "RRUS_MGMT init success, enter while...\n");

    while (1)
    {
        usleep(250000);
        memset(buffer, 0, MAX_MSGQ_MSG_LEN);
        if ((rl = receive_msgq(MSGQ_ID_RRUS_MGMT, buffer, MAX_MSGQ_MSG_LEN, WAIT_FOREVER)) == FUNC_ERR)
        {
            log_msg(LOG_ERR, RRUS_MGMT, "Receive message from RRUS_MGMT_MSG_QUEUE failed in %s function at %d lines.\n", __FUNCTION__, __LINE__);
            continue;
        }
        else 
        {
            CommMsgHead* tmp = (CommMsgHead*)buffer;
            
            switch (tmp->msg_type)
            {
                case MODULE_CELL_MGMT:
                {
                    if (handle_from_cell_mgmt_msg((char*)buffer, rl) == FUNC_ERR)
                    {
                        log_msg(LOG_ERR, RRUS_MGMT, "Function handle_from_cell_mgmt_msg return error.\n");
                    }

                    break;
                }                        

                case MODULE_CFIG_MGMT:
                {
                    if (handle_from_cfig_mgmt_msg((char*)buffer, rl) == FUNC_ERR)
                    {
                        log_msg(LOG_ERR, RRUS_MGMT, "Function handle_from_cfig_mgmt_msg return error.\n");
                    }

                    break;
                }

                case MODULE_MAIN_CTRL:
                {
                    if (handle_from_main_ctrl_msg((char*)buffer, rl) == FUNC_ERR)
                    {
                        log_msg(LOG_ERR, RRUS_MGMT, "Function handle_from_main_ctrl_msg return error.\n");
                    }

                    break;
                }

                case MODULE_BORD_MGMT:
                {
                    if (handle_from_mont_mgmt_msg((char*)buffer, rl) == FUNC_ERR)
                    {
                        log_msg(LOG_ERR, RRUS_MGMT, "Function handle_from_mont_mgmt_msg return error.\n");
                    }

                    break;
                }

                case MODULE_DATA_FPGA:
                {
                    if (handle_from_data_fpga_msg((char*)buffer, rl) == FUNC_ERR)
                    {
                        log_msg(LOG_ERR, RRUS_MGMT, "Function handle_from_data_fpga_msg return error.\n");
                    }

                    break;
                }

                case MODULE_MAC:
                {
                    if (handle_from_mac_msg((char*)buffer, rl) == FUNC_ERR)
                    {
                        log_msg(LOG_ERR, RRUS_MGMT, "Function handle_from_mac_msg return error.\n");
                    }

                    break;
                }

                case MODULE_RRU:
                {
                    if (handle_from_rru_msg((char*)buffer, rl) == FUNC_ERR)
                    {
                        log_msg(LOG_ERR, RRUS_MGMT, "Function handle_from_rru_msg return error.\n");
                    }

                    break;
                }

                case MODULE_TIMER:
                {
                    if (handle_from_timer_msg((char*)buffer, rl) == FUNC_ERR)
                    {
                        log_msg(LOG_ERR, RRUS_MGMT, "Function handle_from_timer_msg return error.\n");
                    }

                    break;
                }


                default:
                	log_msg(LOG_ERR, RRUS_MGMT, "Receive out of range message and msg_type is %d.\n", ntohs(tmp->msg_type));
                    break;
            }
        }
    }
}

