#!/usr/bin/env bash
set -Eeuo pipefail

# Clyth build script v2
# - Linux package-manager agnostic dependency checks
# - Uses zig-c.sh / zig-c++.sh wrappers
# - Builds fmt, ANTLR4 C++ runtime, bundled LLVM+LLD, and Clyth
#
# Usage:
#   ./build_compiler.sh
#   ./build_compiler.sh --skip-llvm
#   ./build_compiler.sh --rebuild-llvm
#   ./build_compiler.sh --rebuild-antlr
#   ./build_compiler.sh --rebuild-all
#
# Env overrides:
#   LLVM_VERSION=19.1.7 LLVM_TARGETS_TO_BUILD="X86;AArch64" JOBS=12 ./build_compiler.sh

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

LLVM_VERSION="${LLVM_VERSION:-19.1.7}"
LLVM_TARGETS_TO_BUILD="${LLVM_TARGETS_TO_BUILD:-X86;AArch64}"
LLVM_GIT_URL="${LLVM_GIT_URL:-https://github.com/llvm/llvm-project.git}"
JOBS="${JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4)}"

ZIG_CC="${ROOT_DIR}/zig-c.sh"
ZIG_CXX="${ROOT_DIR}/zig-c++.sh"

ANTLR_SRC="${ROOT_DIR}/antlr4-cpp-runtime-src"
ANTLR_INSTALL="${ROOT_DIR}/antlr4-cpp-runtime-lib"
FMT_SRC="${ROOT_DIR}/compiler-src/fmt"
LLVM_SRC="${ROOT_DIR}/llvm-project-src"
LLVM_BUILD="${ROOT_DIR}/llvm-build"
LLVM_INSTALL="${ROOT_DIR}/llvm-bundled"
CLYTH_SRC="${ROOT_DIR}/compiler-src"
CLYTH_BUILD="${ROOT_DIR}/build-compiler"
CLYTH_GRAMMAR_DIRECTORY="${ROOT_DIR}/Grammar-and-antlr4-files/"
CLYTH_GRAMMAR_FILE="ClythV1.g4"
CLYTH_RUNTIME_DIR="${ROOT_DIR}/clyth-runtime"
CLYTH_RUNTIME_C_BINDINGS="${CLYTH_RUNTIME_DIR}/c-bindings"

BUILD_LLVM=1
REBUILD_LLVM=0
REBUILD_ANTLR=0
REBUILD_ALL=0
PACKAGE_DIST=1
DIST_DIR="${ROOT_DIR}/dist-clyth"

info(){ printf "\033[1;34m[INFO]\033[0m %s\n" "$*"; }
ok(){ printf "\033[1;32m[ OK ]\033[0m %s\n" "$*"; }
warn(){ printf "\033[1;33m[WARN]\033[0m %s\n" "$*" >&2; }
err(){ printf "\033[1;31m[ERR ]\033[0m %s\n" "$*" >&2; }
die(){ err "$*"; exit 1; }
trap 'err "Build failed near line $LINENO. Scroll up for the first real compiler/CMake error."' ERR

usage(){ cat <<USAGE
Usage: ./build_compiler.sh [options]
  --skip-llvm       Do not build bundled LLVM; use system LLVM via CMake.
  --rebuild-llvm    Delete llvm-build/llvm-bundled and rebuild LLVM.
  --rebuild-antlr   Delete antlr4-cpp-runtime-lib and rebuild ANTLR runtime.
  --rebuild-all     Rebuild ANTLR runtime, LLVM, and Clyth compiler.
  --no-package      Skip creating dist-clyth distribution directory.
  --skip-dist       Alias for --no-package; skip dist-clyth packaging for fast dev builds.
  -h, --help        Show this help.
USAGE
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --skip-llvm) BUILD_LLVM=0 ;;
    --rebuild-llvm) REBUILD_LLVM=1 ;;
    --rebuild-antlr) REBUILD_ANTLR=1 ;;
    --rebuild-all) REBUILD_ALL=1; REBUILD_LLVM=1; REBUILD_ANTLR=1 ;;
    --no-package|--skip-dist) PACKAGE_DIST=0 ;;
    -h|--help) usage; exit 0 ;;
    *) die "Unknown option: $1" ;;
  esac
  shift
