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
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <sys/ioctl.h>
#include <libgen.h>
#include <pwd.h>
#include <unistd.h>

#include "util_systemer.h"

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
char *get_system_time()
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    char *str = asctime(timeinfo);
    str[strlen(str)-1] = '\0';
    
    return str;
}

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
char *get_system_username()
{
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    return pwd->pw_name;
}

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
char *get_system_shellname()
{
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    return pwd->pw_shell;
}

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
char *get_system_hostname()
{
    char *host = (char*)malloc(_STRING_LENGTH);
    gethostname(host, _STRING_LENGTH);
    return host;
}

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
char *get_system_current_dir()
{
    char *dir = (char*)malloc(_STRING_LENGTH);
    getcwd(dir, _STRING_LENGTH);
    return dir;
}

/******************************************************************************
 * Description:
 *  
 * Input: 
 *
 * Output:  
 ******************************************************************************/
char* sh_cmd(const char *msg, ...)
{
    va_list arg;
    char *MSG = (char *)malloc(strlen(msg)+2);
    sprintf(MSG, "%s\n", msg);
    va_start(arg, MSG);
    
    char *str = (char *)MSG;
    
    char Cmd[_STRING_LENGTH], tmp[_STRING_LENGTH];
    memset(Cmd, 0, _STRING_LENGTH);
    memset(tmp, 0, _STRING_LENGTH);
    
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
                        sprintf(tmp, "%d", _i);
                        strcat(Cmd, tmp);
                        break;
                        
                    case 'c':
                        _c = va_arg(arg, int);
                        sprintf(tmp, "%c", _c);
                        strcat(Cmd, tmp);
                        break;
                        
                    case 's':
                        _s = va_arg(arg, char *);
                        sprintf(tmp, "%s", _s);
                        strcat(Cmd, tmp);
                        break;
                }
                str++;
            default:
                sprintf(tmp, "%c", *str);
                strcat(Cmd, tmp);
                break;
        }
        str++;
    }
    if(strlen(Cmd) <= 0)
    {
        goto Error_flag;
    }
    printf("\033[31m[%s:%s %s]%c\033[m %s\n", 
            get_system_username(), 
            basename(get_system_shellname()), 
            basename(get_system_current_dir()), 
            strncmp(get_system_username(), 
                    "root", 
                    sizeof(get_system_username()))==0?'#':'$',
            Cmd);
            
    system(Cmd);
    va_end(arg);
    return (char*)Cmd;
Error_flag:
    return NULL;
}
