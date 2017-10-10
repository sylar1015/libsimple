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

#ifndef _SP_TASK_H_
#define _SP_TASL_H_

typedef void (*sp_task_callback)(int msg, void *data, int length, void *arg);

typedef struct
{
    int node;
    int type;
    int instance;
} sp_task_addr_t;

void *sp_task_new(const char *name, int type, int instance, sp_task_callback cb);
void sp_task_free(void *h);
int sp_task_sendto(int msg, void *data, int length, sp_task_addr_t *dst);
int sp_task_sendto_anywhere(int msg, void *data, int length, sp_task_addr_t *dst);
int sp_task_instance();
int sp_task_type();
const char* sp_task_name();
int sp_task_addr(int type, int instance, sp_task_addr_t *addr);
void *sp_task_self();

#endif
