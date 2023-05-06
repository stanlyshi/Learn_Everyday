#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])

AC_INIT(jt_sran, 1.0.1, RongTao@sylincom.com)
AM_INIT_AUTOMAKE(jt_sran, 1.0.1)

AC_CONFIG_SRCDIR([common_lib/include/],
                 [common_lib/src/],
                 [product_om/bord_mgmt/src/],
                 [product_om/bord_mgmt/H/],
                 [product_om/cell_mgmt/src/],
                 [product_om/cell_mgmt/include/],
                 [product_om/cfig_mgmt/src/],
                 [product_om/cfig_mgmt/include/],
                 [product_om/common],
                 [product_om/inet_mgmt/],
                 [product_om/inet_mgmt/inet_mgmt_h/],
                 [product_om/main_mgmt/src/],
                 [product_om/main_mgmt/include/],
                 [product_om/rrus_mgmt/src/],
                 [product_om/rrus_mgmt/include/],
                 [product_om/timer/src/],
                 [product_om/timer/include/],
                 [product_om])
                 
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h malloc.h memory.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h termio.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero endpwent getcwd gethostname gettimeofday inet_ntoa memset select socket strerror strstr])

AC_OUTPUT
