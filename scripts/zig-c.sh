#!/usr/bin/env bash
set -Eeuo pipefail

# Zig is the native C/C++ compiler driver for Clyth module wrappers. It is not
# the libc provider: all libc headers come from the Clyth-managed musl sysroot.
: "${CLYTH_ZIG:=zig}"
: "${CLYTH_ZIG_TARGET:=x86_64-linux-musl}"
: "${CLYTH_ZIG_STATIC:=1}"
: "${CLYTH_MUSL_SYSROOT:?CLYTH_MUSL_SYSROOT must point at the Clyth-managed musl sysroot}"

if [[ ! -d "$CLYTH_MUSL_SYSROOT/include" || ! -f "$CLYTH_MUSL_SYSROOT/lib/libc.a" ]]; then
  printf 'error: invalid Clyth musl sysroot: %s\n' "$CLYTH_MUSL_SYSROOT" >&2
  exit 2
fi

# Never allow CMake or a dependency probe to inject host glibc headers into
# the managed musl build.
args=()
raw=("$@")
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
link_mode=1
for arg in "${args[@]}"; do
  case "$arg" in
    -target|--target|--target=*) has_target=1 ;;
    -c|-S|-E|-M|-MM) link_mode=0 ;;
  esac
done

cmd=("$CLYTH_ZIG" cc -fno-sanitize=all -nostdinc -isystem "$CLYTH_MUSL_SYSROOT/include")
if [[ "$has_target" -eq 0 && -n "$CLYTH_ZIG_TARGET" ]]; then
  cmd+=(-target "$CLYTH_ZIG_TARGET")
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
