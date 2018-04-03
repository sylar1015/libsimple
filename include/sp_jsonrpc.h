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

#ifndef _SP_JSONRPC_H_
#define _SP_JSONRPC_H_

#include "sp_json.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    sp_json_t *params;
    sp_json_t *id;
    void *session;
    void *server;
} sp_jsonrpc_t;

typedef sp_json_t *(*sp_jsonrpc_func_t)(sp_jsonrpc_t *rpc);

void *sp_jsonrpc_server_new(const char *ipv4, int port);

void sp_jsonrpc_server_free(void *h);

int sp_jsonrpc_server_run(void *h);

int sp_jsonrpc_server_register(void *h, const char *method, sp_jsonrpc_func_t func, void *arg);

int sp_jsonrpc_server_unregister(void *h, const char *method);

void sp_jsonrpc_session_close(void *session);

void sp_jsonrpc_session_reply_error(void *session, int code, const char *message, sp_json_t *id);
void sp_jsonrpc_session_reply_result(void *session, sp_json_t *result, sp_json_t *id);

#ifdef __cplusplus
}
#endif

#endif
