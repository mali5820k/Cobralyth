#!/usr/bin/env bash
# Build the intermediate 0.5.1 compiler with the trusted Stage 0 seed.
# The script also rebuilds the native runtime archives and assembles the Stage 1
# module store so no stale binary artifacts are inherited from the checkout.
set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
STAGE1_DIR="$(cd "$SCRIPT_DIR/../0.5.1-compiler" && pwd)"
PROJECT_ROOT="$(cd "$STAGE1_DIR/../.." && pwd)"
STAGE0_DIST="${CLYTH_STAGE0_DIST:-$PROJECT_ROOT/bootstrap/0.5.0-compiler/dist}"
STAGE0="${CLYTH_STAGE0_COMPILER:-$STAGE0_DIST/bin/clyth_compiler_bin}"
ZIG="${ZIG:-zig}"
ZIG_CC="$SCRIPT_DIR/zig-c.sh"
ZIG_CXX="$SCRIPT_DIR/zig-c++.sh"
TARGET="${CLYTH_ZIG_TARGET:-native-linux-musl}"
export CLYTH_ZIG_CC="$ZIG_CC"
export CLYTH_ZIG_CXX="$ZIG_CXX"
ARCH="${CLYTH_ARCH:-x86_64}"
RUNTIME="$STAGE1_DIR/clyth-runtime"
STORE="$STAGE1_DIR/module-store"
DIST="$STAGE1_DIR/dist"
BUILD="$STAGE1_DIR/.bootstrap-build"

# Direct invocation must work after cleanup or from a fresh checkout.
mkdir -p "$STAGE0_DIST" "$STAGE1_DIR"

[[ -x "$STAGE0" ]] || { printf 'missing Stage 0 compiler: %s\n' "$STAGE0" >&2; exit 1; }
command -v "$ZIG" >/dev/null 2>&1 || [[ -x "$ZIG" ]] || { printf 'missing Zig compiler: %s\n' "$ZIG" >&2; exit 1; }
[[ -x "$ZIG_CC" && -x "$ZIG_CXX" ]] || { printf 'missing executable Zig wrappers under %s\n' "$SCRIPT_DIR" >&2; exit 1; }
"$SCRIPT_DIR/fetch_runtime_vendors.sh" "$RUNTIME"

rm -rf "$BUILD" "$STORE" "$DIST"
mkdir -p "$BUILD/libuv" "$STORE/modules" "$DIST/bin" "$DIST/libexec" "$DIST/share/clyth"

LIBUV="$RUNTIME/c-bindings/libuv/vendor/libuv"
LIBUV_ARCHIVE="$RUNTIME/c-bindings/libuv/$ARCH/libclyth_libuv.a"
LIBUV_SOURCES=(
  src/fs-poll.c src/idna.c src/inet.c src/random.c src/strscpy.c src/strtok.c
  src/thread-common.c src/threadpool.c src/timer.c src/uv-common.c
  src/uv-data-getter-setters.c src/version.c src/unix/async.c src/unix/core.c
  src/unix/dl.c src/unix/fs.c src/unix/getaddrinfo.c src/unix/getnameinfo.c
  src/unix/linux.c src/unix/loop-watcher.c src/unix/loop.c src/unix/pipe.c
  src/unix/poll.c src/unix/process.c src/unix/procfs-exepath.c
  src/unix/proctitle.c src/unix/random-devurandom.c
  src/unix/random-getrandom.c src/unix/signal.c src/unix/stream.c
  src/unix/tcp.c src/unix/thread.c src/unix/tty.c src/unix/udp.c
)

build_runtime_archive() {
  local binding_name="$1"
  local source_path="$2"
  local archive_path="$3"
  shift 3
  local object_path="$BUILD/${binding_name}.o"

  [[ -f "$source_path" ]] || {
    printf 'missing runtime binding source: %s\n' "$source_path" >&2
    exit 1
  }

  mkdir -p "$(dirname "$archive_path")"
  "$ZIG_CC" -target "$TARGET" -O2 "$@" -c "$source_path" -o "$object_path"
  "$ZIG" ar rcs "$archive_path" "$object_path"
}