done

pm(){
  command -v apt-get >/dev/null && { echo apt; return; }
  command -v dnf >/dev/null && { echo dnf; return; }
  command -v yum >/dev/null && { echo yum; return; }
  command -v pacman >/dev/null && { echo pacman; return; }
  command -v zypper >/dev/null && { echo zypper; return; }
  command -v apk >/dev/null && { echo apk; return; }
  echo unknown
}

suggest(){
  local p="$1"
  printf "\nSuggested packages for %s:\n\n" "$p"
  case "$p" in
    apt) cat <<'TXT'
sudo apt update
sudo apt install -y git cmake ninja-build make python3 python3-pip default-jre curl ca-certificates tar xz-utils unzip pkg-config
Install Zig from https://ziglang.org/download/ if your distro package is unavailable/outdated.
TXT
      ;;
    dnf) cat <<'TXT'
sudo dnf install -y git cmake ninja-build make python3 python3-pip java-latest-openjdk curl ca-certificates tar xz unzip pkgconf-pkg-config
Install Zig from https://ziglang.org/download/ if your distro package is unavailable/outdated.
TXT
      ;;
    pacman) cat <<'TXT'
sudo pacman -S --needed git cmake ninja make python python-pip jdk-openjdk curl ca-certificates tar xz unzip pkgconf zig
TXT
      ;;
    zypper) cat <<'TXT'
sudo zypper install git cmake ninja make python3 python3-pip java curl ca-certificates tar xz unzip pkg-config
Install Zig from https://ziglang.org/download/ if your distro package is unavailable/outdated.
TXT
      ;;
    apk) cat <<'TXT'
sudo apk add git cmake ninja make python3 py3-pip openjdk17 curl ca-certificates tar xz unzip pkgconf
Install Zig from https://ziglang.org/download/ if your distro package is unavailable/outdated.
TXT
      ;;
    *) cat <<'TXT'
Required commands: git cmake ninja-or-make python3 pip java curl tar unzip zig
Python package: python3 -m pip install --user antlr4-tools
TXT
      ;;
  esac
  printf "\nANTLR tool if missing:\npython3 -m pip install --user antlr4-tools\n\n"
}

need_cmd(){
  local c="$1"; local label="${2:-$1}"
  if command -v "$c" >/dev/null 2>&1; then ok "Found $label: $(command -v "$c")"; return 0; fi
  err "Missing $label ($c)"; return 1
}

check_deps(){
  info "Checking host dependencies..."
  local missing=0
  need_cmd git || missing=1
  need_cmd cmake || missing=1
  need_cmd python3 || missing=1
  need_cmd java "Java runtime for ANTLR" || missing=1
  need_cmd curl || missing=1
  need_cmd tar || missing=1
  need_cmd unzip || missing=1
  need_cmd zig || missing=1
  if command -v ninja >/dev/null 2>&1; then ok "Found ninja"; elif command -v make >/dev/null 2>&1; then ok "Found make"; else err "Missing ninja or make"; missing=1; fi
  [[ -x "$ZIG_CC" ]] && ok "Found zig C wrapper: $ZIG_CC" || { err "Missing/executable bit not set: $ZIG_CC"; missing=1; }
  [[ -x "$ZIG_CXX" ]] && ok "Found zig C++ wrapper: $ZIG_CXX" || { err "Missing/executable bit not set: $ZIG_CXX"; missing=1; }
  if command -v antlr4 >/dev/null 2>&1; then ok "Found antlr4 CLI"; elif python3 -m pip show antlr4-tools >/dev/null 2>&1; then ok "Found antlr4-tools Python package"; else err "Missing antlr4 / antlr4-tools"; missing=1; fi
  [[ -d "$ANTLR_SRC" ]] && ok "Found ANTLR runtime source: $ANTLR_SRC" || { err "Missing $ANTLR_SRC"; missing=1; }
  if [[ "$missing" -ne 0 ]]; then local p; p="$(pm)"; err "Missing dependencies."; info "Detected package manager: $p"; suggest "$p"; exit 1; fi
}

