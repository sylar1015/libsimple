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

#ifndef _SP_JSON_H_
#define _SP_JSON_H_

#include "sp_memory.h"
#include <cjson/cJSON.h>

typedef cJSON sp_json_t;

void sp_json_init();
sp_json_t *sp_json_parse(const char *text);
char *sp_json_text(sp_json_t *json);

sp_json_t *sp_json_array_new();
int sp_json_array_size(sp_json_t *json);
sp_json_t *sp_json_array_item(sp_json_t *json, int index);
void sp_json_array_add(sp_json_t *json, sp_json_t *item);


sp_json_t *sp_json_object_new();
sp_json_t *sp_json_object_item(sp_json_t *json, const char* key);
sp_json_t *sp_json_object_item_i(sp_json_t *json, const char *key);
void sp_json_object_add(sp_json_t *json, const char *key, sp_json_t *item);

sp_json_t *sp_json_int(int val);
sp_json_t *sp_json_string(const char *string);
sp_json_t *sp_json_double(double val);

void sp_json_free(sp_json_t *json);

#define sp_json_foreach cJSON_ArrayForEach

#endif
