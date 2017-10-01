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

#ifndef _SP_XML_H_
#define _SP_XML_H_

typedef struct
{
    void **nodes;
    int n_nodes;
} sp_xpath_result_t;

void *sp_xml_parse(const char *buffer);
void *sp_xml_parse_file(const char *filename);
void sp_xml_free(void *h);
int sp_xml_xpath(void *h, const char *xpath, sp_xpath_result_t *result);
void *sp_xml_get_child(void *node, const char *name);
char *sp_xml_get_text(void *node);
char *sp_xml_get_prop(void *node, const char *name);

#endif