gen_args(){ if command -v ninja >/dev/null 2>&1; then printf '%s\n' -G Ninja; else printf '%s\n' -G "Unix Makefiles"; fi; }
cmake_build(){ cmake --build "$1" --parallel "$JOBS"; }

setup_antlr(){
  local v="${ANTLR_VERSION:-4.13.2}"
  if command -v antlr4 >/dev/null 2>&1; then
    info "Preparing ANTLR tool version $v..."
    antlr4 -v "$v" >/dev/null 2>&1 || warn "Could not force antlr4 version $v; continuing with installed antlr4."
  fi
}

ensure_fmt(){
  if [[ ! -d "$FMT_SRC/.git" ]]; then
    info "Cloning fmt..."
    rm -rf "$FMT_SRC"
    git clone --depth 1 https://github.com/fmtlib/fmt.git "$FMT_SRC"
  else ok "fmt already present"; fi
}

build_antlr(){
  [[ "$REBUILD_ALL" -eq 1 || "$REBUILD_ANTLR" -eq 1 ]] && rm -rf "$ANTLR_INSTALL"
  if [[ -d "$ANTLR_INSTALL/usr/local/include" ]]; then ok "ANTLR runtime already built"; return; fi
  info "Building ANTLR4 C++ runtime..."
  local b="$ANTLR_SRC/build"
  rm -rf "$ANTLR_INSTALL" "$b"; mkdir -p "$ANTLR_INSTALL" "$b"
  mapfile -t g < <(gen_args)
  cmake -S "$ANTLR_SRC" -B "$b" "${g[@]}" \
    -DCMAKE_C_COMPILER="$ZIG_CC" \
    -DCMAKE_CXX_COMPILER="$ZIG_CXX" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON
  cmake_build "$b"
  DESTDIR="$ANTLR_INSTALL" cmake --install "$b"
  ok "ANTLR4 runtime installed to $ANTLR_INSTALL"
}

clone_llvm(){
  if [[ ! -d "$LLVM_SRC/.git" ]]; then
    info "Cloning LLVM llvmorg-$LLVM_VERSION..."
    rm -rf "$LLVM_SRC"
    git clone --depth 1 --branch "llvmorg-$LLVM_VERSION" "$LLVM_GIT_URL" "$LLVM_SRC"
  else
    info "Updating LLVM source to llvmorg-$LLVM_VERSION..."
    git -C "$LLVM_SRC" fetch --depth 1 origin "llvmorg-$LLVM_VERSION"
    git -C "$LLVM_SRC" checkout "llvmorg-$LLVM_VERSION"
  fi
}

build_llvm(){
  [[ "$BUILD_LLVM" -eq 0 ]] && { warn "Skipping LLVM bundle"; return; }
  [[ "$REBUILD_ALL" -eq 1 || "$REBUILD_LLVM" -eq 1 ]] && rm -rf "$LLVM_BUILD" "$LLVM_INSTALL"
  if [[ -x "$LLVM_INSTALL/bin/llvm-config" && -x "$LLVM_INSTALL/bin/lld" ]]; then
    ok "Bundled LLVM already built: $LLVM_INSTALL"
    "$LLVM_INSTALL/bin/llvm-config" --version || true
    return
  fi
  clone_llvm
  info "Configuring LLVM+LLD bundle..."
  rm -rf "$LLVM_BUILD"; mkdir -p "$LLVM_BUILD"
  mapfile -t g < <(gen_args)
  cmake -S "$LLVM_SRC/llvm" -B "$LLVM_BUILD" "${g[@]}" \
    -DCMAKE_C_COMPILER="$ZIG_CC" \
    -DCMAKE_CXX_COMPILER="$ZIG_CXX" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$LLVM_INSTALL" \
    -DLLVM_ENABLE_PROJECTS="lld" \
    -DLLVM_ENABLE_RUNTIMES="" \
    -DLLVM_TARGETS_TO_BUILD="$LLVM_TARGETS_TO_BUILD" \
    -DLLVM_BUILD_LLVM_DYLIB=OFF \
    -DLLVM_LINK_LLVM_DYLIB=OFF \
    -DLLVM_BUILD_TOOLS=ON \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DLLVM_INCLUDE_EXAMPLES=OFF \
    -DLLVM_INCLUDE_BENCHMARKS=OFF \
    -DLLVM_ENABLE_TERMINFO=OFF \
    -DLLVM_ENABLE_LIBXML2=OFF \
    -DLLVM_ENABLE_ZLIB=OFF \
    -DLLVM_ENABLE_ZSTD=OFF \
    -DLLVM_ENABLE_RTTI=ON \
    -DLLVM_ENABLE_EH=ON \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON
  info "Building LLVM. This can take a while..."
  cmake_build "$LLVM_BUILD"
  cmake --install "$LLVM_BUILD"
  ok "LLVM installed to $LLVM_INSTALL"
}


