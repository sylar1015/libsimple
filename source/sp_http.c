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
#include <curl/curl.h>

static struct curl_slist *sp_headers_2_curl_headers(sp_http_headers_t *headers);
static size_t sp_http_write_callback(void *data, size_t size, size_t nmemb, void *arg);
static size_t sp_http_header_callback(void *data, size_t size, size_t nmemb, void *arg);

void sp_http_headers_set(sp_http_headers_t *headers, const char *key, const char *val)
{
    sp_return_if_fail(headers && key && val);

    if (!headers->headers)
    {
        headers->headers = sp_json_object_new();
    }

    sp_json_object_add(headers->headers, key, sp_json_string(val));
}

char *sp_http_headers_get(sp_http_headers_t *headers, const char *key)
{
    sp_return_val_if_fail(headers && key && headers->headers, NULL);

    sp_json_t *item = sp_json_object_item_i(headers->headers, key);

    sp_return_val_if_fail(item, NULL);

    return item->valuestring;
}

void sp_http_headers_init(sp_http_headers_t *headers)
{
    sp_return_if_fail(headers);

    headers->headers = NULL;
}

void sp_http_headers_fini(sp_http_headers_t *headers)
{
    sp_return_if_fail(headers);

    if (headers->headers)
    {
        sp_json_free(headers->headers);
        headers->headers = NULL;
    }
}

sp_http_response_t *sp_http_get(const char *url, sp_http_headers_t *headers, int timeout)
{
    sp_return_val_if_fail(url, NULL);

    CURL *curl = curl_easy_init();
    sp_return_val_if_fail(curl, NULL);

    curl_easy_setopt(curl, CURLOPT_URL, url);

    /* headers */
    struct curl_slist *curl_headers = sp_headers_2_curl_headers(headers);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_headers);

    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    /* timeout */
    if (timeout > 0)
    {
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);        
    }

    sp_http_response_t *res = sp_http_response_new();

    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION,
                   sp_http_header_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, res);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, sp_http_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, res);

    do
    {
        CURLcode ret = curl_easy_perform(curl);
        sp_break_if_fail(ret == CURLE_OK);

        long status_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);

        res->status_code = status_code;
        
    } while(0);

    curl_slist_free_all(curl_headers);
    curl_easy_cleanup(curl);
    return res;
}

sp_http_response_t *sp_http_post(const char *url, sp_http_headers_t *headers, int timeout)
{
    sp_return_val_if_fail(url, NULL);
}

sp_http_response_t *sp_http_response_new()
{
    sp_http_response_t *res = sp_calloc(1, sizeof(sp_http_response_t));

    sp_http_headers_init(&res->headers);

    res->raw_headers = sp_string_buffer_new(0);
    res->raw_body = sp_string_buffer_new(0);

    return res;
}

void sp_http_response_free(sp_http_response_t *res)
{
    sp_return_if_fail(res);

    sp_string_buffer_free(res->raw_body);
    sp_string_buffer_free(res->raw_headers);

    sp_http_headers_fini(&res->headers);
}

static struct curl_slist *sp_headers_2_curl_headers(sp_http_headers_t *headers)
{
    sp_return_val_if_fail(headers && headers->headers, NULL);

    struct curl_slist *curl_headers = NULL;

    sp_json_t *item = NULL;
    sp_json_foreach(item, headers->headers)
    {
        char header_kv[1024];
        sp_string_clear(header_kv);
        sp_string_append(header_kv, "%s: %s", item->string, item->valuestring);

        curl_headers = curl_slist_append(curl_headers, header_kv);
    }

    return curl_headers;
}

static size_t sp_http_write_callback(void *data, size_t size, size_t nmemb, void *arg)
{
    sp_http_response_t *res = (sp_http_response_t *)arg;

    size_t length = size * nmemb;
    sp_string_buffer_append(res->raw_body, data, length);

    return length;
}

static size_t sp_http_header_callback(void *data, size_t size, size_t nmemb, void *arg)
{    
    sp_http_response_t *res = (sp_http_response_t *)arg;

    size_t length = size * nmemb;
    sp_string_buffer_append(res->raw_headers, data, length);

    return length;
}
