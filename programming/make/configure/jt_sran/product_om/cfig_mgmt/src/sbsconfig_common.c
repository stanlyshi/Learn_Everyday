/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:33 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:45 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:29 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:04 CST. */
/*
 * sbs_common.c
 *
 *  Created on: Jul 12, 2018
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>
#include <string.h>
#include "sbsconfig_common.h"
#include "util_xml.h"
#include <sys/socket.h>

/*******************************************************************************
 * generate the protocol file .
 *
 * Input: source_path: the path of the source file;
 *        det_path:the path of the  destination file;
 * Output:  return -1,failed;
 *          return 0, success
 ******************************************************************************/
pid_t generate_protocol_docu(char *source_path, char *det_path)
{
	/*define local variable.*/
	pid_t 	status = -1; 			/*store the value returned by system().*/
	char	arg_tmp[STR_LEN_256];
 	/* Check for null pointer.*/
 	if ((NULL == source_path)||(NULL == det_path))
	{
 		//log_msg(LOG_ERR, VBS_MGMT, "the path is NULL.\n");
 		return ERROR;
 	}
 	sprintf (arg_tmp, "mkdir -p %s;cp -r %s/* %s", det_path, source_path, det_path);
    status = system(arg_tmp);

    return status;
}

pid_t remove_protocol_docu(char *source_path)
{
	/*define local variable.*/
	pid_t 	status = -1; 			/*store the value returned by system().*/
	char	arg_tmp[STR_LEN_256];
 	/* Check for null pointer.*/
 	if (NULL == source_path)
	{
 		//log_msg(LOG_ERR, VBS_MGMT, "the path is NULL.\n");
 		return ERROR;
 	}
 	sprintf (arg_tmp, "rm -rf %s", source_path);
    status = system(arg_tmp);

    return status;
}

char *generate_protocol_dic(char *des, int index)
{
	memset(des, 0, sizeof(STR_LEN_256));
	char pro_path_index[8] = {0};
    sprintf(pro_path_index, "/%d", index);
    strcat(des, PROTOCOL_PATH);
    strcat(des, pro_path_index);
    return des;
}


/*operate_type:0 modify value 1: add new node
 *filetype: 0:SICfg  1: Cfg*/
int modify_cfg_file( const char *pathname, const char *nodename, const char *nodevalue, const int operate_type, const int file_type,const int index)
{
	xmlDocPtr doc;
	xmlXPathContextPtr context;
	xmlDocPtr doc1;
	xmlXPathContextPtr context1;
	char str[18] = { 0 };
	char arg_tmp[STR_LEN_256] = { 0 };
	/*generate cfg.xer.*/
	if (FILE_TYPE_SICFG == file_type)
	{
		sprintf(arg_tmp, "%s/cfgfile/SIConfig.xer", pathname);
	}
	else if(FILE_TYPE_CFG == file_type)
	{
		sprintf(arg_tmp, "%s/cfgfile/cfg.xer", pathname);
	}
	doc = cfg_parse(arg_tmp);
	if (doc != NULL)
	{
		context = cfg_context(doc);
	}
	else
	{
		return ERROR;
	}
	if (context == NULL)
	{
		return ERROR;
	}

	/*alter argument.*/
	if (OPERATE_MODIFY_VALUE == operate_type)
	{
		alter_cfg_node_value(context, nodename, nodevalue);
	}
	else if (OPERATE_MODIFY_NAME == operate_type)
	{
		//alter_cfg_node_attr(context1,(const xmlChar *)"/SIConfig/mib/dl-Bandwidth/","n6");
		alter_cfg_node_attr(context, (const xmlChar *)nodename, nodevalue);
	}
	else if(OPERATE_MODIFY_NAME_NEXT == operate_type)
	{
		alter_cfg_node_attr_next(context, (const xmlChar *)nodename, nodevalue);
	}
	else if (OPERATE_MODIFY_VALUE_NEXT == operate_type)
	{
		alter_cfg_node_value_next(context, nodename, nodevalue,index);
	}


	/*save the alter file.*/

	save_cfg(arg_tmp, doc, context);


	return 0;

}
/*operate_type:0 modify value 1: add new node
 *filetype: 0:SICfg  1: Cfg*/
int modify_phycfg_file( const char *pathname, const char *nodename, const char *nodevalue)
{
	xmlDocPtr doc;
	xmlXPathContextPtr context;
	char str[18] = { 0 };
	doc = cfg_parse(pathname);
	if (doc != NULL)
	{
		context = cfg_context(doc);
	}
	else
	{
		return ERROR;
	}
	if (context == NULL)
	{
		return ERROR;
	}

	/*alter argument.*/
    alter_cfg_node_value(context, nodename, nodevalue);



	/*save the alter file.*/

	save_cfg(pathname, doc, context);


	return 0;

}

int modify_proTophycfg_file( const char *pathname, const char *nodename, const char *nodevalue, const int operate_type)
{
	xmlDocPtr doc;
	xmlXPathContextPtr context;
	char str[18] = { 0 };
	doc = cfg_parse(pathname);
	if (doc != NULL)
	{
		context = cfg_context(doc);
	}
	else
	{
		return ERROR;
	}
	if (context == NULL)
	{
		return ERROR;
	}
	if (OPERATE_MODIFY_VALUE == operate_type)
	{
		alter_cfg_node_value(context, nodename, nodevalue);
	}
	else if (OPERATE_MODIFY_NAME == operate_type)
	{
		//alter_cfg_node_attr(context1,(const xmlChar *)"/SIConfig/mib/dl-Bandwidth/","n6");
		alter_cfg_node_attr(context, (const xmlChar *)nodename, nodevalue);
	}


	/*save the alter file.*/

	save_cfg(pathname, doc, context);


	return 0;

}


char* itobs(int n, char *ps)
{
    int i;
    static int size = 8 * sizeof(int);
    for (i = size-1; i>=0; i--, n=n >> 1)
    {
    	ps[i]=( 01 & n) + '0';


    }
    ps[size]='\0';
    return ps;
}

void
init_sbs_mibs (void)
{

  /*
   * here we initialize all the tables we're planning on supportinfg
   */
  protocolCommConfigTable_createEntry (1);
  protocolAccessConfigTable_createEntry (1);
  protocolPowerConfigTable_createEntry (1);
  protocolLogConfigTable_createEntry (1);

}