runtime_arch_name(){
  case "$(uname -m)" in
    x86_64|amd64) echo "x86_64" ;;
    aarch64|arm64) echo "arm64" ;;
    armv7*|armhf) echo "armv7" ;;
    riscv64) echo "riscv64" ;;
    *) uname -m ;;
  esac
}

build_one_runtime_c_binding(){
  local binding_name="$1"
  local source_dir="$2"
  local source_file="$3"
  local archive_name="$4"
  shift 4
  local module_dirs=("$@")

  local source_path="$source_dir/$source_file"
  if [[ ! -f "$source_path" ]]; then
    warn "Runtime C binding source not found: $source_path"
    return
  fi

  local arch
  arch="$(runtime_arch_name)"
  local binding_arch_dir="$source_dir/$arch"
  local object_path="$binding_arch_dir/${binding_name}.o"
  local archive_path="$binding_arch_dir/$archive_name"
  mkdir -p "$binding_arch_dir"

  "$ZIG_CC" -O2 -c "$source_path" -o "$object_path"
  zig ar rcs "$archive_path" "$object_path"
  ok "Built runtime C binding object: $object_path"
  ok "Built runtime C binding archive: $archive_path"

  local module_dir
  for module_dir in "${module_dirs[@]}"; do
    mkdir -p "$module_dir"
    cp "$archive_path" "$module_dir/$archive_name"
    ok "Copied runtime module archive: $module_dir/$archive_name"
  done
}

ensure_rapidhash_source(){
  local vendor_dir="$CLYTH_RUNTIME_C_BINDINGS/rapidhash/vendor/rapidhash"
  local header="$vendor_dir/rapidhash.h"
  if [[ -f "$header" ]]; then
    ok "rapidhash v4 source already present: $vendor_dir"
    return
  fi
  info "Fetching upstream rapidhash v4 source..."
  rm -rf "$vendor_dir"
  mkdir -p "$(dirname "$vendor_dir")"
  git clone --depth 1 --branch v4 https://github.com/Nicoshev/rapidhash.git "$vendor_dir"
  [[ -f "$header" ]] || die "rapidhash.h was not found after cloning upstream rapidhash v4."
}

build_runtime_c_bindings(){
  info "Building Clyth runtime C binding archives..."
  local arch
  arch="$(runtime_arch_name)"

  build_one_runtime_c_binding "dma" \
    "$CLYTH_RUNTIME_C_BINDINGS/dma" "dma.c" "libclyth_dma.a" \
    "$CLYTH_RUNTIME_DIR/modules/module-dma/$arch"

  build_one_runtime_c_binding "file_io" \
    "$CLYTH_RUNTIME_C_BINDINGS/file-io" "file.c" "libclyth_file_io.a" \
    "$CLYTH_RUNTIME_DIR/modules/module-file-io/$arch"

  ensure_rapidhash_source
  build_one_runtime_c_binding "rapidhash" \
    "$CLYTH_RUNTIME_C_BINDINGS/rapidhash" "rapidhash_bindings.c" "libclyth_rapidhash.a" \
    "$CLYTH_RUNTIME_DIR/modules/module-hash/$arch"

  build_one_runtime_c_binding "http" \
    "$CLYTH_RUNTIME_C_BINDINGS/http" "web.c" "libclyth_http.a" \
    "$CLYTH_RUNTIME_DIR/modules/module-router/$arch" \
    "$CLYTH_RUNTIME_DIR/modules/module-http/$arch" \
    "$CLYTH_RUNTIME_DIR/modules/module-https/$arch"
}

