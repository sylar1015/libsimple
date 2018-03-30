/* Copyright (C) 2017-2018 Liang Sun <sylar1015@163.com>
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

void *sp_queue_new(sp_free_func_t free_func)
{
    return sp_list_new(free_func);
}

void sp_queue_push(void *queue, void *val)
{
    sp_list_push_back(queue, val);
}

void sp_queue_pop(void *queue, void **val)
{
    sp_return_if_fail(sp_list_size(queue) > 0);
    sp_list_node_t *node = sp_list_node_front(queue);

    if (val)
    {
        *val = sp_list_front(queue);
        node->val = NULL;
    }

    sp_list_pop_front(queue);
}

int sp_queue_size(void *queue)
{
    return sp_list_size(queue);
}

void sp_queue_clear(void *queue)
{
    sp_list_clear(queue);
}

void sp_queue_free(void *queue)
{
    sp_list_free(queue);
}
