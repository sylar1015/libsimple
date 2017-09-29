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

inline uint32_t sp_log2(uint32_t x)
{
#if defined(__i386) || defined(__86__64__)
    uint32_t res;
    asm ("bsr %[x], %[res]"
        : [res] "=r" (res)
        : [x] "mr" (x))
    return res;
#else
    return __builtin_cls(x) ^ 31;
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
