//===-- Standard C header <setjmp.h> --===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//

#ifndef _LLVM_LIBC_SETJMP_H
#define _LLVM_LIBC_SETJMP_H

#include "__llvm-libc-common.h"
#include "llvm-libc-types/jmp_buf.h"
#include "llvm-libc-types/sigjmp_buf.h"

__BEGIN_C_DECLS

_Noreturn void longjmp(jmp_buf, int) __NOEXCEPT;

_Returns_twice int setjmp(jmp_buf) __NOEXCEPT;

_Noreturn void siglongjmp(sigjmp_buf, int) __NOEXCEPT;

_Returns_twice int sigsetjmp(sigjmp_buf, int) __NOEXCEPT;

__END_C_DECLS

#endif // _LLVM_LIBC_SETJMP_H