package_distribution(){
  [[ "$PACKAGE_DIST" -eq 1 ]] || { info "Skipping distribution packaging."; return; }

  info "Creating Clyth distribution directory..."
  rm -rf "$DIST_DIR"
  mkdir -p "$DIST_DIR/bin" "$DIST_DIR/licenses" "$DIST_DIR/share/clyth/samples" "$DIST_DIR/share/clyth/runtime"

  local compiler_bin="$CLYTH_BUILD/clyth_compiler_bin"
  [[ -x "$compiler_bin" ]] || die "Compiler binary not found at $compiler_bin"
  cp "$compiler_bin" "$DIST_DIR/bin/"

  # Clyth uses Zig's clang-compatible frontend for final musl static links.
  # The Zig binary itself is not vendored here; users should install Zig and keep it on PATH.
  cp "$ZIG_CC" "$DIST_DIR/bin/zig-c.sh" 2>/dev/null || true
  cp "$ZIG_CXX" "$DIST_DIR/bin/zig-c++.sh" 2>/dev/null || true

  # Ship the small LLVM tools that are useful for inspecting/debugging generated IR.
  for tool in llc lli llvm-as llvm-dis llvm-link opt lld ld.lld; do
    if [[ -x "$LLVM_INSTALL/bin/$tool" ]]; then
      cp "$LLVM_INSTALL/bin/$tool" "$DIST_DIR/bin/"
    fi
  done

  # Project and third-party license files travel with every distribution.
  cp "$ROOT_DIR/LICENSE" "$DIST_DIR/licenses/CLYTH_LICENSE" 2>/dev/null || true
  cp "$CLYTH_SRC/EXTERNAL_LIBRARIES_LICENSES.md" "$DIST_DIR/licenses/EXTERNAL_LIBRARIES_LICENSES.md" 2>/dev/null || true
  cp "$ROOT_DIR/clyth-runtime/EXTERNAL_LIBRARIES_LICENSES.md" "$DIST_DIR/licenses/RUNTIME_EXTERNAL_LIBRARIES_LICENSES.md" 2>/dev/null || true
  cp "$ANTLR_SRC/LICENSE.txt" "$DIST_DIR/licenses/ANTLR4_RUNTIME_LICENSE.txt" 2>/dev/null || true
  cp "$FMT_SRC/LICENSE" "$DIST_DIR/licenses/FMT_LICENSE" 2>/dev/null || true
  if [[ -f "$LLVM_SRC/llvm/LICENSE.TXT" ]]; then
    cp "$LLVM_SRC/llvm/LICENSE.TXT" "$DIST_DIR/licenses/LLVM_LICENSE.TXT"
  elif [[ -f "$LLVM_INSTALL/LICENSE.TXT" ]]; then
    cp "$LLVM_INSTALL/LICENSE.TXT" "$DIST_DIR/licenses/LLVM_LICENSE.TXT"
  fi

  cp "$ROOT_DIR/README.md" "$DIST_DIR/README.md" 2>/dev/null || true
  cp "$ROOT_DIR/RELEASE_NOTES_ALPHA_0_3_0.md" "$DIST_DIR/RELEASE_NOTES_ALPHA_0_3_0.md" 2>/dev/null || true
  cp "$ROOT_DIR/install.sh" "$DIST_DIR/install.sh" 2>/dev/null || true
  cp "$ROOT_DIR/uninstall.sh" "$DIST_DIR/uninstall.sh" 2>/dev/null || true
  chmod +x "$DIST_DIR/install.sh" "$DIST_DIR/uninstall.sh" 2>/dev/null || true
  cp "$CLYTH_SRC/release_info.json" "$DIST_DIR/release_info.json" 2>/dev/null || true
  cp "$ROOT_DIR/sample-clyth-programs"/*.clyth "$DIST_DIR/share/clyth/samples/" 2>/dev/null || true
  
  # Ship runtime module sources and built per-architecture archives, but never vendor/source caches.
  mkdir -p "$DIST_DIR/share/clyth/runtime"
  tar -C "$ROOT_DIR/clyth-runtime" \
    --exclude='c-bindings/*/vendor' \
    --exclude='c-bindings/*/build' \
    --exclude='c-bindings/*/.git' \
    --exclude='**/.git' \
    --exclude='**/CMakeFiles' \
    --exclude='**/CMakeCache.txt' \
    --exclude='**/*.o' \
    -cf - . | tar -C "$DIST_DIR/share/clyth/runtime" -xf -


  cat > "$DIST_DIR/README_DIST.md" <<'TXT'
# Clyth Distribution

This directory contains the Clyth compiler binary, useful LLVM inspection tools,
project documentation, Alpha release notes, sample programs, runtime modules, and
license texts for bundled project components.

The compiler can emit LLVM IR and can invoke `zig cc` to produce a statically
linked musl-targeted executable from supported Clyth programs:

```bash
bin/clyth_compiler_bin -c share/clyth/samples/printf_test.clyth -o printf_test
./printf_test
```

Zig is expected to be installed and available on PATH for final executable
linking. Use `--emit-ir-only` to stop after LLVM IR emission.

For a user-local Linux install from this distribution directory:

```bash
./install.sh
```

This installs to `$HOME/.local/share/clyth` and symlinks `clyth` into
`$HOME/.local/bin`.

```bash
bin/clyth_compiler_bin -c input.clyth -o output --emit-ir-only
```

Show bundled license text:

```bash
bin/clyth_compiler_bin --show-licenses
```
TXT

  tar -C "$DIST_DIR/.." -czf "$ROOT_DIR/clyth-dist.tar.gz" "$(basename "$DIST_DIR")"
  ok "Distribution directory created: $DIST_DIR"
  ok "Distribution tarball created: $ROOT_DIR/clyth-dist.tar.gz"
}

build_clyth(){
  info "Building Clyth compiler..."
  rm -rf "$CLYTH_BUILD"; mkdir -p "$CLYTH_BUILD"
  rm -rf "$CLYTH_SRC/src/clyth_antlr_files/"
  cd "$CLYTH_GRAMMAR_DIRECTORY"
  ./generateAntlr4RuntimeCpp.sh "$CLYTH_GRAMMAR_FILE"
  cd -
  mapfile -t g < <(gen_args)
  extra=()
  if [[ "$BUILD_LLVM" -eq 1 && -d "$LLVM_INSTALL/lib/cmake/llvm" ]]; then
    extra+=("-DLLVM_DIR=$LLVM_INSTALL/lib/cmake/llvm" "-DCMAKE_PREFIX_PATH=$LLVM_INSTALL")
    info "Using bundled LLVM_DIR=$LLVM_INSTALL/lib/cmake/llvm"
  else
    warn "Bundled LLVM not enabled/found; CMake will search for system LLVM."
  fi
  cmake -S "$CLYTH_SRC" -B "$CLYTH_BUILD" "${g[@]}" \
    -DCMAKE_C_COMPILER="$ZIG_CC" \
    -DCMAKE_CXX_COMPILER="$ZIG_CXX" \
    -DCMAKE_BUILD_TYPE=Release \
    "${extra[@]}"
  cmake_build "$CLYTH_BUILD"
  ok "Clyth compiler built."
}

main(){
  cd "$ROOT_DIR"
  export PATH="$ROOT_DIR:$PATH"
  info "Starting Clyth build at $ROOT_DIR"
  info "Parallel jobs: $JOBS"
  check_deps
  setup_antlr
  ensure_fmt
  build_antlr
  build_llvm
  build_clyth
  build_runtime_c_bindings
  package_distribution
  cat <<SUMMARY

Build summary:
  ANTLR runtime: $ANTLR_INSTALL
  LLVM install:  $LLVM_INSTALL
  LLVM_DIR:      $LLVM_INSTALL/lib/cmake/llvm
  llvm-config:   $LLVM_INSTALL/bin/llvm-config
  llc:           $LLVM_INSTALL/bin/llc
  lld:           $LLVM_INSTALL/bin/lld
  Clyth build:   $CLYTH_BUILD
  Runtime dir:   $CLYTH_RUNTIME_DIR
  Distribution:  $([[ "$PACKAGE_DIST" -eq 1 ]] && echo "$DIST_DIR" || echo "skipped (--skip-dist/--no-package)")

SUMMARY
  ok "Build completed successfully."
}

main "$@"
