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

#include <event.h>

typedef struct
{
   struct event_base *base;
   void *thread;
} sp_reactor_handle_t;

typedef struct
{
    void *arg;
    sp_reactor_callback cb;
} sp_arg_t;

typedef struct
{
    void *ev;
    void *arg;
} sp_attach_t;

static void reactor_event_callback(int sock, short, void *arg);
static void *reactor_thread_callback(void *arg);
static void *reactor_attach(
    void *handle, int sock, sp_reactor_callback cb, short ev, void *arg);

void *sp_reactor_new(const char *method)
{
    sp_reactor_handle_t *h = sp_calloc(1, sizeof(sp_reactor_handle_t));

    if (!method || sp_string_equal(method, "epoll"))
    {
        h->base = event_base_new();
    }
    else
    {
        struct event_config *config = event_config_new();
        event_config_avoid_method(config, "epoll");
        if (sp_string_equal(method, "poll"))
        {
            event_config_avoid_method(config, "select");
        }
        else
        {
            event_config_avoid_method(config, "poll");
        }

        h->base = event_base_new_with_config(config);
        event_config_free(config);
    }
    return h;
}

void sp_reactor_free(void *handle)
{
    sp_reactor_handle_t *h = (sp_reactor_handle_t *)handle;

    event_base_loopbreak(h->base);

#if 1
    if (h->thread)
    {
        sp_thread_free(h->thread);
    }
#endif

    event_base_free(h->base);

    sp_free(h);
}

void *sp_reactor_attach(void *handle, int sock, sp_reactor_callback cb, void *arg)
{
    return reactor_attach(handle, sock, cb, EV_READ | EV_PERSIST, arg);
}

void *sp_reactor_attach_once(void *handle, int sock, sp_reactor_callback cb, void *arg)
{
    return reactor_attach(handle, sock, cb, EV_READ, arg);
}

void sp_reactor_detach(void *ev)
{
    sp_attach_t *attach_h = (sp_attach_t *)ev;

    event_free(attach_h->ev);
    sp_free(attach_h->arg);
    sp_free(attach_h);
}

void sp_reactor_run(void *handle, int flag)
{
    sp_reactor_handle_t *h = (sp_reactor_handle_t *)handle;

    if (SP_REACTOR_RUN_FOREVER == flag)
    {
        event_base_dispatch(h->base);
    }
    else
    {
        h->thread = sp_thread_new(reactor_thread_callback, h);
    }
}

void sp_reactor_stop(void *handle)
{
    sp_reactor_handle_t *h = (sp_reactor_handle_t *)handle;

    event_base_loopbreak(h->base);
}

static void reactor_event_callback(int sock, short flag, void *arg)
{
    sp_arg_t *sp_arg = (sp_arg_t *)arg;
    sp_arg->cb(sock, sp_arg->arg);
}

static void *reactor_thread_callback(void *arg)
{
    sp_reactor_handle_t *h = (sp_reactor_handle_t *)arg;

    event_base_dispatch(h->base);

    return NULL;
}

static void *reactor_attach(
    void *handle, int sock, sp_reactor_callback cb, short flag, void *arg)
{
    sp_reactor_handle_t *h = (sp_reactor_handle_t *)handle;
    
    sp_arg_t *inner_arg = sp_calloc(1, sizeof(sp_arg_t));
    inner_arg->cb = cb;
    inner_arg->arg = arg;
    
    struct event *ev = event_new(h->base, sock,
        flag,
        reactor_event_callback, inner_arg);
    
    event_base_set(h->base, ev);
    event_add(ev, NULL);

    sp_attach_t *attach_h = sp_calloc(1, sizeof(sp_attach_t));
    attach_h->ev = ev;
    attach_h->arg = inner_arg;

    return attach_h;
}
