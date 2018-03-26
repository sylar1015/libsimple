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

#include <pthread.h>

typedef struct
{
    pthread_t pid;
} sp_thread_handle_t;

void *sp_thread_new(sp_thread_callback cb, void *arg)
{
    sp_thread_handle_t *h = sp_calloc(1, sizeof(sp_thread_handle_t));

    if (pthread_create(&h->pid, NULL, cb, arg) != 0)
    {
        sp_free(h);
        return NULL;
    }

    return h;
}

void sp_thread_free(void *handle)
{
    sp_thread_handle_t *h = (sp_thread_handle_t *)handle;

    pthread_join(h->pid, NULL);

    sp_free(h);
}

void *sp_lock_new()
{
    pthread_mutex_t *lock = sp_calloc(1, sizeof(pthread_mutex_t));

    pthread_mutex_init(lock, NULL);

    return lock;
}

void sp_lock_acquire(void *lock)
{
    sp_return_if_fail(lock);

    pthread_mutex_lock(lock);
}

void sp_lock_release(void *lock)
{
    sp_return_if_fail(lock);

    pthread_mutex_unlock(lock);
}

void sp_lock_free(void *lock)
{
    sp_return_if_fail(lock);

    pthread_mutex_t *lk = (pthread_mutex_t *)lock;

    pthread_mutex_destroy(lk);

    sp_free(lk);
}

void *sp_tls_new()
{
    pthread_key_t *h = sp_malloc(sizeof(pthread_key_t));

    pthread_key_create(h, NULL);

    return h;
}

void sp_tls_set(void *h, void *ptr)
{
    pthread_key_t *key = (pthread_key_t *)h;
    pthread_setspecific(*key, ptr);
}

void *sp_tls_get(void *h)
{
    pthread_key_t *key = (pthread_key_t *)h;

    return pthread_getspecific(*key);
}

void sp_tls_free(void *h)
{
    sp_return_if_fail(h);

    pthread_key_t *key = (pthread_key_t *)h;

    pthread_key_delete(*key);

    sp_free(key);
}
