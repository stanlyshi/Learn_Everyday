/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:21 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:07 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:48 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:19 CST. */
#ifndef _COMMON_H
#define _COMMON_H


#define PATH "/tmp/UNIX"

#define HELLO       "Hello"
#define GOODBYE     "Goodbye"
#define QUIT        "Quit"


#define debug() fprintf(stderr, "%s:%d\n", __func__, __LINE__)


#define VCM_STR_LEN                     256
#define MAC_MAX_NUM                     20


int unsocket_server(const char *unix_path);
int unsocket_client(const char *unix_path);


#endif /*<_COMMON_H>*/