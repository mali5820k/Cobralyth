#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SEED="${CLYTH_BOOTSTRAP_BIN:-$ROOT/dist/bootstrap/clyth}"
SOURCE="${CLYTH_COMPILER_SOURCE:-$ROOT/compiler/clyth/bootstrap-v1/cmd/clyth/main.clyth}"
OUT="${CLYTH_SELFHOST_OUT:-$ROOT/dist/clyth/clyth}"
[[ -x "$SEED" ]] || { printf 'error: Stage-0 seed compiler not built: %s\n' "$SEED" >&2; exit 1; }
mkdir -p "$(dirname "$OUT")"

# Stage 0 is used exactly once here as the seed that materializes Stage 1.
# The resulting Stage-1 executable contains and runs its own parser, semantic
# analyzer, LLVM lowering, toolchain resolution, object compilation and linker
# pipeline; it has no runtime path back to the seed compiler.
stage1_args=()
if declare -p CLYTH_STAGE1_BUILD_ARGS >/dev/null 2>&1; then
    stage1_args=("${CLYTH_STAGE1_BUILD_ARGS[@]}")
fi
"$SEED" build "$SOURCE" -o "$OUT" --mecc=true "${stage1_args[@]}"

# Keep the materialization gate deliberately smaller than the canonical corpus.
# native_backend.clyth exercises file-io and other surfaces that belong to the
# Stage-1 regression/final-smoke gates.  Blocking here would prevent the runner
# from reporting the complete independent Stage-1 failure surface.
SMOKE_SOURCE="${CLYTH_STAGE1_BOOTSTRAP_SMOKE_SOURCE:-$ROOT/sample-clyth-programs/01_0_1_printf_extern_c.clyth}"
SMOKE_IR="$(mktemp "${TMPDIR:-/tmp}/clyth-stage1-bootstrap-smoke.XXXXXX.ll")"
trap 'rm -f "$SMOKE_IR"' EXIT
"$OUT" build "$SMOKE_SOURCE" --emit-llvm-ir -o "$SMOKE_IR" >/dev/null
grep -q '^target triple = ' "$SMOKE_IR" || { printf 'error: Stage-1 bootstrap smoke did not emit LLVM IR\n' >&2; exit 1; }

printf 'Built independent Stage-1 compiler: %s\n' "$OUT"
printf 'Independent Stage-1 bootstrap frontend/LLVM smoke passed: %s\n' "$SMOKE_SOURCE"
