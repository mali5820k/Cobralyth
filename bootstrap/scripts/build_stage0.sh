#!/usr/bin/env bash
# Build the trusted 0.5.0 seed compiler from its C++ source tree.
#
# Stage 0 is the only non-self-hosted compiler generation. Its output is placed
# in bootstrap/0.5.0-compiler/dist and is then consumed by the Stage 1 build.
set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
STAGE0_DIR="$(cd "$SCRIPT_DIR/../0.5.0-compiler" && pwd)"
SOURCE_DIR="$STAGE0_DIR/src"
BUILD_DIR="$STAGE0_DIR/.build/compiler"
DIST_DIR="$STAGE0_DIR/dist"
COMPILER_OUT="$BUILD_DIR/clyth_compiler_bin"
ZIG_CC="$SCRIPT_DIR/zig-c.sh"
ZIG_CXX="$SCRIPT_DIR/zig-c++.sh"
LLVM_INSTALL="$STAGE0_DIR/build-dependencies/llvm-bundled"
LLVM_DIR="$LLVM_INSTALL/lib/cmake/llvm"

# Build ANTLR before configuring the compiler because Stage 0 links against the
# installed static runtime discovered by its CMake configuration.
"$SCRIPT_DIR/build_dependencies.sh"

command -v cmake >/dev/null 2>&1 || {
  printf 'missing required build tool: cmake\n' >&2
  exit 1
}
[[ -f "$SOURCE_DIR/CMakeLists.txt" ]] || {
  printf 'missing Stage 0 CMake project: %s\n' "$SOURCE_DIR" >&2
  exit 1
}

if [[ -n "${CMAKE_GENERATOR:-}" ]]; then
  GENERATOR_ARGS=(-G "$CMAKE_GENERATOR")
elif command -v ninja >/dev/null 2>&1; then
  GENERATOR_ARGS=(-G Ninja)
else
  GENERATOR_ARGS=()
fi

# dist is generated state and may not exist after cleanup or in a fresh clone.
[[ -d "$LLVM_DIR" && -x "$LLVM_INSTALL/bin/llvm-config" ]] || {
  printf 'bundled Zig/musl LLVM is missing or incomplete: %s\n' "$LLVM_INSTALL" >&2
  exit 1
}

mkdir -p "$BUILD_DIR" "$DIST_DIR/bin"
cmake -S "$SOURCE_DIR" -B "$BUILD_DIR" \
  "${GENERATOR_ARGS[@]}" \
  -DCMAKE_C_COMPILER="$ZIG_CC" \
  -DCMAKE_CXX_COMPILER="$ZIG_CXX" \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_DIR="$LLVM_DIR" \
  -DCMAKE_PREFIX_PATH="$LLVM_INSTALL"
cmake --build "$BUILD_DIR" --parallel "${CLYTH_BUILD_JOBS:-$(nproc)}"

[[ -x "$COMPILER_OUT" ]] || {
  printf 'Stage 0 build completed without producing: %s\n' "$COMPILER_OUT" >&2
  exit 1
}
install -m 0755 "$COMPILER_OUT" "$DIST_DIR/bin/clyth_compiler_bin"

# Stage 0 links against the C++ frontend/toolchain dependencies documented by
# the source-owned manifest. Ship that manifest beside the compiler so the
# seed distribution is independently license-complete.
mkdir -p "$DIST_DIR/share/clyth"
cp "$SOURCE_DIR/EXTERNAL_LIBRARIES_LICENSES.md" \
  "$DIST_DIR/share/clyth/EXTERNAL_LIBRARIES_LICENSES.md"

"$SCRIPT_DIR/audit_no_gnu_abi.sh" "$DIST_DIR"

printf 'Built Stage 0 compiler: %s\n' "$DIST_DIR/bin/clyth_compiler_bin"