mkdir -p "$(dirname "$LIBUV_ARCHIVE")"
objects=()
for source in "${LIBUV_SOURCES[@]}"; do
  object="$BUILD/libuv/${source//\//_}.o"
  "$ZIG_CC" -target "$TARGET" -O2 -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112L \
    -D_FILE_OFFSET_BITS=64 -I"$LIBUV/include" -I"$LIBUV/src" \
    -c "$LIBUV/$source" -o "$object"
  objects+=("$object")
done
"$ZIG" ar rcs "$LIBUV_ARCHIVE" "${objects[@]}"

# Rebuild every native archive referenced by the Stage 1 runtime manifests.
# Stage 1 must not inherit stale binaries from the source snapshot because its
# module store becomes the runtime payload for Stage 2 and the final compiler.
build_runtime_archive dma \
  "$RUNTIME/c-bindings/dma/dma.c" \
  "$RUNTIME/modules/module-dma/$ARCH/libclyth_dma.a"

build_runtime_archive rapidhash \
  "$RUNTIME/c-bindings/rapidhash/rapidhash_bindings.c" \
  "$RUNTIME/modules/module-hash/$ARCH/libclyth_rapidhash.a"

build_runtime_archive json \
  "$RUNTIME/c-bindings/yyjson/json_bindings.c" \
  "$RUNTIME/modules/module-json/$ARCH/libclyth_json.a"

build_runtime_archive concurrency \
  "$RUNTIME/c-bindings/concurrency/concurrency.c" \
  "$RUNTIME/modules/module-concurrency/$ARCH/libclyth_concurrency.a"

build_runtime_archive http \
  "$RUNTIME/c-bindings/http/web.c" \
  "$RUNTIME/c-bindings/http/$ARCH/libclyth_http.a" \
  -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112L -D_FILE_OFFSET_BITS=64 \
  -I"$LIBUV/include" -I"$LIBUV/src"
mkdir -p \
  "$RUNTIME/modules/module-router/$ARCH" \
  "$RUNTIME/modules/module-https/$ARCH"
cp "$RUNTIME/c-bindings/http/$ARCH/libclyth_http.a" \
  "$RUNTIME/modules/module-router/$ARCH/libclyth_http.a"
cp "$RUNTIME/c-bindings/http/$ARCH/libclyth_http.a" \
  "$RUNTIME/modules/module-https/$ARCH/libclyth_http.a"

FILE_OBJECT="$BUILD/file_io.o"
FILE_ARCHIVE="$RUNTIME/modules/module-file-io/$ARCH/libclyth_file_io.a"
BOOTSTRAP_SUPPORT_SOURCE="$RUNTIME/c-bindings/bootstrapping-stage1/bootstrapping-stage1.c"
BOOTSTRAP_SUPPORT_OBJECT="$BUILD/bootstrapping-stage1.o"
BOOTSTRAP_SUPPORT_ARCHIVE="$RUNTIME/modules/module-bootstrapping-stage1/$ARCH/libclyth_bootstrapping-stage1.a"
mkdir -p "$(dirname "$FILE_ARCHIVE")" "$(dirname "$BOOTSTRAP_SUPPORT_ARCHIVE")"
"$ZIG_CC" -target "$TARGET" -O2 -D_GNU_SOURCE -D_POSIX_C_SOURCE=200112L \
  -D_FILE_OFFSET_BITS=64 -I"$LIBUV/include" -I"$LIBUV/src" \
  -c "$RUNTIME/c-bindings/file-io/file.c" -o "$FILE_OBJECT"
"$ZIG_CC" -target "$TARGET" -O2 -c "$BOOTSTRAP_SUPPORT_SOURCE" -o "$BOOTSTRAP_SUPPORT_OBJECT"

# Stage 0 has a frozen, hard-coded runtime archive map. Since it already links
# module-file-io for both bootstrap executables, place the temporary Stage 1
# support object in that archive as well. Keep the standalone archive for the
# module store, but do not depend on Stage 0 discovering it.
"$ZIG" ar rcs "$FILE_ARCHIVE" "$FILE_OBJECT" "$BOOTSTRAP_SUPPORT_OBJECT"
"$ZIG" ar rcs "$BOOTSTRAP_SUPPORT_ARCHIVE" "$BOOTSTRAP_SUPPORT_OBJECT"

