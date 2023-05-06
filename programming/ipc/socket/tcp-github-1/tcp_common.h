/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:17 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:05 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:46 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:17 CST. */
#ifndef __TCP_COMMON_H_INCLUDED__
#define __TCP_COMMON_H_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT        "8888"  // the default port users will be connecting to
#define BACKLOG     10      // how many pending connections queue will hold
#define MAXDATASIZE 100     // max number of bytes we can get at once 

/** get the ip address as a string
 *
 *  return
 *      error -> NULL
 *      success -> ip address
 *
 *  note:- caller has to free the memory returned 
 *         by the function on success.
 */
char* get_ip_addr(struct sockaddr_storage* ss);

/** get the IPv4 or IPv6 address  
 *
 *  return
 *      error -> NULL
 *      success -> sockaddr_in* or sockaddr_in6*
 */
void* get_in_addr(struct sockaddr_storage* ss);

/** parse the port number from the given string.
 *
 *  return
 *      error -> -1
 *      success -> port
 */
short parse_port(char* s);

/** connect to a host on the given port
 *
 *  return
 *      error -> -1
 *      success -> socket fd
 */
int tcp_connect(char* hostname, char* port);

/** bind to the given port 
 *
 *  return
 *      error -> -1
 *      success -> socket fd
 */
int tcp_bind(char* port);

/** put the given sock fd in listen mode
 *
 *  return
 *      error -> -1
 *      success -> 1
 */
int tcp_listen(int ssock, size_t blog);

/** accept connection on the given sock fd
 *
 *  return
 *      error -> -1
 *      success -> socket fd
 */
int tcp_accept(int ssock, struct sockaddr_storage* sa);

void print_usage(char* app);

int start_server_select(char* port, int backlog);
int start_server_iteration(char* port, int backlog);
int start_server_fork(char* port, int backlog);

void sigchld_handler(int s);

#endif /* __TCP_COMMON_H_INCLUDED__ */
