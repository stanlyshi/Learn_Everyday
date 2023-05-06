AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=lib/libcutil.so
lib_libcutil_so_SOURCES=${top_srcdir}/common/include/util_biter.h \ 
                       ${top_srcdir}/common/include/util_characters.h \ 
                       ${top_srcdir}/common/include/util_dir.h \ 
                       ${top_srcdir}/common/include/util_endianer.h \ 
                       ${top_srcdir}/common/include/util_exec.h \ 
                       ${top_srcdir}/common/include/util_omp.h \ 
                       ${top_srcdir}/common/include/util_printer.h \ 
                       ${top_srcdir}/common/include/util_systemer.h \ 
                       ${top_srcdir}/common/include/util_terminal.h \ 
                       ${top_srcdir}/common/include/util_xml.h \ 
                       ${top_srcdir}/common/src/util_biter.c \ 
                       ${top_srcdir}/common/src/util_characters.c \ 
                       ${top_srcdir}/common/src/util_dir.c \ 
                       ${top_srcdir}/common/src/util_endianer.c \ 
                       ${top_srcdir}/common/src/util_exec.c \ 
                       ${top_srcdir}/common/src/util_omp.c \ 
                       ${top_srcdir}/common/src/util_printer.c \ 
                       ${top_srcdir}/common/src/util_systemer.c \ 
                       ${top_srcdir}/common/src/util_terminal.c \ 
                       ${top_srcdir}/common/src/util_xml.c

AM_CFLAGS=-I ${top_srcdir}/common/include/ \
            -I /usr/include/libxml2

CFLAGS=-lxml2 -w -lm -shared -fPIC -fopenmp