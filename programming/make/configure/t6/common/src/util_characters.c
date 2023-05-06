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

#include <stdint.h>

#include "util_terminal.h"
#include "util_characters.h"

static unsigned char characters_8x8[] = {
    /*列扫描 阴码*/
    0x00, 0x3e, 0x7f, 0x41, 0x7f, 0x3e, 0x00, 0x00, /*0*/
    0x00, 0x42, 0x7f, 0x7f, 0x40, 0x00, 0x00, 0x00, /*1*/
    0x00, 0x62, 0x73, 0x59, 0x4f, 0x46, 0x00, 0x00, /*2*/
    0x00, 0x22, 0x6b, 0x49, 0x7f, 0x36, 0x00, 0x00, /*3*/
    0x00, 0x38, 0x3e, 0x67, 0x7f, 0x20, 0x00, 0x00, /*4*/
    0x00, 0x4f, 0x4f, 0x49, 0x79, 0x31, 0x00, 0x00, /*5*/
    0x00, 0x3e, 0x7f, 0x49, 0x79, 0x30, 0x00, 0x00, /*6*/
    0x00, 0x03, 0x73, 0x79, 0x0f, 0x07, 0x00, 0x00, /*7*/
    0x00, 0x36, 0x7f, 0x49, 0x7f, 0x36, 0x00, 0x00, /*8*/
    0x00, 0x26, 0x6f, 0x49, 0x7f, 0x3e, 0x00, 0x00, /*9*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*void*/
};

char itoc(uint8_t i)
{
    switch(i)
    {
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        default: return 0;
    }
}

void draw_character(const ts_t position, PRINT_COLOR pixel_color, const char ch)
{
    int ix, iy, i;
    ts_t xy;
    unsigned char flag;
    
    switch(ch)
    {
        case '0': i = 0;    break;
        case '1': i = 1;    break;
        case '2': i = 2;    break;
        case '3': i = 3;    break;
        case '4': i = 4;    break;
        case '5': i = 5;    break;
        case '6': i = 6;    break;
        case '7': i = 7;    break;
        case '8': i = 8;    break;
        case '9': i = 9;    break;
        default:            break;
    }
    for(ix=i*_SIZE_COL_DIGIST; ix<_SIZE_COL_DIGIST*(i+1);ix+=1)
    {
        flag = 0x80;
        for(iy=0;iy<_SIZE_ROW_DIGIST;iy+=1)
        {
            if(characters_8x8[ix]&flag)
            {   
                xy.x = position.x+ix%_SIZE_COL_DIGIST;
                xy.y = position.y+_SIZE_ROW_DIGIST-iy%_SIZE_ROW_DIGIST;
                setTerminalPixelColor(xy, pixel_color);
            }
            flag /= 2;
        } 
    }
}
