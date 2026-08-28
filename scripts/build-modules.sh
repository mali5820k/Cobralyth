#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TARGET="${CLYTH_ZIG_TARGET:-x86_64-linux-musl}"
ARTIFACT_TARGET="${CLYTH_ARTIFACT_TARGET:-x86_64-linux-musl}"
CC="${CLYTH_CC:-$ROOT/scripts/zig-c.sh}"
AR="${CLYTH_AR:-$ROOT/scripts/zig-ar.sh}"
MODE="${1:-build}"

if [[ "$MODE" == "--clean" ]]; then
  find "$ROOT/module-src" -type d \( -name build -o -name artifacts \) -prune -exec rm -rf {} +
  rm -rf "$ROOT/modules/core" "$ROOT/modules/runtime"
  exit 0
fi

export CLYTH_ZIG_TARGET="$TARGET"
for module in "$ROOT"/module-src/runtime/*; do
  [[ -d "$module" ]] || continue
  binding="$module/c-binding"
  [[ -d "$binding" ]] || continue
  name="$(basename "$module")"
  artifact="$module/artifacts/$ARTIFACT_TARGET"
  build="$module/build/native/$ARTIFACT_TARGET"
  mkdir -p "$artifact" "$build"

  export CLYTH_CC="$CC"
  export CLYTH_AR="$AR"
  export CLYTH_ARTIFACT_ROOT="$artifact"
  vendor_root=""
  if [[ -x "$binding/fetch-vendors.sh" ]]; then
    vendor_root="$("$binding/fetch-vendors.sh")"
  fi
  [[ -x "$binding/build-vendors.sh" ]] && "$binding/build-vendors.sh"

  objs=()
  while IFS= read -r -d '' src; do
    obj="$build/$(basename "${src%.c}").o"
    inc=(-I"$binding/include")
    if [[ -n "$vendor_root" ]]; then
      [[ -d "$vendor_root/include" ]] && inc+=(-I"$vendor_root/include")
      [[ -d "$vendor_root/src" ]] && inc+=(-I"$vendor_root/src")
      inc+=(-I"$vendor_root")
    fi
    "$CC" -std=gnu11 -O2 -fPIC "${inc[@]}" -c "$src" -o "$obj"
    objs+=("$obj")
  done < <(find "$binding/src" -maxdepth 1 -type f -name '*.c' -print0 | sort -z)
  if ((${#objs[@]})); then
    "$AR" rcs "$artifact/lib${name}_binding.a" "${objs[@]}"
    printf 'Built module binding: %s\n' "$name"
  fi
done
