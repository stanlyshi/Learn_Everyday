/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:40 CST. */
#ifndef _VCM_H
#define _VCM_H


#define CELL_NUM_MIN    1
#define CELL_NUM_MAX    19
#define CELL_NUM        (CELL_NUM_MAX-CELL_NUM_MIN+1)

typedef struct Cell_t {
    int id;
    int pid;
    int fd;
}cell_t;

void show_cell(cell_t *cell);

#endif /*<_VCM_H>*/
