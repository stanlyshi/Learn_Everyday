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
 * @file vos_types.h
 * @brief vos 类型定义 头文�?
 * @details 定义VOS 基本类型
 * @version 1.0
 * @author  wujianming  (wujianming@sylincom.com)
 * @date 2019.01.10
 * @copyright 
 *    Copyright (c) 2018 Sylincom.
 *    All rights reserved.
*/


#include "vos_sys_diff.h"
#include <stdint.h>

#ifndef _VOS_TYPES_H_
#define _VOS_TYPES_H_

#ifdef __cplusplus
    extern "C" {
#endif
#include <stddef.h>
#ifndef VOS_VER
#define VOS_VER 0x0200
#endif

#ifndef VOS_YES
#define VOS_YES 1
#endif

#ifndef VOS_NO
#define VOS_NO 0
#endif

#ifndef VOS_OK
#define VOS_OK 0
#endif

#ifndef VOS_ERROR
#define VOS_ERROR (-1)
#endif

typedef int8_t    INT8;
typedef int16_t   INT16;
typedef int32_t   INT32;
typedef int64_t   INT64;
typedef uint8_t   UINT8;
typedef uint16_t  UINT16;
typedef uint32_t  UINT32;
typedef uint64_t  UINT64;

#ifndef HAVE_CHAR
#define HAVE_CHAR
typedef char                CHAR;
#endif

#ifndef HAVE_BYTE
#define HAVE_BYTE
typedef unsigned char       BYTE;
#endif

#ifndef HAVE_SHORT
#define HAVE_SHORT
typedef short               SHORT;
#endif

#ifndef HAVE_WORD
#define HAVE_WORD
typedef unsigned short      WORD;
#endif

#ifndef HAVE_LONG
#define HAVE_LONG
typedef long                LONG;
#endif

#ifndef HAVE_DWORD
#define HAVE_DWORD
typedef unsigned long       DWORD;
#endif

#ifndef HAVE_INT
#define HAVE_INT
typedef int                 INT;
#endif

#ifndef HAVE_CONST
#define HAVE_CONST
#define CONST               const
#endif

#ifndef HAVE_PULONG
#define HAVE_PULONG
typedef unsigned long       *PULONG;
#endif

#ifndef HAVE_LPSTR
#define HAVE_LPSTR
typedef char                *LPSTR;
#endif

#ifndef HAVE_LPCSTR
#define HAVE_LPCSTR
typedef const char          *LPCSTR;
#endif

#ifndef HAVE_HANDLE
#define HAVE_HANDLE
typedef void                *VOS_HANDLE;
#endif

#ifndef HAVE_FALSE
#define HAVE_FALSE
#define FALSE               0
#endif

#ifndef HAVE_TRUE
#define HAVE_TRUE
#define TRUE                1
#endif

#ifndef HAVE_LRESULT
#define HAVE_LRESULT
typedef LONG                LRESULT;
#endif

#ifndef HAVE_UINT8
#define HAVE_UINT8
typedef unsigned char uint8;
#endif

#ifndef HAVE_UINT16
#define HAVE_UINT16
typedef unsigned short uint16;
#endif

#ifndef HAVE_UINT32
#define HAVE_UINT32
typedef unsigned int uint32;
#endif

#ifndef HAVE_FLOAT
#define HAVE_FLOAT
typedef float               FLOAT;
#endif

#ifdef SOCKLEN_T_UNSIGNED
//typedef	unsigned int socklen_t;
#else
//typedef	int socklen_t;
#endif


#ifndef NULL
#if defined __GNUG__
#define NULL (__null)
#else
#if !defined(__cplusplus) && 0
#define NULL ((void*)0)
#else
#define NULL (0)
#endif
#endif
#endif

#if OS_WINDOWS/* OS_WINDOWS */

#ifndef HAVE_longlong
#define HAVE_longlong
typedef __int64 longlong;
#endif

#ifndef HAVE_ulonglong
#define HAVE_ulonglong
typedef unsigned __int64 ulonglong;
#endif

#ifndef HAVE_LONGLONG
#define HAVE_LONGLONG
typedef __int64 LONGLONG;
#endif

#ifndef HAVE_ULONGLONG
#define HAVE_ULONGLONG
typedef unsigned __int64 ULONGLONG;
#endif

#ifndef HAVE_INT
#define HAVE_INT
typedef int INT;
#endif

/*#ifndef SOCKET
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
#endif
*/
typedef char *caddr_t;

#define inline _inline
#define _inline _inline
#define __inline__ _inline


/*windows not suport this, so add these*/
#define aligned(a) 
#define __attribute(a) 
#define __attribute__(a)

#define __const__  const
#define __signed__ signed
#define __const__ const

#elif  defined(OS_VXWORKS) || defined (OS_LINUX)

#ifndef HAVE_DOUBLE
#define HAVE_DOUBLE
typedef double              DOUBLE;
#endif

#ifndef HAVE_PUSHORT
#define HAVE_PUSHORT
typedef unsigned short      *PUSHORT;
#endif

#ifndef HAVE_PUCHAR
#define HAVE_PUCHAR
typedef unsigned char       *PUCHAR;
#endif

#ifndef HAVE_FALSE
#define HAVE_FALSE
#define FALSE               0
#endif

#ifndef HAVE_TRUE
#define HAVE_TRUE
#define TRUE                1
#endif

#ifndef HAVE_PFLOAT
#define HAVE_PFLOAT
typedef FLOAT               *PFLOAT;
#endif

#ifndef HAVE_PBOOL
#define HAVE_PBOOL
typedef int                 *PBOOL;
#endif

#ifndef HAVE_LPBOOL
#define HAVE_LPBOOL
typedef int                 *LPBOOL;
#endif

#ifndef HAVE_PBYTE
#define HAVE_PBYTE
typedef BYTE                *PBYTE;
#endif

#ifndef HAVE_LPBYTE
#define HAVE_LPBYTE
typedef BYTE                *LPBYTE;
#endif

#ifndef HAVE_PINT
#define HAVE_PINT
typedef int                 *PINT;
#endif

#ifndef HAVE_LPINT
#define HAVE_LPINT
typedef int                 *LPINT;
#endif

#ifndef HAVE_PWORD
#define HAVE_PWORD
typedef WORD                *PWORD;
#endif

#ifndef HAVE_LPWORD
#define HAVE_LPWORD
typedef WORD                *LPWORD;
#endif

#ifndef HAVE_LPLONG
#define HAVE_LPLONG
typedef long                *LPLONG;
#endif

#ifndef HAVE_PDWORD
#define HAVE_PDWORD
typedef DWORD               *PDWORD;
#endif

#ifndef HAVE_LPDWORD
#define HAVE_LPDWORD
typedef DWORD               *LPDWORD;
#endif

#ifndef HAVE_LPVOID
#define HAVE_LPVOID
typedef void                *LPVOID;
#endif

#ifndef HAVE_LPCVOID
#define HAVE_LPCVOID
typedef const void          *LPCVOID;
#endif

#ifndef HAVE_PUINT
#define HAVE_PUINT
typedef unsigned int        *PUINT;
#endif

/* Types use for passing & returning polymorphic values */
#ifndef HAVE_WPARAM
#define HAVE_WPARAM
typedef unsigned int        WPARAM;
#endif

#ifndef HAVE_LPARAM
#define HAVE_LPARAM
typedef LONG                LPARAM;
#endif

#ifndef HAVE_LRESULT
#define HAVE_LRESULT
typedef LONG                LRESULT;
#endif


#ifndef HAVE_LONGLONG
#define HAVE_LONGLONG
typedef long long           LONGLONG;
#endif

#ifndef HAVE_ULONGLONG
#define HAVE_ULONGLONG
typedef unsigned long long  ULONGLONG;
#endif

#ifndef HAVE_longlong
#define HAVE_longlong
typedef long long longlong;
#endif

#ifndef HAVE_ulonglong
#define HAVE_ulonglong
typedef unsigned long long ulonglong;
#endif
#if OS_VXWORKS
#if (defined(__DCC__)&&(VX_VERSION==55))

#define inline  
#define _inline 
#define __inline__ 

#define __const__  const
#define __signed__ signed
#define __const__ const

#else
#define inline  __inline
#define _inline __inline
#define __inline__ __inline

#endif
#endif
#if OS_LINUX
#define inline  
#define _inline 
#define __inline__ 
#define __const__  const
#define __signed__ signed
#define __const__ const
#endif

/* base 32*/

#define VOS_ERR_OUT VOS_Printf("error! %s(%u),%s\r\n",__FILE__,__LINE__,__func__)
#define VOS_VAL_OUT(a) VOS_Printf("\r\n var(%s),value(%d),%s(%u),%s\r\n",(#a),(a),__FILE__,__LINE__,__func__)
#define VOS_VAL_OUT_U(a) VOS_Printf("\r\n var(%s),value(%u),%s(%u),%s\r\n",(#a),(a),__FILE__,__LINE__,__func__)
#define VOS_STR_OUT(a) VOS_Printf("\r\n %s,%s(%u),%s\r\n",(a),__FILE__,__LINE__,__func__)


/* 求余 */
#define __VOS_RESIDUE( _n, _2_exp )     (((_n)<<(32-(_2_exp)))>>(32-(_2_exp)))

/* 求商 */
#define __VOS_QUOTIENT( _n, _2_exp )    ((_n)>>(_2_exp))

/* 去尾取整 */
#define __VOS_INTZERO( _n, _2_exp )     (__VOS_QUOTIENT( _n, _2_exp )<<(_2_exp))

/* 进位取整 */
#define __VOS_INTINCR( _n, _2_exp )     \
        ((__VOS_QUOTIENT( _n, _2_exp ) + (0==__VOS_QUOTIENT( __VOS_RESIDUE( _n, _2_exp ), _2_exp ))?0:1)<<(_2_exp))


#endif /* OS OVER */

typedef unsigned long   ulong_t;

typedef void            VOID;
typedef int             BOOL;
typedef unsigned char   UCHAR;
typedef unsigned short  USHORT;
typedef unsigned int    UINT;
typedef unsigned long   ULONG;

typedef	short		INT16;


#define VOS_NAME_MAX_LENGTH     128                 /* VOS有名字的对象的名字的�?大长�?. */


#ifdef __cplusplus
    }
#endif

#endif /*_VOS_TYPES_H_*/
