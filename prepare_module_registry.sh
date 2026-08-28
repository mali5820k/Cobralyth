#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CLYTH_BIN="${CLYTH_BOOTSTRAP_BIN:-$ROOT/dist/bootstrap/clyth}"
REGISTRY_BIN="${CLYTH_REGISTRY_BIN:-$ROOT/dist/bootstrap/clyth-registry}"
[[ -x "$CLYTH_BIN" && -x "$REGISTRY_BIN" ]] || "$ROOT/scripts/build-bootstrap.sh"

for group in core runtime; do
  src="$ROOT/module-src/$group"
  dst="$ROOT/modules/$group"
  rm -rf "$dst"; mkdir -p "$dst"
  while IFS= read -r -d '' module; do
    [[ -f "$module/module-info.json" ]] || continue
    archive="$($CLYTH_BIN package "$module")"
    cp "$archive" "$dst/"
  done < <(find "$src" -mindepth 1 -maxdepth 1 -type d -print0 | sort -z)
  "$REGISTRY_BIN" "$dst"
done
printf 'Prepared module registries under %s/modules\n' "$ROOT"
