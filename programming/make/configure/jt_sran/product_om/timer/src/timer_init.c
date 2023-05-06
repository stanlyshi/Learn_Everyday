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
 *File name: timer_init.c
 *Description: the entrance of timer.
 *
 *Current Version: 0.l
 *Author: WangCheng (wangcheng2017@ict.ac.cn)
 *Date: 2018.08.27
 *
 ************************************************************************/
 /* Dependencies ------------------------------------------------------------- */
 #include "sbs_task.h"
 #include "om_common_h.h"
 #include "sbs_type.h"
 #include "sbs_log.h"

 /*******************************declarations*****************************/
 extern TaskType THREAD_TIMER;
 extern UINT8 flag_thread_timer;
 extern void *timer_entrance();

 /* Functions ------------------------------------------------------------- */
 INT32 timer_module_init()
 {
     // TIMER thread
     if ((THREAD_TIMER = create_pthread(1, SCHED_RR, 1, timer_entrance, NULL)) == FUNC_ERR)
     {
         log_msg(LOG_ERR, MAIN_CTRL, "Create THREAD_TIMER failed.\n");
         return FUNC_ERR;
     }
     else
     {
         log_msg(LOG_INFO, MAIN_CTRL, "Create THREAD_TIMER success, the thread_id is %d\n", THREAD_TIMER);
         flag_thread_timer = 1;
     }

     return FUNC_OK;
}
