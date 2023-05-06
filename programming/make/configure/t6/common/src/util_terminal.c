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
#include <stdio.h>
#include <sys/ioctl.h>
#include <termio.h>

#include "util_terminal.h"

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

ts_t getTerminalSize()
{
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);
    
    ts_t size = 
    {
        .x      = sz.ws_col,
        .y      = sz.ws_row,
    };
    
    return size;
}

void setTerminalPixelColor(ts_t xy, PRINT_COLOR background)
{
    gotoxy(xy.x, xy.y);
    printf("\033[%dm \033[m",background + 40);
    fflush(stdout);
}