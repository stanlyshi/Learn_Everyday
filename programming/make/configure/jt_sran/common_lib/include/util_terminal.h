/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:40 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:24 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:00 CST. */
#ifndef _UTIL_TERMINAL_H
#define _UTIL_TERMINAL_H

#include <stdint.h>

typedef struct terminalsize_s{
    uint32_t x;
    uint32_t y;
}ts_t;

typedef enum{
    COLOR_MIN,
    COLOR_Red     = 1,
    COLOR_Green,
    COLOR_Yellow,
    COLOR_Blue,
    COLOR_Purple,
    COLOR_Cyan,
    COLOR_Gray,
    COLOR_MAX,
}PRINT_COLOR;

void gotoxy(int x, int y);

ts_t getTerminalSize();

void setTerminalPixelColor(ts_t xy, PRINT_COLOR background);

ts_t getMourcePosition();

#endif /*<_UTIL_TERMINAL_H>*/