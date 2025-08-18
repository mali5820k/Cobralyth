//===-- POSIX header sys/socket.h -----------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SYS_SOCKET_H
#define LLVM_LIBC_SYS_SOCKET_H

#include "__llvm-libc-common.h"

#include "../llvm-libc-macros/sys-socket-macros.h"

#include "../llvm-libc-types/sa_family_t.h"
#include "../llvm-libc-types/size_t.h"
#include "../llvm-libc-types/socklen_t.h"
#include "../llvm-libc-types/ssize_t.h"
#include "../llvm-libc-types/struct_iovec.h"
#include "../llvm-libc-types/struct_msghdr.h"
#include "../llvm-libc-types/struct_sockaddr.h"
#include "../llvm-libc-types/struct_sockaddr_un.h"

__BEGIN_C_DECLS

int bind(int, const struct sockaddr *, socklen_t) __NOEXCEPT;

ssize_t recv(int, void *, size_t, int) __NOEXCEPT;

ssize_t recvfrom(int, void *, size_t, int, struct sockaddr *__restrict, socklen_t *__restrict) __NOEXCEPT;

ssize_t recvmsg(int, struct msghdr *, int) __NOEXCEPT;

ssize_t send(int, const void*, size_t, int) __NOEXCEPT;

ssize_t sendmsg(int, const struct msghdr *, int) __NOEXCEPT;

ssize_t sendto(int, const void *, size_t, int, const struct sockaddr *, socklen_t) __NOEXCEPT;

int socket(int, int, int) __NOEXCEPT;

int socketpair(int, int, int, int*) __NOEXCEPT;

__END_C_DECLS

#endif // LLVM_LIBC_SYS_SOCKET_H
