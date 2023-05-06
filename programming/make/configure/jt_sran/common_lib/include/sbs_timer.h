/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:40 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:24 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:00 CST. */

/*! \defgroup timer Sylincom timers
 *  @{
 */

/*! \file timer.h
 *  \brief Sylincom timer handling routines
 */

#ifndef SBS_TIMER_H
#define SBS_TIMER_H

#include <sys/time.h>

#include "linuxlist.h"
#include "linuxrbtree.h"

/**
 * Timer management:
 *      - Create a struct sbs_timer_list
 *      - Fill out timeout and use add_timer or
 *        use schedule_timer to schedule a timer in
 *        x seconds and microseconds from now...
 *      - Use del_timer to remove the timer
 *
 *  Internally:
 *      - We hook into select.c to give a timeval of the
 *        nearest timer. On already passed timers we give
 *        it a 0 to immediately fire after the select
 *      - update_timers will call the callbacks and remove
 *        the timers.
 *
 */
/*! \brief A structure representing a single instance of a timer */
struct sbs_timer_list {
	struct rb_node node;	  /*!< \brief rb-tree node header */
	struct llist_head list;   /*!< \brief internal list header */
	struct timeval timeout;   /*!< \brief expiration time */
	unsigned int active : 1  ; /*!< \brief is it active? */

	void (*cb)(void*);	  /*!< \brief call-back called at timeout */
	void *data;		  /*!< \brief user data for callback */
};

/**
 * timer management
 */

void sbs_timer_add(struct sbs_timer_list *timer);

void sbs_timer_schedule(struct sbs_timer_list *timer, int seconds, int microseconds);

void sbs_timer_del(struct sbs_timer_list *timer);

int sbs_timer_pending(struct sbs_timer_list *timer);

int sbs_timer_remaining(const struct sbs_timer_list *timer,
			 const struct timeval *now,
			 struct timeval *remaining);

int32_t init_timer_mutex();
/*
 * internal timer list management
 */
struct timeval *sbs_timers_nearest(void);
void sbs_timers_prepare(void);
int sbs_timers_update(void);
int sbs_timers_check(void);

/*! @} */

#endif
