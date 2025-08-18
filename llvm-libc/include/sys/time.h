//===-- Linux header time.h -----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SYS_TIME_H
#define LLVM_LIBC_SYS_TIME_H

#include "__llvm-libc-common.h"

#include "../llvm-libc-macros/sys-time-macros.h"

#include "../llvm-libc-types/struct_itimerval.h"
#include "../llvm-libc-types/struct_timeval.h"

__BEGIN_C_DECLS

int getitimer(int, struct itimerval *) __NOEXCEPT;

int setitimer(int, const struct itimerval *__restrict, struct itimerval *__restrict) __NOEXCEPT;

int utimes(const char*, const struct timeval*) __NOEXCEPT;

__END_C_DECLS

#endif // LLVM_LIBC_SYS_TIME_H