CLYTH_RUNTIME_DIR="$RUNTIME" "$STAGE0" -c "$STAGE1_DIR/tools/module-manager/main.clyth" -o "$BUILD/clyth-module"

cp "$RUNTIME/runtime_libraries.json" "$STORE/runtime_libraries.json"
for module_dir in "$RUNTIME"/modules/module-*; do
  module="${module_dir##*/module-}"
  printf '%s\n%s\n%s\n%s\n' \
    'build-install' "$RUNTIME" "$module" "$STORE" \
    > "$BUILD/clyth-module.request"
  (cd "$STAGE1_DIR" && "$BUILD/clyth-module")
done
cp "$LIBUV_ARCHIVE" "$STORE/libclyth_libuv.a"
mkdir -p "$STORE/c-bindings/libuv/$ARCH"
cp "$LIBUV_ARCHIVE" "$STORE/c-bindings/libuv/$ARCH/libclyth_libuv.a"

CLYTH_RUNTIME_DIR="$STORE" "$STAGE0" -c "$STAGE1_DIR/main.clyth" -o "$BUILD/clyth-stage1-driver"

cp "$BUILD/clyth-stage1-driver" "$DIST/libexec/"
cp "$BUILD/clyth-module" "$DIST/bin/"
cp -R "$STORE" "$DIST/share/clyth/module-store"
# The Stage 1 manifest is cumulative: it contains both runtime dependency
# notices and the Stage 0 compiler/frontend/toolchain notices inherited by the
# bootstrap chain. Keep a canonical compiler-level copy and a compatibility
# copy in the historical runtime location.
mkdir -p "$DIST/share/clyth/clyth-runtime"
cp "$STAGE1_DIR/EXTERNAL_LIBRARIES_LICENSES.md" \
  "$DIST/share/clyth/EXTERNAL_LIBRARIES_LICENSES.md"
cp "$STAGE1_DIR/EXTERNAL_LIBRARIES_LICENSES.md" \
  "$DIST/share/clyth/clyth-runtime/EXTERNAL_LIBRARIES_LICENSES.md"

cat > "$DIST/bin/clyth-stage1" <<'WRAPPER'
#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SOURCE=""
OUTPUT="a.out"
EMIT_IR_ONLY=0

while [[ $# -gt 0 ]]; do
  case "$1" in
    -c)
      [[ $# -ge 2 ]] || { printf 'missing source file after -c\n' >&2; exit 2; }
      SOURCE="$2"
      shift 2
      ;;
    -o)
      [[ $# -ge 2 ]] || { printf 'missing output name after -o\n' >&2; exit 2; }
      OUTPUT="$2"
      shift 2
      ;;
    --emit-ir-only)
      EMIT_IR_ONLY=1
      shift
      ;;
    -h|--help)
      printf 'usage: clyth-stage1 -c <source> -o <output> [--emit-ir-only]\n'
      exit 0
      ;;
    -v|--version)
      printf 'Clyth bootstrap compiler 0.5.1\n'
      exit 0
      ;;
    *)
      printf 'unrecognized argument: %s\n' "$1" >&2
      exit 2
      ;;
  esac
done

[[ -n "$SOURCE" ]] || { printf 'no source file supplied\n' >&2; exit 2; }
SOURCE="$(realpath -m "$SOURCE")"
OUTPUT="$(realpath -m "$OUTPUT")"
printf '%s\n%s\n%s\n%s\n%s\n' \
  "$SOURCE" "$OUTPUT" \
  "${CLYTH_STAGE0_COMPILER:-$ROOT/../../0.5.0-compiler/dist/bin/clyth_compiler_bin}" \
  "$ROOT/share/clyth/module-store" \
  "$EMIT_IR_ONLY" \
  > "$ROOT/share/clyth/stage1.request"
(cd "$ROOT" && exec "$ROOT/libexec/clyth-stage1-driver")
WRAPPER
chmod +x "$DIST/bin/clyth-stage1" "$DIST/bin/clyth-module" "$DIST/libexec/"*
"$SCRIPT_DIR/audit_no_gnu_abi.sh" "$DIST"

printf 'Built Stage 1 distribution: %s\n' "$DIST"
