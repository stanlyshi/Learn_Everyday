/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:28 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 19日 星期二 14:31:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 19日 星期二 14:31:24 CST. 
*/
/**********************************************************************************************************************
** 
** Copyright (c) 2008-2019 ICT/CAS.
** All rights reserved.
**
** File name: syswrapper.h
** Description: 
**
** Current Version: 
** $Revision$ 
** Author: Rong Tao
** Date: 2019.03 - 
**
***********************************************************************************************************************/

#ifndef _VCM_SYS_WRAPPER_H
#define _VCM_SYS_WRAPPER_H 1


/* Dependencies ------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "vos_types.h"
#include "vos_lib.h"
#include "vos_string.h"
#include "vos_timer.h"
#include "vos_que.h"
#include "vos_task.h"
#include "vos_sem.h"
#include "vos_ctype.h"


/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/
#define MallocVos(size, moduleID)   VOS_Malloc(size, moduleID)
#define Memcpy(dst, src, len)       VOS_MemCpy((CHAR*)dst, (CHAR*)src, len)
#define Memmove                     VOS_MemMove
#define Memset                      VOS_MemSet
#define Memzero                     VOS_MemZero
#define Memcmp                      VOS_MemCmp

#define Printf(fmt...)      VOS_Printf((CHAR*)fmt)
#define Sprintf(s, fmt...)  VOS_Sprintf((CHAR*)s, (CHAR*)fmt)
#define Snprintf            VOS_Snprintf
#define Vsprintf            VOS_Vsprintf 
#define Vsnprintf           VOS_Vsnprintf
#define Sscanf(s, fmt...)   VOS_Sscanf((CONST CHAR*)s, (CONST CHAR*)fmt)
#define Qsort               VOS_qsort
#define ASSERT              VOS_ASSERT


#define Strchr              VOS_StrChr
#define Strrchr             VOS_StrRChr
#define Strcat              VOS_StrCat
#define Strncat             VOS_StrnCat
#define Strcpy(dst, src)    VOS_StrCpy((CHAR*)dst, (CHAR*)src)
#define Strncpy             VOS_StrnCpy
#define Strcmp              VOS_StrCmp
#define Strncmp             VOS_StrnCmp
#define Strlen(str)         VOS_StrLen((const CHAR *)str)
#define Strstr              VOS_StrStr
#define Strpbrk             VOS_StrpBrk
#define Strtol              VOS_StrToL
#define Strtoul             VOS_StrToUL
#define Strtoll             VOS_StrToLL
#define Strtoull            VOS_StrToULL
#define Strtok              VOS_StrToK
#define Strncasecmp         VOS_StrnCaseCmp
#define Strcasecmp          VOS_StrCaseCmp
#define Strnlen             VOS_StrnLen
#define Strtolower          VOS_StrToLower
#define Strtoupper          VOS_StrToUpper
#define Strdup              VOS_StrDup
#define Strduptolower       VOS_StrDupToLower


#define Atol                VOS_AtoL
#define Atoi                VOS_AtoI


#define TaskCreateRet       VOID*
#define TaskCreate(name, func, arg) \
            (VOS_HANDLE)VOS_TaskCreate(name, TASK_PRIORITY_NORMAL, (VOS_TASK_ENTRY)func, arg)
#define TaskDestroy(task)   VOS_TaskTerminate(task, 0)


#define MsgqCreate()                VOS_QueCreate(VCM_MAX_MSGQ_MSG_NUM, VOS_MSG_Q_FIFO)
#define MsgqRecv(msgqid, aulMsg)    VOS_QueReceive(msgqid, aulMsg, VOS_WAIT_FOREVER)
#define MsgqSend(msgqid, aulMsg)    VOS_QueSend(msgqid, aulMsg, VOS_WAIT_FOREVER, MSG_PRI_NORMAL)
#define MsgqDelete(msgqid)          VOS_QueDelete(msgqid)


#define Sem_t                   ULONG
#define Sem_init()              VOS_SemMCreate(VOS_SEM_Q_FIFO)
#define Sem_wait(psem)          VOS_SemTake(psem, VOS_WAIT_FOREVER)
#define Sem_post(psem)          VOS_SemGive(psem)
#define Sem_destroy(psem)       VOS_SemDelete(psem)


#define Timer_t     ULONG  
#define TimerCreateLoopVos(moduleID, MsgqID, callbackFn, arg, ms) \
            VOS_TimerCreate(moduleID, MsgqID, ms, callbackFn, arg, VOS_TIMER_LOOP)
#define TimerCreateNoLoopVos(moduleID, MsgqID, callbackFn, arg, ms) \
            VOS_TimerCreate(moduleID, MsgqID, ms, callbackFn, arg, VOS_TIMER_NO_LOOP)
#define TimerChangeVos(moduleID, timer, ms)  VOS_TimerChange(moduleID, timer, ms)
#define TimerDeleteVos(moduleID, timer)      VOS_TimerDelete(moduleID, timer)



#define Isalnum     vos_isalnum
#define Isalpha     vos_isalpha
#define Isascii     vos_isascii
#define Iscntrl     vos_iscntrl
#define Isdigit     vos_isdigit
#define Isgraph     vos_isgraph
#define Islower     vos_islower
#define Isprint     vos_isprint
#define Ispunct     vos_ispunct
#define Isspace     vos_isspace
#define Isupper     vos_isupper
#define Isxdigit    vos_isxdigit
#define Toascii     vos_toascii
#define Tolower     vos_tolower
#define Toupper     vos_toupper
#define Ischar(ch) \
   ( Isalnum(ch)|Isalpha(ch)|Isascii(ch)|Iscntrl(ch)|\
    Isdigit(ch)|Isgraph(ch)|Islower(ch)|Isprint(ch)|Ispunct(ch)|\
    Isspace(ch)|Isupper(ch)|Isxdigit(ch) )



/**
 *  Wrapper system interface, below this sentence
 */
