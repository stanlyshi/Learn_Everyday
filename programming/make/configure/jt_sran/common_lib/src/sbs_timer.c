/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:31 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:41 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:25 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:00 CST. */
/* These store the amount of time that we wait until next timer expires. */
static struct timeval nearest;
static struct timeval *nearest_p;

#include <assert.h>
#include <string.h>
#include <limits.h>
#include "sbs_timer.h"
#include "timer_compat.h"
#include "linuxlist.h"
#include <stdio.h>
#include "thread.h"
#include "linuxrbtree.h"

static struct rb_root timer_root = RB_ROOT;
struct mutex timer_mutex; /*!< mutex for list */

int32_t init_timer_mutex()/*initial a mutex and set it can recursive*/
{
	int32_t res = -1;

	/*!< initilize timer_mutex */
	res = pthread_mutexattr_init(&timer_mutex.mAttribs);/*default attribute*/
	assert(!res);
	res = pthread_mutexattr_settype(&timer_mutex.mAttribs,PTHREAD_MUTEX_RECURSIVE);/*can  recursive*/
	assert(!res);
	res = pthread_mutex_init(&timer_mutex.mMutex,&timer_mutex.mAttribs);
	assert(!res);

	return res;
}

static void __add_timer(struct sbs_timer_list *timer)
{
	struct rb_node **new = &(timer_root.rb_node);
	struct rb_node *parent = NULL;

	lock(timer_mutex);

	while (*new) 
    {
    	struct sbs_timer_list *this;

    	this = container_of(*new, struct sbs_timer_list, node);

    	parent = *new;
    	if (timercmp(&timer->timeout, &this->timeout, <))
    		new = &((*new)->rb_left);
    	else
    		new = &((*new)->rb_right);
    }

    rb_link_node(&timer->node, parent, new);
    rb_insert_color(&timer->node, &timer_root);

    unlock(timer_mutex);
}

/*! \brief add a new timer to the timer management
 *  \param[in] timer the timer that should be added
 */
void sbs_timer_add(struct sbs_timer_list *timer)
{
	sbs_timer_del(timer);
	timer->active = 1;

	lock(timer_mutex);
	INIT_LLIST_HEAD(&timer->list);
	unlock(timer_mutex);

	__add_timer(timer);
}

/*! \brief schedule a timer at a given future relative time
 *  \param[in] timer the to-be-added timer
 *  \param[in] seconds number of seconds from now
 *  \param[in] microseconds number of microseconds from now
 *
 * This function can be used to (re-)schedule a given timer at a
 * specified number of seconds+microseconds in the future.  It will
 * internally add it to the timer management data structures, thus
 * sbs_timer_add() is automatically called.
 */
void sbs_timer_schedule(struct sbs_timer_list *timer, int seconds, int microseconds)
{
	struct timeval current_time;

	gettimeofday(&current_time, NULL);
	timer->timeout.tv_sec = seconds;
	timer->timeout.tv_usec = microseconds;

	lock(timer_mutex);
	timeradd(&timer->timeout, &current_time, &timer->timeout);
	unlock(timer_mutex);

	sbs_timer_add(timer);
}

/*! \brief delete a timer from timer management
 *  \param[in] timer the to-be-deleted timer
 *
 * This function can be used to delete a previously added/scheduled
 * timer from the timer management code.
 */
void sbs_timer_del(struct sbs_timer_list *timer)
{
	if (timer->active) 
    {
		//printf("%d",timer->active);
		timer->active = 0;

		lock(timer_mutex);

		rb_erase(&timer->node, &timer_root);
		/* make sure this is not already scheduled for removal. */
		if (!llist_empty(&timer->list))
			llist_del_init(&timer->list);

		unlock(timer_mutex);

	}
}

/*! \brief check if given timer is still pending
 *  \param[in] timer the to-be-checked timer
 *  \return 1 if pending, 0 otherwise
 *
 * This function can be used to determine whether a given timer
 * has alredy expired (returns 0) or is still pending (returns 1)
 */
int sbs_timer_pending(struct sbs_timer_list *timer)
{
	return timer->active;
}

