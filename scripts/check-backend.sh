#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CLYTH_BIN="${CLYTH_BIN:-$ROOT/dist/bootstrap/clyth}"
LLC="${CLYTH_LLC:-$(command -v llc || true)}"
LLD="${CLYTH_LLD:-$(command -v ld.lld || command -v lld || true)}"

[[ -x "$CLYTH_BIN" ]] || "$ROOT/scripts/build-bootstrap.sh"
if [[ -z "$LLC" ]]; then
  printf 'Skipping LLVM backend check: llc not found (set CLYTH_LLC).\n'
  exit 0
fi

TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

check_ir_set() {
  local target="$1" triple="$2" label="$3"
  local count=0
  for src in "$ROOT"/sample-clyth-programs/*.clyth; do
    local base="$(basename "${src%.clyth}")"
    "$CLYTH_BIN" build "$src" --target "$target" --emit-llvm-ir -o "$TMP/$base-$label.ll"
    "$LLC" -filetype=obj -mtriple="$triple" "$TMP/$base-$label.ll" -o "$TMP/$base-$label.o"
    count=$((count + 1))
  done
  while IFS= read -r src; do
    local base="selfhost-$(basename "${src%.clyth}")"
    "$CLYTH_BIN" build "$src" --target "$target" --emit-llvm-ir -o "$TMP/$base-$label.ll"
    "$LLC" -filetype=obj -mtriple="$triple" "$TMP/$base-$label.ll" -o "$TMP/$base-$label.o"
  done < <(find "$ROOT/compiler/clyth/bootstrap-v1/internal" "$ROOT/compiler/clyth/bootstrap-v1/cmd" -type f -name '*.clyth' | sort)
  printf 'LLVM backend %s: %d samples + self-host sources passed llc.\n' "$label" "$count"
}

check_ir_set x86_64-linux-musl x86_64-unknown-linux-musl x86_64

if [[ -n "${CLYTH_MUSL_X86_64_SYSROOT:-}" && -n "$LLD" ]]; then
  "$CLYTH_BIN" build "$ROOT/sample-clyth-programs/27_0_6_control_flow.clyth" \
    --target x86_64-linux-musl --llc "$LLC" --lld "$LLD" \
    --musl-sysroot "$CLYTH_MUSL_X86_64_SYSROOT" -o "$TMP/control-flow"
  "$TMP/control-flow" >/dev/null
  printf 'x86_64 static native execution passed.\n'

  for sample in 04_0_4_runtime_list_collection 05_0_4_runtime_set_collection 06_0_4_runtime_map_collection; do
    "$CLYTH_BIN" build "$ROOT/sample-clyth-programs/$sample.clyth" \
      --target x86_64-linux-musl --zig "${CLYTH_ZIG:-$(command -v zig || true)}" --llc "$LLC" --lld "$LLD" \
      --musl-sysroot "$CLYTH_MUSL_X86_64_SYSROOT" -o "$TMP/$sample"
    "$TMP/$sample" >/dev/null
  done
  printf 'x86_64 native collections (List/Set/Map) passed.\n'
fi

if [[ -n "${CLYTH_MUSL_AARCH64_SYSROOT:-}" && -n "${CLYTH_ZIG:-}" && -n "$LLD" ]]; then
  check_ir_set aarch64-linux-musl aarch64-unknown-linux-musl aarch64
  "$CLYTH_BIN" build "$ROOT/compiler/clyth/bootstrap-v1/cmd/clyth/main.clyth" \
    --target aarch64-linux-musl --zig "$CLYTH_ZIG" --llc "$LLC" --lld "$LLD" \
    --musl-sysroot "$CLYTH_MUSL_AARCH64_SYSROOT" -o "$TMP/clyth-selfhost-aarch64"
  file "$TMP/clyth-selfhost-aarch64"
  printf 'AArch64 static self-host executable link passed.\n'
fi
