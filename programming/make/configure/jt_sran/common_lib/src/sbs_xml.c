/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:06:31 CST. */
/* 
 Copyright (C) Rong Tao @Beijing

 Permission is granted to copy, distribute and/or modify this document
 under the terms of the GNU Free Documentation License, Version 1.3
 or any later version published by the Free Software Foundation;
 with no Invariant Sections, no Front-Cover Texts, and no Back-Cover
 Texts. A copy of the license is included in the section entitled ‘‘GNU
 Free Documentation License’’.
   2019年 03月 14日 星期四 19:24:41 CST. 
*/
/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 08日 星期五 08:10:25 CST. */
/* Copyright (c) Colorado School of Mines, CST.*/
/* All rights reserved.                       */

/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 03月 07日 星期四 20:28:00 CST. */
/************************************************************************
 *Copyright (c) 2018 Sylincom.
 *All rights reserved.
 *
 *File name: sbs_xml.c
 *Description: xml parser utils.
 *
 *Current Version: 0.l
 *Author: wangcheng2017@ict.ac.cn
 *Date: 2018.09.11
 *
 ************************************************************************/

/* Includes-------------------------------------------------------------*/
#include "sbs_xml.h"
#include <pthread.h>
#include <string.h>

/* Variables------------------------------------------------------------*/
char ret[RETSET][RETSET];

static pthread_mutex_t         xml_mutex;
static pthread_mutexattr_t     xml_mutex_attr;


/* Macros---------------------------------------------------------------*/
#define XML_ERROR   -1
#define XML_OK      0

/* Declarations---------------------------------------------------------*/
/* Constants------------------------------------------------------------*/
/* Functions------------------------------------------------------------*/

INT32 init_xml_mutex()
{
    int res;
    res = pthread_mutexattr_init(&xml_mutex_attr);
	assert(!res);
	res = pthread_mutexattr_settype(&xml_mutex_attr, PTHREAD_MUTEX_RECURSIVE);
	assert(!res);
	res = pthread_mutex_init(&xml_mutex, &xml_mutex_attr);
	assert(!res);

    return XML_OK;
}

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
xmlDocPtr utils_xml_open(const char* xml_name)
{
    init_xml_mutex();
    pthread_mutex_lock(&xml_mutex);
    xmlDocPtr pdoc = NULL;	
	
	/*****Open Xml Doc*****/
	xmlKeepBlanksDefault(0);
	pdoc = xmlReadFile(xml_name, "UTF-8", XML_PARSE_NOBLANKS);
	if (pdoc == NULL)
	{
		printf("Error: can't open file!\n");
		return NULL;
	}	


    return pdoc;
}

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
xmlXPathContextPtr utils_xml_xpath_context(xmlDocPtr doc)
{
	xmlXPathContextPtr context = xmlXPathNewContext(doc);
	if (context ==	NULL)
	{
	  	printf("context is null\n");
	  	return NULL;
	}
    
	return context;
}

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
xmlXPathObjectPtr utils_xml_get_node_set(xmlDocPtr pdoc, const xmlChar *xpath)
{
	xmlXPathContextPtr context = NULL;
	xmlXPathObjectPtr result = NULL;
	context = xmlXPathNewContext(pdoc);
	
	if (pdoc == NULL)
	{
		printf("pdoc is NULL!\n");
		return NULL;
	}
	
	if (xpath)
	{
		if (context == NULL)
		{
			printf("context is NULL\n");
			return NULL;
		}
		
		result = xmlXPathEvalExpression(xpath, context);
		xmlXPathFreeContext(context);
		if (result == NULL)
		{
			printf("xmlXPathEvalExpression return NULL\n");
			return NULL;
		}
		
		if (xmlXPathNodeSetIsEmpty(result->nodesetval))
		{
			xmlXPathFreeObject(result);
			printf("nodeset is empty\n");
			return NULL;
		}
	}
	
	return result;
}


