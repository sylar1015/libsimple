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

#ifndef _SP_SOCKET_H_
#define _SP_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <linux/netfilter_ipv4.h>

int sp_socket(int domain, int type, int protocol);
void sp_socket_close(int sock);
int sp_socket_bind(int sock, struct sockaddr *addr, int addr_len);
int sp_socket_listen(int sock, int backlog);
int sp_socket_accept(int sock, struct sockaddr *addr, int *size);
int sp_socket_write(int sock, const void *data, int length);
int sp_socket_read(int sock, void *data, int length);
int sp_socket_connect(int sock, struct sockaddr *addr, int addr_len);

int sp_socket_reuseable(int sock);
int sp_socket_nonblocking(int sock);
int sp_socket_readable(int sock);
int sp_socket_originaldst(int sock, struct sockaddr *addr);
int sp_socket_tcpinfo(int sock, struct tcp_info *ti);

uint32_t sp_htonl(uint32_t hostlong);
uint16_t sp_htons(uint16_t hostshort);
uint32_t sp_ntohl(uint32_t netlong);
uint16_t sp_ntohs(uint16_t netshort);

#endif
