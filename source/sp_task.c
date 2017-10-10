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

#include <linux/tipc.h>

typedef struct
{
    int node;
    int type;
    int instance;
    char name[64];
    void (*cb)(int msg_type, void *data, int length, void *param);

    int rsock;
    int wsock;
    void *reactor;
    void *reactor_ev;

    char rbuffer[66 * 1024];
    
    bool first_cb;
} sp_task_handle_t;

static void *s_tls_handle = NULL;//sp_tls_new();

static void task_cb(int sock, void *param);
static void taskaddr2tipcaddr(struct sockaddr_tipc *tipc_addr, sp_task_addr_t *task_addr);

void *sp_task_new(const char *name, int type, int instance, sp_task_callback cb)
{
    if (!s_tls_handle)
    {
        s_tls_handle = sp_tls_new();
    }

    int rsock = sp_socket(AF_TIPC, SOCK_RDM, 0);
    sp_return_val_if_fail(rsock > 0, NULL);

    int wsock = sp_socket(AF_TIPC, SOCK_RDM, 0);
    sp_return_val_if_fail(wsock > 0, NULL);

    struct sockaddr_tipc tipc_addr;
    sp_task_addr_t task_addr;

    task_addr.type = type;
    task_addr.instance = instance;
    taskaddr2tipcaddr(&tipc_addr, &task_addr);

    int ret = sp_socket_bind(rsock, (struct sockaddr *)&tipc_addr, sizeof(tipc_addr));
    sp_return_val_if_fail(ret == 0, NULL);

    sp_task_handle_t *h = sp_calloc(1, sizeof(sp_task_handle_t));
    h->rsock = rsock;
    h->wsock = wsock;
    h->cb = cb;
    h->instance = instance;
    sp_string_copy(h->name, name);
    h->type = type;
    h->reactor = sp_reactor_new("poll");
    h->reactor_ev = sp_reactor_attach(h->reactor, rsock, task_cb, h);
    h->first_cb = true;

    sp_reactor_run(h->reactor, SP_REACTOR_RUN_THREAD);

    return h;
}

void sp_task_free(void *handle)
{
    sp_return_if_fail(handle);

    sp_task_handle_t *h = (sp_task_handle_t *)handle;
    sp_reactor_detach(h->reactor_ev);
    sp_reactor_free(h->reactor);

    sp_socket_close(h->wsock);
    sp_socket_close(h->rsock);

    sp_free(h);
}

static void task_cb(int sock, void *arg)
{
    sp_task_handle_t *h = (sp_task_handle_t *)arg;

    int msg;
    int length = recvfrom(sock, &msg, sizeof(int), 0, NULL, 0);
    sp_return_if_fail(length == sizeof(int));
    
    length = sp_socket_readable(sock);
    length = recvfrom(sock, h->rbuffer, length, 0, NULL, 0);

    if (h->first_cb)
    {
        sp_tls_set(s_tls_handle, arg);
        h->first_cb = false;
    }

    h->cb(msg, h->rbuffer, length, arg);
}

int sp_task_sendto(int msg, void *data, int length, sp_task_addr_t *dst)
{
    sp_task_handle_t *h = (sp_task_handle_t *)sp_task_self();

    struct sockaddr_tipc tipc_addr;
    taskaddr2tipcaddr(&tipc_addr, dst);

    int ret = sendto(h->wsock, &msg, sizeof(int), 0, (struct sockaddr *)&tipc_addr, sizeof(struct sockaddr_tipc));
    sp_return_val_if_fail(ret == sizeof(int), -1);

    ret = sendto(h->wsock, data, length, 0,
        (struct sockaddr *)&tipc_addr, sizeof(struct sockaddr_tipc));
    sp_return_val_if_fail(ret == length, -1);

    return 0;
}

int sp_task_sendto_anywhere(int msg, void *data, int length, sp_task_addr_t *dst)
{
    int wsock = sp_socket(AF_TIPC, SOCK_RDM, 0);
    sp_return_val_if_fail(wsock > 0, -1);

    struct sockaddr_tipc tipc_addr;
    taskaddr2tipcaddr(&tipc_addr, dst);

    int ret = sendto(wsock, &msg, sizeof(int), 0, 
        (struct sockaddr *)&tipc_addr, sizeof(struct sockaddr_tipc));
    sp_return_val_if_fail(ret == sizeof(int), -1);

    ret = sendto(wsock, data, length, 0,
        (struct sockaddr *)&tipc_addr, sizeof(struct sockaddr_tipc));
    sp_return_val_if_fail(ret == length, -1);

    sp_socket_close(wsock);

    return 0;
}

int sp_task_instance()
{
    sp_task_handle_t *h = (sp_task_handle_t *)sp_task_self();
    return h->instance;
}

int sp_task_type()
{
    sp_task_handle_t *h = (sp_task_handle_t *)sp_task_self();
    return h->type;
}

const char* sp_task_name()
{
    sp_task_handle_t *h = (sp_task_handle_t *)sp_task_self();
    return h->name;
}

void *sp_task_self()
{
    return sp_tls_get(s_tls_handle);
}

int sp_task_addr(int type, int instance, sp_task_addr_t *addr)
{
    return 0; 
}

static void taskaddr2tipcaddr(struct sockaddr_tipc *tipc_addr, sp_task_addr_t *task_addr)
{
    sp_bzero(tipc_addr, sizeof(struct sockaddr_tipc));

    tipc_addr->family = AF_TIPC;
    tipc_addr->addrtype = TIPC_ADDR_NAME;
    tipc_addr->addr.name.name.type = task_addr->type;
    tipc_addr->addr.name.name.instance = task_addr->instance;
    tipc_addr->scope = TIPC_CLUSTER_SCOPE;
}
