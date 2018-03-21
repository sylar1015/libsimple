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

#ifndef _SP_LIST_H_ 
#define _SP_LIST_H_

#include "sp_memory.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    struct sp_list_node_t *prev;
    struct sp_list_node_t *next;

    void *val;
} sp_list_node_t;

void *sp_list_new(sp_free_func_t func);
void sp_list_free(void *list);
int sp_list_size(void *list);
void sp_list_clear(void *list);
void sp_list_push_back(void *list, void *val);
void sp_list_push_front(void *list, void *val);
void *sp_list_back(void *list);
void *sp_list_front(void *list);
void sp_list_pop_back(void *list);
void sp_list_pop_front(void *list);
void sp_list_pop(void *list, sp_list_node_t *node);
sp_list_node_t *sp_list_node_next(sp_list_node_t *node);
sp_list_node_t *sp_list_node_front(void *list);
sp_list_node_t *sp_list_node_back(void *list);

#ifdef __cplusplus
}
#endif

#endif
