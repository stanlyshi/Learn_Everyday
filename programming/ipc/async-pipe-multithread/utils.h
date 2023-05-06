#ifndef __UTILS_H
#define __UTILS_H

#include <stdio.h>
#include <sys/time.h>
#include <stdarg.h>


/**
 *  打印log debug信息
 *  作者： 荣涛
 *  时间： s2020年7月15日10:12:24
 */
enum {
    __LV_INFO,
    __LV_WARNING,
    __LV_ERR,
    __LV_DEBUG,
};


#define log_info(fmt...) ___debug_log(__LV_INFO, __FILE__, __func__ ,__LINE__, fmt)
#define log_warn(fmt...) ___debug_log(__LV_WARNING, __FILE__, __func__ ,__LINE__, fmt)
#define log_error(fmt...) ___debug_log(__LV_ERR, __FILE__, __func__ ,__LINE__, fmt)
#define log_debg(fmt...) ___debug_log(__LV_DEBUG, __FILE__, __func__ ,__LINE__, fmt)


inline long int gettimeval(struct timeval *tv);

inline double statistic_throughput(char *description, 
            struct timeval *before, struct timeval *after, unsigned long int bytes, long int npkg);

inline int ___debug_log(int level, char *file, const char *func, int line, char *fmt, ...);


#endif
