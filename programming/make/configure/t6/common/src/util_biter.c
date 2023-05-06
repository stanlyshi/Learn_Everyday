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

/* Dependencies --------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "util_biter.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/
/* uint8_t ********************************************************************/
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/

int show_uint8_t(const uint8_t val)
{
    char str[8 +1];
    
    int i;
    for(i=0; i<8; i++)
    {
        if(get_uint8_t_bit(val, i+1) == BIT_1)
        {
            str[8-1-i] = '1';
        }
        else if(get_uint8_t_bit(val, i+1) == BIT_0)
        {
            str[8-1-i] = '0';
        }
    }
    str[8] = '\0';
    printf("%s\n", str);
    //printf("%x\n", val);
    return 0;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int get_uint8_t_bit(const uint8_t val, uint8_t index)
{
    if(index > 8)
    {
        return -1;
    }
    uint8_t flag = 1;
    if((val&(flag<<(index-1))) == BIT_0)
    {
        return BIT_0;
    }
    else 
    {
        return BIT_1;
    }
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
uint8_t get_uint8_t_bits(const uint8_t val, int start, int end)
{
    if(start < 1 || end > 8 || start > end)
    {
        printf("Index error, out of 8 bit range.\n");
        return -1;
    }
    uint8_t out = 0;
    
    int i;
    
    for(i=end; i>=start; i--)
    {
        out = out*2 + get_uint8_t_bit(val, i);
    }
    return out;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint8_t_bit(uint8_t *val, uint8_t index, BitType bittype)
{
    if(index > 8)
    {
        return -1;
    }
    if(val == NULL)
    {
        return -1;
    }
    
    uint8_t flag = 1;
    
    if(get_uint8_t_bit(*val, index) == BIT_0  && bittype == BIT_1)
    {
        *val = *val|(flag<<(index - 1));
        return 0;
    }
    if(get_uint8_t_bit(*val, index) == BIT_1  && bittype == BIT_0)
    {
        *val = *val-(flag<<(index - 1));
        return 0;
    }
    return -1;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint8_t_bits(uint8_t *val, int start, int end, BitType bittype)
{
    if(val == NULL)
    {
        return -1;
    }
    
    int i;
    for(i=start; i<=end; i++)
    {
        set_uint8_t_bit(val, i, bittype);
    }
    
    return 0;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint8_t_zero(uint8_t *val)
{
    if(val == NULL)
    {
        return -1;
    }
    *val = 0;
    return 0;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint8_t_one(uint8_t *val)
{
    if(val == NULL)
    {
        return -1;
    }
    int i = 0;
    uint8_t flag = 1;
    for(i=0; i< 8; i++)
    {
        *val = *val|(flag<<i);
    }
    return 0;
}


/* uint16_t *******************************************************************/
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/

int show_uint16_t(const uint16_t val)
{
    char str[16 +1];
    
    int i;
    for(i=0; i<16; i++)
    {
        if(get_uint16_t_bit(val, i+1) == BIT_1)
        {
            str[16-1-i] = '1';
        }
        else if(get_uint16_t_bit(val, i+1) == BIT_0)
        {
            str[16-1-i] = '0';
        }
    }
    str[16] = '\0';
    printf("%s\n", str);
    
    return 0;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int get_uint16_t_bit(const uint16_t val, uint16_t index)
{
    if(index > 16)
    {
        return -1;
    }
    uint16_t flag = 1;
    if((val&(flag<<(index-1))) == BIT_0)
    {
        return BIT_0;
    }
    else 
    {
        return BIT_1;
    }
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
uint16_t get_uint16_t_bits(const uint16_t val, int start, int end)
{
    if(start < 1 || end > 16 || start > end)
    {
        printf("Index error, out of 16 bit range.\n");
        return -1;
    }
    uint16_t out = 0;
    
    int i;
    
    for(i=end; i>=start; i--)
    {
        out = out*2 + get_uint16_t_bit(val, i);
    }
    return out;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint16_t_bit(uint16_t *val, uint16_t index, BitType bittype)
{
    if(index > 16)
    {
        return -1;
    }
    if(val == NULL)
    {
        return -1;
    }
    
    uint16_t flag = 1;
    
    if(get_uint16_t_bit(*val, index) == BIT_0  && bittype == BIT_1)
    {
        *val = *val|(flag<<(index - 1));
        return 0;
    }
    if(get_uint16_t_bit(*val, index) == BIT_1  && bittype == BIT_0)
    {
        *val = *val-(flag<<(index - 1));
        return 0;
    }
    return -1;
}

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint16_t_bits(uint16_t *val, int start, int end, BitType bittype)
{
    if(val == NULL)
    {
        return -1;
    }
    
    int i;
    for(i=start; i<=end; i++)
    {
        set_uint16_t_bit(val, i, bittype);
    }
    
    return 0;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint16_t_zero(uint16_t *val)
{
    if(val == NULL)
    {
        return -1;
    }
    *val = 0;
    return 0;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint16_t_one(uint16_t *val)
{
    if(val == NULL)
    {
        return -1;
    }
    int i = 0;
    uint16_t flag = 1;
    for(i=0; i< 16; i++)
    {
        *val = *val|(flag<<i);
    }
    return 0;
}



/* uint32_t *******************************************************************/
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int show_uint32_t(const uint32_t val)
{
    char str[32 +1];
    
    int i;
    for(i=0; i<32; i++)
    {
        if(get_uint32_t_bit(val, i+1) == BIT_1)
        {
            str[32-1-i] = '1';
        }
        else if(get_uint32_t_bit(val, i+1) == BIT_0)
        {
            str[32-1-i] = '0';
        }
    }
    str[32] = '\0';
    printf("%s\n", str);
    
    return 0;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int get_uint32_t_bit(const uint32_t val, uint32_t index)
{
    if(index > 32)
    {
        return -1;
    }
    uint32_t flag = 1;
    if((val&(flag<<(index-1))) == BIT_0)
    {
        return BIT_0;
    }
    else 
    {
        return BIT_1;
    }
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
uint32_t get_uint32_t_bits(const uint32_t val, int start, int end)
{
    if(start < 1 || end > 32 || start > end)
    {
        printf("Index error, out of 32 bit range.\n");
        return -1;
    }
    uint32_t out = 0;
    
    int i;
    
    for(i=end; i>=start; i--)
    {
        out = out*2 + get_uint32_t_bit(val, i);
    }
    return out;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint32_t_bit(uint32_t *val, uint32_t index, BitType bittype)
{
    if(index > 32)
    {
        return -1;
    }
    if(val == NULL)
    {
        return -1;
    }
    
    uint32_t flag = 1;
    
    if(get_uint32_t_bit(*val, index) == BIT_0  && bittype == BIT_1)
    {
        *val = *val|(flag<<(index - 1));
        return 0;
    }
    if(get_uint32_t_bit(*val, index) == BIT_1  && bittype == BIT_0)
    {
        *val = *val-(flag<<(index - 1));
        return 0;
    }
    return -1;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint32_t_bits(uint32_t *val, int start, int end, BitType bittype)
{
    if(val == NULL)
    {
        return -1;
    }
    
    int i;
    for(i=start; i<=end; i++)
    {
        set_uint32_t_bit(val, i, bittype);
    }
    
    return 0;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint32_t_zero(uint32_t *val)
{
    if(val == NULL)
    {
        return -1;
    }
    *val = 0;
    return 0;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint32_t_one(uint32_t *val)
{
    if(val == NULL)
    {
        return -1;
    }
    int i = 0;
    uint32_t flag = 1;
    for(i=0; i< 32; i++)
    {
        *val = *val|(flag<<i);
    }
    return 0;
}

/* uint64_t *******************************************************************/
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int show_uint64_t(const uint64_t val)
{
    char str[64 +1];
    
    int i;
    for(i=0; i<64; i++)
    {
        if(get_uint64_t_bit(val, i+1) == BIT_1)
        {
            str[64-1-i] = '1';
        }
        else if(get_uint64_t_bit(val, i+1) == BIT_0)
        {
            str[64-1-i] = '0';
        }
    }
    str[64] = '\0';
    printf("%s\n", str);
    //printf("%x\n", val);
    return 0;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int get_uint64_t_bit(const uint64_t val, uint64_t index)
{
    if(index > 64)
    {
        return -1;
    }
    uint64_t flag = 1;
    if((val&(flag<<(index-1))) == BIT_0)
    {
        return BIT_0;
    }
    else 
    {
        return BIT_1;
    }
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
uint64_t get_uint64_t_bits(const uint64_t val, int start, int end)
{
    if(start < 1 || end > 64 || start > end)
    {
        printf("Index error, out of 64 bit range.\n");
        return -1;
    }
    uint64_t out = 0;
    
    int i;
    
    for(i=end; i>=start; i--)
    {
        out = out*2 + get_uint64_t_bit(val, i);
    }
    return out;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint64_t_bit(uint64_t *val, uint64_t index, BitType bittype)
{
    if(index > 64)
    {
        return -1;
    }
    if(val == NULL)
    {
        return -1;
    }
    
    uint64_t flag = 1;
    
    if(get_uint64_t_bit(*val, index) == BIT_0  && bittype == BIT_1)
    {
        *val = *val|(flag<<(index - 1));
        return 0;
    }
    if(get_uint64_t_bit(*val, index) == BIT_1  && bittype == BIT_0)
    {
        *val = *val-(flag<<(index - 1));
        return 0;
    }
    return -1;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint64_t_bits(uint64_t *val, int start, int end, BitType bittype)
{
    if(val == NULL)
    {
        return -1;
    }
    
    int i;
    for(i=start; i<=end; i++)
    {
        set_uint64_t_bit(val, i, bittype);
    }
    
    return 0;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint64_t_zero(uint64_t *val)
{
    if(val == NULL)
    {
        return -1;
    }
    *val = 0;
    return 0;
}
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int set_uint64_t_one(uint64_t *val)
{
    if(val == NULL)
    {
        return -1;
    }
    int i = 0;
    uint64_t flag = 1;
    for(i=0; i< 64; i++)
    {
        *val = *val|(flag<<i);
    }
    return 0;
}

