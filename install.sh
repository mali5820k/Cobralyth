#!/usr/bin/env bash
set -euo pipefail

INSTALL_ROOT="${CLYTH_INSTALL_ROOT:-$HOME/.local/share/clyth}"
BIN_DIR="${CLYTH_BIN_DIR:-$HOME/.local/bin}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

mkdir -p "$INSTALL_ROOT" "$BIN_DIR"

# Install the current distribution directory contents. This script is intended
# to live inside clyth-dist. Avoid requiring rsync so the installer works on
# minimal Linux/WSL environments.
find "$INSTALL_ROOT" -mindepth 1 -maxdepth 1 -exec rm -rf {} +
cp -a "$SCRIPT_DIR/." "$INSTALL_ROOT/"

if [[ -x "$INSTALL_ROOT/clyth_compiler_bin" ]]; then
  ln -sf "$INSTALL_ROOT/clyth_compiler_bin" "$BIN_DIR/clyth"
elif [[ -x "$INSTALL_ROOT/bin/clyth_compiler_bin" ]]; then
  ln -sf "$INSTALL_ROOT/bin/clyth_compiler_bin" "$BIN_DIR/clyth"
else
  echo "[WARN] Could not find clyth_compiler_bin in $INSTALL_ROOT. Install copied files, but no clyth symlink was created." >&2
fi

echo "Installed Clyth to: $INSTALL_ROOT"
echo "Symlink target: $BIN_DIR/clyth"

case ":$PATH:" in
  *":$BIN_DIR:"*) ;;
  *)
    echo
    echo "Add this to your shell config if needed:"
    echo "export PATH=\"$BIN_DIR:\$PATH\""
    ;;
esac
