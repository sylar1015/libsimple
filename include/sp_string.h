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

#ifndef _SP_STRING_H_
#define _SP_STRING_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

size_t sp_string_length(const char *str);
void sp_string_copy(char *dst, const char *src);
void sp_string_ncopy(char *dst, const char *src, int n);
void sp_string_clear(char *str);
bool sp_string_empty(const char *str);
bool sp_string_equal(const char *str1, const char *str2);
bool sp_string_startswith(const char *str, const char*sub);
bool sp_string_endswith(const char *str, const char *sub);
int sp_string_find(const char *str, const char *sub);
char *sp_string_between(const char *str, const char *start, const char *end, char *sub);
void sp_string_append(char *buffer, const char *format, ...);
void sp_string_trim_left(const char *buffer, char *trim_buffer);
void sp_string_trim_right(const char *buffer, char *trim_buffer);
void sp_string_trim(const char *buffer, char *trim_buffer);
void sp_string_trim_inplace(char *buffer);
void sp_string_trim_left_inplace(char *buffer);
void sp_string_trim_right_inplace(char *buffer);


void *sp_string_buffer_new(int size);
void sp_string_buffer_append(void *buffer, const char *string, int size);
void sp_string_buffer_free(void *buffer);
int sp_string_buffer_capacity(void *buffer);
int sp_string_buffer_size(void *buffer);
void sp_string_buffer_resize(void *buffer, int size);
char *sp_string_buffer_string(void *buffer);

#ifdef __cplusplus
}
#endif

#endif
