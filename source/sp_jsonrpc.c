/* Copyright (C) 2017-2018 Liang Sun <sylar1015@163.com>
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

typedef struct
{
    void *reactor;
    void *server_ev;
    int server_sock;

    void *rpc_table;
} sp_jsonrpc_server_t;

typedef struct
{
    int sock;
    void *ev;
    sp_jsonrpc_server_t *server;

    void *buffer;
} sp_jsonrpc_session_t;

typedef struct
{
    sp_jsonrpc_func_t func;
    void *arg;
} sp_jsonrpc_register_t;

static void handle_connect(int sock, void *arg);
static void handle_read(int sock, void *arg);
static void session_close(sp_jsonrpc_session_t *session);
static void send_error(sp_jsonrpc_session_t *session, int code, const char *message, sp_json_t *id);
static void send_response(sp_jsonrpc_session_t *session, const char *res);

void *sp_jsonrpc_server_new(const char *ipv4, int port)
{
    sp_jsonrpc_server_t *server = sp_calloc(1, sizeof(sp_jsonrpc_server_t));
    sp_return_val_if_fail(server, NULL);

    server->reactor = sp_reactor_new(NULL);
    sp_return_val_if_fail(server->reactor, NULL);

    int sock = sp_tcp_server_socket(ipv4, port);
    sp_return_val_if_fail(sock > 0, NULL);

    server->server_ev = sp_reactor_attach(server->reactor, sock, handle_connect, server);
    server->server_sock = sock;

    server->rpc_table = sp_hashtable_new(256,
        sp_hashtable_str_hash,
        sp_hashtable_str_equal,
        sp_free, sp_free);

    return server;
}

void sp_jsonrpc_server_free(void *h)
{
    sp_return_if_fail(h);

    sp_jsonrpc_server_t *server = (sp_jsonrpc_server_t *)h;

    sp_reactor_detach(server->server_ev);
    sp_socket_close(server->server_sock);
    sp_reactor_free(server->reactor);
    server->reactor = NULL;

    sp_hashtable_free(server->rpc_table);

    sp_free(h);
}

int sp_jsonrpc_server_run(void *h)
{
    sp_return_val_if_fail(h, -1);
    sp_jsonrpc_server_t *server = (sp_jsonrpc_server_t *)h;

    sp_reactor_run(server->reactor, 0);

    return 0;
}

int sp_jsonrpc_server_stop(void *h)
{
    sp_return_val_if_fail(h, -1);
    sp_jsonrpc_server_t *server = (sp_jsonrpc_server_t *)h;

    sp_reactor_stop(server->reactor);

    return 0;
}

int sp_jsonrpc_server_register(void *h, const char *method, sp_jsonrpc_func_t func, void *arg)
{
    sp_jsonrpc_server_t *server = (sp_jsonrpc_server_t *)h;

    sp_jsonrpc_register_t *rpc_register = sp_calloc(1, sizeof(sp_jsonrpc_register_t));

    rpc_register->func = func;
    rpc_register->arg = arg;

    sp_hashtable_put(server->rpc_table, sp_strdup(method), rpc_register);

    return 0;
}

int sp_jsonrpc_server_unregister(void *h, const char *method)
{
    sp_jsonrpc_server_t *server = (sp_jsonrpc_server_t *)h;

    sp_hashtable_remove(server->rpc_table, method);

    return 0;
}

static void handle_connect(int sock, void *arg)
{
    sp_jsonrpc_server_t *server = (sp_jsonrpc_server_t *)arg;

    struct sockaddr_in addr;
    size_t addr_size = sizeof(addr);
    sp_bzero(&addr, sizeof(struct sockaddr_in));

    int s = sp_socket_accept(sock, (struct sockaddr *)&addr, &addr_size);
    if (s < 0)
    {
        return;
    }

    sp_jsonrpc_session_t *session = sp_calloc(1, sizeof(sp_jsonrpc_session_t)); 

    session->sock = s;
    session->ev = sp_reactor_attach(server->reactor, s, handle_read, session);
    session->server = server;
    session->buffer = sp_string_buffer_new(0);
}

static void handle_read(int sock, void *arg)
{
    sp_jsonrpc_session_t *session = (sp_jsonrpc_session_t *)arg;
    sp_jsonrpc_server_t *server = session->server;

    /* fixMe: cache in the future */

    int readable = sp_socket_readable(sock);

    if (readable <= 0)
    {
        session_close(session);
        return;
    }
    
    void *buffer = session->buffer;

    if (readable > (sp_string_buffer_capacity(buffer) - sp_string_buffer_size(buffer)))
    {
        sp_string_buffer_resize(buffer, sp_string_buffer_size(buffer) + readable);
    }

    char *string = sp_string_buffer_string(buffer);
    int length = sp_socket_read(sock, string, readable);
    if (length <= 0)
    {
        session_close(session);
        return;
    }

    string[length] = 0;

    /* fixMe: process when read done */
    
    sp_json_t *json = sp_json_parse(string);
    sp_json_t *id = NULL;

    do
    {
        sp_break_if_fail(json);

        sp_json_t *method = sp_json_object_item(json, "method");
        sp_json_t *version = sp_json_object_item(json, "jsonrpc");
        sp_json_t *params = sp_json_object_item(json, "params");
        id = sp_json_object_item(json, "id");

        sp_break_if_fail(method && version && params);

        sp_jsonrpc_register_t *rpc_register = sp_hashtable_get(server->rpc_table, method->valuestring);
        sp_break_if_fail(rpc_register);

        sp_jsonrpc_t rpc;
        rpc.params = params;
        rpc.id = id;
        rpc.session = session;
        rpc.server = server;

        sp_json_t *result = rpc_register->func(&rpc);

        if (result)
        {
            sp_jsonrpc_session_reply_result(session, result, id);
            sp_json_free(result);

            session_close(session);
        }
        else
        {
            /* dispatch to otherwhere */
        }

        sp_json_free(json);
        return;
    } while(0);

    send_error(session, -32603, "rpc failed", id);

    if (json)
    {
        sp_json_free(json);
    }
}

