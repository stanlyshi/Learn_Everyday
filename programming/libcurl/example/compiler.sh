#!/bin/bash
# rongtao

gcc $* `curl-config --libs --cflags` `pkg-config --libs --cflags openssl gtk+-2.0 gthread-2.0 libevent` \
		-I /usr/include/libxml2/ -lxml2 -leventviews -levent_pthreads /usr/local/lib/libuv.a -ldl -lgnutls -lgthread-2.0 -ldl \
		-lexpat