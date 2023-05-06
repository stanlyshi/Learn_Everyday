/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:16 CST. */
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
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:45 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:17 CST. */
/*
 * delay_test.h
 *
 *  Created on: Jun 19, 2015
 *      Author: wei
 */

#ifndef DELAY_TEST_H_
#define DELAY_TEST_H_

static int i;
static int num;

static int ln;
static int lst[99];
static int *ue[100];

typedef  struct data
{
	int _ln;
	int _lst[99];
	int _ue[100];
} data_ue;

static data_ue data_ue_1;
////////////simulate ue assigment////////////

void simuue();

/////////  gather data  ////////////////////

data_ue packagedata();

/*  printf  */

void msgprintf(struct sockaddr_in client);

#endif /* DELAY_TEST_H_ */