static void session_close(sp_jsonrpc_session_t *session)
{
    sp_return_if_fail(session);

    sp_reactor_detach(session->ev);
    sp_socket_close(session->sock);
    sp_string_buffer_free(session->buffer);
    sp_free(session);
}

static void send_error(sp_jsonrpc_session_t *session, int code, const char *message, sp_json_t *id)
{
    sp_json_t *json = sp_json_object_new();

    sp_json_t *error = sp_json_object_new();
    sp_json_object_add(json, "error", error);

    sp_json_object_add(error, "code", sp_json_int(code));
    sp_json_object_add(error, "message", sp_json_string(message));

    if (id)
    {
        sp_json_object_add(json, "id", sp_json_int(id->valueint));
    }

    const char *text = sp_json_text(json);

    send_response(session, text);

    sp_free(text);
    sp_json_free(json);
}

static void send_response(sp_jsonrpc_session_t *session, const char *res)
{
    sp_return_if_fail(session && res);

    sp_socket_write(session->sock, res, sp_string_length(res));
}

void sp_jsonrpc_session_close(void *session)
{
    session_close(session);
}

void sp_jsonrpc_session_reply_error(void *session, int code, const char *message, sp_json_t *id)
{
    send_error(session, code, message, id);
}

void sp_jsonrpc_session_reply_result(void *session, sp_json_t *result, sp_json_t *id)
{
    sp_json_t *json = sp_json_object_new();

    sp_json_object_add(json, "jsonrpc", sp_json_string("2.0"));
    sp_json_object_add(json, "result", sp_json_clone(result));
    if (id)
    {
        sp_json_object_add(json, "id", sp_json_clone(id));
    }

    const char *text = sp_json_text(json);
    send_response(session, text);

    sp_free(text);
    sp_json_free(json);
}

