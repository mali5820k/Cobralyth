#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DIST="$ROOT/dist"

usage() {
  cat <<'USAGE'
Usage: ./scripts/build-all.sh [--clean] [--skip-modules] [--skip-selfhost]

Build every Clyth project artifact into the root dist/ tree:
  dist/bootstrap/   Go bootstrap compiler + registry utility
  dist/lsp/         packaged editor/LSP integrations
  dist/modules/     packaged module registry snapshot
  dist/clyth/       self-hosted compiler, when native lowering is available

Options:
  --clean           remove generated outputs before building (vendor source is preserved)
  --skip-modules    skip native module bindings and module packaging
  --skip-selfhost   skip the self-hosted compiler build

Native module/self-host builds require Zig, llc, lld, and a Clyth-managed musl
sysroot. Compatible system LLVM tools are reused when available; otherwise a pinned
llvm-project checkout is fetched and only the required llc/lld tools are built. build-all.sh now prepares the default host musl sysroot automatically:
it reuses CLYTH_MUSL_SYSROOT when supplied, otherwise reuses/builds precompiled/<target>,
and fetches pinned musl source only when third_party/musl is absent. Set CLYTH_ZIG,
CLYTH_LLC, or CLYTH_LLD when those tools are not discoverable system-wide.
Use --skip-selfhost when only packaging the bootstrap/compiler tooling. Pinned vendor
source under module-src/vendor is preserved by --clean; use scripts/clean.sh --vendors
to discard it explicitly.
USAGE
}

clean=0
skip_modules=0
skip_selfhost=0
while (($#)); do
  case "$1" in
    --clean) clean=1 ;;
    --skip-modules) skip_modules=1 ;;
    --skip-selfhost) skip_selfhost=1 ;;
    -h|--help) usage; exit 0 ;;
    *) printf 'error: unknown option: %s\n' "$1" >&2; usage >&2; exit 2 ;;
  esac
  shift
done

((clean == 0)) || "$ROOT/scripts/clean.sh"
mkdir -p "$DIST"


prepare_llvm() {
  local llc="${CLYTH_LLC:-$(command -v llc || true)}"
  local lld="${CLYTH_LLD:-$(command -v ld.lld || command -v lld || true)}"
  if [[ -n "$llc" && -x "$llc" && -n "$lld" && -x "$lld" ]]; then
    printf '\n==> Reusing LLVM tools: %s, %s\n' "$llc" "$lld"
    export CLYTH_LLC="$llc"
    export CLYTH_LLD="$lld"
    return
  fi

  local source="${CLYTH_LLVM_SOURCE:-$ROOT/third_party/llvm-project}"
  local build="${CLYTH_LLVM_BUILD:-$ROOT/.build/llvm}"

  # Prefer an already-built repository LLVM toolchain before configuring or
  # rebuilding anything. This is the normal fast path for iterative Stage-1
  # compiler work after the first successful LLVM build.
  local built_llc="$build/bin/llc"
  local built_lld=""
  if [[ -x "$build/bin/ld.lld" ]]; then built_lld="$build/bin/ld.lld"
  elif [[ -x "$build/bin/lld" ]]; then built_lld="$build/bin/lld"
  fi
  if [[ -x "$built_llc" && -n "$built_lld" && -x "$built_lld" ]]; then
    printf '\n==> Reusing repository LLVM tools: %s, %s\n' "$built_llc" "$built_lld"
    export CLYTH_LLC="$built_llc"
    export CLYTH_LLD="$built_lld"
    return
  fi

  if [[ ! -f "$source/llvm/CMakeLists.txt" ]]; then
    printf '\n==> Fetching pinned LLVM source\n'
    "$ROOT/scripts/fetch_llvm.sh"
  fi
  command -v cmake >/dev/null || { printf 'error: cmake is required to build LLVM\n' >&2; exit 1; }
  command -v ninja >/dev/null || { printf 'error: ninja is required to build LLVM\n' >&2; exit 1; }

  # LLVM is built through the repository's canonical Zig compiler wrappers.
  # Do not fall back to ambient cc/c++/gcc/clang: the wrappers are part of the
  # Clyth build contract and keep C/C++ compiler selection explicit.
  local zig_cc="$ROOT/scripts/zig-c.sh"
  local zig_cxx="$ROOT/scripts/zig-c++.sh"
  [[ -x "$zig_cc" ]] || { printf 'error: missing executable Zig C wrapper: %s\n' "$zig_cc" >&2; exit 1; }
  [[ -x "$zig_cxx" ]] || { printf 'error: missing executable Zig C++ wrapper: %s\n' "$zig_cxx" >&2; exit 1; }

  # A CMake tree configured with ambient GCC/Clang cannot safely be reused
  # after restoring the canonical Zig wrappers. Drop only the generated LLVM
  # build directory when its cached compiler contract differs.
  if [[ -f "$build/CMakeCache.txt" ]]; then
    local cached_cc cached_cxx
    cached_cc="$(sed -n 's/^CMAKE_C_COMPILER:FILEPATH=//p' "$build/CMakeCache.txt" | head -n1)"
    cached_cxx="$(sed -n 's/^CMAKE_CXX_COMPILER:FILEPATH=//p' "$build/CMakeCache.txt" | head -n1)"
    if [[ "$cached_cc" != "$zig_cc" || "$cached_cxx" != "$zig_cxx" ]]; then
      printf '==> Resetting LLVM CMake cache to restore Zig compiler wrappers\n'
      rm -rf "$build"
    fi
  fi

  local targets
  case "$(uname -m)" in
    x86_64|amd64) targets=X86 ;;
    aarch64|arm64) targets=AArch64 ;;
    *) targets=Native ;;
  esac
  printf '\n==> Configuring LLVM toolchain (%s)\n' "$targets"
  printf '==> LLVM C compiler wrapper: %s\n' "$zig_cc"
  printf '==> LLVM C++ compiler wrapper: %s\n' "$zig_cxx"
  cmake -S "$source/llvm" -B "$build" -G Ninja \
    -DCMAKE_C_COMPILER="$zig_cc" \
    -DCMAKE_CXX_COMPILER="$zig_cxx" \
    -DCMAKE_FIND_ROOT_PATH="$CLYTH_MUSL_SYSROOT" \
    -DCMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER \
    -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY \
    -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_ENABLE_PROJECTS=lld \
    -DLLVM_TARGETS_TO_BUILD="$targets" \
    -DLLVM_INCLUDE_TESTS=OFF \
    -DLLVM_INCLUDE_EXAMPLES=OFF \
    -DLLVM_INCLUDE_BENCHMARKS=OFF \
    -DLLVM_ENABLE_TERMINFO=OFF \
    -DLLVM_ENABLE_ZLIB=OFF \
    -DLLVM_ENABLE_ZSTD=OFF \
    -DLLVM_ENABLE_LIBXML2=OFF \
    -DLLVM_ENABLE_CURL=OFF \
    -DLLVM_ENABLE_LIBEDIT=OFF \
    -DLLVM_ENABLE_FFI=OFF
  printf '\n==> Building llc and lld\n'
  cmake --build "$build" --target llc lld
  [[ -x "$build/bin/llc" ]] || { printf 'error: LLVM build did not produce llc\n' >&2; exit 1; }
  if [[ -x "$build/bin/ld.lld" ]]; then lld="$build/bin/ld.lld"; else lld="$build/bin/lld"; fi
  [[ -x "$lld" ]] || { printf 'error: LLVM build did not produce lld\n' >&2; exit 1; }
  export CLYTH_LLC="$build/bin/llc"
  export CLYTH_LLD="$lld"
}