/* Constants ---------------------------------------------------------------------------------------------------------*/
#ifndef STDOUT
#define STDOUT      stdout
#endif
#ifndef STDIN
#define STDIN       stdin
#endif
#ifndef STDERR
#define STDERR      stderr
#endif
#ifndef STDOUTNO
#define STDOUTNO    fileno(stdout)
#endif
#ifndef STDINNO
#define STDINNO     fileno(stdin)
#endif
#ifndef STDERRNO
#define STDERRNO    fileno(stderr)
#endif

/* Types -------------------------------------------------------------------------------------------------------------*/
/* Macros ------------------------------------------------------------------------------------------------------------*/
#ifndef Free
#define Free(ptr)   if(ptr != NULL){free(ptr);ptr = NULL;}
#endif
#include <dirent.h>
#ifndef Opendir
#define Opendir     opendir
#endif 
#ifndef Readdir
#define Readdir     readdir
#endif 
#ifndef Closedir
#define Closedir    closedir
#endif
#ifndef Seekdir
#define Seekdir     seekdir
#endif
#ifndef Rewinddir
#define Rewinddir   rewinddir
#endif
#ifndef Telldir
#define Telldir     telldir
#endif
#ifndef Scandir
#define Scandir     scandir
#endif

#ifndef Exit
#define Exit        exit
#endif

#ifndef Sleep
#define Sleep sleep
#endif

#ifndef Fdopen
#define Fdopen      fdopen
#endif
#ifndef Fputs
#define Fputs       fputs
#endif
#ifndef Fprintf
#define Fprintf     fprintf
#endif
#ifndef Fileno
#define Fileno      fileno
#endif
#ifndef Fflush
#define Fflush      fflush
#endif
#ifndef Fclose
#define Fclose      fclose
#endif
#ifndef Fgets
#define Fgets       fgets
#endif



