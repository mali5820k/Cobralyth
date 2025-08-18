//===-- C standard library header wchar.h ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_WCHAR_H
#define LLVM_LIBC_WCHAR_H

#include "__llvm-libc-common.h"
#include "llvm-libc-macros/wchar-macros.h"

#include "llvm-libc-macros/null-macro.h"
#include "llvm-libc-types/mbstate_t.h"
#include "llvm-libc-types/size_t.h"
#include "llvm-libc-types/wchar_t.h"
#include "llvm-libc-types/wint_t.h"

__BEGIN_C_DECLS

wint_t btowc(int) __NOEXCEPT;

int mblen(const char *, size_t) __NOEXCEPT;

size_t mbrlen(const char *__restrict, size_t, mbstate_t *__restrict) __NOEXCEPT;

size_t mbrtowc(wchar_t *__restrict, const char *__restrict, size_t, mbstate_t *__restrict) __NOEXCEPT;

int mbsinit(mbstate_t *) __NOEXCEPT;

size_t mbsnrtowcs(wchar_t *__restrict, const char **__restrict, size_t, size_t, mbstate_t *__restrict) __NOEXCEPT;

size_t mbsrtowcs(wchar_t *__restrict, const char **__restrict, size_t, mbstate_t *__restrict) __NOEXCEPT;

size_t mbstowcs(wchar_t *__restrict, const char *__restrict, size_t) __NOEXCEPT;

int mbtowc(wchar_t *__restrict, const char *__restrict, size_t) __NOEXCEPT;

wchar_t *wcpcpy(wchar_t *__restrict, const wchar_t *__restrict) __NOEXCEPT;

wchar_t *wcpncpy(wchar_t *__restrict, const wchar_t *__restrict, size_t) __NOEXCEPT;

size_t wcrtomb(char *__restrict, wchar_t, mbstate_t *__restrict) __NOEXCEPT;

wchar_t *wcscat(wchar_t *__restrict, const wchar_t *__restrict) __NOEXCEPT;

const wchar_t *wcschr(const wchar_t *, wchar_t) __NOEXCEPT;

int wcscmp(const wchar_t *, const wchar_t *) __NOEXCEPT;

wchar_t *wcscpy(wchar_t *__restrict, const wchar_t *__restrict) __NOEXCEPT;

size_t wcscspn(const wchar_t *, const wchar_t *) __NOEXCEPT;

wchar_t *wcsdup(const wchar_t *) __NOEXCEPT;

size_t wcslcat(wchar_t *__restrict, const wchar_t *__restrict, size_t) __NOEXCEPT;

size_t wcslcpy(wchar_t *__restrict, const wchar_t *__restrict, size_t) __NOEXCEPT;

size_t wcslen(const wchar_t *) __NOEXCEPT;

wchar_t *wcsncat(wchar_t *__restrict, const wchar_t *__restrict, size_t) __NOEXCEPT;

int wcsncmp(const wchar_t *, const wchar_t *, size_t) __NOEXCEPT;

wchar_t *wcsncpy(wchar_t *__restrict, const wchar_t *__restrict, size_t) __NOEXCEPT;

size_t wcsnlen(const wchar_t *, size_t) __NOEXCEPT;

size_t wcsnrtombs(char *__restrict, const wchar_t **__restrict, size_t, size_t, mbstate_t) __NOEXCEPT;

const wchar_t *wcspbrk(const wchar_t *, const wchar_t *) __NOEXCEPT;

const wchar_t *wcsrchr(const wchar_t *, wchar_t) __NOEXCEPT;

size_t wcsrtombs(char *__restrict, const wchar_t **__restrict, size_t, mbstate_t) __NOEXCEPT;

size_t wcsspn(const wchar_t *, const wchar_t *) __NOEXCEPT;

const wchar_t *wcsstr(const wchar_t *, const wchar_t *) __NOEXCEPT;

wchar_t *wcstok(wchar_t *__restrict, const wchar_t *__restrict, wchar_t** __restrict) __NOEXCEPT;

long wcstol(const wchar_t *__restrict, wchar_t **__restrict, int) __NOEXCEPT;

long long wcstoll(const wchar_t *__restrict, wchar_t **__restrict, int) __NOEXCEPT;

size_t wcstombs(char *__restrict, const wchar_t *__restrict, size_t) __NOEXCEPT;

unsigned long wcstoul(const wchar_t *__restrict, wchar_t **__restrict, int) __NOEXCEPT;

unsigned long long wcstoull(const wchar_t *__restrict, wchar_t **__restrict, int) __NOEXCEPT;

int wctob(wint_t) __NOEXCEPT;

int wctomb(char *, wchar_t) __NOEXCEPT;

const wchar_t *wmemchr(const wchar_t *, wchar_t, size_t) __NOEXCEPT;

int wmemcmp(const wchar_t *, const wchar_t *, size_t) __NOEXCEPT;

wchar_t *wmemcpy(wchar_t *__restrict, const wchar_t *__restrict, size_t) __NOEXCEPT;

wchar_t *wmemmove(wchar_t *, const wchar_t *, size_t) __NOEXCEPT;

wchar_t *wmempcpy(wchar_t *, const wchar_t *, size_t) __NOEXCEPT;

wchar_t*wmemset(wchar_t*, wchar_t, size_t) __NOEXCEPT;

__END_C_DECLS

#endif // LLVM_LIBC_WCHAR_H
