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

#include <stdlib.h>

inline void *sp_malloc(size_t size)
{
    size_t aligned_size = sp_roundup2n(size);
    return malloc(aligned_size);
}

inline void sp_free(void *ptr)
{
    free(ptr);
}

void *sp_calloc(size_t n, size_t size)
{
    size_t malloc_size = n * size;
    void *ptr = sp_malloc(malloc_size);

    if (likely(ptr))
    {
        sp_bzero(ptr, malloc_size);
    }

    return ptr;
}

void *sp_realloc(void *ptr, size_t size)
{
    size_t old_size = malloc_usable_size(ptr);
    size_t new_size = sp_roundup2n(size);

    if (new_size > old_size)
    {
        void *new_ptr = sp_malloc(new_size);
        sp_copy(new_ptr, ptr, old_size);
        sp_free(ptr);
        return new_ptr;
    }

    return ptr;
}

void *sp_strdup(const char *str)
{
    size_t len = sp_string_length(str);
    char *ptr = (char *)sp_malloc(len + 1);
    sp_string_copy(ptr, str);
    return ptr;
}
