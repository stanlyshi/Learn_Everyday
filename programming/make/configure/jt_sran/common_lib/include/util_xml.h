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
/******************************************************************************
** 
** Copyright (c) 2008-2018 ICT/CAS.
** All rights reserved.
**
** File name: 
** Description:
**
** Current Version: 
** $Revision$
** Author: 
** Date: 
**
******************************************************************************/
#ifndef _UTIL_XML_H
#define _UTIL_XML_H

/* Dependencies --------------------------------------------------------------*/
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/
/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
char* get_xml_para(const char *filename, const char *label);

/******************************************************************************
 * Description:
 *
 * Input: 
 *
 * Output: 
 ******************************************************************************/
char* 
get_xml_label_value(const char *docname, const int argc, const char **argv);

/******************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int change_lable(xmlNodePtr curnode, const char *lable, const char *value);

/******************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ******************************************************************************/
xmlDocPtr cfg_parse(char *szDocName);

/******************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ******************************************************************************/
xmlXPathContextPtr cfg_context(xmlDocPtr doc);

/******************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int alter_cfg_node_value(xmlXPathContextPtr context, 
                            const xmlChar *xpath, const xmlChar *content);
int alter_cfg_node_value_next(xmlXPathContextPtr context, const xmlChar *xpath, const xmlChar *content, int index);

/******************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ******************************************************************************/
xmlChar* get_cfg_node_value(xmlXPathContextPtr context, const xmlChar *xpath);

/******************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int alter_cfg_node_attr(xmlXPathContextPtr context, 
                          const xmlChar *xpath, const xmlChar *attr);
int alter_cfg_node_attr_next(xmlXPathContextPtr context, const xmlChar *xpath, const xmlChar *attr);
/******************************************************************************
 * Description: 
 *      Copyright (C) VBSM Code
 * Input: 
 *
 * Output: 
 ******************************************************************************/
int save_cfg(char *szDocName,xmlDocPtr doc,xmlXPathContextPtr context);


#endif /*<_UTIL_XML_H>*/