/*
 * XXX This sample code was once meant to show how to use the basic Libevent
 * interfaces, but it never worked on non-Unix platforms, and some of the
 * interfaces have changed since it was first written.  It should probably
 * be removed or replaced with something better.
 *
 * Compile with:
 * cc -I/usr/local/include -o time-test time-test.c -L/usr/local/lib -levent
 */

#include <sys/types.h>

#include <event2/event-config.h>

#include <sys/stat.h>

#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>


struct timeval lasttime;

int event_is_persistent;

static void timeout_cb(evutil_socket_t fd, short event, void *arg)
{
	struct timeval newtime, difference;
	struct event *timeout = arg;
	double elapsed;

	evutil_gettimeofday(&newtime, NULL);
	evutil_timersub(&newtime, &lasttime, &difference);
	elapsed = difference.tv_sec +
	    (difference.tv_usec / 1.0e6);

	printf("timeout_cb called at %d: %.3f seconds elapsed.\n",
	    (int)newtime.tv_sec, elapsed);
	lasttime = newtime;

	if (! event_is_persistent) {
		struct timeval tv;
		evutil_timerclear(&tv);
		tv.tv_sec = 2;
		event_add(timeout, &tv);
	}
}

static void timeout_cb2(evutil_socket_t fd, short event, void *arg)
{
	struct event *timeout = arg;
    
    static int callcnt = 0;
    printf("callcnt = %d\n", ++callcnt);

    
	struct timeval tv;
	evutil_timerclear(&tv);
	tv.tv_sec = 2;
	event_add(timeout, &tv);
}

int main(int argc, char **argv)
{
	struct event timeout, timeout2;
	struct timeval tv;
	struct event_base *base;
	int flags;

	if (argc == 2 && !strcmp(argv[1], "-p")) {
		event_is_persistent = 1;
		flags = EV_PERSIST;
	} else {
		event_is_persistent = 0;
		flags = 0;
	}

	/* Initialize the event library */
	base = event_base_new();

	/* Initialize one event */
	event_assign(&timeout, base, -1, flags, timeout_cb, (void*) &timeout);
	event_assign(&timeout2, base, -1, flags, timeout_cb2, (void*) &timeout2);

	evutil_timerclear(&tv);
	tv.tv_sec = 2;
	event_add(&timeout, &tv);
	event_add(&timeout2, &tv);

	evutil_gettimeofday(&lasttime, NULL);

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	event_base_dispatch(base);

	return (0);
}

