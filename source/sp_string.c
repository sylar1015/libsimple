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

#include <stdarg.h>

inline size_t sp_string_length(const char *str)
{
    return strlen(str); 
}

inline void sp_string_copy(char *dst, const char *src)
{
    strcpy(dst, src);
}

inline void sp_string_clear(char *str)
{
    str[0] = 0;
}

inline bool sp_string_empty(const char *str)
{
    return str[0] == 0;
}

bool sp_string_equal(const char *str1, const char *str2)
{
    return 0 == strcmp(str1, str2);
}

int sp_string_find(const char *str, const char *sub)
{
    const char *p = strstr(str, sub);
    return p ? p - str : -1;
}

bool sp_string_startswith(const char *str, const char* sub)
{
    return 0 == sp_string_find(str, sub);
}

bool sp_string_endswith(const char *str, const char *sub)
{
    int pos = sp_string_find(str, sub);
    return sp_string_length(sub) + pos == sp_string_length(str);
}

void sp_string_append(char *buffer, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    vsprintf(buffer + sp_string_length(buffer), format, args);

    va_end(args);
}
