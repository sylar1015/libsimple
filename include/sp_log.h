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

#ifndef _SP_LOG_H_
#define _SP_LOG_H_

#ifdef __cplusplus
extern "C"
{
#endif

void *sp_log_new(const char *conf, const char *rule);
void sp_log_free(void *h);
void sp_log(void *h, const char *file, size_t filelen,
    const char *func, size_t funclen,
    long line, int level,
    const char *format, ...);

enum {
    SP_LOG_DEBUG = 20,
    SP_LOG_INFO = 40,
    SP_LOG_NOTICE = 60,
    SP_LOG_WARN = 80,
    SP_LOG_ERROR = 100,
    SP_LOG_FATAL = 120
} ;

#define sp_log_fatal(h, ...) \
    sp_log(h, __FILE__, sizeof(__FILE__) - 1, \
        __func__, sizeof(__func__) - 1, __LINE__, \
        SP_LOG_FATAL, __VA_ARGS__)

#define sp_log_error(h, ...) \
    sp_log(h, __FILE__, sizeof(__FILE__) - 1, \
        __func__, sizeof(__func__) - 1, __LINE__, \
        SP_LOG_ERROR, __VA_ARGS__)

#define sp_log_warn(h, ...) \
    sp_log(h, __FILE__, sizeof(__FILE__) - 1, \
        __func__, sizeof(__func__) - 1, __LINE__, \
        SP_LOG_WARN, __VA_ARGS__)

#define sp_log_notice(h, ...) \
    sp_log(h, __FILE__, sizeof(__FILE__) - 1, \
        __func__, sizeof(__func__) - 1, __LINE__, \
        SP_LOG_NOTICE, __VA_ARGS__)

#define sp_log_info(h, ...) \
    sp_log(h, __FILE__, sizeof(__FILE__) - 1, \
        __func__, sizeof(__func__) - 1, __LINE__, \
        SP_LOG_INFO, __VA_ARGS__)

#define sp_log_debug(h, ...) \
    sp_log(h, __FILE__, sizeof(__FILE__) - 1, \
        __func__, sizeof(__func__) - 1, __LINE__, \
        SP_LOG_DEBUG, __VA_ARGS__)


#ifdef __cplusplus
}
#endif

#endif
