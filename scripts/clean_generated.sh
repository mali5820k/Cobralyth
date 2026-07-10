#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

rm -rf \
  build-compiler \
  llvm-build \
  llvm-bundled \
  antlr4-cpp-runtime-lib \
  antlr4-cpp-runtime-src/build \
  dist \
  dist-clyth \
  sample-clyth-programs/.sample-bin

find clyth-runtime -type d \( -name x86_64 -o -name aarch64 \) -prune -exec rm -rf {} + 2>/dev/null || true
find . -type f \( -name '*.o' -o -name '*.obj' -o -name '*.ll' -o -name '*.log' -o -name '*.tmp' \) -delete
find . -maxdepth 2 -type f \( -name '*.tar' -o -name '*.tar.gz' -o -name '*.tgz' -o -name '*.zip' \) -delete

echo "[ OK ] Removed generated Clyth build artifacts."
