#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

clean_vendors=0
while (($#)); do
  case "$1" in
    --vendors) clean_vendors=1 ;;
    -h|--help)
      cat <<'USAGE'
Usage: ./scripts/clean.sh [--vendors]

Remove generated compiler, LSP, module, and release outputs.
Downloaded/pinned vendor source under module-src/vendor is preserved by default.
Use --vendors only when a fresh vendor fetch is intentionally required.
USAGE
      exit 0
      ;;
    *) printf 'error: unknown clean option: %s\n' "$1" >&2; exit 2 ;;
  esac
  shift
done

rm -rf \
  "$ROOT/dist" \
  "$ROOT/modules" \
  "$ROOT/lsp/.build" \
  "$ROOT/lsp/dist" \
  "$ROOT/lsp/editors/vscode/bin"
find "$ROOT/module-src" -type d \( -name build -o -name artifacts \) -prune -exec rm -rf {} +

if ((clean_vendors != 0)); then
  rm -rf "$ROOT/module-src/vendor"
  printf 'Removed generated outputs and pinned vendor source cache.\n'
else
  printf 'Removed generated outputs; preserved module-src/vendor.\n'
fi
