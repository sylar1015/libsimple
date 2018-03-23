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

#ifndef _SP_MEMORY_H_
#define _SP_MEMOEY_H_

#include "sp_macros.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void *(*sp_malloc_func_t)(size_t size);
typedef void (*sp_free_func_t)(void *ptr);

void *sp_malloc(size_t size) __malloc__;
void sp_free(void *ptr);
void *sp_calloc(size_t n, size_t size) __malloc__;
void *sp_realloc(void *ptr, size_t size) __malloc__;
void *sp_strdup(const char *str);

#ifdef __cplusplus
}
#endif

#endif
