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
#ifndef _UTIL_DIR_H
#define _UTIL_DIR_H

/* Dependencies --------------------------------------------------------------*/
/* Constants -----------------------------------------------------------------*/
#define USR_CHMOD_ALL       S_IRUSR|S_IWUSR|S_IXUSR
#define GRP_CHMOD_ALL       S_IRGRP|S_IWGRP|S_IXGRP
#define OTH_CHMOD_ALL       S_IROTH|S_IWOTH|S_IXOTH
#define READ_CHMOD_ALL      S_IRUSR|S_IRGRP|S_IROTH
#define WRITE_CHMOD_ALL     S_IWUSR|S_IWGRP|S_IWOTH
#define EXECUTE_CHMOD_ALL   S_IXUSR|S_IXGRP|S_IXOTH
#define CHMOD_ALL           USR_CHMOD_ALL|GRP_CHMOD_ALL|OTH_CHMOD_ALL

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
int is_exist(const char *dirname);
int is_readable(const char *dirname);
int is_writeable(const char *dirname);
int is_executable(const char *dirname);

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int add_read_permission(const char *dirname);
int add_write_permission(const char *dirname);
int add_execute_permission(const char *dirname);

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int add_usr_permission(const char *dirname);
int add_grp_permission(const char *dirname);
int add_oth_permission(const char *dirname);
int add_all_permission(const char *dirname);

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int create_dir(const char *dirname);
int delete_dir(const char *dirname);

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int copy_file(const char *filename, const char *target_dir);
int delete_file(const char *filename, ...);

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
pid_t copy_to_ip(char *name, char ip[16], char *desc_path, char *username);

#endif /*<_UTIL_DIR_H>*/