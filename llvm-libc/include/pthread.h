//===-- POSIX header pthread.h --------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_PTHREAD_H
#define LLVM_LIBC_PTHREAD_H

#include "__llvm-libc-common.h"
#include "llvm-libc-macros/pthread-macros.h"

#include "llvm-libc-types/__atfork_callback_t.h"
#include "llvm-libc-types/__pthread_once_func_t.h"
#include "llvm-libc-types/__pthread_start_t.h"
#include "llvm-libc-types/__pthread_tss_dtor_t.h"
#include "llvm-libc-types/clockid_t.h"
#include "llvm-libc-types/pthread_attr_t.h"
#include "llvm-libc-types/pthread_barrier_t.h"
#include "llvm-libc-types/pthread_barrierattr_t.h"
#include "llvm-libc-types/pthread_condattr_t.h"
#include "llvm-libc-types/pthread_key_t.h"
#include "llvm-libc-types/pthread_mutex_t.h"
#include "llvm-libc-types/pthread_mutexattr_t.h"
#include "llvm-libc-types/pthread_once_t.h"
#include "llvm-libc-types/pthread_rwlock_t.h"
#include "llvm-libc-types/pthread_rwlockattr_t.h"
#include "llvm-libc-types/pthread_spinlock_t.h"
#include "llvm-libc-types/pthread_t.h"
#include "llvm-libc-types/size_t.h"
#include "llvm-libc-types/struct_timespec.h"

__BEGIN_C_DECLS

int pthread_atfork(__atfork_callback_t, __atfork_callback_t, __atfork_callback_t) __NOEXCEPT;

int pthread_attr_destroy(pthread_attr_t *) __NOEXCEPT;

int pthread_attr_getdetachstate(const pthread_attr_t *, int *) __NOEXCEPT;

int pthread_attr_getguardsize(const pthread_attr_t *__restrict, size_t *__restrict) __NOEXCEPT;

int pthread_attr_getstack(const pthread_attr_t *__restrict, void **__restrict, size_t *__restrict) __NOEXCEPT;

int pthread_attr_getstacksize(const pthread_attr_t *__restrict, size_t *__restrict) __NOEXCEPT;

int pthread_attr_init(pthread_attr_t *) __NOEXCEPT;

int pthread_attr_setdetachstate(pthread_attr_t *, int) __NOEXCEPT;

int pthread_attr_setguardsize(pthread_attr_t *, size_t) __NOEXCEPT;

int pthread_attr_setstack(pthread_attr_t *, void *, size_t) __NOEXCEPT;

int pthread_attr_setstacksize(pthread_attr_t *, size_t) __NOEXCEPT;

int pthread_barrier_destroy(pthread_barrier_t *) __NOEXCEPT;

int pthread_barrier_init(pthread_barrier_t *__restrict, const pthread_barrierattr_t *__restrict, int) __NOEXCEPT;

int pthread_barrier_wait(pthread_barrier_t *) __NOEXCEPT;

int pthread_condattr_destroy(pthread_condattr_t *) __NOEXCEPT;

int pthread_condattr_getclock(const pthread_condattr_t *__restrict, clockid_t *__restrict) __NOEXCEPT;

int pthread_condattr_getpshared(const pthread_condattr_t *__restrict, int *__restrict) __NOEXCEPT;

int pthread_condattr_init(pthread_condattr_t *) __NOEXCEPT;

int pthread_condattr_setclock(pthread_condattr_t *, clockid_t) __NOEXCEPT;

int pthread_condattr_setpshared(pthread_condattr_t *, int) __NOEXCEPT;

int pthread_create(pthread_t *__restrict, const pthread_attr_t *__restrict, __pthread_start_t, void *) __NOEXCEPT;

int pthread_detach(pthread_t) __NOEXCEPT;

int pthread_equal(pthread_t, pthread_t) __NOEXCEPT;

_Noreturn void pthread_exit(void *) __NOEXCEPT;

int pthread_getname_np(pthread_t, char *, size_t) __NOEXCEPT;

void *pthread_getspecific(pthread_key_t) __NOEXCEPT;

int pthread_join(pthread_t, void **) __NOEXCEPT;

