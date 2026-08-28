#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
STAGE1="${CLYTH_STAGE1_BIN:-$ROOT/dist/clyth/clyth}"
SOURCE="${CLYTH_COMPILER_SOURCE:-$ROOT/compiler/clyth/bootstrap-v1/cmd/clyth/main.clyth}"
STAGE2="${CLYTH_STAGE2_OUT:-$ROOT/dist/clyth/clyth-stage2}"
BOOTSTRAP="$ROOT/dist/bootstrap/clyth"

[[ -x "$STAGE1" ]] || { printf 'error: Stage-1 compiler not built: %s\n' "$STAGE1" >&2; exit 1; }
[[ "$STAGE1" != "$BOOTSTRAP" ]] || { printf 'error: Stage-2 must be built by independent Stage-1, not Stage-0\n' >&2; exit 1; }
mkdir -p "$(dirname "$STAGE2")"
rm -f "$STAGE2"

printf 'Stage-1 compiler: %s\n' "$STAGE1"
printf 'Stage-2 output:   %s\n' "$STAGE2"
CLYTH_KEEP_TEMP=1 CLYTH_STAGE2_TRACE=1 "$STAGE1" build "$SOURCE" -o "$STAGE2" --mecc=true --verbose
[[ -x "$STAGE2" ]] || { printf 'error: Stage-1 did not produce executable Stage-2: %s\n' "$STAGE2" >&2; exit 1; }

printf '\n==> GNU ABI audit (Stage-2)\n'
"$ROOT/scripts/audit_no_gnu_abi.sh" "$STAGE2"

printf '\n==> Independent Stage-2 regression suite\n'
CLYTH_TEST_COMPILER="$STAGE2" "$ROOT/scripts/run-regression-tests.sh"

printf '\n==> Independent Stage-2 full native backend smoke\n'
SMOKE_IR="$(mktemp "${TMPDIR:-/tmp}/clyth-stage2-native-backend.XXXXXX.ll")"
trap 'rm -f "$SMOKE_IR"' EXIT
"$STAGE2" build "$ROOT/compiler/clyth/stage1-tests/native_backend.clyth" --emit-llvm-ir -o "$SMOKE_IR"
grep -q '^target triple = ' "$SMOKE_IR" || { printf 'error: Stage-2 native backend smoke did not emit LLVM IR\n' >&2; exit 1; }
rm -f "$SMOKE_IR"
trap - EXIT
printf '\nStage-1 -> Stage-2 validation passed.\n'
