#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT="${CLYTH_BOOTSTRAP_OUT:-$ROOT/dist/bootstrap}"
mkdir -p "$OUT"
cd "$ROOT/compiler/go"
CGO_ENABLED=0 go build -trimpath -o "$OUT/clyth" ./cmd/clyth
CGO_ENABLED=0 go build -trimpath -o "$OUT/clyth-registry" ./cmd/clyth-registry
printf 'Built Go bootstrap compiler: %s\n' "$OUT/clyth"