#ifndef Scanf
#define Scanf       scanf
#endif
#ifndef Fscanf
#define Fscanf      fscanf
#endif
#ifndef Vscanf
#define Vscanf      vscanf
#endif
#ifndef Vfscanf
#define Vfscanf     vfscanf
#endif
#ifndef Vsscanf
#define Vsscanf     vsscanf
#endif
#ifndef Vprintf
#define Vprintf     vprintf
#endif
#ifndef Vfprintf
#define Vfprintf    vfprintf
#endif


#ifndef Open
#define Open        open
#endif
#ifndef Write
#define Write       write
#endif
#ifndef Read
#define Read        read
#endif
#ifndef Close
#define Close       close
#endif

#ifndef Popen
#define Popen       popen
#endif
#ifndef Pclose
#define Pclose      pclose
#endif

#ifndef Access
#define Access      access
#endif
#ifndef Remove
#define Remove      remove
#endif

//#include <stdlib.h>
#ifndef Qsort
#define Qsort       qsort 
#endif 
#ifndef Bsearch
#define Bsearch     bsearch
#endif 


#include <sys/stat.h>
#ifndef Stat
#define Stat        stat
#endif

/* default file access permissions for new files */
#ifndef FILE_MODE
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#endif 
/* default permissions for new directories */
#ifndef DIR_MODE
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)
#endif

/**
 *  Chmod
 */
#ifndef S_USR
#define S_USR       (S_IRUSR|S_IWUSR|S_IXUSR)
#endif
#ifndef S_GRP
#define S_GRP       (S_IRGRP|S_IWGRP|S_IXGRP)
#endif
#ifndef S_OTH
#define S_OTH       (S_IROTH|S_IWOTH|S_IXOTH)
#endif
#ifndef S_R
#define S_R         (S_IRUSR|S_IRGRP|S_IROTH)
#endif
#ifndef S_W
#define S_W         (S_IWUSR|S_IWGRP|S_IWOTH)
#endif
#ifndef S_X
#define S_X         (S_IXUSR|S_IXGRP|S_IXOTH)
#endif
#ifndef S_ALL
#define S_ALL       (S_USR|S_GRP|S_OTH)
#endif
#ifndef Chmod
#define Chmod           chmod
#endif
#ifndef Chmod_R
#define Chmod_R(f)      Chmod(f, S_R);
#endif
#ifndef Chmod_W
#define Chmod_W(f)      Chmod(f, S_W);
#endif
#ifndef Chmod_X
#define Chmod_X(f)      Chmod(f, S_X);
#endif
#ifndef Chmod_USR
#define Chmod_USR(f)    Chmod(f, S_USR);
#endif
#ifndef Chmod_GRP
#define Chmod_GRP(f)    Chmod(f, S_GRP);
#endif
#ifndef Chmod_OTH
#define Chmod_OTH(f)    Chmod(f, S_OTH);
#endif
#ifndef Chmod_ALL
#define Chmod_ALL(f)    Chmod(f, S_ALL);
#endif




#include <unistd.h>
#ifndef Rmdir
#define Rmdir               rmdir
#endif
#ifndef Mkdir
#define Mkdir(dir, mask)    mkdir(dir, mask)
#endif
#ifndef Mkdir2
#define Mkdir2(dir)         mkdir(dir, 0755)
#endif


#ifndef Pipe
#define Pipe        pipe
#endif


/**
 *  Access
 */
#ifndef Access
#define Access                  access
#endif
#ifndef ACCESS_IS_EXIST
#define ACCESS_IS_EXIST(f)      Access(f, F_OK)
#endif
#ifndef ACCESS_IS_READABLE
#define ACCESS_IS_READABLE(f)   Access(f, R_OK)
#endif
#ifndef ACCESS_IS_WRITEABLE
#define ACCESS_IS_WRITEABLE(f)  Access(f, W_OK)
#endif
#ifndef ACCESS_IS_EXECUTABLE
#define ACCESS_IS_EXECUTABLE(f) Access(f, X_OK)
#endif

#include <libgen.h>
#ifndef Dirname
#define Dirname     dirname
#endif
#ifndef Basename
#define Basename    basename
#endif




