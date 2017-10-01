/* Copyright (C) 2017-2017 Liang Sun <sylar1015@163.com>
   This file is part of the Simple C Library
 
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
 
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
 
   You should have received a copy of the GNU Lesser General Public
   License along with the Simple Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include "sp.h"

#include <libxml/xpath.h>

typedef struct
{
    xmlDocPtr doc;
    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr result;
} sp_xml_handle_t;

static void *xml_parse_doc(xmlDocPtr doc);

void *sp_xml_parse(const char *buffer)
{
    xmlDocPtr doc = xmlParseMemory(buffer, sp_string_length(buffer));
    return xml_parse_doc(doc);
}

void *sp_xml_parse_file(const char *filename)
{
    xmlDocPtr doc = xmlParseFile(filename);
    return xml_parse_doc(doc);
}

void sp_xml_free(void *handle)
{
    sp_xml_handle_t *h = (sp_xml_handle_t *)handle;

    if (h->result)
        xmlXPathFreeObject(h->result);

    if (h->ctx)
        xmlXPathFreeContext(h->ctx);

    if (h->doc)
        xmlFreeDoc(h->doc);

    sp_free(h);
}

int sp_xml_xpath(void *handle, const char *xpath, sp_xpath_result_t *result)
{
    sp_xml_handle_t *h = (sp_xml_handle_t *)handle;

    if (h->result)
    {
        xmlXPathFreeObject(h->result);
        h->result = NULL;
    }

    h->result = xmlXPathEvalExpression(xpath, h->ctx);
    if (!h->result)
        return -1;

    if (xmlXPathNodeSetIsEmpty(h->result->nodesetval))
    {
        xmlXPathFreeObject(h->result);
        h->result = NULL;
        return -1;
    }

    xmlNodeSetPtr nodeset = h->result->nodesetval;
    result->n_nodes = nodeset->nodeNr;
    result->nodes = nodeset->nodeTab;

    return 0;
}

void *sp_xml_get_child(void *n, const char *name)
{
    xmlNodePtr node = (xmlNodePtr)n;
    node = node->children;

    while (node)
    {
        if (XML_ELEMENT_NODE == node->type
            && sp_string_equal(name, node->name))
            return node;

        node = node->next;
    }

    return NULL;
}

char *sp_xml_get_text(void *node)
{
    return xmlNodeGetContent(node);
}

char *sp_xml_get_prop(void *node, const char *name)
{
    return xmlGetProp(node, name);
}

static void *xml_parse_doc(xmlDocPtr doc)
{
    sp_return_val_if_fail(doc, NULL);
    sp_xml_handle_t *h = sp_calloc(1, sizeof(sp_xml_handle_t));
    h->doc = doc;
    h->ctx = xmlXPathNewContext(doc);
    
    return h;
}
