#!/usr/bin/env sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$ROOT/lsp/server"
go test ./...
go vet ./...
printf 'clyth-lsp tests passed\n'
