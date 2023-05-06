AUTOMAKE_OPTIONS=foreign

SUBDIRS=common

bin_PROGRAMS=main
main_SOURCES=${top_srcdir}/main.c


AM_CFLAGS=-I ${top_srcdir}/common/include/ ${top_srcdir}/common/lib/libcutil.so

CFLAGS=-lxml2 -w -lm -fopenmp