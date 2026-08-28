#!/usr/bin/env bash
# Install the current Clyth distribution.
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DIST="$ROOT/dist"
[[ -d "$DIST" ]] || {
  printf 'missing Clyth distribution: %s\n' "$DIST" >&2
  exit 1
}

# Prefer a distribution-owned installer when packaging supplies one.
if [[ -x "$DIST/install.sh" ]]; then
  exec "$DIST/install.sh" "$@"
fi

PREFIX="${PREFIX:-$HOME/.local}"
mkdir -p "$PREFIX"
cp -a "$DIST/." "$PREFIX/"