/*
INT32 utils_xml_get_node_value(xmlDocPtr pdoc, const xmlcChar* node_name, char* buf)
{
    xmlNodePtr proot = NULL;
    //Get Xml Doc Root Object
	proot = xmlDocGetRootElement(pdoc);
	if (proot == NULL)
	{
		printf("Error: file is empty!\n");
		return XML_ERROR;
	}
	
	xmlNodePtr pcur;
    pcur = proot->xmlChildrenNode;
	while (pcur != NULL)
	{
		if (!xmlStrcmp(pcur->name, BAD_CAST(node_name)))
		{
            int len = strlen((char*)XML_GET_CONTENT(pcur->xmlChildrenNode));
            if (len >= RETSET)
            {
                printf("node value is too large.\n");
                return XML_ERROR;
            }
                
            memcpy((void*)buf, (void*)(char*)XML_GET_CONTENT(pcur->xmlChildrenNode), len);
            buf[len] = '\0';
			return XML_OK;
		}
		pcur = pcur->next;
	}

    return XML_ERROR;
}
*/

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
INT32 utils_xml_get_node_value_by_xpath(xmlDocPtr pdoc, const xmlChar* path, char* buf)
{
    xmlChar *xpath = BAD_CAST(path);
	xmlXPathObjectPtr xpop = utils_xml_get_node_set(pdoc, xpath);
    if (xpop)
	{
		xmlNodeSetPtr nodeset = xpop->nodesetval;
		xmlNodePtr cur;
		
		int i = 0;
		for (; i < nodeset->nodeNr; i++)
		{
			cur = nodeset->nodeTab[i];
            
			xmlChar *tmp = NULL;
            tmp = xmlNodeGetContent(cur);
            int len = strlen(tmp);
            if (tmp)
            {
                memcpy((void*)buf, (void*)tmp, len);
                buf[len] = '\0';
    			return XML_OK;
            }
		}
	}

    return XML_ERROR;
}

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
INT32 utils_xml_get_node_value_by_node_set(xmlXPathObjectPtr xpop, const xmlChar* node_name, char* buf)
{
    if (xpop)
	{
		xmlNodeSetPtr nodeset = xpop->nodesetval;
		xmlNodePtr cur;
		
		int i = 0;
		for (; i < nodeset->nodeNr; i++)
		{
			cur = nodeset->nodeTab[i];
			cur = cur->xmlChildrenNode;
			
			while (cur != NULL)
			{
				if (!xmlStrcmp(cur->name, BAD_CAST(node_name)))
				{
					int len = strlen((char*)XML_GET_CONTENT(cur->xmlChildrenNode));
                    if (len >= RETSET)
                    {
                        printf("node value is too large.\n");
                        return XML_ERROR;
                    }
                        
                    memcpy((void*)buf, (void*)(char*)XML_GET_CONTENT(cur->xmlChildrenNode), len);
                    buf[len] = '\0';
        			return XML_OK;
				}
				cur = cur->next;
			}
		}
	}

    return XML_ERROR;
}


/*
INT32 utils_xml_set_node_value(xmlDocPtr pdoc, const xmlChar* node_name, const xmlChar* new_value)
{
    xmlNodePtr proot = NULL;
    //Get Xml Doc Root Object
	proot = xmlDocGetRootElement(pdoc);
	if (proot == NULL)
	{
		printf("Error: file is empty!\n");
		return XML_ERROR;
	}
	
	xmlNodePtr pcur;
    pcur = proot->xmlChildrenNode;
	while (pcur != NULL)
	{
		if (!xmlStrcmp(pcur->name, BAD_CAST(node_name)))
		{
            xmlNodeSetContent(pcur, new_value);
            return XML_OK;
		}
		pcur = pcur->next;
	}

    return XML_ERROR;
}
*/

/**********************************************
** set xml node value by xpath.
**
** Input:
**      xmlDocPtr, xmlChar*, xmlChar*.
** Output:
**      
** Return:
**      success 0
**      error   -1
**
***********************************************/
INT32 utils_xml_set_node_value_by_xpath(xmlDocPtr pdoc, const xmlChar* path, const xmlChar* new_value)
{
    xmlChar *xpath = BAD_CAST(path);
	xmlXPathObjectPtr xpop = utils_xml_get_node_set(pdoc, xpath);
    if (xpop)
	{
		xmlNodeSetPtr nodeset = xpop->nodesetval;
		xmlNodePtr cur;
		
		int i = 0;
		for (; i < nodeset->nodeNr; i++)
		{
			cur = nodeset->nodeTab[i];
            xmlNodeSetContent(cur, new_value);
            return XML_OK;			
		}
	}
    utils_xml_xpath_clean(xpop);

    return XML_ERROR;
}

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
INT32 utils_xml_set_node_value_by_node_set(xmlXPathObjectPtr xpop, const xmlChar* node_name, const xmlChar* new_value)
{
    if (xpop)
	{
		xmlNodeSetPtr nodeset = xpop->nodesetval;
		xmlNodePtr cur;
		
		int i = 0;
		for (; i < nodeset->nodeNr; i++)
		{
			cur = nodeset->nodeTab[i];
			cur = cur->xmlChildrenNode;
			
			while (cur != NULL)
			{
				if (!xmlStrcmp(cur->name, BAD_CAST(node_name)))
				{
					xmlNodeSetContent(cur, new_value);
        			return XML_OK;
				}
				cur = cur->next;
			}
		}
	}

    return XML_ERROR;
}

/**********************************************
** save xml file.
**
** Input:
**      xmlDocPtr, xml_new_name.
** Output:
**      
** Return:
**      success 0
**      error   -1
**
***********************************************/
INT32 utils_xml_save(xmlDocPtr pdoc, const char* xml_name)
{
    xmlSaveFormatFile(xml_name, pdoc, 1);
    return XML_OK;
}

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
INT32 utils_xml_xpath_clean(xmlXPathObjectPtr xpop)
{
    xmlXPathFreeObject(xpop);
    return XML_OK;
}

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
INT32 utils_xml_clean(xmlDocPtr pdoc)
{
    xmlFreeDoc(pdoc);
	//xmlCleanupParser();
	xmlMemoryDump();
    pthread_mutex_unlock(&xml_mutex);
    pthread_mutex_destroy(&xml_mutex);
    pthread_mutexattr_destroy(&xml_mutex_attr);

    return XML_OK;
}
