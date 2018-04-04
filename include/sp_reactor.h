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

#ifndef _SP_REACTOR_H_
#define _SP_REACTOR_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*sp_reactor_callback)(int sock, void *arg);
   
enum
{
    SP_REACTOR_RUN_FOREVER,
    SP_REACTOR_RUN_THREAD,
};
   
void *sp_reactor_new(const char *method);
void sp_reactor_free(void *h);
void *sp_reactor_attach(void *h, int sock, sp_reactor_callback cb, void *arg);
void *sp_reactor_attach_once(void *h, int sock, sp_reactor_callback cb, void *arg);
void sp_reactor_detach(void *ev);
void sp_reactor_run(void *h, int flag);
void sp_reactor_stop(void *h);

#ifdef __cplusplus
}
#endif
   
#endif 
