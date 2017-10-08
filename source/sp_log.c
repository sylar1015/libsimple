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

#include <zlog.h>
#include <stdarg.h>

typedef struct
{
    zlog_category_t *category;  
} sp_log_handle_t;

void *sp_log_new(const char *conf, const char *rule)
{
    int ret = zlog_init(conf);
    sp_return_val_if_fail(ret == 0, NULL);

    zlog_category_t *category = zlog_get_category(rule);
    sp_return_val_if_fail(category, NULL);

    sp_log_handle_t *h = sp_calloc(1, sizeof(sp_log_handle_t));
    h->category = category;

    return h;
}

void sp_log_free(void *handle)
{
    zlog_fini();

    sp_free(handle);
}

void sp_log(void *handle, const char *file, size_t filelen,
    const char *func, size_t funclen,
    long line, int level,
    const char *format, ...)
{
    sp_log_handle_t *h = (sp_log_handle_t *)handle;
    va_list args;
    va_start(args, format);

    vzlog(h->category, file, filelen,
        func, funclen,
        line, level, format, args);

    va_end(args);
}