int pthread_key_create(pthread_key_t *, __pthread_tss_dtor_t) __NOEXCEPT;

int pthread_key_delete(pthread_key_t) __NOEXCEPT;

int pthread_mutex_destroy(pthread_mutex_t *) __NOEXCEPT;

int pthread_mutex_init(pthread_mutex_t *__restrict, const pthread_mutexattr_t *__restrict) __NOEXCEPT;

int pthread_mutex_lock(pthread_mutex_t *) __NOEXCEPT;

int pthread_mutex_unlock(pthread_mutex_t *) __NOEXCEPT;

int pthread_mutexattr_destroy(pthread_mutexattr_t *) __NOEXCEPT;

int pthread_mutexattr_getpshared(const pthread_mutexattr_t *__restrict, int *__restrict) __NOEXCEPT;

int pthread_mutexattr_getrobust(const pthread_mutexattr_t *__restrict, int *__restrict) __NOEXCEPT;

int pthread_mutexattr_gettype(const pthread_mutexattr_t *__restrict, int *__restrict) __NOEXCEPT;

int pthread_mutexattr_init(pthread_mutexattr_t *) __NOEXCEPT;

int pthread_mutexattr_setpshared(pthread_mutexattr_t *__restrict, int) __NOEXCEPT;

int pthread_mutexattr_setrobust(pthread_mutexattr_t *__restrict, int) __NOEXCEPT;

int pthread_mutexattr_settype(pthread_mutexattr_t *__restrict, int) __NOEXCEPT;

int pthread_once(pthread_once_t *, __pthread_once_func_t) __NOEXCEPT;

int pthread_rwlock_clockrdlock(pthread_rwlock_t *__restrict, clockid_t, const struct timespec *__restrict) __NOEXCEPT;

int pthread_rwlock_clockwrlock(pthread_rwlock_t *__restrict, clockid_t, const struct timespec *__restrict) __NOEXCEPT;

int pthread_rwlock_destroy(pthread_rwlock_t *) __NOEXCEPT;

int pthread_rwlock_init(pthread_rwlock_t *, const pthread_rwlockattr_t *__restrict) __NOEXCEPT;

int pthread_rwlock_rdlock(pthread_rwlock_t *) __NOEXCEPT;

int pthread_rwlock_timedrdlock(pthread_rwlock_t *__restrict, const struct timespec *__restrict) __NOEXCEPT;

int pthread_rwlock_timedwrlock(pthread_rwlock_t *__restrict, const struct timespec *__restrict) __NOEXCEPT;

int pthread_rwlock_tryrdlock(pthread_rwlock_t *) __NOEXCEPT;

int pthread_rwlock_trywrlock(pthread_rwlock_t *) __NOEXCEPT;

int pthread_rwlock_unlock(pthread_rwlock_t *) __NOEXCEPT;

int pthread_rwlock_wrlock(pthread_rwlock_t *) __NOEXCEPT;

int pthread_rwlockattr_destroy(pthread_rwlockattr_t *) __NOEXCEPT;

int pthread_rwlockattr_getkind_np(pthread_rwlockattr_t *, int *) __NOEXCEPT;

int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *, int *) __NOEXCEPT;

int pthread_rwlockattr_init(pthread_rwlockattr_t *) __NOEXCEPT;

int pthread_rwlockattr_setkind_np(pthread_rwlockattr_t *, int) __NOEXCEPT;

int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *, int) __NOEXCEPT;

pthread_t pthread_self(void) __NOEXCEPT;

int pthread_setname_np(pthread_t, const char *) __NOEXCEPT;

int pthread_setspecific(pthread_key_t, const void *) __NOEXCEPT;

int pthread_spin_destroy(pthread_spinlock_t *) __NOEXCEPT;

int pthread_spin_init(pthread_spinlock_t *, int) __NOEXCEPT;

int pthread_spin_lock(pthread_spinlock_t *) __NOEXCEPT;

int pthread_spin_trylock(pthread_spinlock_t *) __NOEXCEPT;

int pthread_spin_unlock(pthread_spinlock_t *) __NOEXCEPT;

__END_C_DECLS

#endif // LLVM_LIBC_PTHREAD_H
