#!/usr/bin/env bash
# Delegate uninstallation to the final distribution, which is the only layer
# capable of knowing exactly which files were installed.
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DIST="$ROOT/dist"
if [[ -x "$DIST/uninstall.sh" ]]; then
  exec "$DIST/uninstall.sh" "$@"
fi

printf 'The final distribution does not provide an uninstall manifest.\n' >&2
exit 1
