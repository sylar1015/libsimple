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

#include <event.h>

inline int sp_socket(int domain, int type, int protocol)
{
    return socket(domain, type, protocol);
}

inline void sp_socket_close(int sock)
{
    close(sock);
}

int sp_socket_bind(int sock, struct sockaddr *addr, int addr_len)
{
    return bind(sock, addr, addr_len);
}

inline int sp_socket_listen(int sock, int backlog)
{
    return listen(sock, backlog);
}

inline int sp_socket_accept(int sock, struct sockaddr *addr, int *addr_len)
{
    return accept(sock, addr, addr_len);
}

inline int sp_socket_write(int sock, const void *data, int length)
{
    return send(sock, data, length, 0);
}

inline int sp_socket_read(int sock, void *data, int length)
{
    return recv(sock, data, length, 0);
}

inline int sp_socket_connect(int sock, struct sockaddr *addr, int addr_len)
{
    return connect(sock, addr, addr_len);
}

inline int sp_socket_reuseable(int sock)
{
    return evutil_make_listen_socket_reuseable(sock);
}

inline int sp_socket_nonblocking(int sock)
{
    return evutil_make_socket_nonblocking(sock);
}

inline int sp_socket_readable(int sock)
{
    int size;
    if (ioctl(sock, FIONREAD, &size) < 0)
        return 16384;

    return size > 16384 ? 16384 : size;
}

int sp_socket_originaldst(int sock, struct sockaddr *addr)
{
    return getsockopt(sock, SOL_IP, SO_ORIGINAL_DST, addr, sizeof(struct sockaddr_in));
}

int sp_socket_tcpinfo(int sock, struct tcp_info *ti)
{
    int len = sizeof(struct tcp_info);
    sp_bzero(ti, len);
    return getsockopt(sock, IPPROTO_TCP, TCP_INFO, ti, &len);
}

inline uint32_t sp_htonl(uint32_t hostlong)
{
    return htonl(hostlong);
}

inline uint16_t sp_htons(uint16_t hostshort)
{
    return htons(hostshort);
}

inline uint32_t sp_ntohl(uint32_t netlong)
{
    return ntohl(netlong);
}

inline uint16_t sp_ntohs(uint16_t netshort)
{
    return ntohs(netshort);
}

