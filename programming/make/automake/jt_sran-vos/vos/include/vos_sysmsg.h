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
   2019年 03月 19日 星期二 14:31:25 CST. 
*/
/************************************************************************
 *Copyright (c) 2018 Sylincom.
 *All rights reserved.
 *
 *File name: vos_prv_sysmsg.h
 *Description: 通用消息头定义，子系统之间和子系统内部传递消息均使用此消息头.
 *
 *Current Version: 0.1
 *Author: wujianming  (wujianming@sylincom.com)
 *Date: 2019.01.04
 *
 ************************************************************************/




#ifndef __VOS_SYSMSG_H__
#define __VOS_SYSMSG_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef struct 
{
     USHORT  usMessageType;     /*用于指示发送或接收*/
     USHORT  usPririty;         /*优先级（用户自己定义）,支持4个优先级 */  
     ULONG   ulSrcModuleID;     /*源模块号*/
     ULONG   ulDstModuleID;     /*目标模块号*/
     ULONG   ulSequence;        /*发送序号,RPC专用*/ 

     ULONG   ulSrcSlotID;	    /*源槽位号*/
     ULONG   ulDstSlotID;       /*目的槽位号*/
     UCHAR   ucMsgType;         /*消息类型，如PDU, TIMER, REQUEST, NOTIFY... */
     UCHAR   ucMsgBodyStyle;    /*用于指明消息头和消息体是否是一体的*/
     USHORT  usMsgCode;	        /*消息码，具体消息类型的细分*/
     
     USHORT  usResponseType;    /*是否要求接收方用户应答：ACK/ NOACK/NEGACK*/
     USHORT  usFrameLen;        /*frame content 的长度 ，用户数据区长度*/
     VOID *  ptrMsgBody;        /*消息体指针*/
     USHORT  usAppSeqNum;       /*应用层系列号，用于发送方和接收方请求应答之间建立关联*/
     USHORT  usReserved;        /*保留 ，以后扩充使用*/
} SYS_MSG_S;


#ifdef __cplusplus
}
#endif

#endif /*__SYSDEF_H__*/


