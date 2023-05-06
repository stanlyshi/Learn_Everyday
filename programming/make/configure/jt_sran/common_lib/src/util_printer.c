/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:31 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:41 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:25 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:00 CST. */
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
#include <stdarg.h>
#include <sys/ioctl.h>

#include "util_printer.h"

/* Constants -----------------------------------------------------------------*/

/* Types -------------------------------------------------------------------- */

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
void print(PRINT_STATE ac, const char *msg, ...)
{
#ifdef _PRINTER_DEBUG
    if(ac == Info)
    {
        printf("    %s[Info]%s%s[CELL_MGMT]%s ", 
                ANSI_BLUE, ANSI_RESET, ANSI_BOLD, ANSI_RESET);
    }
    else if(ac == Warning)
    {
        printf(" %s[Warning]%s%s[CELL_MGMT]%s ", 
                ANSI_PURPLE, ANSI_RESET, ANSI_BOLD, ANSI_RESET);
    }
    else if(ac == Error)
    {
        printf("   %s[Error]%s%s[CELL_MGMT]%s ", 
                ANSI_RED, ANSI_RESET, ANSI_BOLD, ANSI_RESET);
    }
    else if(ac == Success)
    {
        printf(" %s[Success]%s%s[CELL_MGMT]%s ", 
                ANSI_GREEN, ANSI_RESET, ANSI_BOLD, ANSI_RESET);
    }
    else if(ac == Debug)
    {
        printf("   %s[Debug]%s%s[CELL_MGMT]%s ", 
                ANSI_GRAY, ANSI_RESET, ANSI_BOLD, ANSI_RESET);
    }
    else if(ac == Null)
    {
        
    }
    
    va_list arg;
    
    va_start(arg, msg);
    
    char *str = (char *)msg;
    
    int _i;
    int _c;
    char * _s;
    
    while(*str != '\0')
    {
        switch(*str)
        {
            case '%':
                str++;
                switch(*str)
                {
                    case 'd':
                        _i = va_arg(arg, int);
                        printf("%d", _i);
                        break;
                    case 'c':
                        _c = va_arg(arg, int);
                        printf("%c", _c);
                        break;
                    case 's':
                        _s = va_arg(arg, char *);
                        printf("%s", _s);
                        break;
                    case 'x':
                        _i = va_arg(arg, int);
                        printf("%#lx", _i);
                        break;
                }
                //if()
                str++;
            default:
                printf("%c", *str);
                break;
        }
        str++;
    }
    
    va_end(arg);
    
#endif
}

/******************************************************************************
 * Description: show progress bar on screen
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int progress_bar(ts_t position, const int iTotal, const int Total)
{
    int percent, ic=0, i, itotal, total;
    struct winsize sz;
    ioctl(0, TIOCGWINSZ, &sz);
    int term_x = sz.ws_col - position.x;
    
    if(term_x < 15)
    {
        return -1;
    }
    else
    {
        term_x -= 10;
        itotal = (int)(iTotal*1.0/Total*term_x);
        total  = term_x;
    }
    gotoxy(position.x, position.y);
    printf("[");
    ic++;
    for(percent=0; percent<=total; percent++)
    {
        if(percent<=itotal)
        {
#ifdef _PRINTER_COLOR
            printf("\033[47m \033[m");   
#else 
            printf("#"); 
#endif
        }
        else
        {
            printf(" ");
        }
        ic++;
    }
#ifdef _PRINTER_COLOR
    printf("]");
    ic++;
    for(i=0; i<ic-1; i++) { printf("\b"); }
    ic = 1;
    printf("\033[1;47m%3d%%\033[m", (int)(itotal*100.0/total));
    ic += 4;
#else
    printf("]%3d%%", (int)(itotal*100.0/total));
    ic += 5;
#endif
    fflush(stdout);
    for(i=0; i<ic; i++) { printf("\b"); }
    return 0;
}