/*! \brief compute the remaining time of a timer
 *  \param[in] timer the to-be-checked timer
 *  \param[in] the current time (NULL if not known)
 *  \param[out] remaining remaining time until timer fires
 *  \return 0 if timer has not expired yet, -1 if it has
 *
 *  This function can be used to determine the amount of time
 *  remaining until the expiration of the timer.
 */
int sbs_timer_remaining(const struct sbs_timer_list *timer,
			 const struct timeval *now,
			 struct timeval *remaining)
{
	struct timeval current_time;

	if (!now) {
		gettimeofday(&current_time, NULL);
		now = &current_time;
	}

	lock(timer_mutex);
	timersub(&timer->timeout, &current_time, remaining);
	unlock(timer_mutex);

	if (remaining->tv_sec < 0)
		return -1;

	return 0;
}

/*! \brief Determine time between now and the nearest timer
 *  \returns pointer to timeval of nearest timer, NULL if there is none
 *
 * if we have a nearest time return the delta between the current
 * time and the time of the nearest timer.
 * If the nearest timer timed out return NULL and then we will
 * dispatch everything after the select
 */
struct timeval *sbs_timers_nearest(void)
{
	/* nearest_p is exactly what we need already: NULL if nothing is
	 * waiting, {0,0} if we must dispatch immediately, and the correct
	 * delay if we need to wait */
	return nearest_p;
}

static void update_nearest(struct timeval *cand, struct timeval *current)
{
	if (cand->tv_sec != LONG_MAX) {
		if (timercmp(cand, current, >))
			timersub(cand, current, &nearest);
		else {
			/* loop again inmediately */
			nearest.tv_sec = 0;
			nearest.tv_usec = 0;
		}
		nearest_p = &nearest;
	} else {
		nearest_p = NULL;
	}
}

/*! \brief Find the nearest time and update nearest_p */
void sbs_timers_prepare(void)
{
	struct rb_node *node;
	struct timeval current;

	gettimeofday(&current, NULL);

	lock(timer_mutex);

	node = rb_first(&timer_root);
	if (node) {
		struct sbs_timer_list *this;
		this = container_of(node, struct sbs_timer_list, node);
		update_nearest(&this->timeout, &current);
	} else {
		nearest_p = NULL;
	}

	unlock(timer_mutex);
}

/*! \brief fire all timers... and remove them */
int sbs_timers_update(void)
{
	struct timeval current_time;
	struct rb_node *node;
	struct llist_head timer_eviction_list;
	struct sbs_timer_list *this;
	int work = 0;
	gettimeofday(&current_time, NULL);

	lock(timer_mutex);

	INIT_LLIST_HEAD(&timer_eviction_list);
	for (node = rb_first(&timer_root); node; node = rb_next(node)) 
    {
		this = container_of(node, struct sbs_timer_list, node);

	//	printf("%d\n", this->timeout.tv_sec);
	//	printf("%d\n", current_time.tv_sec);
		if (timercmp(&this->timeout, &current_time, >))
			break;
		llist_add(&this->list, &timer_eviction_list);
	}

	unlock(timer_mutex);
	/*
	 * The callbacks might mess with our list and in this case
	 * even llist_for_each_entry_safe is not safe to use. To allow
	 * sbs_timer_del to be called from within the callback we need
	 * to restart the iteration for each element scheduled for removal.
	 *
	 * The problematic scenario is the following: Given two timers A
	 * and B that have expired at the same time. Thus, they are both
	 * in the eviction list in this order: A, then B. If we remove
	 * timer B from the A's callback, we continue with B in the next
	 * iteration step, leading to an access-after-release.
	 */
restart:
	llist_for_each_entry(this, &timer_eviction_list, list) 
    {
		sbs_timer_del(this);
		this->cb(this->data);
		work = 1;
		goto restart;
	}

	return work;
}

int sbs_timers_check(void)
{
	struct rb_node *node;
	int i = 0;

	lock(timer_mutex);

	for (node = rb_first(&timer_root); node; node = rb_next(node)) {
		i++;
	}

	unlock(timer_mutex);

	return i;
}
