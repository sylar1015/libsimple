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

sp_hashtable_t *sp_hashtable_new(size_t size,
				 sp_hash_func_t hash_fn,
				 sp_equal_func_t equal_fn,
				 sp_free_func_t key_del_fn,
				 sp_free_func_t value_del_fn)
{
    return zc_hashtable_new(size, hash_fn, equal_fn,  key_del_fn, value_del_fn);
}

void sp_hashtable_free(sp_hashtable_t *table)
{
    zc_hashtable_del(table);
}

void sp_hashtable_clear(sp_hashtable_t *table)
{
    return zc_hashtable_clean(table);
}

int sp_hashtable_put(sp_hashtable_t *table, void *key, void *value)
{
    return zc_hashtable_put(table, key, value);
}

void *sp_hashtable_get(sp_hashtable_t *table, const void *key)
{
    return zc_hashtable_get(table, key);
}

void sp_hashtable_remove(sp_hashtable_t *table, const void *key)
{
    zc_hashtable_remove(table, key);
}

sp_hashtable_t *sp_hashtable_begin(sp_hashtable_t *table)
{
    return zc_hashtable_begin(table);
}

sp_hashtable_t *sp_hashtable_next(sp_hashtable_t *table, sp_hashtable_t *entry)
{
    return zc_hashtable_next(table, entry);
}

unsigned int sp_hashtable_str_hash(const void *str)
{
    return zc_hashtable_str_hash(str);
}

int sp_hashtable_str_equal(const void *key1, const void *key2)
{
    return zc_hashtable_str_equal(key1, key2);
}

int sp_hashtable_direct_equal(const void *key1, const void *key2)
{
    return key1 == key2;
}
