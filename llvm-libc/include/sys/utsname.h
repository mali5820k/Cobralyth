//===-- POSIX header <sys/utsname.h> --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//

#ifndef _LLVM_LIBC_SYS_UTSNAME_H
#define _LLVM_LIBC_SYS_UTSNAME_H

#include "../__llvm-libc-common.h"
#include "../llvm-libc-types/struct_utsname.h"

__BEGIN_C_DECLS

int uname(struct utsname *) __NOEXCEPT;

__END_C_DECLS

#endif // _LLVM_LIBC_SYS_UTSNAME_H
