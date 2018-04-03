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

#ifndef _SP_UTIL_H_
#define _SP_UTIL_H_

#include <inttypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

uint32_t sp_log2(uint32_t x);
uint32_t sp_roundup2n(uint32_t x);
uint32_t sp_roundup4(uint32_t x);

void sp_bzero(void *ptr, size_t size);
void sp_copy(void *dst, const void *src, size_t size);

int sp_regex(const char *pattern, const char *text, ...);

uint64_t sp_now();
uint64_t sp_timeofday();
void sp_usleep(uint32_t usec);

int sp_tcp_server_socket(const char *ipv4, int port);

#ifdef __cplusplus
}
#endif

#endif
