#!/usr/bin/env bash
# Build all native dependencies required by the trusted Stage 0 compiler.
# Every compiled dependency is built through the repository Zig wrappers.
set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
STAGE0_DIR="$PROJECT_ROOT/bootstrap/0.5.0-compiler"
DEPS_DIR="$STAGE0_DIR/build-dependencies"
ZIG_CC="$SCRIPT_DIR/zig-c.sh"
ZIG_CXX="$SCRIPT_DIR/zig-c++.sh"
JOBS="${CLYTH_BUILD_JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)}"

LLVM_VERSION="${LLVM_VERSION:-19.1.7}"
LLVM_TARGETS_TO_BUILD="${LLVM_TARGETS_TO_BUILD:-X86;AArch64}"
LLVM_GIT_URL="${LLVM_GIT_URL:-https://github.com/llvm/llvm-project.git}"
CLYTH_ZIG_TARGET="${CLYTH_ZIG_TARGET:-native-linux-musl}"
CLYTH_ZIG_STATIC="${CLYTH_ZIG_STATIC:-1}"
CLYTH_ZIG_CXX_STDLIB="${CLYTH_ZIG_CXX_STDLIB:-libc++}"
export CLYTH_ZIG_TARGET CLYTH_ZIG_STATIC CLYTH_ZIG_CXX_STDLIB

case "$(uname -m)" in
  x86_64|amd64) DEFAULT_LLVM_TRIPLE=x86_64-unknown-linux-musl ;;
  aarch64|arm64) DEFAULT_LLVM_TRIPLE=aarch64-unknown-linux-musl ;;
  *) DEFAULT_LLVM_TRIPLE="$(uname -m)-unknown-linux-musl" ;;
esac
CLYTH_LLVM_TARGET_TRIPLE="${CLYTH_LLVM_TARGET_TRIPLE:-$DEFAULT_LLVM_TRIPLE}"
export CLYTH_LLVM_TARGET_TRIPLE

ANTLR_SOURCE="$DEPS_DIR/antlr4-cpp-runtime-src"
ANTLR_BUILD="$STAGE0_DIR/.build/antlr4-runtime"
ANTLR_DESTDIR="$DEPS_DIR/antlr4-cpp-runtime-lib"
ANTLR_PREFIX="$ANTLR_DESTDIR/usr/local"
FMT_SOURCE="$DEPS_DIR/fmt"
LLVM_SOURCE="$DEPS_DIR/llvm-project-src"
LLVM_BUILD="$STAGE0_DIR/.build/llvm"
LLVM_INSTALL="$DEPS_DIR/llvm-bundled"

if [[ -n "${CMAKE_GENERATOR:-}" ]]; then
  GENERATOR_ARGS=(-G "$CMAKE_GENERATOR")
elif command -v ninja >/dev/null 2>&1; then
  GENERATOR_ARGS=(-G Ninja)
else
  GENERATOR_ARGS=()
fi

for tool in cmake git zig; do command -v "$tool" >/dev/null 2>&1 || { printf 'missing required build tool: %s\n' "$tool" >&2; exit 1; }; done
[[ -x "$ZIG_CC" && -x "$ZIG_CXX" ]] || { printf 'missing executable Zig wrappers under %s\n' "$SCRIPT_DIR" >&2; exit 1; }

if [[ ! -f "$FMT_SOURCE/CMakeLists.txt" ]]; then
  rm -rf "$FMT_SOURCE"
  git clone --depth 1 https://github.com/fmtlib/fmt.git "$FMT_SOURCE"
fi

build_antlr() {
  [[ -f "$ANTLR_SOURCE/CMakeLists.txt" ]] || { printf 'missing ANTLR runtime source: %s\n' "$ANTLR_SOURCE" >&2; exit 1; }
  if [[ -f "$ANTLR_PREFIX/lib/libantlr4-runtime.a" || -f "$ANTLR_PREFIX/lib64/libantlr4-runtime.a" ]]; then
    printf 'ANTLR4 runtime already built: %s\n' "$ANTLR_PREFIX"
    return
  fi
  rm -rf "$ANTLR_BUILD" "$ANTLR_DESTDIR"
  mkdir -p "$ANTLR_BUILD" "$ANTLR_DESTDIR"
  cmake -S "$ANTLR_SOURCE" -B "$ANTLR_BUILD" "${GENERATOR_ARGS[@]}" \
    -DCMAKE_C_COMPILER="$ZIG_CC" \
    -DCMAKE_CXX_COMPILER="$ZIG_CXX" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
    -DANTLR_BUILD_CPP_TESTS=OFF \
    -DANTLR_BUILD_SHARED=OFF \
    -DANTLR_BUILD_STATIC=ON \
    -DBUILD_SHARED_LIBS=OFF \
    -DWITH_DEMO=OFF
  cmake --build "$ANTLR_BUILD" --parallel "$JOBS"
  DESTDIR="$ANTLR_DESTDIR" cmake --install "$ANTLR_BUILD"
  printf 'Built ANTLR4 C++ runtime: %s\n' "$ANTLR_PREFIX"
}

