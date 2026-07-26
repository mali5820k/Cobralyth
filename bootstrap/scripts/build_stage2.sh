#!/usr/bin/env bash
# Build the final 0.6.0 compiler with the validated Stage 1 compiler.
# Stage 2 receives its own distribution while consuming the runtime/module store
# assembled during Stage 1, keeping bootstrap and product ownership separate.
set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
STAGE2_DIR="$PROJECT_ROOT/0.6.0-compiler"
STAGE1_DIST="${CLYTH_STAGE1_DIST:-$PROJECT_ROOT/bootstrap/0.5.1-compiler/dist}"
STAGE1="${CLYTH_STAGE1_COMPILER:-$STAGE1_DIST/bin/clyth-stage1}"
STAGE0="${CLYTH_STAGE0_COMPILER:-$PROJECT_ROOT/bootstrap/0.5.0-compiler/dist/bin/clyth_compiler_bin}"
MODULE_STORE="${CLYTH_MODULE_STORE:-$STAGE1_DIST/share/clyth/module-store}"
BUILD="$STAGE2_DIR/.build"
FINAL_DIST="$STAGE2_DIR/dist"
FINAL_TARBALL="$PROJECT_ROOT/clyth-0.6.0-compiler-dist.tar.gz"

# Direct invocation must work after cleanup or from a fresh checkout.
mkdir -p "$STAGE1_DIST" "$FINAL_DIST" "$PROJECT_ROOT/build"

[[ -x "$STAGE1" ]] || { printf 'missing Stage 1 compiler: %s\n' "$STAGE1" >&2; exit 1; }
[[ -x "$STAGE0" ]] || { printf 'missing frozen Stage 0 compiler: %s\n' "$STAGE0" >&2; exit 1; }
[[ -d "$MODULE_STORE" ]] || { printf 'missing Stage 1 module store: %s\n' "$MODULE_STORE" >&2; exit 1; }

rm -rf "$BUILD" "$FINAL_DIST"
mkdir -p "$BUILD" "$FINAL_DIST/bin" "$FINAL_DIST/share/clyth"

"$STAGE1" -c "$STAGE2_DIR/src/main.clyth" -o "$BUILD/clyth_compiler_bin"

cp "$BUILD/clyth_compiler_bin" "$FINAL_DIST/bin/"
cp -R "$MODULE_STORE" "$FINAL_DIST/share/clyth/module-store"
mkdir -p "$FINAL_DIST/share/clyth/clyth-runtime"
LICENSE_MANIFEST="$STAGE2_DIR/EXTERNAL_LIBRARIES_LICENSES.md"
[[ -f "$LICENSE_MANIFEST" ]] || { printf 'missing Stage 2 license manifest: %s\n' "$LICENSE_MANIFEST" >&2; exit 1; }

# Install the complete Stage 2 notice set at the distribution root. Keep the
# runtime-path copy for compatibility with existing tooling that reads it there.
cp "$LICENSE_MANIFEST" "$FINAL_DIST/share/clyth/EXTERNAL_LIBRARIES_LICENSES.md"
cp "$LICENSE_MANIFEST" "$FINAL_DIST/share/clyth/clyth-runtime/EXTERNAL_LIBRARIES_LICENSES.md"

# The source archive currently names the project license LICENSE. Accept either
# spelling so packaging remains robust if it is later renamed to LICENSE.md.
PROJECT_LICENSE="$PROJECT_ROOT/LICENSE"
[[ -f "$PROJECT_LICENSE" ]] || PROJECT_LICENSE="$PROJECT_ROOT/LICENSE.md"
[[ -f "$PROJECT_LICENSE" ]] || { printf 'missing project license at repository root\n' >&2; exit 1; }
cp "$PROJECT_LICENSE" "$FINAL_DIST/LICENSE.md"
cp "$PROJECT_ROOT/README.md" "$FINAL_DIST/README.md"
cp "$PROJECT_ROOT/install.sh" "$FINAL_DIST/install.sh"
cp "$PROJECT_ROOT/uninstall.sh" "$FINAL_DIST/uninstall.sh"
chmod +x "$FINAL_DIST/bin/clyth_compiler_bin" "$FINAL_DIST/install.sh" "$FINAL_DIST/uninstall.sh"

"$SCRIPT_DIR/audit_no_gnu_abi.sh" "$FINAL_DIST"

rm -f "$FINAL_TARBALL"
tar -C "$FINAL_DIST" -czf "$FINAL_TARBALL" .
printf 'Built Stage 2 distribution: %s\n' "$FINAL_DIST"
printf 'Built Stage 2 tarball: %s\n' "$FINAL_TARBALL"
