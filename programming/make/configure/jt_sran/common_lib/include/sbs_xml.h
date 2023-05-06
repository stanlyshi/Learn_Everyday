/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:30 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:40 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:24 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:00 CST. */
/************************************************************************
 *Copyright (c) 2018 Sylincom.
 *All rights reserved.
 *
 *File name: sbs_xml.h
 *Description: xml parser utils.
 *
 *Current Version: 0.l
 *Author: wangcheng2017@ict.ac.cn
 *Date: 2018.09.11
 *
 ************************************************************************/

#ifndef SBS_XML_UTIL_H
#define SBS_XML_UTIL_H
/* Includes-------------------------------------------------------------*/
#include "sbs_type.h"
#include <assert.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpointer.h>



/* Macros---------------------------------------------------------------*/
#define RETSET  256
/* Variables------------------------------------------------------------*/
extern char ret[RETSET][RETSET];

/* Declarations---------------------------------------------------------*/
/* Constants------------------------------------------------------------*/
/* Functions------------------------------------------------------------*/

/**********************************************
** open xml file.
**
** Input:
** 		xml_name.
** Output:
** 		xmlDocPtr.
** Return:
**      
**      
**
***********************************************/
extern xmlDocPtr utils_xml_open(const char* xml_name);

/**********************************************
** get xmlXPathContextPtr.
**
** Input:
** 		xmlDocPtr.
** Output:
** 		xmlXPathContextPtr.
** Return:
**      
**      
**
***********************************************/
extern xmlXPathContextPtr utils_xml_xpath_context(xmlDocPtr doc);

/**********************************************
** get xml nodeset.
**
** Input:
** 		xmlDocPtr, xpath.
** Output:
** 		xmlXPathObjectPtr.
** Return:
**      
**      
**
***********************************************/
extern xmlXPathObjectPtr utils_xml_get_node_set(xmlDocPtr pdoc, const xmlChar *xpath);

/**********************************************
** get xml node value by xpath.
**
** Input:
** 		xmlDocPtr, path, buf.
** Output:
** 		
** Return:
**      success 0
**      error   -1
**
***********************************************/
extern INT32 utils_xml_get_node_value_by_xpath(xmlDocPtr pdoc, const xmlChar* path, char* buf);

/**********************************************
** get xml node value by node set.
**
** Input:
** 		xmlXPathObjectPtr, xmlChar*, buf.
** Output:
** 		
** Return:
**      success 0
**      error   -1
**
***********************************************/
extern INT32 utils_xml_get_node_value_by_node_set(xmlXPathObjectPtr xpop, const xmlChar* node_name, char* buf);

/**********************************************
** set xml node value by xpath.
**
** Input:
** 		xmlDocPtr, xmlChar*, xmlChar*.
** Output:
** 		
** Return:
**      success 0
**      error   -1
**
***********************************************/
extern INT32 utils_xml_set_node_value_by_xpath(xmlDocPtr pdoc, const xmlChar* path, const xmlChar* new_value);

/**********************************************
** set xml node value by nodeset.
**
** Input:
** 		xmlXPathObjectPtr, xmlChar*, xnlChar*.
** Output:
** 		
** Return:
**      success 0
**      error   -1
**
***********************************************/
extern INT32 utils_xml_set_node_value_by_node_set(xmlXPathObjectPtr xpop, const xmlChar* node_name, const xmlChar* new_value);

/**********************************************
** clean xpath Object.
**
** Input:
** 		xmlXPathObjectPtr.
** Output:
** 		
** Return:
**      success 0
**      error   -1
**
***********************************************/
extern INT32 utils_xml_xpath_clean(xmlXPathObjectPtr xpop);

/**********************************************
** save xml file.
**
** Input:
** 		xmlDocPtr, xml_new_name.
** Output:
** 		
** Return:
**      success 0
**      error   -1
**
***********************************************/
extern INT32 utils_xml_save(xmlDocPtr pdoc, const char* xml_name);

/**********************************************
** clean xml parser sources.
**
** Input:
** 		xmlDocPtr.
** Output:
** 		
** Return:
**      success 0
**      error   -1
**
***********************************************/
extern INT32 utils_xml_clean(xmlDocPtr pdoc);

#endif
