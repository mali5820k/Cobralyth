#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
GO_ROOT="$ROOT/compiler/go"

printf '==> Formatting bootstrap compiler\n'
(
  cd "$GO_ROOT"
  go fmt ./...
)

printf '==> Testing bootstrap compiler\n'
(
  cd "$GO_ROOT"
  go test ./...
)

printf '==> Vetting bootstrap compiler\n'
(
  cd "$GO_ROOT"
  go vet ./...
)

printf '==> Building bootstrap compiler\n'
"$ROOT/scripts/build-bootstrap.sh"

printf '\nBootstrap compiler checks passed.\n'
printf 'Compiler: %s\n' "$ROOT/dist/bootstrap/clyth"
printf 'Registry: %s\n' "$ROOT/dist/bootstrap/clyth-registry"