prepare_musl() {
  local target="${CLYTH_ZIG_TARGET:-}"
  if [[ -z "$target" ]]; then
    case "$(uname -m)" in
      x86_64|amd64) target="x86_64-linux-musl" ;;
      aarch64|arm64) target="aarch64-linux-musl" ;;
      armv7l|armv7*) target="arm-linux-musleabihf" ;;
      *)
        printf 'error: cannot infer the default musl target for host architecture %s; set CLYTH_ZIG_TARGET\n' "$(uname -m)" >&2
        exit 1
        ;;
    esac
  fi

  local source="${MUSL_SOURCE:-$ROOT/third_party/musl}"
  local sysroot="${CLYTH_MUSL_SYSROOT:-$ROOT/precompiled/$target}"
  local explicit_sysroot=0
  [[ -z "${CLYTH_MUSL_SYSROOT:-}" ]] || explicit_sysroot=1

  if [[ -x "$source/configure" ]]; then
    [[ -f "$source/VERSION" ]] || { printf 'error: musl source has no VERSION file: %s\n' "$source" >&2; exit 1; }
    [[ "$(tr -d '[:space:]' < "$source/VERSION")" == "1.2.6" ]] || {
      printf 'error: Clyth requires musl 1.2.6, but %s reports %s\n' "$source" "$(tr -d '[:space:]' < "$source/VERSION")" >&2
      exit 1
    }
    [[ -f "$source/COPYRIGHT" ]] || { printf 'error: musl source has no COPYRIGHT file: %s\n' "$source" >&2; exit 1; }
  elif [[ -n "${MUSL_SOURCE:-}" ]]; then
    printf 'error: MUSL_SOURCE does not contain a configured musl source tree: %s\n' "$source" >&2
    exit 1
  fi

  if [[ ! -d "$sysroot/include" || ! -f "$sysroot/lib/libc.a" || ! -f "$sysroot/lib/crt1.o" ]]; then
    if ((explicit_sysroot == 1)); then
      printf 'error: CLYTH_MUSL_SYSROOT is incomplete: %s\n' "$sysroot" >&2
      printf 'unset CLYTH_MUSL_SYSROOT to let build-all.sh prepare %s automatically.\n' "$ROOT/precompiled/$target" >&2
      exit 1
    fi

    if [[ ! -x "$source/configure" ]]; then
      printf '\n==> Fetching pinned musl source\n'
      "$ROOT/scripts/fetch_musl.sh"
    else
      printf '\n==> Using existing verified musl 1.2.6 source: %s\n' "$source"
    fi

    printf '\n==> Building musl sysroot (%s)\n' "$target"
    MUSL_SOURCE="$source" \
      MUSL_OUTPUT_ROOT="$ROOT/precompiled" \
      ZIG="${CLYTH_ZIG:-${ZIG:-zig}}" \
      "$ROOT/scripts/build_musl.sh" "$target"
  else
    printf '\n==> Reusing musl sysroot: %s\n' "$sysroot"
  fi

  export CLYTH_ZIG_TARGET="$target"
  export CLYTH_ARTIFACT_TARGET="${CLYTH_ARTIFACT_TARGET:-$target}"
  export CLYTH_MUSL_SYSROOT="$sysroot"
  case "$target" in
    x86_64-linux-musl) export CLYTH_MUSL_X86_64_SYSROOT="${CLYTH_MUSL_X86_64_SYSROOT:-$sysroot}" ;;
    aarch64-linux-musl) export CLYTH_MUSL_AARCH64_SYSROOT="${CLYTH_MUSL_AARCH64_SYSROOT:-$sysroot}" ;;
  esac
}

