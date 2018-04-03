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

#include <regex.h>
#include <stdarg.h>
#include <sys/time.h>

inline uint32_t sp_log2(uint32_t x)
{
#if defined(__i386) || defined(__x86_64)
    uint32_t res;
    asm ("bsr %[x], %[res]"
        : [res] "=r" (res)
        : [x] "mr" (x));
    return res;
#else
    return __builtin_clz(x) ^ 31;
#endif
}

inline uint32_t sp_roundup2n(uint32_t x)
{
    return 2U << sp_log2(x - 1);
}

inline uint32_t sp_roundup4(uint32_t x)
{
    return (x + 3) &~ (3);
}

inline void sp_bzero(void *ptr, size_t size)
{
    bzero(ptr, size);
}

inline void sp_copy(void *dst, const void *src, size_t size)
{
    memcpy(dst, src, size);
}

int sp_regex(const char *pattern, const char *text, ...)
{
    const int count = 10;
    regmatch_t sz_match[count];
    regex_t reg;
    int ret = regcomp (&reg, pattern, REG_EXTENDED);
    sp_return_val_if_fail(ret == 0, -1);
    
    ret = regexec(&reg, text, count, sz_match, 0);
    if (ret != 0)
    {
        regfree(&reg);
        return -1;
    }
    
    va_list args;
    va_start(args, text);
    int i;
    for (i = 1; i < count && sz_match[i].rm_so != -1; i++)
    {
        char * buffer = va_arg(args, char *);
        int length = sz_match[i].rm_eo - sz_match[i].rm_so;
        sp_copy(buffer, text + sz_match[i].rm_so, length);
        buffer[length] = 0;
    }
    va_end(args);

    regfree(&reg);

    return 0;
}

uint64_t sp_now()
{
    return time(NULL);
}

uint64_t sp_timeofday()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (tv.tv_sec * 1000000 + tv.tv_usec);
}

void sp_usleep(uint32_t usec)
{
    usleep(usec);
}

int sp_tcp_server_socket(const char *ipv4, int port)
{
    int sock = sp_socket(AF_INET, SOCK_STREAM , 0);
    sp_return_val_if_fail(sock > 0, -1);
    sp_socket_reuseable(sock);

    struct sockaddr_in addr;
    sp_bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ipv4);

    if (sp_socket_bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        sp_socket_close(sock);
        return -1;
    }

    if (sp_socket_listen(sock, 5) < 0)
    {
        sp_socket_close(sock);
        return -1;
    }

    return sock;
}
