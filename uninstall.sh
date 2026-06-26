#!/usr/bin/env bash
set -euo pipefail

INSTALL_ROOT="${CLYTH_INSTALL_ROOT:-$HOME/.local/share/clyth}"
BIN_DIR="${CLYTH_BIN_DIR:-$HOME/.local/bin}"

rm -f "$BIN_DIR/clyth"
rm -rf "$INSTALL_ROOT"

echo "Removed Clyth from: $INSTALL_ROOT"
echo "Removed symlink: $BIN_DIR/clyth"
