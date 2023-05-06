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
/**
 * @file vos_string.h
 * @brief vos string 头文�?
 * @details 包含常用字符串处理函�?
 * @version 1.0
 * @author  wujianming  (wujianming@sylincom.com)
 * @date 2019.01.09
 * @copyright 
 *    Copyright (c) 2018 Sylincom.
 *    All rights reserved.
*/


#ifndef __VOS_STRING_H__
#define __VOS_STRING_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

//#include "vos_types.h"

/** �? Memcpy */
VOID *VOS_MemCpy(VOID * pcDest, const VOID * pcSrc, ULONG ulLen);

/** �? memmove */
VOID *VOS_MemMove(VOID * pcDest, const VOID * pcSrc, ULONG ulLen);

/** �? Memset */
VOID *VOS_MemSet(VOID *pcBuf, CHAR cWhat, ULONG ulHowMany);

/** �? Memset(&var,0,sizeof(var)) */
VOID *VOS_MemZero(VOID *pcBuf, ULONG ulHowMany);

/** �? memcmp */
LONG VOS_MemCmp(const VOID *pcBuf1, const VOID *pcBuf2, ULONG ulLen);

/** �? strchr */
CHAR *VOS_StrChr(const CHAR *pcStr, CHAR cCh);

/** �? strrchr */
CHAR *VOS_StrRChr(const CHAR *pcStr, CHAR cCh);

/** �? strcat */
CHAR *VOS_StrCat(CHAR *pcDest, const CHAR *pcAppend);

/** �? strncat */
CHAR *VOS_StrnCat(CHAR *pcDest, const CHAR *pcAppend, ULONG ulLen);

/** �? Strcpy */
CHAR *VOS_StrCpy(CHAR *pcDest, const CHAR *pcSrc);

/** �? Strncpy */
CHAR *VOS_StrnCpy(CHAR * pcDest,const CHAR *pcSrc, ULONG ulCount);

/** �? Strncmp */
ULONG VOS_StrnCmp(const CHAR * cs, const CHAR * ct, ULONG count);

/** �? Strcmp */
LONG VOS_StrCmp(const CHAR *szStr1, const CHAR *szStr2);

/** �? Strlen */
ULONG VOS_StrLen(const CHAR *szStr);

/** �? strstr */
CHAR *VOS_StrStr(const CHAR *szStr, const CHAR *szToFind);

/** �? Strncmp */
CHAR *VOS_StrpBrk(const CHAR *szStr, const CHAR *szToFind);

/** �? atol */
LONG VOS_AtoL(const CHAR *szString);

/** �? strtol */
LONG VOS_StrToL(const CHAR *cp,CHAR **endp,INT base);

/** �? strtoul */
ULONG VOS_StrToUL(const CHAR *szNptr, CHAR **szEndptr, INT ulBase );

/** �? strtoll */
LONGLONG VOS_StrToLL(const CHAR *cp,CHAR **endp,INT base);

/** �? strtoull */
ULONGLONG VOS_StrToULL(const CHAR *cp,CHAR **endp,INT base);

/** �? strtok */
CHAR * VOS_StrToK(CHAR* strToken, const CHAR * strDelimit);

/** �? atoi */
INT VOS_AtoI(const CHAR* s);

/** �? strncasecmp */
ULONG VOS_StrnCaseCmp(const CHAR*, const CHAR*, ULONG ulLen);

/** �? strcasecmp */
LONG VOS_StrCaseCmp(const CHAR *cs, const CHAR *ct);

/** �? strnlen */
ULONG VOS_StrnLen(const CHAR * s, ULONG count);

/** 将字符串转换为小�? */
CHAR *VOS_StrToLower(CHAR *src);

/** 将字符串转换为大�? */
CHAR *VOS_StrToUpper(CHAR *src);


/** 
 * 复制字符串，申请大小为strlen(src)+1内存，然后将字符串拷入，并返�?
 * @param[in]   src       源字符串
 * @param[in]   mod_id    模块ID
 * @return      成功返回新字符串的地�?，失败返回NULL
 */
CHAR *VOS_StrDup (CHAR *src,INT mod_id);


/** 
 * 复制字符串，并转换为小写，申请大小为strlen(src)+1内存，然后将字符串拷入，并返�?
 * @param[in]   src       源字符串
 * @param[in]   mod_id    模块ID
 */
CHAR *VOS_StrDupToLower (CHAR *src,INT mod_id);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*__VOS_STRING_H__*/
