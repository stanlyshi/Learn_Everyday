/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:40 CST. */
#include <stdio.h>

#include "vcm.h"

void show_cell(cell_t *cell)
{
    if(cell != NULL)
    printf("Cell: id = %d, pid = %d, fd = %d\n",
            cell->id, cell->pid, cell->fd);
}