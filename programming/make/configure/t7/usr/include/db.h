/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:40 CST. */
#ifndef _DB_H
#define _DB_H 1

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



#endif /*<_DB_H>*/