#include <sys/socket.h>
#ifndef Socket
#define Socket  socket
#endif

#include <sys/ioctl.h>
#ifndef Ioctl
#define Ioctl   ioctl
#endif

#include <arpa/inet.h>
#ifndef Inet_ntoa
#define Inet_ntoa inet_ntoa
#endif
#ifndef Inet_aton
#define Inet_aton inet_aton
#endif

/**
 *  Signal
 */
#include <signal.h>
#ifndef Signal
#define Signal signal
#endif
#ifndef Kill
#define Kill kill 
#endif
#ifndef Killpg
#define Killpg killpg
#endif

#include <fcntl.h>
#ifndef Fcntl
#define Fcntl fcntl
#endif

#include <ctype.h>
#ifndef Isalnum
#define Isalnum     isalnum
#endif
#ifndef Isalpha
#define Isalpha     isalpha
#endif
#ifndef Isascii
#define Isascii     isascii
#endif
#ifndef Iscntrl
#define Iscntrl     iscntrl
#endif
#ifndef Isdigit
#define Isdigit     isdigit
#endif
#ifndef Isgraph
#define Isgraph     isgraph
#endif
#ifndef Islower
#define Islower     islower
#endif
#ifndef Isprint
#define Isprint     isprint
#endif
#ifndef Ispunct
#define Ispunct     ispunct
#endif
#ifndef Isspace
#define Isspace     isspace
#endif
#ifndef Isupper
#define Isupper     isupper
#endif
#ifndef Isxdigit
#define Isxdigit    isxdigit
#endif
#ifndef Toascii
#define Toascii     toascii
#endif
#ifndef Tolower
#define Tolower     tolower
#endif
#ifndef Toupper
#define Toupper     toupper
#endif
#ifndef Ischar
#define Ischar(ch) \
   ( Isalnum(ch)|Isalpha(ch)|Isascii(ch)|Iscntrl(ch)|\
    Isdigit(ch)|Isgraph(ch)|Islower(ch)|Isprint(ch)|Ispunct(ch)|\
    Isspace(ch)|Isupper(ch)|Isxdigit(ch) )
#endif



#include <semaphore.h>
#ifndef Sem_t
#define Sem_t                 ULONG
#endif
#ifndef Sem_init
extern  Sem_t                VCM_SemCreate();
#define Sem_init()           VCM_SemCreate()
#endif
#ifndef Sem_wait
#define Sem_wait(psem)       sem_wait(psem)
#endif
#ifndef Sem_post
#define Sem_post(psem)       sem_post(psem)
#endif
#ifndef Sem_destroy
#define Sem_destroy(psem)    sem_destroy(psem)
#endif

#include <sys/time.h>
/*
struct timeval{
	long int tv_sec;
	long int tv_usec;
};*/

#include <pwd.h>

#include <locale.h>

#include <libintl.h>


/**
 *  MySQL wrapper
 */
#include <mysql/mysql.h>
#define MYSQL                       MYSQL
#define Mysql_init(conn)            mysql_init(conn)
#define Mysql_close(conn)           mysql_close(conn)
#define Mysql_real_connect          mysql_real_connect
/**
 * /usr/include/mysql/mysql.h
424 MYSQL *     STDCALL mysql_real_connect(MYSQL *mysql, const char *host,
425                        const char *user,
426                        const char *passwd,
427                        const char *db,
428                        unsigned int port,
429                        const char *unix_socket,
430                        unsigned long clientflag);
*/
#define Mysql_query                 mysql_query
#define Mysql_error                 mysql_error
#define Mysql_store_result          mysql_store_result
#define Mysql_num_rows              mysql_num_rows
#define Mysql_fetch_row             mysql_fetch_row
#define Mysql_free_result           mysql_free_result

#if 0
/**
 *
 */
#include <libpq-fe.h>
#endif

/* Globals -----------------------------------------------------------------------------------------------------------*/
/* Functions ---------------------------------------------------------------------------------------------------------*/



#endif /*<_VCM_SYS_WRAPPER_H>*/
