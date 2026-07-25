#!/usr/bin/env bash
set -Eeuo pipefail

STAGE2_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$STAGE2_DIR/.." && pwd)"
STAGE1_DIST="${CLYTH_STAGE1_DIST:-$PROJECT_ROOT/0.6.0-stage1-compiler/dist}"
STAGE1="${CLYTH_STAGE1_COMPILER:-$STAGE1_DIST/bin/clyth-stage1}"
STAGE0="${CLYTH_STAGE0_COMPILER:-$PROJECT_ROOT/0.5.0-compiler-dist/bin/clyth_compiler_bin}"
MODULE_STORE="${CLYTH_MODULE_STORE:-$STAGE1_DIST/share/clyth/module-store}"
BUILD="$STAGE2_DIR/.build"
FINAL_DIST="$PROJECT_ROOT/0.6.0-compiler-dist"
FINAL_TARBALL="$PROJECT_ROOT/clyth-0.6.0-compiler-dist.tar.gz"

[[ -x "$STAGE1" ]] || { printf 'missing Stage 1 compiler: %s\n' "$STAGE1" >&2; exit 1; }
[[ -x "$STAGE0" ]] || { printf 'missing frozen Stage 0 compiler: %s\n' "$STAGE0" >&2; exit 1; }
[[ -d "$MODULE_STORE" ]] || { printf 'missing Stage 1 module store: %s\n' "$MODULE_STORE" >&2; exit 1; }

rm -rf "$BUILD" "$FINAL_DIST"
mkdir -p "$BUILD" "$FINAL_DIST/bin" "$FINAL_DIST/libexec" "$FINAL_DIST/share/clyth"

"$STAGE1" -c "$STAGE2_DIR/src/main.clyth" -o "$BUILD/clyth-stage2-driver"

cp "$BUILD/clyth-stage2-driver" "$FINAL_DIST/libexec/"
cp -R "$MODULE_STORE" "$FINAL_DIST/share/clyth/module-store"
mkdir -p "$FINAL_DIST/share/clyth/clyth-runtime"
cp "$PROJECT_ROOT/clyth-runtime/EXTERNAL_LIBRARIES_LICENSES.md" "$FINAL_DIST/share/clyth/clyth-runtime/EXTERNAL_LIBRARIES_LICENSES.md"
cp "$STAGE2_DIR/EXTERNAL_LIBRARIES_LICENSES.md" "$FINAL_DIST/share/clyth/EXTERNAL_LIBRARIES_LICENSES.md"
cat > "$FINAL_DIST/bin/clyth_compiler_bin" <<'WRAPPER'
#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SOURCE=""
OUTPUT="a.out"
STAGE0="$ROOT/libexec/clyth-stage0"
MODULE_STORE="$ROOT/share/clyth/module-store"
EMIT_IR_ONLY=0

while [[ $# -gt 0 ]]; do
  case "$1" in
    -c)
      [[ $# -ge 2 ]] || { printf 'missing source file after -c\n' >&2; exit 2; }
      SOURCE="$2"; shift 2 ;;
    -o)
      [[ $# -ge 2 ]] || { printf 'missing output name after -o\n' >&2; exit 2; }
      OUTPUT="$2"; shift 2 ;;
    --stage0)
      [[ $# -ge 2 ]] || { printf 'missing compiler path after --stage0\n' >&2; exit 2; }
      STAGE0="$2"; shift 2 ;;
    --module-store)
      [[ $# -ge 2 ]] || { printf 'missing path after --module-store\n' >&2; exit 2; }
      MODULE_STORE="$2"; shift 2 ;;
    --emit-ir-only) EMIT_IR_ONLY=1; shift ;;
    -h|--help)
      printf 'usage: clyth_compiler_bin -c <source> -o <output> [--stage0 <compiler>] [--module-store <path>] [--emit-ir-only]\n'
      exit 0 ;;
    -v|--version) printf 'Clyth bootstrap compiler 0.6.0-stage2\n'; exit 0 ;;
    *) printf 'unrecognized argument: %s\n' "$1" >&2; exit 2 ;;
  esac
done

[[ -n "$SOURCE" ]] || { printf 'no source file supplied\n' >&2; exit 2; }
SOURCE="$(realpath -m "$SOURCE")"
OUTPUT="$(realpath -m "$OUTPUT")"
STAGE0="$(realpath -m "$STAGE0")"
MODULE_STORE="$(realpath -m "$MODULE_STORE")"
printf '%s\n%s\n%s\n%s\n%s\n' \
  "$SOURCE" "$OUTPUT" "$STAGE0" "$MODULE_STORE" "$EMIT_IR_ONLY" \
  > "$ROOT/share/clyth/stage2.request"
(cd "$ROOT" && exec "$ROOT/libexec/clyth-stage2-driver")
WRAPPER
chmod +x "$FINAL_DIST/bin/clyth_compiler_bin" "$FINAL_DIST/libexec/clyth-stage2-driver"
cp "$PROJECT_ROOT/LICENSE" "$FINAL_DIST/LICENSE"
cp "$PROJECT_ROOT/README.md" "$FINAL_DIST/README.md"
cp "$PROJECT_ROOT/install.sh" "$FINAL_DIST/install.sh"
cp "$PROJECT_ROOT/uninstall.sh" "$FINAL_DIST/uninstall.sh"
chmod +x "$FINAL_DIST/bin/clyth_compiler_bin" "$FINAL_DIST/libexec/clyth-stage2-driver" "$FINAL_DIST/install.sh" "$FINAL_DIST/uninstall.sh"

rm -f "$FINAL_TARBALL"
tar -C "$PROJECT_ROOT" -czf "$FINAL_TARBALL" "$(basename "$FINAL_DIST")"
printf 'Built Stage 2 distribution: %s\n' "$FINAL_DIST"
printf 'Built Stage 2 tarball: %s\n' "$FINAL_TARBALL"
