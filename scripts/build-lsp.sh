#!/usr/bin/env sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD="$ROOT/lsp/.build/bin"
mkdir -p "$BUILD"
cd "$ROOT/lsp/server"

build() {
    goos=$1
    goarch=$2
    out=$3
    printf 'Building clyth-lsp for %s/%s...\n' "$goos" "$goarch"
    CGO_ENABLED=0 GOOS="$goos" GOARCH="$goarch" go build -trimpath -ldflags='-s -w' -o "$BUILD/$out" ./cmd/clyth-lsp
}

build linux amd64 linux-x64/clyth-lsp
build linux arm64 linux-arm64/clyth-lsp
build darwin amd64 darwin-x64/clyth-lsp
build darwin arm64 darwin-arm64/clyth-lsp
build windows amd64 win32-x64/clyth-lsp.exe

printf 'LSP binaries built under %s\n' "$BUILD"
