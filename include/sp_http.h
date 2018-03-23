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


#ifndef _SP_HTTP_H_
#define _SP_HTTP_H_

#include "sp_json.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    sp_json_t *headers;
} sp_http_headers_t;

typedef struct
{
    int status_code;

    sp_http_headers_t headers;

    void *raw_headers;
    void *raw_body;
} sp_http_response_t;

sp_http_response_t *sp_http_get(const char *url, sp_http_headers_t *headers, int timeout);

sp_http_response_t *sp_http_post(const char *url,
    sp_http_headers_t *headers,
    const char *payload, int length, int timeout);

void *sp_http_session_new();

void sp_http_session_free(void *session);

int sp_http_session_timeout(void *session);

void sp_http_session_set_timeout(void *session, int timeout);

sp_http_headers_t *sp_http_session_headers(void *session);

void sp_http_session_set_headers(void *session, sp_http_headers_t *headers);

sp_http_response_t *sp_http_session_get(void *session, const char *url);

sp_http_response_t *sp_http_session_post(void *session, const char *url, const char *payload, int length);


sp_http_response_t *sp_http_response_new();
void sp_http_response_free(sp_http_response_t *res);

void sp_http_headers_set(sp_http_headers_t *headers, const char *key, const char *val);

char *sp_http_headers_get(sp_http_headers_t *headers, const char *key);

void sp_http_headers_init(sp_http_headers_t *headers);

void sp_http_headers_fini(sp_http_headers_t *headers);


#ifdef __cplusplus
}
#endif

#endif
