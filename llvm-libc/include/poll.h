//===-- C standard library header poll.h ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_POLL_H
#define LLVM_LIBC_POLL_H

#include "__llvm-libc-common.h"
#include "llvm-libc-macros/poll-macros.h"

#include "llvm-libc-types/nfds_t.h"
#include "llvm-libc-types/struct_pollfd.h"

__BEGIN_C_DECLS

int poll(struct pollfd *, nfds_t, int) __NOEXCEPT;

__END_C_DECLS

#endif // LLVM_LIBC_POLL_H
