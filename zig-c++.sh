#!/usr/bin/env bash
set -euo pipefail

# Clyth Zig C++ wrapper
# Default invariant: C++ uses Zig/LLVM libc++ on the same musl target as C.
: "${CLYTH_ZIG_TARGET:=native-linux-musl}"
: "${CLYTH_ZIG_STATIC:=1}"
: "${CLYTH_ZIG_CXX_STDLIB:=libc++}"

args=("$@")
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

cmd=(zig c++)
if [[ "$has_target" -eq 0 && -n "$CLYTH_ZIG_TARGET" ]]; then
  cmd+=(-target "$CLYTH_ZIG_TARGET")
fi
if [[ "$has_stdlib" -eq 0 && -n "$CLYTH_ZIG_CXX_STDLIB" ]]; then
  cmd+=(-stdlib="$CLYTH_ZIG_CXX_STDLIB")
fi
if [[ "$CLYTH_ZIG_STATIC" == "1" && "$link_mode" -eq 1 ]]; then
  cmd+=(-static)
fi
cmd+=("${args[@]}")
exec "${cmd[@]}"
