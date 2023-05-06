/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:45 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:25:11 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:52 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:23 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:24:10 CST. */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <string.h>

/**
 * 编译：
 *	g++ `net-snmp-config --cflags --libs --external-libs` \
 *		 snmp_test.c -o snmp_test
 */

/**
 *
 */
int find_last_oid (netsnmp_session * ss,oid *base,int base_length){
	netsnmp_pdu *response;
	netsnmp_pdu *pdu;
	int running = 1;
	int status;
	int length=0;
	pdu = snmp_pdu_create (SNMP_MSG_GETNEXT);
	snmp_add_null_var (pdu, base, base_length);
	while (running)
	{
		status = snmp_synch_response (ss, pdu, &response);
		if (status != STAT_SUCCESS || !response)
		{
			snmp_sess_perror ("snmp_synch_response", ss);
			exit (1);
		}
		if (response->errstat != SNMP_ERR_NOERROR)
		{
			fprintf (stderr, "snmp: Error in packet: %s\n",snmp_errstring
			(response->errstat));
			exit (1);
		}
		if (response &&snmp_oid_compare (response->variables->name,
			SNMP_MIN(base_length,response->variables->name_length),base, base_length) != 0)
		{
			running = 0;
		}
		else
		{
			memcpy(base,response->variables->name,response->variables->name_length*sizeof(oid));
			length=response->variables->name_length;
			pdu = snmp_pdu_create (SNMP_MSG_GETNEXT);
			snmp_add_null_var (pdu, response->variables->name, response->variables->name_length);
		}
		snmp_free_pdu (response);
	}
	return length;
}
/**
 * main function
 */
int main()
{
	netsnmp_session session, *ss;
	netsnmp_pdu *pdu;
	netsnmp_pdu *response;
	struct variable_list *vars;
	oid base[128]={1,3,6,1,4,1,2021,50};
	size_t base_length=8;
	int status;
	init_snmp ("APC Check");
	snmp_sess_init (&session);
	session.version = SNMP_VERSION_1;
	session.community = (u_char*)"public";
	session.community_len = strlen ((const char*)session.community);
	session.peername = "127.0.0.1";//被监控主机的 IP 地址
	ss = snmp_open (&session);
	if (ss == NULL)
	{
		snmp_sess_perror ("snmp_open", &session);
		exit (1);
	}
	int new_length=find_last_oid (ss, base, base_length);
	pdu = snmp_pdu_create (SNMP_MSG_GET);
	snmp_add_null_var (pdu, base, new_length);
	status = snmp_synch_response (ss, pdu, &response);
	if (status != STAT_SUCCESS || !response)
	{
		snmp_sess_perror ("snmp_synch_response", ss);
		exit (1);
	}
	for(vars = response->variables; vars; vars = vars->next_variable)
	{
		int i;
		for(i=0;i<vars->name_length;i++)
		{
			printf("%d ",vars->name[i]);
		}
		print_value(vars->name, vars->name_length, vars);
	}
	snmp_free_pdu (response);
	snmp_close (ss);
	return 0;
}
