#!/usr/bin/env bash
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

printf 'Formatting Go bootstrap compiler...\n'
(
  cd "$ROOT/compiler/go"
  go fmt ./...
)

printf 'Formatting Clyth LSP server...\n'
(
  cd "$ROOT/lsp/server"
  go fmt ./...
)

printf 'Go formatting complete.\n'
