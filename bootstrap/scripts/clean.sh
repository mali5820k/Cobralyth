#!/usr/bin/env bash
# Remove every reproducible project-generated build, test, and distribution
# artifact. A subsequent build recreates all required directories automatically.
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
STAGE0="$ROOT/bootstrap/0.5.0-compiler"
STAGE1="$ROOT/bootstrap/0.5.1-compiler"
STAGE2="$ROOT/0.6.0-compiler"

rm -rf \
  "$STAGE0/.build" \
  "$STAGE0/dist" \
  "$STAGE0/build-dependencies/antlr4-cpp-runtime-lib" \
  "$STAGE1/.build" \
  "$STAGE1/.bootstrap-build" \
  "$STAGE1/.regression-build" \
  "$STAGE1/clyth-runtime/build" \
  "$STAGE1/module-store" \
  "$STAGE1/dist" \
  "$STAGE2/.build" \
  "$STAGE2/.regression-build" \
  "$STAGE2/dist" \
  "$ROOT/build" \
  "$ROOT/clyth-0.6.0-compiler-dist.tar.gz"

# Runtime native objects and archives are outputs, even when placed beside the
# source files for module discovery. Remove them so rebuilds cannot reuse stale
# machine code from another toolchain or host.
find "$STAGE0/runtime" "$STAGE1/clyth-runtime" \
  -type f \( -name '*.o' -o -name '*.a' -o -name '*.so' -o -name '*.ll' \) \
  -delete 2>/dev/null || true

printf 'Removed generated bootstrap, dependency, runtime, test, and distribution outputs.\n'
