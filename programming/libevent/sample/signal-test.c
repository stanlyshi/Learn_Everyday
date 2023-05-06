/*
 * Compile with:
 * cc -I/usr/local/include -o signal-test \
 *   signal-test.c -L/usr/local/lib -levent
 */

#include <sys/types.h>

#include <event2/event-config.h>

#include <sys/stat.h>
#include <sys/queue.h>
#include <unistd.h>
#include <sys/time.h>

#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <event2/event.h>

int called = 0;

static void
signal_cb(evutil_socket_t fd, short event, void *arg)
{
	struct event *signal = arg;

	printf("signal_cb: count %d\n", called);

	if (called >= 2)
		event_del(signal);

	called++;
}

int
main(int argc, char **argv)
{
	struct event *signal_int = NULL;
	struct event_base* base;
	int ret = 0;


	/* Initialize the event library */
	base = event_base_new();
	if (!base) {
		ret = 1;
		goto out;
	}

	/* Initialize one event */
	signal_int = evsignal_new(base, SIGINT, signal_cb, NULL);
	if (!signal_int) {
		ret = 2;
		goto out;
	}
    
	event_add(signal_int, NULL);

	event_base_dispatch(base);

out:
	if (signal_int)
		event_free(signal_int);
	if (base)
		event_base_free(base);
	return ret;
}

