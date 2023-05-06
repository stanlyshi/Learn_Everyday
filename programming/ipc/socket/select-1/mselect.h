/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:41 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:10 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:50 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:21 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:24:10 CST. */
#ifndef _M_SELECT_H
#define _M_SELECT_H

#include <malloc.h>
#include <string.h>


#define M_SETSIZE		__M_SETSIZE

#define __NMBITS	(8 * sizeof(unsigned long))
#define __M_SETSIZE	1024
#define __MSET_LONGS	(__M_SETSIZE/__NMBITS)

#define	__MELT(d)	((d) / __NMBITS)
#define	__MMASK(d)	(1UL << ((d) % __NMBITS))

typedef struct {
	unsigned long ms_bits [__MSET_LONGS];
} __m_set;
typedef __m_set m_set;

#define	M_SET(d, set)	((set)->ms_bits[__MELT(d)] |= __MMASK(d))
#define	M_CLR(d, set)	((set)->ms_bits[__MELT(d)] &= ~__MMASK(d))
#define	M_ISSET(d, set)	(((set)->ms_bits[__MELT(d)] & __MMASK(d)) != 0)
#define	M_ZERO(set)	\
	  ((void) memset ((void *) (set), 0, sizeof (__m_set)))
      
#define rselect(fd, pset) select(fd, pset, NULL, NULL, NULL);
#define wselect(fd, pset) select(fd, NULL, pset, NULL, NULL);
#define eselect(fd, pset) select(fd, NULL, NULL, pset, NULL);

#endif /*<_M_SELECT_H>*/