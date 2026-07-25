#!/usr/bin/env bash
# Inspect ELF files in a compiler distribution and reject dependencies on the
# GNU C/C++ ABI. This protects the intended musl-oriented release boundary.
set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
DIST="${1:-$PROJECT_ROOT/0.6.0-compiler/dist}"

[[ -d "$DIST" ]] || {
  printf 'missing distribution: %s\n' "$DIST" >&2
  exit 1
}
command -v file >/dev/null 2>&1 || { printf 'missing required tool: file\n' >&2; exit 1; }
command -v readelf >/dev/null 2>&1 || { printf 'missing required tool: readelf\n' >&2; exit 1; }

status=0
while IFS= read -r -d '' candidate; do
  # Non-ELF files are part of the distribution but do not have dynamic tags.
  if file "$candidate" | grep -q ELF && \
     readelf -d "$candidate" 2>/dev/null | grep -Eq 'libstdc\+\+|libgcc_s|GLIBC_'; then
    printf 'GNU ABI dependency found: %s\n' "$candidate" >&2
    status=1
  fi
done < <(find "$DIST" -type f -print0)

exit "$status"