if ((skip_modules == 0 || skip_selfhost == 0)); then
  # The Zig wrappers consume the managed musl sysroot, so provision it first.
  prepare_musl
  prepare_llvm
fi

printf '================================\n'
printf ' Building all Clyth artifacts\n'
printf '================================\n\n'

printf '==> Bootstrap compiler and registry utility\n'
"$ROOT/scripts/check-bootstrap.sh"

printf '\n==> LSP tests\n'
"$ROOT/scripts/test-lsp.sh"

printf '\n==> Editor/LSP release packages\n'
"$ROOT/scripts/package-lsp.sh"

if ((skip_modules == 0)); then
  printf '\n==> Native module bindings\n'
  "$ROOT/scripts/build-modules.sh"

  printf '\n==> Module registries\n'
  "$ROOT/prepare_module_registry.sh"

  printf '\n==> Copying packaged module registry into dist/modules\n'
  rm -rf "$DIST/modules"
  mkdir -p "$DIST/modules"
  cp -R "$ROOT/modules"/. "$DIST/modules/"
else
  printf '\n==> Skipping native modules (--skip-modules)\n'
fi

if ((skip_selfhost == 0)); then
  printf '\n==> Self-hosted Clyth compiler\n'
  if ! "$ROOT/scripts/build-clyth-compiler.sh"; then
    cat >&2 <<'MSG'

error: self-hosted compiler build did not complete.
The failing command above is authoritative; resolve that phase and retry.
Use --skip-selfhost when intentionally producing only bootstrap/tooling artifacts.
MSG
    exit 1
  fi
  printf '\n==> GNU ABI audit (self-hosted compiler)\n'
  "$ROOT/scripts/audit_no_gnu_abi.sh" "$DIST/clyth/clyth"

  printf '\n==> Independent Stage-1 regression suite\n'
  CLYTH_TEST_COMPILER="$DIST/clyth/clyth" "$ROOT/scripts/run-regression-tests.sh"

  # This broader smoke intentionally runs only after the canonical regression
  # suite is green. It exercises file-io plus native backend lowering and must
  # still pass before bootstrap validation can proceed to Stage-2.
  printf '\n==> Independent Stage-1 full native backend smoke\n'
  STAGE1_SMOKE_IR="$(mktemp "${TMPDIR:-/tmp}/clyth-stage1-native-backend.XXXXXX.ll")"
  trap 'rm -f "$STAGE1_SMOKE_IR"' EXIT
  "$DIST/clyth/clyth" build "$ROOT/compiler/clyth/stage1-tests/native_backend.clyth" --emit-llvm-ir -o "$STAGE1_SMOKE_IR"
  grep -q '^target triple = ' "$STAGE1_SMOKE_IR" || { printf 'error: Stage-1 native backend smoke did not emit LLVM IR\n' >&2; exit 1; }
  rm -f "$STAGE1_SMOKE_IR"
  trap - EXIT

  printf '\n==> Stage-1 -> Stage-2 validation\n'
  "$ROOT/scripts/check-stage2.sh"
else
  printf '\n==> Skipping self-hosted compiler (--skip-selfhost)\n'
fi

printf '\nBuild complete. User-facing artifacts are under:\n  %s\n' "$DIST"
find "$DIST" -maxdepth 3 -type f -print | sort | sed 's#^#  #'
