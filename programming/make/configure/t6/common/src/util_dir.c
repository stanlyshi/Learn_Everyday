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
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>

#include "util_printer.h"
#include "util_dir.h"

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
int is_exist(const char *dirname)
{
    if(dirname == NULL)
    {
        error("dirname is NULL\n");
        return -1;
    }
    return access(dirname, F_OK);
}

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int is_readable(const char *dirname)
{
    if(dirname == NULL)
    {
        error("dirname is NULL\n");
        return -1;
    }
    return access(dirname, R_OK);
}

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int is_writeable(const char *dirname)
{
    if(dirname == NULL)
    {
        error("dirname is NULL\n");
        return -1;
    }
    return access(dirname, W_OK);
}

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int is_executable(const char *dirname)
{
    if(dirname == NULL)
    {
        error("dirname is NULL\n");
        return -1;
    }
    return access(dirname, X_OK);
}


/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int add_read_permission(const char *dirname)
{
    if(dirname == NULL)
    {
        error("dirname is NULL\n");
        return -1;
    }
    if(-1 == access(dirname, F_OK))
    {
        return -1;
    }
    else
    {
        return chmod(dirname, READ_CHMOD_ALL);
    }
}

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int add_write_permission(const char *dirname)
{
    if(dirname == NULL)
    {
        error("dirname is NULL\n");
        return -1;
    }
    if(-1 == access(dirname, F_OK))
    {
        return -1;
    }
    else
    {
        return chmod(dirname, WRITE_CHMOD_ALL);
    }
}


/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int add_execute_permission(const char *dirname)
{
    if(dirname == NULL)
    {
        error("dirname is NULL\n");
        return -1;
    }
    if(-1 == access(dirname, F_OK))
    {
        return -1;
    }
    else
    {
        return chmod(dirname, EXECUTE_CHMOD_ALL);
    }
}


/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int add_usr_permission(const char *dirname)
{
    if(dirname == NULL)
    {
        error("dirname is NULL\n");
        return -1;
    }
    if(-1 == access(dirname, F_OK))
    {
        return -1;
    }
    else
    {
        return chmod(dirname, USR_CHMOD_ALL);
    }
}


/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int add_grp_permission(const char *dirname)
{
    if(dirname == NULL)
    {
        error("dirname is NULL\n");
        return -1;
    }
    if(-1 == access(dirname, F_OK))
    {
        return -1;
    }
    else
    {
        return chmod(dirname, GRP_CHMOD_ALL);
    }
}


/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int add_oth_permission(const char *dirname)
{
    if(dirname == NULL)
    {
        error("dirname is NULL\n");
        return -1;
    }
    if(-1 == access(dirname, F_OK))
    {
        return -1;
    }
    else
    {
        return chmod(dirname, OTH_CHMOD_ALL);
    }
}


/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int add_all_permission(const char *dirname)
{
    if(dirname == NULL)
    {
        error("dirname is NULL\n");
        return -1;
    }
    if(-1 == access(dirname, F_OK))
    {
        return -1;
    }
    else
    {
        return chmod(dirname, USR_CHMOD_ALL|GRP_CHMOD_ALL|OTH_CHMOD_ALL);
    }
}


/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int create_dir(const char *dirname)
{
    if(dirname == NULL)
    {
        error("dirname is NULL\n");
        return -1;
    }
    
    char tmp[strlen(dirname) + 100];
    
    if(-1 == access(dirname, F_OK))
    {
        sprintf(tmp, "mkdir -p %s", dirname);
        system(tmp);
        success("%s\n", tmp);
        return 0;
    }
    else
    {
        warning("%s already exist.\n", dirname);
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
int delete_dir(const char *dirname)
{
    if(dirname == NULL)
    {
        error("dirname is NULL\n");
        return -1;
    }
    
    char tmp[strlen(dirname) + 100];
    if(-1 == access(dirname, F_OK))
    {
        warning("%s don't exist.\n", dirname);
        return -1;
    }
    else
    {
        sprintf(tmp, "rm -rf %s", dirname);
        system(tmp);
        success("%s\n", tmp);
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
int copy_file(const char *filename, const char *target_dir)
{
    if(filename == NULL || target_dir == NULL)
    {
        error("invalid arg pointer.\n");
        return -1;
    }
    
    struct stat st;
    stat(target_dir, &st);
    
    char tmp[strlen(filename) + strlen(target_dir) + 100];
    
    if(S_ISDIR(st.st_mode))
    {
        if(-1 == access(target_dir, F_OK))
        {
            error("%s doesn't exist.\n", target_dir);
            return -1;
        }
        else
        {
            if(-1 == access(target_dir, W_OK|R_OK|X_OK))
            {
                chmod(target_dir, CHMOD_ALL);
            }
            if(-1 == access(filename, F_OK))
            {
                error("%s doesn't exist.\n", filename);
                return -1;
                
            }
            else
            {
                stat(filename, &st);
                if(S_ISDIR(st.st_mode))
                {
                    sprintf(tmp, "cp -a -r %s %s", filename, target_dir);
                }
                else
                {
                    sprintf(tmp, "cp -a %s %s", filename, target_dir);
                }
                system(tmp);
                success("%s\n", tmp);
                return 0;
            }
        }
    }
    else
    {
        error("%s is not a directory.\n", target_dir);
        return -1;
    }
}

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int delete_file(const char *filename, ...)
{
    if(filename == NULL)
    {
        error("invalid pointer.\n");
        return -1;
    }
    
    
    struct stat st;
    char tmp[strlen(filename) + 100];
    
    if(-1 == access(filename, F_OK))
    {
        error("%s doesn't exist.\n", filename);
        return -1;
    }
    else
    {
        if(-1 == access(filename, W_OK|R_OK|X_OK))
        {
            chmod(filename, CHMOD_ALL);
        }
        else
        {
            stat(filename, &st);
            if(S_ISDIR(st.st_mode))
            {
                sprintf(tmp, "rm -rf %s", filename);
            }
            else
            {
                sprintf(tmp, "rm %s", filename);
            }
            system(tmp);
            success("%s\n", tmp);
            
            /*处理 "..." 内容*/
            va_list arg;
            va_start(arg, filename);
            char *filenames;
            while(1)
            {
                filenames = va_arg(arg, char *);
                if(0 == access(filenames, F_OK))
                {
                    delete_file(filenames);
                }
                else
                {
                    break;
                }
            }
            va_end(arg);
            
            return 0;
        }
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
pid_t copy_to_ip(char *name, char ip[16], char *desc_path, char *username)
{
    /*define local variable.*/
    pid_t 	status = -1; 			/*store the value returned by system().*/
    char 	buffer[256]; 			/*store the string for system().*/

 	/* Check for null pointer.*/
 	if (NULL == ip || name == NULL || desc_path == NULL) 
	{
        error("NULL error in copy_to_ip.\n");
 		return status;
 	}
    if(username == NULL)
    {
        sprintf (buffer, "scp -r %s root@%s:%s/", name, ip, desc_path);
    }
    else 
    {
        sprintf (buffer, "scp -r %s %s@%s:%s/", name, username, ip,  desc_path);
    }
    
    /*判断文件和路径是否存在*/
    if((-1 == access(name, F_OK)) || (-1 == access(desc_path, F_OK)))
    {
        error("%s or desc_path not exist.\n", name, desc_path);
        return status;
    }
    else
    {
        chmod(desc_path, CHMOD_ALL);
        status = system(buffer);
        success("%s\n",buffer);
    }
    return status;
}