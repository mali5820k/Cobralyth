//===-- POSIX header <sys/uio.h> --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//

#ifndef _LLVM_LIBC_SYS_UIO_H
#define _LLVM_LIBC_SYS_UIO_H

#include "../__llvm-libc-common.h"
#include "../llvm-libc-types/ssize_t.h"
#include "../llvm-libc-types/struct_iovec.h"

__BEGIN_C_DECLS

ssize_t readv(int, const struct iovec *, int) __NOEXCEPT;

ssize_t writev(int, const struct iovec *, int) __NOEXCEPT;

__END_C_DECLS

#endif // _LLVM_LIBC_SYS_UIO_H
