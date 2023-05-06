/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:38 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:48 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:32 CST. */
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:06 CST. */
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

/* Dependencies --------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>
#include <libgen.h>

#include "xml.h"

/* Constants -----------------------------------------------------------------*/
/* Types -------------------------------------------------------------------- */
/* Macros --------------------------------------------------------------------*/
/* Globals -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/
/******************************************************************************
 * Description: 
 *
 * Input:
 * Output: 
 ******************************************************************************/
char* get_xml_para(const char *filename, const char *label)
{
    //debug();

    if(filename == NULL || label == NULL)
    {
        return NULL;
    }
    
    FILE    *fp = NULL;
    char    buffer[4096] = {0};
    char*   start = NULL;
    char*   end = NULL;
    
    char l1[strlen(label)+2], l2[strlen(label)+3];
    sprintf(l1, "<%s>", label);
    sprintf(l2, "</%s>", label);
    
    if ((fp = fopen(filename, "r")) == NULL)
    {
        return NULL;
    }
    while (fgets(buffer, 4096, fp))
    {
        if ((start = strstr(buffer, l1)) != NULL)
        {
            if ((end = strstr(start, l2)) != NULL)
            {
                *end = '\0';
                fclose(fp);
                return (start + strlen(label)+2);
            }
        }
    }
    fclose(fp);
    return NULL;
}

/******************************************************************************
 * Description:
 *
 * Input:
 * Output:
 ******************************************************************************/
char* get_xml_label_value(const char *docname, 
                          const int argc, 
                          const char **argv)
{
    //debug();

    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlChar *keyword = NULL;
    
    int flag = 0;
    
    doc = xmlParseFile(docname);
    if(doc == NULL)
    {
    	return NULL;
    }
    cur = xmlDocGetRootElement(doc);
    if(cur == NULL)
    {
        xmlFreeDoc(doc);
        return NULL;
    }
    
    if(xmlStrcmp(cur->name, (const xmlChar *)argv[flag]))
    {
        xmlFreeDoc(doc);
        return NULL;
    }
    
    
    cur = cur->xmlChildrenNode;
    flag++;
    while(cur != NULL)
    {
        if((!xmlStrcmp(cur->name, (const xmlChar *)argv[flag])))
        {
            if(flag == argc-1 &&
                !xmlStrcmp(cur->name, (const xmlChar *)argv[argc-1]))
            {
                keyword = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                change_lable(cur, argv[argc-1], "change");
                break;
            }
            flag++;
            
            cur = cur->xmlChildrenNode;
        }
        cur = cur->next;
    }
    xmlSaveFile("test.xml", doc);
    xmlFreeDoc(doc);
    return (char *)keyword;
}



/******************************************************************************
 * Description: modify the argument of cfg.xer.
 *
 * Input: the pathname + filename
 *
 * Output: return -1,   system error;
 *         return 0,    success
 ******************************************************************************/
int change_lable(xmlNodePtr curnode, const char *lable, const char *value)
{
    //debug();

    while (curnode != NULL)
    {
        if (0 == xmlStrcmp(curnode->name, (const xmlChar *)lable))
        {
            xmlNodeSetContent(curnode, (const xmlChar *)value);
            return 0;
        }
        else
        {
            if (0 == change_lable(curnode->xmlChildrenNode, lable, value))
            {
                return 0;
            }
            curnode = curnode->next;
        }
    }
    return -1;
}

/******************************************************************************
 * Description: parse the doc.
 *
 * Input: szdocname: doc name;
 *
 * Output:  return -1,failed;
 *          return 0, success
 ******************************************************************************/
xmlDocPtr cfg_parse(char *szDocName)
{
    //debug();

    xmlDocPtr  doc = xmlReadFile(szDocName, "utf-8", XML_PARSE_RECOVER);
    if (doc == NULL)
    {
        return NULL;
    }
    return doc;
}

/******************************************************************************
 * Description: get the context of the doc.
 *
 * Input: szdocname: doc name;
 *
 * Output:  return -1,failed;
 *          return 0, success
 ******************************************************************************/
xmlXPathContextPtr cfg_context(xmlDocPtr doc)
{
    //debug();

    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context ==  NULL)
    {
        return NULL;
    }
    return context;
}

