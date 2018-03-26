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

#ifndef _SP_THREAD_H_
#define _SP_THREAD_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef void *(*sp_thread_callback) (void *);

void *sp_thread_new(sp_thread_callback cb, void *arg);
void sp_thread_free(void *h);

void *sp_lock_new();
void sp_lock_acquire(void *lock);
void sp_lock_release(void *lock);
void sp_lock_free(void *lock);

void *sp_tls_new();
void sp_tls_set(void *h, void *ptr);
void *sp_tls_get(void *h);
void sp_tls_free(void *h);

#ifdef __cplusplus
}
#endif

#endif
