#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(jt_sran, 0.8, rongtao@sylincom.com)
AM_INIT_AUTOMAKE(jt_sran, 0.8)

AC_CONFIG_SRCDIR([product_om/cell_mgmt/include],
				 [product_om/cell_mgmt/src/],
				 [product_om/inet_mgmt/include/],
				 [product_om/inet_mgmt/src/],
				 [product_om],
				 [product_om/perf_mgmt/include/],
				 [product_om/perf_mgmt/src/],
				 [vos/include/],
				 [vos])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

# Checks for programs.
#AC_PROG_AWK
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([mysql/mysql.h libpq-fe.h  arpa/inet.h fcntl.h libintl.h locale.h malloc.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero getcwd inet_ntoa rpmatch select strerror])

#AC_CONFIG_FILES([product_om/cell_mgmt/Makefile
#                 product_om/inet_mgmt/Makefile])

AC_OUTPUT
