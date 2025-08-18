//===-- POSIX header <sys/statvfs.h> --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//

#ifndef _LLVM_LIBC_SYS_STATVFS_H
#define _LLVM_LIBC_SYS_STATVFS_H

#include "../__llvm-libc-common.h"
#include "../llvm-libc-types/fsblkcnt_t.h"
#include "../llvm-libc-types/fsfilcnt_t.h"
#include "../llvm-libc-types/struct_statvfs.h"

__BEGIN_C_DECLS

int fstatvfs(int, struct statvfs *) __NOEXCEPT;

int statvfs(const char *__restrict, struct statvfs *__restrict) __NOEXCEPT;

__END_C_DECLS

#endif // _LLVM_LIBC_SYS_STATVFS_H
