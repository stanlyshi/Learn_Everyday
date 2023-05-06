/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:40 CST. */
#include <stdio.h>

#include "bord.h"

void show_bord(bord_t *bord)
{
    if(bord != NULL)
    printf("Bord: id = %d, pid = %d, fd = %d\n",
            bord->id, bord->pid, bord->fd);
}