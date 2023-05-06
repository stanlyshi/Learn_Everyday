/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:40 CST. */
#ifndef _BORD_H
#define _BORD_H


#define BORD_NUM_MIN    1
#define BORD_NUM_MAX    19
#define BORD_NUM        (BORD_NUM_MAX-BORD_NUM_MIN+1)

typedef struct Bord_t {
    int id;
    int pid;
    int fd;
}bord_t;

void show_bord(bord_t *bord);

#endif /*<_BORD_H>*/