/******************************************************************************
 * Description: alter the node value in xml files.
 *
 * Input: context: file context;
 *          xpath: node path;
 *        content: the value of node;
 *
 * Output:  return -1,failed;
 *          return 0, success
 ******************************************************************************/
int alter_cfg_node_value(xmlXPathContextPtr context, 
                            const xmlChar *xpath, 
                            const xmlChar *content)
{
    //debug();

    xmlXPathObjectPtr result;
    result = xmlXPathEvalExpression(xpath, context);
    if (result == NULL)
    {
        return -1;
    }
    if (xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
        xmlXPathFreeObject(result);
        return -1;
    }
    xmlNodeSetContent(result->nodesetval->nodeTab[0], content);
    
    return 0;
}


/*******************************************************************************
 * alter the node value in xml files.
 *
 * Input: context: file context;
 *        xpath: node path;
 *        content: the value of node;
 * Output:  return -1,failed;
 *          return 0, success
 ******************************************************************************/
int alter_cfg_node_value_next(xmlXPathContextPtr context, const xmlChar *xpath, const xmlChar *content, int index)
{
	xmlXPathObjectPtr result;
	result = xmlXPathEvalExpression(xpath, context);
	if (result == NULL)
	{
		return -1;
	}
	if (xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
   	xmlXPathFreeObject(result);
  	 	printf("nodeset is empty\n");
   	return -1;
	}
	xmlNodeSetContent(result->nodesetval->nodeTab[index], content);
	return 0;
}

/******************************************************************************
 * Description: alter the node value in xml files.
 *
 * Input: context: file context;
 *          xpath: node path;
 *        content: the value of node;
 *
 * Output:  return -1,failed;
 *          return 0, success
 ******************************************************************************/
xmlChar* get_cfg_node_value(xmlXPathContextPtr context, const xmlChar *xpath)
{
    //debug();

    xmlXPathObjectPtr result;
    result = xmlXPathEvalExpression(xpath, context);
    if (result == NULL)
    {
        return NULL;
    }
    if (xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
        xmlXPathFreeObject(result);
        return NULL;
    }
    
    return xmlNodeGetContent(result->nodesetval->nodeTab[0]);;
}

/******************************************************************************
 * Description: save the  xml files.
 *
 * Input: szdocname: file name;
 *              doc: the pointer to doc name;
 *          context:the doc context;
 * 
 * Output:  return -1,failed;
 *          return 0, success
 ******************************************************************************/
int alter_cfg_node_attr(xmlXPathContextPtr context, 
                            const xmlChar *xpath, 
                            const xmlChar *attr)
{
    //debug();
    xmlXPathObjectPtr result;
    result = xmlXPathEvalExpression(xpath,context);
    if (result == NULL)
    {
        return -1;
    }
    if (xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
        xmlXPathFreeObject(result);
        return -1;
    }
    if (result->nodesetval->nodeTab[0]->children != NULL )
	{
		result->nodesetval->nodeTab[0]->children->name = attr;
	}
	else
	{
		result->nodesetval->nodeTab[0]->children->next->name = attr;
	}
    //result->nodesetval->nodeTab[0]->name = attr;

    return 0;
}

int alter_cfg_node_attr_next(xmlXPathContextPtr context, const xmlChar *xpath, const xmlChar *attr)
{
	xmlXPathObjectPtr result;
	result = xmlXPathEvalExpression(xpath,context);
	if (result == NULL)
	{
		return -1;
	}
	if (xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
  		xmlXPathFreeObject(result);
		return -1;
	}
    if (result->nodesetval->nodeTab[0]->children != NULL )
    {
    	result->nodesetval->nodeTab[0]->children->next->name = attr;
    }
	
	return 0;
}



/******************************************************************************
 * Description: save the  xml files.
 *
 * Input: szdocname: file name;
 *          doc: the pointer to doc name;
 *          context:the doc context;
 *
 * Output: return -1,failed;
 *          return 0, success
 *          
 ******************************************************************************/
int save_cfg(char *szDocName,xmlDocPtr doc, xmlXPathContextPtr context)
{
    //debug();
    xmlXPathFreeContext(context);
    xmlSaveFile(szDocName, doc);
    return 0;
}
