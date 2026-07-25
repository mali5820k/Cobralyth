#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
STAGE1_SCRIPT="$ROOT/0.6.0-stage1-compiler/build_stage1.sh"
STAGE2_SCRIPT="$ROOT/0.6.0-stage2-compiler/build_stage2.sh"

section() {
  printf '\n==================================================\n%s\n==================================================\n' "$1"
}

[[ -x "$STAGE1_SCRIPT" ]] || { printf 'missing Stage 1 build script: %s\n' "$STAGE1_SCRIPT" >&2; exit 1; }
[[ -x "$STAGE2_SCRIPT" ]] || { printf 'missing Stage 2 build script: %s\n' "$STAGE2_SCRIPT" >&2; exit 1; }

section 'Bootstrapping Stage 1 Compiler'
"$STAGE1_SCRIPT"

section 'Building Stage 2 Compiler'
"$STAGE2_SCRIPT"

section 'Clyth 0.6.0 Bootstrap Complete'
printf 'Final distribution: %s\n' "$ROOT/0.6.0-compiler-dist"
printf 'Release tarball:    %s\n' "$ROOT/clyth-0.6.0-compiler-dist.tar.gz"
