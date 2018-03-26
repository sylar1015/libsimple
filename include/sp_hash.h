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

#ifndef _SP_HASH_H_
#define _SP_HASH_H_

#include "sp_memory.h"
#include <zc_hashtable.h>

typedef struct zc_hashtable_entry_t sp_hashtable_t;

typedef unsigned int (*sp_hash_func_t) (const void *key);
typedef int (*sp_equal_func_t) (const void *key1, const void *key2);

sp_hashtable_t *sp_hashtable_new(size_t size,
				 sp_hash_func_t hash_fn,
				 sp_equal_func_t equal_fn,
				 sp_free_func_t key_del_fn,
				 sp_free_func_t value_del_fn);

void sp_hashtable_free(sp_hashtable_t *table);
void sp_hashtable_clear(sp_hashtable_t * table);
int sp_hashtable_put(sp_hashtable_t *table, void *key, void *value);
void *sp_hashtable_get(sp_hashtable_t *table, const void *key);
void sp_hashtable_remove(sp_hashtable_t *table, const void *key);
sp_hashtable_t *sp_hashtable_begin(sp_hashtable_t *table);
sp_hashtable_t *sp_hashtable_next(sp_hashtable_t *table, sp_hashtable_t *entry);

#define sp_hashtable_foreach(table, entry) \
    for(entry = sp_hashtable_begin(table); entry; entry = sp_hashtable_next(table, entry))

unsigned int sp_hashtable_str_hash(const void *str);
int sp_hashtable_str_equal(const void *key1, const void *key2);

#endif
