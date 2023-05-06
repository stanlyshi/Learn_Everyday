AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=bin/jt_sran

bin_jt_sran_SOURCES=${top_srcdir}/product_om/cell_mgmt/include/cell_mgmt_api.h \
					${top_srcdir}/product_om/cell_mgmt/include/vcm_alarm.h \
					${top_srcdir}/product_om/cell_mgmt/include/vcm_cell_entity.h \
					${top_srcdir}/product_om/cell_mgmt/include/vcm_config.h \
					${top_srcdir}/product_om/cell_mgmt/include/vcm_db.h \
					${top_srcdir}/product_om/cell_mgmt/include/vcm_errno.h \
					${top_srcdir}/product_om/cell_mgmt/include/vcm.h \
					${top_srcdir}/product_om/cell_mgmt/include/vcm_msgs.h \
					${top_srcdir}/product_om/cell_mgmt/include/vcm_plugin.h \
					${top_srcdir}/product_om/cell_mgmt/include/vcm_sm.h \
					${top_srcdir}/product_om/cell_mgmt/include/vcm_sys_wrapper.h \
					${top_srcdir}/product_om/cell_mgmt/include/vcm_tools.h \
					${top_srcdir}/product_om/cell_mgmt/include/vcm_types.h \
					${top_srcdir}/product_om/cell_mgmt/src/vcm_cell.c \
					${top_srcdir}/product_om/cell_mgmt/src/vcm_cell_setup.c \
					${top_srcdir}/product_om/cell_mgmt/src/vcm_init.c \
					${top_srcdir}/product_om/cell_mgmt/src/vcm_module_register.c \
					${top_srcdir}/product_om/cell_mgmt/src/vcm_tools.c \
					${top_srcdir}/product_om/cell_mgmt/src/vcm_cell_delete.c \
					${top_srcdir}/product_om/cell_mgmt/src/vcm_cell_stop.c \
					${top_srcdir}/product_om/cell_mgmt/src/vcm_l1l2l3.c \
					${top_srcdir}/product_om/cell_mgmt/src/vcm_sm.c \
					${top_srcdir}/product_om/inet_mgmt/include/inet_mgmt_api.h \
					${top_srcdir}/product_om/inet_mgmt/include/inet_mgmt_config.h \
					${top_srcdir}/product_om/inet_mgmt/include/inet_mgmt.h \
					${top_srcdir}/product_om/inet_mgmt/include/inet_mgmt_types.h \
					${top_srcdir}/product_om/inet_mgmt/src/inet_mgmt.c \
					${top_srcdir}/product_om/inet_mgmt/src/inet_mgmt_socket_register.c \
					${top_srcdir}/product_om/om_main.c \
					${top_srcdir}/product_om/perf_mgmt/include/perf_mgmt_api.h \
					${top_srcdir}/product_om/perf_mgmt/include/perfs_config.h \
					${top_srcdir}/product_om/perf_mgmt/include/perfs_l1l2l3_perf.h \
					${top_srcdir}/product_om/perf_mgmt/include/perfs_sm.h \
					${top_srcdir}/product_om/perf_mgmt/include/perfs_cell_perf.h \
					${top_srcdir}/product_om/perf_mgmt/include/perfs.h \
					${top_srcdir}/product_om/perf_mgmt/include/perfs_msgs.h  \
					${top_srcdir}/product_om/perf_mgmt/include/perfs_types.h \
					${top_srcdir}/product_om/perf_mgmt/src/perfs_handler.c  \
					${top_srcdir}/product_om/perf_mgmt/src/perfs_init.c \
					${top_srcdir}/product_om/perf_mgmt/src/perfs_tools.c \
					${top_srcdir}/vos/include/vos_types.h \
					${top_srcdir}/vos/include/vos_cli.h \
					${top_srcdir}/vos/include/vos_ctype.h \
					${top_srcdir}/vos/include/vos_linklist.h \
					${top_srcdir}/vos/include/vos_que.h \
					${top_srcdir}/vos/include/vos_string.h \
					${top_srcdir}/vos/include/vos_sysmsg.h \
					${top_srcdir}/vos/include/vos_timer.h \
					${top_srcdir}/vos/include/vos_common.h  \
					${top_srcdir}/vos/include/vos_lib.h \
					${top_srcdir}/vos/include/vos_module.h \
					${top_srcdir}/vos/include/vos_sem.h \
					${top_srcdir}/vos/include/vos_sys_diff.h \
					${top_srcdir}/vos/include/vos_task.h

AM_CFLAGS= -I ${top_srcdir}/vos/include/ \
		   -I ${top_srcdir}/product_om/perf_mgmt/include/ \
		   -I ${top_srcdir}/product_om/inet_mgmt/include/ \
		   -I ${top_srcdir}/product_om/cell_mgmt/include/ \
		   -I /usr/include/mysql \
		   -I /usr/include/libxml2

CFLAGS= -lconfig -pthread -lxml2 -lrt -lnetsnmp -lnetsnmpmibs -lnetsnmpagent -ldl -w -lm -DOS_LINUX -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -lssl -lcrypto -ldl

#LDFLAGS=${top_srcdir}/vos/libgros.a 
LDADD=${top_srcdir}/vos/libgros.a