clone_llvm() {
  if [[ ! -d "$LLVM_SOURCE/.git" ]]; then
    rm -rf "$LLVM_SOURCE"
    git clone --depth 1 --branch "llvmorg-$LLVM_VERSION" "$LLVM_GIT_URL" "$LLVM_SOURCE"
  else
    git -C "$LLVM_SOURCE" fetch --depth 1 origin "llvmorg-$LLVM_VERSION"
    git -C "$LLVM_SOURCE" checkout "llvmorg-$LLVM_VERSION"
  fi
}

build_llvm() {
  if [[ -x "$LLVM_INSTALL/bin/llvm-config" && -x "$LLVM_INSTALL/bin/llc" && -x "$LLVM_INSTALL/bin/lld" && -d "$LLVM_INSTALL/lib/cmake/llvm" ]]; then
    printf 'Bundled LLVM already built: %s\n' "$LLVM_INSTALL"
    return
  fi
  clone_llvm
  rm -rf "$LLVM_BUILD" "$LLVM_INSTALL"
  mkdir -p "$LLVM_BUILD" "$LLVM_INSTALL"
  cmake -S "$LLVM_SOURCE/llvm" -B "$LLVM_BUILD" "${GENERATOR_ARGS[@]}" \
    -DCMAKE_C_COMPILER="$ZIG_CC" \
    -DCMAKE_CXX_COMPILER="$ZIG_CXX" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$LLVM_INSTALL" \
    -DCMAKE_C_FLAGS="-target $CLYTH_ZIG_TARGET" \
    -DCMAKE_CXX_FLAGS="-target $CLYTH_ZIG_TARGET -stdlib=$CLYTH_ZIG_CXX_STDLIB" \
    -DCMAKE_EXE_LINKER_FLAGS="-target $CLYTH_ZIG_TARGET -stdlib=$CLYTH_ZIG_CXX_STDLIB -static" \
    -DCMAKE_SHARED_LINKER_FLAGS="-target $CLYTH_ZIG_TARGET -stdlib=$CLYTH_ZIG_CXX_STDLIB" \
    -DCMAKE_MODULE_LINKER_FLAGS="-target $CLYTH_ZIG_TARGET -stdlib=$CLYTH_ZIG_CXX_STDLIB" \
    -DLLVM_ENABLE_PROJECTS=lld \
    -DLLVM_ENABLE_RUNTIMES= \
    -DLLVM_TARGETS_TO_BUILD="$LLVM_TARGETS_TO_BUILD" \
    -DLLVM_DEFAULT_TARGET_TRIPLE="$CLYTH_LLVM_TARGET_TRIPLE" \
    -DLLVM_HOST_TRIPLE="$CLYTH_LLVM_TARGET_TRIPLE" \
    -DLLVM_BUILD_LLVM_DYLIB=OFF \
    -DLLVM_LINK_LLVM_DYLIB=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -DLLVM_BUILD_TOOLS=ON \
    -DLLVM_BUILD_UTILS=ON \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DLLVM_INCLUDE_EXAMPLES=OFF \
    -DLLVM_INCLUDE_BENCHMARKS=OFF \
    -DLLVM_INCLUDE_DOCS=OFF \
    -DLLVM_ENABLE_TERMINFO=OFF \
    -DLLVM_ENABLE_LIBXML2=OFF \
    -DLLVM_ENABLE_ZLIB=OFF \
    -DLLVM_ENABLE_ZSTD=OFF \
    -DLLVM_ENABLE_LIBEDIT=OFF \
    -DLLVM_ENABLE_LIBPFM=OFF \
    -DLLVM_ENABLE_Z3_SOLVER=OFF \
    -DLLVM_ENABLE_FFI=OFF \
    -DLLVM_ENABLE_LIBCXX=ON \
    -DLLVM_STATIC_LINK_CXX_STDLIB=ON \
    -DLLVM_ENABLE_LTO=OFF \
    -DLLVM_ENABLE_RTTI=ON \
    -DLLVM_ENABLE_EH=ON \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON
  cmake --build "$LLVM_BUILD" --parallel "$JOBS"
  cmake --install "$LLVM_BUILD"
  printf 'Built bundled LLVM+LLD: %s\n' "$LLVM_INSTALL"
}

build_antlr
build_llvm
