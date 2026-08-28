#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
VERSION="${CLYTH_LLVM_VERSION:-20.1.8}"
TAG="llvmorg-${VERSION}"
SOURCE="${CLYTH_LLVM_SOURCE:-$ROOT/third_party/llvm-project}"

if [[ -d "$SOURCE/llvm" && -f "$SOURCE/llvm/CMakeLists.txt" ]]; then
  printf 'Using existing LLVM source: %s\n' "$SOURCE"
  exit 0
fi
command -v git >/dev/null || { printf 'error: git is required to fetch LLVM\n' >&2; exit 1; }
mkdir -p "$(dirname "$SOURCE")"
printf 'Fetching LLVM %s into %s\n' "$VERSION" "$SOURCE"
git clone --depth 1 --branch "$TAG" https://github.com/llvm/llvm-project.git "$SOURCE"
