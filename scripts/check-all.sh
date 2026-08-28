#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

printf '==============================\n'
printf ' Clyth developer quick check\n'
printf '==============================\n\n'

"$ROOT/scripts/check-bootstrap.sh"

printf '\n==> Testing Clyth LSP\n'
"$ROOT/scripts/test-lsp.sh"

printf '\nAll quick checks passed.\n'
printf 'Bootstrap compiler: %s\n' "$ROOT/dist/bootstrap/clyth"
printf 'Registry utility:   %s\n' "$ROOT/dist/bootstrap/clyth-registry"
printf 'LSP release packages are produced by scripts/package-lsp.sh.\n'
