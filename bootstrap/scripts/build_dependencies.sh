#!/usr/bin/env bash
# Build the native dependencies required by the trusted Stage 0 compiler.
#
# At present, fmt and cxxopts are consumed directly from their vendored source
# trees. The ANTLR4 C++ runtime is the only dependency that must be compiled and
# installed before Stage 0's CMake project can link successfully.
set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
STAGE0_DIR="$(cd "$SCRIPT_DIR/../0.5.0-compiler" && pwd)"
DEPS_DIR="$STAGE0_DIR/build-dependencies"
ANTLR_SOURCE="$DEPS_DIR/antlr4-cpp-runtime-src"
ANTLR_BUILD="$STAGE0_DIR/.build/antlr4-runtime"
ANTLR_INSTALL="$DEPS_DIR/antlr4-cpp-runtime-lib/usr/local"

# Permit callers to select a generator while preferring Ninja when available.
if [[ -n "${CMAKE_GENERATOR:-}" ]]; then
  GENERATOR_ARGS=(-G "$CMAKE_GENERATOR")
elif command -v ninja >/dev/null 2>&1; then
  GENERATOR_ARGS=(-G Ninja)
else
  GENERATOR_ARGS=()
fi

command -v cmake >/dev/null 2>&1 || {
  printf 'missing required build tool: cmake\n' >&2
  exit 1
}
[[ -f "$ANTLR_SOURCE/CMakeLists.txt" ]] || {
  printf 'missing vendored ANTLR4 runtime source: %s\n' "$ANTLR_SOURCE" >&2
  exit 1
}

# Reconfiguration is intentionally idempotent. CMake updates an existing build
# tree, while a clean checkout creates it from scratch.
mkdir -p "$ANTLR_BUILD" "$ANTLR_INSTALL"
cmake -S "$ANTLR_SOURCE" -B "$ANTLR_BUILD" \
  "${GENERATOR_ARGS[@]}" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="$ANTLR_INSTALL" \
  -DANTLR_BUILD_CPP_TESTS=OFF \
  -DANTLR_BUILD_SHARED=OFF \
  -DANTLR_BUILD_STATIC=ON \
  -DANTLR4_INSTALL=ON \
  -DWITH_DEMO=OFF
cmake --build "$ANTLR_BUILD" --target install --parallel "${CLYTH_BUILD_JOBS:-$(nproc)}"

printf 'Built ANTLR4 C++ runtime: %s\n' "$ANTLR_INSTALL"
