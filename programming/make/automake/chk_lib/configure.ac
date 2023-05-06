#                                               -*- Autoconf -*-
# 脚本功能描述：检测OM依赖环境并进行环境部署
# 
# 开发部门：实训平台研发部
# 开发人员:	荣涛
# 开发时间: 2020.01
# 

AC_PREREQ([2.69])
AC_INIT([OMDeployChk], [0.1], [rongtao@sylincom.com])

AC_CONFIG_HEADERS([omdeploychk_config.h])

###########################################################################################
# Checks for programs.
AC_PROG_CC #cc
#AC_PROG_GCC #gcc
AC_PROG_GREP #grep
AC_PROG_SED #sed

#############################################################################################
# Checks for libraries.
# 检查“数学库”是否存在，math.h
AC_CHECK_LIB(m, pow, [echo "checking find -lm, ok"], [echo "checking find -lm, not ok"; exit -1])

# 检查“消息队列库”是否存在，mqueue.h
AC_CHECK_LIB(rt, mq_open, [
		AC_CHECK_HEADER([mqueue.h], [echo "checking support message queue, ok"], 
									[echo "not support message queue, not ok"; exit -1])
	], [
		echo "Did not find rt library, mqueue.h not exist"; exit -1
	])
# 检查“动态链接加载器”是否存在，dlfcn.h
AC_CHECK_LIB(dl, dlopen, [
		AC_CHECK_HEADER([dlfcn.h], [echo "checking support dlfcn, ok"],
									[echo "ERROR:::: not support dlfcn, not ok"; exit -1])
	], [
		echo "ERROR:::: Did not find dl library, dlfcn.h not exist"; exit -1
	])

# 检查“libxml2库”是否存在，libxml2/xxx.h
AC_CHECK_LIB(xml2, xmlReadFile, [
#		AC_CHECK_HEADER([libxml2/libxml/parser.h libxml2/libxml/tree.h],
#						[echo "checking support xml2, ok"],
#						[echo "ERROR:::: not support xml2, not ok"; exit -1]) 	
	], [
		echo "ERROR::::: Did not find xml2 library, libxml2/libxml/parser.h not exist"; exit -1
	])

# 检查“线程库”是否存在
AC_CHECK_LIB(pthread, pthread_create, [echo "checking pthread.h, ok"], [echo "checking pthread, not ok"; exit -1])

# 检查“config库”是否存在
#AC_CHECK_LIB(config, )

# Checks for header files.
AC_CHECK_HEADER([mysql/mysql.h],[echo "SUCC:::: Find mysql.h"], [echo "ERROR:::: Did not find mysql.h"; exit -1])

AC_CHECK_HEADER_STDBOOL
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT64_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero getcwd inet_ntoa rpmatch select strerror])


AC_OUTPUT
