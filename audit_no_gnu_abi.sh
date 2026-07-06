#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

fail=0

# This audit intentionally avoids raw `strings` scans for LLVM/lld artifacts.
# lld is a linker, so it legitimately contains textual names such as
# "libstdc++", "GLIBC_2.x", and "GLIBC_ABI_DT_RELR" as data it can parse or
# emit for target programs. Those strings are not proof that lld itself was
# linked against glibc or GNU libstdc++.
#
# Release blockers are actual ABI/linkage fingerprints:
#   - DT_NEEDED on libstdc++/libgcc_s/GNU C++ ABI libraries
#   - PT_INTERP pointing at GNU ld-linux
#   - version-needed records for GLIBC_/GLIBCXX_/CXXABI_
#   - undefined references to glibc-only symbols such as __isoc23_* or
#     gnu_get_libc_version
#
# musl commonly uses the dynamic soname "libc.so", so NEEDED libc.so is allowed.

is_elf(){ file -b "$1" 2>/dev/null | grep -q 'ELF'; }
is_archive(){ file -b "$1" 2>/dev/null | grep -q 'current ar archive'; }

report_bad(){
  local path="$1"
  local hits="$2"
  if [[ -n "$hits" ]]; then
    printf '[BAD] %s\n%s\n' "$path" "$hits"
    fail=1
  fi
}

check_elf(){
  local path="$1" hits=""

  if command -v readelf >/dev/null 2>&1; then
    # Actual shared-library dependencies and interpreter path.
    hits+="$(readelf -d --wide "$path" 2>/dev/null | grep -E 'NEEDED.*\[(libstdc\+\+|libgcc_s|libsupc\+\+|libc\+\+abi|libcxxrt)' || true)"
    hits+=$'\n'
    hits+="$(readelf -l --wide "$path" 2>/dev/null | grep -E '/ld-linux[^ ]*|/lib64/ld-linux|/lib/ld-linux' || true)"
    hits+=$'\n'

    # Actual ELF version requirements, not arbitrary embedded strings.
    hits+="$(readelf -V --wide "$path" 2>/dev/null | grep -E 'Name: (GLIBC_|GLIBCXX_|CXXABI_)|\b(GLIBC_|GLIBCXX_|CXXABI_)[0-9]' || true)"
    hits+=$'\n'
  fi

  if command -v nm >/dev/null 2>&1; then
    # Undefined dynamic symbols that prove glibc leakage.
    hits+="$(nm -A -D -u "$path" 2>/dev/null | grep -E '__isoc23_|gnu_get_libc_version|@GLIBC_|@GLIBCXX_|@CXXABI_' || true)"
    hits+=$'\n'
    # Static executables may not have a dynamic table.
    hits+="$(nm -A -u "$path" 2>/dev/null | grep -E '__isoc23_|gnu_get_libc_version|@GLIBC_|@GLIBCXX_|@CXXABI_' || true)"
    hits+=$'\n'
  fi

  hits="$(printf '%s\n' "$hits" | sed '/^[[:space:]]*$/d' | sort -u)"
  report_bad "$path" "$hits"
}

check_archive(){
  local path="$1" hits=""

  if command -v nm >/dev/null 2>&1; then
    # For static archives, undefined symbols are the useful signal. Do not raw
    # scan strings: LLVM/lld archives contain target/lib names as data.
    hits+="$(nm -A -u "$path" 2>/dev/null | grep -E '__isoc23_|gnu_get_libc_version|@GLIBC_|@GLIBCXX_|@CXXABI_' || true)"
    hits+=$'\n'
  fi

  hits="$(printf '%s\n' "$hits" | sed '/^[[:space:]]*$/d' | sort -u)"
  report_bad "$path" "$hits"
}

while IFS= read -r -d '' path; do
  case "$path" in
    */CMakeFiles/*/CompilerIdC/a.out|*/CMakeFiles/*/CompilerIdCXX/a.out|*/CMakeFiles/*/CMakeDetermineCompilerABI_*.bin)
      # CMake compiler probes are generated diagnostics, not shipped Clyth artifacts.
      continue
      ;;
  esac

  if is_archive "$path"; then
    check_archive "$path"
  elif is_elf "$path"; then
    check_elf "$path"
  fi
done < <(find \
  build-compiler \
  antlr4-cpp-runtime-lib \
  llvm-bundled/bin llvm-bundled/lib \
  clyth-runtime/modules \
  clyth-runtime/c-bindings \
  -type f \( -name '*.a' -o -name '*.so*' -o -perm -111 \) -print0 2>/dev/null)

if [[ "$fail" -ne 0 ]]; then
  echo '[FAIL] GNU/glibc/libstdc++ ABI markers found.'
  exit 1
fi

echo '[ OK ] No GNU/glibc/libstdc++ ABI markers found in checked release artifacts.'
