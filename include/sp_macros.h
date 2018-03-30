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

#ifndef _SP_MACROS_H_
#define _SP_MACROS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#if __GUNC__ >= 3
#undef inline
#define inline __attribute__ ((always_inline))
#define __noinline__ __attribute__ ((noinline))
#define __pure__ __attribute__ ((pure))
#define __const__ __attribute__ ((const))
#define __noreturn__ __attribute__ ((noreturn))
#define __malloc__ __attribute__((malloc))
#define __mustcheck__ __attribute__ ((warn_unused_result))
#define __deprecated__ __attribute__ ((deprecated))
#define __used__ __attribute__ ((used))
#define __unused__ __attribute__ ((unused))
#define __packed__ __attribute__((packed))
#define __align__(x) __attribute__((aligned(x)))
#define __align_max__ __attribute__((aligned))
#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)
#define __check_printf__(m, n) __attribute__ ((format(printf, m, n)))
#else
#define __noinline__
#define __pure__
#define __const__
#define __noreturn__
#define __malloc__
#define __mustcheck__
#define __deprecated__
#define __used__
#define __unused__
#define __packed__
#define __align(x)__
#define __align_max__
#define likely(x) (x)
#define unlikely(x) (x)
#define __check_printf__(m, n)
#endif

#define sp_return_if_fail(expr) \
    if likely(expr) {} else { \
        return; \
    }

#define sp_return_val_if_fail(expr, val) \
    if likely(expr) {} else { \
        return (val); \
    }

#define sp_break_if_fail(expr) \
    if likely(expr) {} else { \
        break; \
    }

#ifdef __cplusplus
}
#endif

#endif
