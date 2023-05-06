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
#ifndef _UTIL_PRINT_H
#define _UTIL_PRINT_H

/* Dependencies --------------------------------------------------------------*/
#include "util_systemer.h"
#include "util_terminal.h"

/* Constants -----------------------------------------------------------------*/
#define ANSI_GRAY       "\x1b[1;37;40m"
#define ANSI_CYAN       "\x1b[1;36m"
#define ANSI_PURPLE     "\x1b[1;35m"
#define ANSI_BLUE       "\x1b[1;34m"
#define ANSI_YELLOW     "\x1b[1;33m"
#define ANSI_GREEN      "\x1b[1;32m"
#define ANSI_RED        "\x1b[1;31m"

#define ANSI_BOLD       "\x1b[1;30m"

#define ANSI_RESET      "\x1b[0m"

#define SIZE_TAB        8

#ifndef _PRINTER_DEBUG
#define _PRINTER_DEBUG
#endif

#ifndef _PRINTER_COLOR
#define _PRINTER_COLOR
#endif

/* Types -------------------------------------------------------------------- */
typedef enum
{
    Info,       //ANSI_BLUE
    Warning,    //ANSI_PURPLE
    Success,    //ANSI_GREEN
    Error,      //ANSI_RED
    Debug,      //ANSI_GRAY
    Null,       //ANSI_NULL
    
}PRINT_STATE;

/* Macros --------------------------------------------------------------------*/
#define debug() \
        print(Debug, "%s:%d %s .%s\n", \
              basename(__FILE__), __LINE__, __func__, get_system_time())

#define info(msg...) \
        print(Info, msg)
        
#define warning(msg...) \
        print(Warning, msg)
        
#define error(msg...) \
        print(Error, msg)
        
#define success(msg...) \
        print(Success, msg)

/* Globals -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
void print(PRINT_STATE ac, const char *msg, ...);

/******************************************************************************
 * Description: show progress bar on screen
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int progress_bar(ts_t position, const int iTotal, const int Total);

#endif  /*<_UTIL_ANSI_PRINT_H>*/