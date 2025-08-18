//===-- POSIX header <dirent.h> --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//

#ifndef _LLVM_LIBC_DIRENT_H
#define _LLVM_LIBC_DIRENT_H

#include "__llvm-libc-common.h"
#include "llvm-libc-types/DIR.h"
#include "llvm-libc-types/ino_t.h"
#include "llvm-libc-types/struct_dirent.h"

__BEGIN_C_DECLS

int closedir(DIR *) __NOEXCEPT;

int dirfd(DIR *) __NOEXCEPT;

DIR *opendir(const char *) __NOEXCEPT;

struct dirent *readdir(DIR *) __NOEXCEPT;

__END_C_DECLS

#endif // _LLVM_LIBC_DIRENT_H
