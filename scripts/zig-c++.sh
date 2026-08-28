#!/usr/bin/env bash
set -Eeuo pipefail

# Canonical Clyth C++ compiler wrapper.
# Zig supplies the compiler driver and libc++; C headers and libc come only from
# the Clyth-managed musl sysroot. Host GNU include directories are forbidden.
: "${CLYTH_ZIG:=zig}"
: "${CLYTH_ZIG_TARGET:=x86_64-linux-musl}"
: "${CLYTH_ZIG_STATIC:=1}"
: "${CLYTH_ZIG_CXX_STDLIB:=libc++}"
: "${CLYTH_MUSL_SYSROOT:?CLYTH_MUSL_SYSROOT must point at the Clyth-managed musl sysroot}"

if [[ ! -d "$CLYTH_MUSL_SYSROOT/include" || ! -f "$CLYTH_MUSL_SYSROOT/lib/libc.a" ]]; then
  printf 'error: invalid Clyth musl sysroot: %s\n' "$CLYTH_MUSL_SYSROOT" >&2
  exit 2
fi

# CMake dependency probes may surface host include paths. Remove them at the
# wrapper boundary so glibc headers cannot mix with the managed musl headers.
raw=("$@")
args=()
for ((i=0; i<${#raw[@]}; i++)); do
  arg="${raw[i]}"
  case "$arg" in
    -I/usr/include|-I/usr/include/*|-I/usr/local/include|-I/usr/local/include/*) continue ;;
    -isystem)
      if ((i+1 < ${#raw[@]})); then
        case "${raw[i+1]}" in
          /usr/include|/usr/include/*|/usr/local/include|/usr/local/include/*) i=$((i+1)); continue ;;
        esac
      fi
      ;;
    -isystem/usr/include|-isystem/usr/include/*|-isystem/usr/local/include|-isystem/usr/local/include/*) continue ;;
  esac
  args+=("$arg")
done

has_target=0
has_stdlib=0
link_mode=1
for arg in "${args[@]}"; do
  case "$arg" in
    -target|--target|--target=*) has_target=1 ;;
    -stdlib=*|--stdlib=*) has_stdlib=1 ;;
    -c|-S|-E|-M|-MM) link_mode=0 ;;
  esac
done

cmd=("$CLYTH_ZIG" c++ -fno-sanitize=all -nostdinc -isystem "$CLYTH_MUSL_SYSROOT/include")
if [[ "$has_target" -eq 0 && -n "$CLYTH_ZIG_TARGET" ]]; then
  cmd+=(-target "$CLYTH_ZIG_TARGET")
fi
if [[ "$has_stdlib" -eq 0 && -n "$CLYTH_ZIG_CXX_STDLIB" ]]; then
  cmd+=(-stdlib="$CLYTH_ZIG_CXX_STDLIB")
fi
if [[ "$link_mode" -eq 1 ]]; then
  # Prefer the Clyth-built musl lib-root for link-time libc/crt resolution.
  cmd+=(-L"$CLYTH_MUSL_SYSROOT/lib")
fi
if [[ "$CLYTH_ZIG_STATIC" == "1" && "$link_mode" -eq 1 ]]; then
  cmd+=(-static)
fi
cmd+=("${args[@]}")
exec "${cmd[@]}"
