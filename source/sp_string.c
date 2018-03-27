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

typedef struct
{
    char *buffer;

    int size;
    int capacity;
} sp_string_buffer_t;

inline size_t sp_string_length(const char *str)
{
    return strlen(str); 
}

inline void sp_string_copy(char *dst, const char *src)
{
    strcpy(dst, src);
}

void sp_string_ncopy(char *dst, const char *src, int n)
{
    strncpy(dst, src, n);
    dst[n] = 0;
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

char *sp_string_between(const char *str, const char *start, const char *end, char *sub)
{
    sp_return_val_if_fail(str && start && end && sub, NULL);

    int pos_start = sp_string_find(str, start) + sp_string_length(start);
    sp_return_val_if_fail(pos_start >= 0, NULL);
    
    int length = sp_string_find(str + pos_start, end);
    sp_return_val_if_fail(length >= 0, NULL);

    sp_string_ncopy(sub, str + pos_start, length);
    sub[length] = 0;

    return sub;
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

void sp_string_trim(const char *buffer, char *trim_buffer)
{
    sp_return_if_fail(buffer && trim_buffer);

    sp_string_trim_left(buffer, trim_buffer);
    sp_string_trim_right_inplace(trim_buffer);
}

void sp_string_trim_left(const char *buffer, char *trim_buffer)
{
    sp_return_if_fail(buffer && trim_buffer);

    const char *p = buffer;

    while(*p == ' '
        || *p == '\r'
        || *p == '\n'
        || *p == '\t')
    {
        p++;
    }

    sp_string_copy(trim_buffer, p);
}

void sp_string_trim_right(const char *buffer, char *trim_buffer)
{
    sp_return_if_fail(buffer && trim_buffer);

    const char *p = buffer + sp_string_length(buffer);
    p--;

    while(*p == ' '
        || *p == '\r'
        || *p == '\n'
        || *p == '\t')
    {
        p--;
    }

    int n = p - buffer + 1;
    sp_string_ncopy(trim_buffer, buffer, n);
    trim_buffer[n] = 0;
}

void sp_string_trim_inplace(char *buffer)
{
    sp_return_if_fail(buffer);

    sp_string_trim_left_inplace(buffer);
    sp_string_trim_right_inplace(buffer);
}

void sp_string_trim_left_inplace(char *buffer)
{
    sp_return_if_fail(buffer);

    const char *p = buffer;
    int n = sp_string_length(buffer);

    while(*p == ' '
        || *p == '\r'
        || *p == '\n'
        || *p == '\t')
    {
        p++;
        n--;
    }

    memmove(buffer, p, n);
}

void sp_string_trim_right_inplace(char *buffer)
{
    sp_return_if_fail(buffer);

    char *p = buffer + sp_string_length(buffer);
    p--;

    while(*p == ' '
        || *p == '\r'
        || *p == '\n'
        || *p == '\t')
    {
        *p = 0;
        p--;
    }
}

void *sp_string_buffer_new(int size)
{
    sp_string_buffer_t *buffer = sp_calloc(1, sizeof(sp_string_buffer_t));

    if (size <= 0)
    {
        size = 4096;
    }

    buffer->buffer = sp_malloc(size);
    buffer->size = 0;
    buffer->capacity = size;
    sp_string_clear(buffer->buffer);

    return buffer;
}

void sp_string_buffer_append(void *buffer, const char *string, int size)
{
    sp_return_if_fail(buffer && string);

    sp_string_buffer_t *buf = (sp_string_buffer_t *)buffer;

    if (size <= 0)
    {
        size = sp_string_length(string);
    }

    int expect_size = size + buf->size;
    if (expect_size >= (buf->capacity >> 1))
    {
        int expect_capacity = (expect_size << 1);
        buf->buffer = sp_realloc(buf->buffer, expect_capacity);
        buf->capacity = expect_capacity;
        buf->buffer[size] = 0;
    }

    buf->size += size;
    sp_string_append(buf->buffer, "%s", string);
}

void sp_string_buffer_free(void *buffer)
{
    sp_return_if_fail(buffer);

    sp_string_buffer_t *buf = (sp_string_buffer_t *)buffer;

    if (buf->buffer)
    {
        sp_free(buf->buffer);
    }

    sp_free(buf);
}

int sp_string_buffer_capacity(void *buffer)
{
    sp_return_val_if_fail(buffer, 0);

    sp_string_buffer_t *buf = (sp_string_buffer_t *)buffer;

    return buf->capacity;
}

int sp_string_buffer_size(void *buffer)
{
    sp_return_val_if_fail(buffer, 0);

    sp_string_buffer_t *buf = (sp_string_buffer_t *)buffer;

    return buf->size;
}

char *sp_string_buffer_string(void *buffer)
{
    sp_return_val_if_fail(buffer, NULL);

    sp_string_buffer_t *buf = (sp_string_buffer_t *)buffer;

    return buf->buffer;
}
