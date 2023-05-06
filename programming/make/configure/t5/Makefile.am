AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=bin/server bin/client

bin_server_SOURCES=${top_srcdir}/src/server.c \
                ${top_srcdir}/common/common.c \
                ${top_srcdir}/common/common.h
                
bin_client_SOURCES=${top_srcdir}/src/client.c  \
                ${top_srcdir}/common/common.c \
                ${top_srcdir}/common/common.h
                
AM_CFLAGS= -I ./common

CFLAGS=-pthread -lxml2 -lrt