#!/usr/bin/env bash
set -Eeuo pipefail

MODULE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SOURCE_DIR="${MUSL_SOURCE:-$MODULE_DIR/third_party/musl}"
BUILD_ROOT="${MUSL_BUILD_ROOT:-$MODULE_DIR/build}"
OUTPUT_ROOT="${MUSL_OUTPUT_ROOT:-$MODULE_DIR/precompiled}"
ZIG="${ZIG:-${CLYTH_ZIG:-zig}}"
JOBS="${JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || printf '1')}"

if [[ ! -x "$SOURCE_DIR/configure" ]]; then
  printf 'musl source is absent; fetching the pinned source before building.\n'
  "$MODULE_DIR/scripts/fetch_musl.sh"
fi
command -v "$ZIG" >/dev/null 2>&1 || { printf 'error: Zig was not found: %s\n' "$ZIG" >&2; exit 1; }

build_target() {
  local module_target="$1"
  local zig_target="$2"
  local musl_target="$3"
  local build_dir="$BUILD_ROOT/$module_target"
  local output_dir="$OUTPUT_ROOT/$module_target"
  local cc_wrapper="$build_dir/zig-cc"

  rm -rf "$build_dir" "$output_dir"
  mkdir -p "$build_dir" "$output_dir"

  cat > "$cc_wrapper" <<WRAPPER
#!/usr/bin/env bash
set -Eeuo pipefail
exec "$ZIG" cc -target "$zig_target" "\$@"
WRAPPER
  chmod +x "$cc_wrapper"

  (
    cd "$build_dir"
    env CC="$cc_wrapper" AR="$ZIG ar" RANLIB="$ZIG ranlib" \
      "$SOURCE_DIR/configure" \
      --prefix=/ \
      --target="$musl_target" \
      --disable-shared
    make -j"$JOBS"
    make DESTDIR="$output_dir" install
  )

  [[ -f "$output_dir/lib/libc.a" ]] || {
    printf 'error: musl libc.a was not produced for %s\n' "$module_target" >&2
    exit 1
  }
  [[ -f "$output_dir/lib/crt1.o" ]] || {
    printf 'error: musl crt1.o was not produced for %s\n' "$module_target" >&2
    exit 1
  }

  printf 'Built musl target %s: %s\n' "$module_target" "$output_dir"
}

case "${1:-all}" in
  all)
    build_target x86_64-linux-musl x86_64-linux-musl x86_64-linux-musl
    build_target aarch64-linux-musl aarch64-linux-musl aarch64-linux-musl
    build_target arm-linux-musleabihf arm-linux-musleabihf arm-linux-musleabihf
    ;;
  x86_64-linux-musl)
    build_target x86_64-linux-musl x86_64-linux-musl x86_64-linux-musl
    ;;
  aarch64-linux-musl)
    build_target aarch64-linux-musl aarch64-linux-musl aarch64-linux-musl
    ;;
  arm-linux-musleabihf)
    build_target arm-linux-musleabihf arm-linux-musleabihf arm-linux-musleabihf
    ;;
  *)
    printf 'usage: %s [all|x86_64-linux-musl|aarch64-linux-musl|arm-linux-musleabihf]\n' "$0" >&2
    exit 2
    ;;
esac
