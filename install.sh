#!/usr/bin/env bash
# Install only the final Stage 2 distribution. Bootstrap compiler generations
# are implementation details and are never copied into the user's prefix.
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DIST="$ROOT/0.6.0-compiler/dist"
[[ -d "$DIST" ]] || {
  printf 'missing bootstrapped compiler distribution: %s\n' "$DIST" >&2
  exit 1
}

# Prefer a distribution-owned installer when packaging supplies one.
if [[ -x "$DIST/install.sh" ]]; then
  exec "$DIST/install.sh" "$@"
fi

PREFIX="${PREFIX:-$HOME/.local}"
mkdir -p "$PREFIX"
cp -a "$DIST/." "$PREFIX/"
