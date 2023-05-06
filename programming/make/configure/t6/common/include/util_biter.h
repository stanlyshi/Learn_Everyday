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
** Author: 
** Date: 
**
******************************************************************************/
#ifndef _UTIL_BITER_H
#define _UTIL_BITER_H

/* Dependencies --------------------------------------------------------------*/
#include <stdint.h>

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
typedef enum{
    BIT_0 = 0,
    BIT_1,    
}BitType;

/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int get_uint8_t_bit(const uint8_t val, uint8_t index);
uint8_t get_uint8_t_bits(const uint8_t val, int start, int end);
int set_uint8_t_bit(uint8_t *val, uint8_t index, BitType bittype);
int set_uint8_t_bits(uint8_t *val, int start, int end, BitType bittype);
int set_uint8_t_zero(uint8_t *val);
int set_uint8_t_one(uint8_t *val);
int show_uint8_t(const uint8_t val);

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int get_uint16_t_bit(const uint16_t val, uint16_t index);
uint16_t get_uint16_t_bits(const uint16_t val, int start, int end);
int set_uint16_t_bit(uint16_t *val, uint16_t index, BitType bittype);
int set_uint16_t_bits(uint16_t *val, int start, int end, BitType bittype);
int set_uint16_t_zero(uint16_t *val);
int set_uint16_t_one(uint16_t *val);
int show_uint16_t(const uint16_t val);

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int get_uint32_t_bit(const uint32_t val, uint32_t index);
uint32_t get_uint32_t_bits(const uint32_t val, int start, int end);
int set_uint32_t_bit(uint32_t *val, uint32_t index, BitType bittype);
int set_uint32_t_bits(uint32_t *val, int start, int end, BitType bittype);
int set_uint32_t_zero(uint32_t *val);
int set_uint32_t_one(uint32_t *val);
int show_uint32_t(const uint32_t val);

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int get_uint64_t_bit(const uint64_t val, uint64_t index);
uint64_t get_uint64_t_bits(const uint64_t val, int start, int end);
int set_uint64_t_bit(uint64_t *val, uint64_t index, BitType bittype);
int set_uint64_t_bits(uint64_t *val, int start, int end, BitType bittype);
int set_uint64_t_zero(uint64_t *val);
int set_uint64_t_one(uint64_t *val);
int show_uint64_t(const uint64_t val);

#endif /*<_UTIL_BITER_H>*/