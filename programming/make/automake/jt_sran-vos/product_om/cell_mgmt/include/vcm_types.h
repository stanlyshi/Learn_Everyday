/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:28 CST. */
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
/**********************************************************************************************************************
** 
** Copyright (c) 2008-2018 ICT/CAS.
** All rights reserved.
**
** File name: vcm_types.h
** Description: 
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao, LiuYixin, Diao Hongman
** Date: 2018.08 - 2019.01 - 
**
***********************************************************************************************************************/
#ifndef _VCM_TYPES_H
#define _VCM_TYPES_H 1

/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include <stdio.h>

#include "vos_types.h"

/* Constants ---------------------------------------------------------------------------------------------------------*/
#define SIZE_T          size_t


/**
 *  Mask code, support totally 64 codes(1L is 64bit in 64bit system)
 *  For module register
 *      Rong Tao: 2019.02
 */
#define CELLSETUP               (1L<<0)             /* cell setup happend */
#define CELLDELETE              (1L<<1)             /* cell delete happend */
#define CELL_EVENT_MAX          (sizeof(ULONG)*8)   /* things hallend max number */

/**
 *  cell mode: fdd tdd ...etc
 */
typedef enum{
    VC_MODE_MIN,
    GSM,
    GMR,
    WCDMA,
    TD_SCDMA,
    TDD_LTE,
    FDD_LTE,  
    VC_MODE_MAX
}cell_mode_t; 

/**
 *  type of cell: virtual, simulate
 */
typedef enum{
    CELL_TYPE_MIN = -1,
    CELL_VIRTUAL,
    CELL_SIMULATE,
    CELL_TYPE_MAX,
}cell_type_t;

/**
 *  unify type of cell: non-unify, unify
 */
typedef enum{
    UNIFY_OFF = 0,
    UNIFY_ON = 1,
}cell_role_t;



typedef enum {
    VCM_IPv4_TYPE_MIN,
    VCM_REGULAR_IP, /* 10.21.3.242 */
    VCM_NETMASK, /* 255.255.0.0 */
    VCM_BROADCAST, /* 10.21.255.255 */
    VCM_IPv4_TYPE_MAX,
}VCM_IPv4_TYPE;

typedef enum {
    VCM_OPERATOR_MIN,
    VCM_DoNothing,  /* Do nothing */
    VCM_Correct,    /* if parameter invalid, correct it */
    VCM_OPERATOR_MAX,
}VCM_OPERATOR_TYPE;


/* Types -------------------------------------------------------------------------------------------------------------*/
typedef struct {
    INT8    flag;
    CHAR    ipv4[16];
}ipv4_t;




/* Macros ------------------------------------------------------------------------------------------------------------*/
/**
 *  VCM set, like fd_set
 */
#define VCM_SETSIZE    __VCM_SETSIZE
#define __VCM_SETSIZE      256 /* max bits of bits, this is max number of Cells */
    
/* It's easier to assume 8-bit bytes than to get CHAR_BIT.  */
#define __NVCMBITS     (8 * (int) sizeof (__vcm_li_mask))
#define __VCM_ELT(d)   ((d) / __NVCMBITS)
#define __VCM_MASK(d)  ((__vcm_li_mask) (1UL << ((d) % __NVCMBITS)))

typedef long int __vcm_li_mask; 

/* VCM_SET for select and pselect.  */
typedef struct {
    __vcm_li_mask __vcms_bits[__VCM_SETSIZE / __NVCMBITS];
#define __VCMS_BITS(set) ((set)->__vcms_bits)
} vcm_set;

/* It's easier to assume 8-bit bytes than to get CHAR_BIT.  */
#define __NVCMBITS     (8 * (int) sizeof (__vcm_li_mask))
#define __VCM_ELT(d)   ((d) / __NVCMBITS)
#define __VCM_MASK(d)  ((__vcm_li_mask) (1UL << ((d) % __NVCMBITS)))

/* We don't use `Memset' because this would require a prototype and
   the array isn't too big.  */
#define __VCM_ZERO(s) \
    do {    \
        unsigned int __i;   \
        vcm_set *__arr = (s);  \
        for (__i = 0; __i < sizeof (vcm_set) / sizeof (__vcm_li_mask); ++__i)    \
            __VCMS_BITS (__arr)[__i] = 0;  \
    } while (0)
#define __VCM_SET(d, s) \
    ((void) (__VCMS_BITS (s)[__VCM_ELT(d)] |= __VCM_MASK(d)))
#define __VCM_CLR(d, s) \
    ((void) (__VCMS_BITS (s)[__VCM_ELT(d)] &= ~ __VCM_MASK(d)))
#define __VCM_ISSET(d, s) \
    ((__VCMS_BITS (s)[__VCM_ELT (d)] & __VCM_MASK (d)) != 0)

/* Access macros for `VCM_SET'.  */
#define    VCM_SET(cell, cellsetp)     __VCM_SET (cell, cellsetp)
#define    VCM_CLR(cell, cellsetp)     __VCM_CLR (cell, cellsetp)
#define    VCM_ISSET(cell, cellsetp)   __VCM_ISSET (cell, cellsetp) //isset: succ(1) error(0)
#define    VCM_ZERO(cellsetp)          __VCM_ZERO (cellsetp)



/**
 *  To make it easy to use
 */
#ifndef _packed
#define _packed __attribute__((packed))
#endif
        
#ifndef _noreturn
#define _noreturn  __attribute__((noreturn))
#endif
        
#ifndef _unused
#define _unused   __attribute__((unused))
#endif
        
#ifndef _const
#define _const    __attribute__((const))
#endif
        
#ifndef _aligned
#define _aligned(x)   __attribute__((aligned(x)))
#endif 
        
#ifndef weak_alias
#define weak_alias(name, aliasname) \
            extern typeof (name) aliasname __attribute__ ((weak, alias(#name)))
#endif
        
#ifndef _toString
#define _toString(val) #val
#endif
        
#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((SIZE_T) &((TYPE *)0)->MEMBER)
#endif

#ifndef indexofarray
#define indexofarray(type, startAddr, endAddr) (abs(((ULONG)endAddr)-((ULONG)startAddr))/sizeof(type))
#endif

#ifndef likely
#define likely(exp) __builtin_expect(!!exp, 1)
#endif
#ifndef unlikely
#define unlikely(exp) __builtin_expect(!!exp, 0)
#endif


#ifndef _format
#define _format(func, a, b) __attribute__((__format__ (func, a, b)))
#endif


/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/






#endif /*<_VCM_TYPES_H>*/
