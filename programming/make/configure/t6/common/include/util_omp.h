/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:39 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:49 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:32 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:06 CST. */
/******************************************************************************
** 
** Copyright (c) 2008-2018 ICT/CAS.
** All rights reserved.
**
** File name: 
** Description:
**
** Current Version: 
** $Revision$
** Author: Rong Tao(RongTao@sylincom.com)
** Date: 
**
******************************************************************************/
#ifndef _UTIL_OMP_H
#define _UTIL_OMP_H

#include <omp.h>

typedef int (*OMP_FUNC_PTR)(int, void*);

typedef struct {
    unsigned int    thread_num;
    void            *arg;
    OMP_FUNC_PTR    callback;
}omp_t;

void omp_parllel_for(int n, omp_t *t);


#endif /*<_UTIL_OMP_H>*/