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

void sp_json_init()
{
    cJSON_Hooks hooks;
    hooks.malloc_fn = sp_malloc;
    hooks.free_fn = sp_free;
    cJSON_InitHooks(&hooks);
}

sp_json_t *sp_json_parse(const char *text)
{
    return cJSON_Parse(text);
}

char *sp_json_text(sp_json_t *json)
{
    return cJSON_Print(json);
}

sp_json_t *sp_json_array_new()
{
    return cJSON_CreateArray();
}

int sp_json_array_size(sp_json_t *json)
{
    return cJSON_GetArraySize(json);
}

sp_json_t *sp_json_array_item(sp_json_t *json, int index)
{
    return cJSON_GetArrayItem(json, index);
}

void sp_json_array_add(sp_json_t *json, sp_json_t *item)
{
    cJSON_AddItemToArray(json, item);
}

sp_json_t *sp_json_object_new()
{
    return cJSON_CreateObject();
}

sp_json_t *sp_json_object_item(sp_json_t *json, const char* key)
{
    return cJSON_GetObjectItem(json, key);
}

sp_json_t *sp_json_object_item_i(sp_json_t *json, const char *key)
{
    return cJSON_GetObjectItemCaseSensitive(json, key);
}

void sp_json_object_add(sp_json_t *json, const char *key, sp_json_t *item)
{
    cJSON_AddItemToObject(json, key, item);
}

sp_json_t *sp_json_int(int val)
{
    return cJSON_CreateNumber(val);
}

sp_json_t *sp_json_string(const char *string)
{
    return cJSON_CreateString(string);
}

sp_json_t *sp_json_doule(double val)
{
    return cJSON_CreateNumber(val);
}

void sp_json_free(sp_json_t *json)
{
    cJSON_Delete(json);
}
