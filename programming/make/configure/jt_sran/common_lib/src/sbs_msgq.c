/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:31 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:41 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:25 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:00 CST. */
/*******************************************************************************
 ** 
 ** Copyright (c) 2010-2015 ICT/CAS.
 ** All rights reserved.
 **
 ** File name: sbs_msgq.c
 ** Description: Source file for wrapping message queue functions.
 **
 ** Current Version: 1.0
 ** Author: zhangzongshuai@ict.ac.cn
 ** Date: 2015-09
 **
 **
 ******************************************************************************/

#include "sbs_msgq.h"
#include "sbs_log.h"
#include "sbs_type.h"
#include "sbs_msgq.h"
#include "sbs_log.h"
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>


/*******************************************************************************
 ** 
 Functions for Linux non-Kernel
 **
 ******************************************************************************/

/* Dependencies ------------------------------------------------------------- */

/* Constants ---------------------------------------------------------------- */

/* Types -------------------------------------------------------------------- */

/* Macros ------------------------------------------------------------------- */
#define MAX_MSGQ_NAME_LEN         256
/* Globals ------------------------------------------------------------------ */

/* Functions ---------------------------------------------------------------- */

/******************************************************************************
 * Open a message queue with a given name.
 *
 * Input: msgq_name: name of message queue to be opened, i.e. /mymsgqname
 *        max_msg_num: Max messages that can be queued.
 *        max_msg_len: Max bytes in a message.
 * 
 * Output: return message queue identifier,if success.
 *                -1, if error.
 ******************************************************************************/
MsgqType open_msgq(char *msgq_name, INT32 max_msg_num, 
								INT32 max_msg_len)
{
    MsgqType msgq_id;
    struct mq_attr attr;
    char mqname[MAX_MSGQ_NAME_LEN];
    struct passwd *pwd = NULL;

    if (!msgq_name) {
        return MSGQ_ERROR;
    }

    if ((pwd = getpwuid(geteuid())) == NULL) {
	    return MSGQ_ERROR;
    }

    snprintf(mqname, MAX_MSGQ_NAME_LEN, "%s_%s", msgq_name, pwd->pw_name);
    //snprintf(mqname, MAX_MSGQ_NAME_LEN, "%s", msgq_name);
    //printf("mqname %s\n",mqname);
    attr.mq_maxmsg = max_msg_num;
    attr.mq_msgsize = max_msg_len;

    /* Open a message queue */
    msgq_id=mq_open(mqname,O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, &attr);
    //printf("msgq_id %ld\n",msgq_id);
    mq_getattr(msgq_id, &attr);
    //printf("Create MQ success!\nMax msgs = %ld, Max bytes/msg = %ld\n", attr.mq_maxmsg, attr.mq_msgsize);
    //printf("max #msgs = %ld, max #bytes/msg = %ld, #nums of msg currently on queue = %ld\n", attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
    if (msgq_id == (mqd_t)-1) {
    	log_msg(LOG_ERR, 0, "open msg_queue err:%s\n", msgq_name);
    	perror("create msgq_id");
        return MSGQ_ERROR;
    }
    endpwent();

    return msgq_id;
}

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
INT32 send_msgq(MsgqType msgq_id, char *buffer_p, UINT32 msg_len,
				INT32 time_out, INT32 prio)
{
    struct mq_attr attr;
    mq_getattr(msgq_id, &attr);
    //printf("#flags = %ld, max #msgs = %ld, max #bytes/msg = %ld, #nums of msg currently on queue = %ld\n", attr.mq_flags, attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
    if (!buffer_p) {
        return -1;
    }
    if (time_out == NO_WAIT) {
        attr.mq_flags = O_NONBLOCK;
        mq_setattr(msgq_id, &attr, NULL);

        return mq_send(msgq_id, buffer_p, msg_len, prio);
    } else if(time_out == WAIT_FOREVER) {
        attr.mq_flags = 0;

        mq_setattr(msgq_id, &attr, NULL);
        //printf("max #msgs = %ld, max #bytes/msg = %ld, #nums of msg currently on queue = %ld\n", attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
        return mq_send(msgq_id, buffer_p, msg_len, prio);
    } else {
        struct timespec tms;

        tms.tv_sec = time_out / CLOCKS_PER_SEC;
        tms.tv_nsec = (time_out % CLOCKS_PER_SEC) *
                        (1000000000 / CLOCKS_PER_SEC);
        return mq_timedsend(msgq_id, buffer_p, msg_len, prio, &tms);
    }
}

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
INT32 receive_msgq(MsgqType msgq_id, char *buffer_p, 
								 UINT32 msg_len, INT32 time_out)
{
    struct mq_attr attr;
    unsigned int prio = 0;
    long rc;

    mq_getattr(msgq_id, &attr);
    if (!buffer_p) {
        return -1;
    }
    if (time_out == NO_WAIT) {
        attr.mq_flags = O_NONBLOCK;
        mq_setattr(msgq_id, &attr, NULL);
		
		/*
        while((rc= mq_receive(msgq_id, buffer_p, msg_len, &prio)))
        {
            //printf("rc= %ld\n", rc);
            //printf("msg is %s\n", buffer_p);
            //mq_setattr(msgq_id, &attr, NULL);
            //printf("max #msgs = %ld, max #bytes/msg = %ld, #nums of msg currently on queue = %ld\n", attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
            sleep(1);
        }
		*/

        return mq_receive(msgq_id, buffer_p, msg_len, NULL);
    } else if(time_out == WAIT_FOREVER) {
        attr.mq_flags = 0;
        mq_setattr(msgq_id, &attr, NULL);

        return mq_receive(msgq_id, buffer_p, msg_len, NULL);
    } else {
        struct timespec tms;

        tms.tv_sec = time_out / CLOCKS_PER_SEC;
        tms.tv_nsec = (time_out % CLOCKS_PER_SEC) *
                        (1000000000 / CLOCKS_PER_SEC);

        return mq_timedreceive(msgq_id, buffer_p, msg_len, NULL, &tms);
    }
}

/*******************************************************************************
 * Get the number of messages queued to a message queue.
 *
 * Input: msgq_id: identifier of message queue to examine.
 *
 * Output: return the number of message queued, if success.
 *                -1, if error.
 ******************************************************************************/
INT32 get_msgq_num(MsgqType msgq_id)
{
	struct mq_attr attr;
    if (mq_getattr(msgq_id, &attr) < 0) {
        return -1;
    }
    return attr.mq_curmsgs;
}

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
INT32 close_msgq(char *msgq_name, MsgqType msgq_id)
{
    INT32 ret;
	char mqname[MAX_MSGQ_NAME_LEN];
	struct passwd *pwd = NULL;

    /* Close */
    ret = mq_close(msgq_id);

    if (!msgq_name) {
        return MSGQ_ERROR;
    }

    if ((pwd = getpwuid(geteuid())) == NULL) {
        return MSGQ_ERROR;
    }

    snprintf(mqname, MAX_MSGQ_NAME_LEN, "%s_%s", msgq_name, pwd->pw_name);
    /* Delete */
    if (mqname!=NULL) {
        mq_unlink(mqname);
    }

    return ret;
}

