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
/*******************************************************************************
 ** 
 ** Copyright (c) 2015 ICT/CAS.
 ** All rights reserved.
 **
 ** File name: sbs_msgq.h
 ** Description: Header file for wrapping message queue functions.
 **
 ** Current Version: 1.0
 ** Author: zhangzongshuai@ict.ac.cn
 ** Date: 2015-09
 **
 **
 ******************************************************************************/
#ifndef SBS_MSGQ_H
#define SBS_MSGQ_H
/*******************************************************************************
 ** 
 Declaration for all **
 ******************************************************************************/

/* Dependencies ------------------------------------------------------------- */
#include "sbs_type.h"
/* Constants ---------------------------------------------------------------- */
#define MSGQ_STATUS_DELETED -2
/* Types -------------------------------------------------------------------- */
/* Macros ------------------------------------------------------------------- */
/* Globals ------------------------------------------------------------------ */
/* Functions ---------------------------------------------------------------- */



/*******************************************************************************
 ** 
 Declaration for VxWorks **
 ******************************************************************************/

/*******************************************************************************
 ** 
 Declaration for Linux(both kernel & non-kernel) **
 ******************************************************************************/
/* __KERNEL && !__KERNEL */
/* Dependencies ------------------------------------------------------------- */
/* Constants ---------------------------------------------------------------- */
#define MSG_QUEUE_NUM   128 /* Limit max. number of message queues to 128 */
#define NO_WAIT 0
#define WAIT_FOREVER -1
#define MSG_PRI_NORMAL 0
#define MSG_PRI_URGENT (MQ_PRIO_MAX - 1)

/* Types -------------------------------------------------------------------- */
/* Macros ------------------------------------------------------------------- */
/* Globals ------------------------------------------------------------------ */
/* Functions ---------------------------------------------------------------- */


/* Dependencies ------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <fcntl.h>

/* Constants ---------------------------------------------------------------- */
#define MSGQ_STATUS_NORMAL 0
#define MSGQ_STATUS_DELETE -1
#define MSGQ_STATUS_FULL -2
#define MSGQ_STATUS_EMPTY -3
#define MSGQ_MSG_SIZE 10

#define MSG_QUEUE_FLAG O_RDWR | O_CREAT
#define MSG_QUEUE_MODE S_IRUSR | S_IWUSR
#define MSGQ_HNAME_LEN  8   /* Length of head part of msg queue name */

#define MSGQ_ERROR      -1

typedef mqd_t MsgqType;


/* Macros ------------------------------------------------------------------- */

/* Globals ------------------------------------------------------------------ */

/* Functions ---------------------------------------------------------------- */
/******************************************************************************
 * Open a message queue with a given name.
 *
 * Input: msgq_name: name of message queue to be opened, i.e. /mymsgqname
 *        max_msg_num: Max messages that can be queued.
 *        max_msg_len: Max bytes in a message.
 *
 * Output: return message queue identifier,
 *                NULL, if error.
 ******************************************************************************/
MsgqType open_msgq(char *msgq_name, INT32 max_msg_num, INT32 max_msg_len);


/*******************************************************************************
 * Send a message to a message queue.
 *
 * Input: msgq_id: identifier of message queue on which to send.
 *        buffer_p: pointer to message to be sent.
 *        msg_len: length of message in bytes.
 *        time_out: ticks to wait.
 *                  this argument can also be NO_WAIT or WAIT_FOREVER.
 *        prio: priority of message to be sent.
 *              in LINUX: prio can be MSG_PRI_NORMAL, MSG_PRI_URGENT,
 *                        or an integer from 1 to MQ_PRIO_MAX.
 *
 * Output: return 0, if success.
 *                -1, if send failed.
 ******************************************************************************/
INT32 send_msgq(MsgqType , char *, UINT32 ,INT32 , INT32 );


/*******************************************************************************
 * Receive a message from a message queue.
 *
 * Input: msgq_id: identifier of message queue from which to receive.
 *        buffer_p: pointer to buffer to receive message.
 *        msg_len: length of buffer which can hold received message.
 *        time_out: ticks to wait.
 *                  this argument can also be NO_WAIT or WAIT_FOREVER.
 *
 * Output: return 0, if success.
 *                -1, if receive failed.
 ******************************************************************************/
INT32 receive_msgq(MsgqType, char *,UINT32, INT32 );


/*******************************************************************************
 * Get the number of messages queued to a message queue.
 *
 * Input: msgq_id: identifier of message queue to examine.
 *
 * Output: return the number of message queued, if success.
 *                -1, if error.
 ******************************************************************************/
INT32 get_msgq_num(MsgqType);


/*******************************************************************************
 * Close and delete a message queue.
 *
 * Input: msgq_name: name of message queue to be deleted,
 *                   if NULL, do not delete it.
 *        msgq_id: identifier of message queue to be closed.
 *
 * Output: return 0, if success.
 *               -1, if close or deletion failed.
 ******************************************************************************/
INT32 close_msgq(char *msgq_name, MsgqType msgq_id);

#endif
