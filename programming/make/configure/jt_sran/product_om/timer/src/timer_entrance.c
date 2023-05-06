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
 *File name: timer_init.h
 *Description: the entrance of timer.
 *
 *Current Version: 0.l
 *Author: WangCheng (wangcheng2017@ict.ac.cn)
 *Date: 2018.08.27
 *
 ************************************************************************/
/* Dependencies ------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
    
#include "sbs_type.h"
#include "sbs_log.h"
#include "om_common_h.h"
#include "sbs_timer.h"

/* Functions ------------------------------------------------------------- */
void *timer_entrance()
{
    // init timer mutex
    if (init_timer_mutex() != FUNC_OK)
    {
        log_msg(LOG_ERR, TIMER, "Init timer mutex failed.\n");
        return NULL;
    }
    
    while(1)
    {       
        sbs_timers_update();
        usleep(25000);
    }



    return NULL;
}
