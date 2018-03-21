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

typedef struct
{
    sp_list_node_t *front;
    sp_list_node_t *back;
    int size;
    sp_free_func_t free_func;
} sp_list_t;

void *sp_list_new(sp_free_func_t free_func)
{
    sp_list_t *list = sp_calloc(1, sizeof(sp_list_t));

    sp_return_val_if_fail(list, NULL);

    list->free_func = free_func;

    return list;
}

void sp_list_free(void *list)
{
    sp_list_clear(list);
    sp_free(list);
}

int sp_list_size(void *list)
{
    sp_return_val_if_fail(list, 0);

    sp_list_t *l = (sp_list_t *)list;

    return l->size;
}

void sp_list_clear(void *list)
{
    sp_return_if_fail(list);
    sp_list_node_t *node = sp_list_node_front(list);
    sp_list_t *l = (sp_list_t *)list;
    sp_free_func_t func = l->free_func;

    while (node != NULL)
    {
        sp_list_node_t *cur = node;
        node = sp_list_node_next(node);

        if (func)
        {
            func(cur->val);
        }

        if (cur->next)
        {
            sp_list_node_t *next = (sp_list_node_t *)cur->next;
            next->prev = cur->prev;
        }

        if (cur->prev)
        {
            sp_list_node_t *prev = (sp_list_node_t *)cur->prev;
            prev->next = cur->next;
        }

        sp_free(cur);
    }

    l->back = NULL;
    l->front = NULL;
    l->size = 0;
}

void sp_list_push_back(void *list, void *val)
{
    sp_return_if_fail(list);

    sp_list_t *l = (sp_list_t *)list;

    sp_list_node_t *node = sp_calloc(1, sizeof(sp_list_node_t));
    node->val = val;

    if (l->back)
    {
        node->prev = l->back;
        l->back->next = node;
        l->back = node;
    }
    else
    {
        l->front = node;
        l->back = node;
    }

    l->size ++;
}

void sp_list_push_front(void *list, void *val)
{
    sp_return_if_fail(list);

    sp_list_t *l = (sp_list_t *)list;

    sp_list_node_t *node = sp_calloc(1, sizeof(sp_list_node_t));
    node->val = val;

    if (l->front)
    {
        node->next = l->front;
        l->front->prev = node;
        l->front = node;
    }
    else
    {
        l->front = node;
        l->back = node;
    }

    l->size ++;
}

void *sp_list_back(void *list)
{
    sp_return_val_if_fail(list, NULL);

    sp_list_node_t *node = sp_list_node_back(list);

    sp_return_val_if_fail(node, NULL);

    return node->val;
}

void *sp_list_front(void *list)
{
    sp_return_val_if_fail(list, NULL);

    sp_list_node_t *node = sp_list_node_front(list);

    sp_return_val_if_fail(node, NULL);

    return node->val;
}

void sp_list_pop_back(void *list)
{
    sp_list_node_t *node = sp_list_node_back(list);

    sp_list_pop(list, node);
}

void sp_list_pop_front(void *list)
{
    sp_list_node_t *node = sp_list_node_front(list);

    sp_list_pop(list, node);
}

void sp_list_pop(void *list, sp_list_node_t *node)
{
    sp_return_if_fail(list && node);

    sp_list_t *l = (sp_list_t *)list;

    if (l->free_func)
    {
        l->free_func(node->val);
    }
    
    if (node->next)
    {
        sp_list_node_t *next = (sp_list_node_t *)node->next;
        next->prev = node->prev;
    }
        
    if (node->prev)
    {
        sp_list_node_t *prev = (sp_list_node_t *)node->prev;
        prev->next = node->next;
    }

    if (node == l->front)
    {
        l->front = l->front->next;
    }

    if (node == l->back)
    {
        l->back = l->back->prev;
    }

    l->size --;
    sp_free(node);
}

sp_list_node_t *sp_list_node_next(sp_list_node_t *node)
{
    sp_return_val_if_fail(node, NULL);

    return node->next;
}

sp_list_node_t *sp_list_node_front(void *list)
{
    sp_return_val_if_fail(list, NULL);

    sp_list_t *l = (sp_list_t *)list;

    return l->front;
}

sp_list_node_t *sp_list_node_back(void *list)
{
    sp_return_val_if_fail(list, NULL);

    sp_list_t *l = (sp_list_t *)list;

    return l->back;
}
