#!/usr/bin/env bash
set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
RUNTIME_ROOT="${1:-$PROJECT_ROOT/bootstrap/0.5.1-compiler/clyth-runtime}"

LIBUV_VERSION="${LIBUV_VERSION:-v1.52.1}"
LLHTTP_VERSION="${LLHTTP_VERSION:-release/v9.4.2}"
OPENSSL_VERSION="${OPENSSL_VERSION:-openssl-4.0.1}"
WSLAY_VERSION="${WSLAY_VERSION:-release-1.1.1}"
YYJSON_VERSION="${YYJSON_VERSION:-0.12.0}"
RAPIDHASH_VERSION="${RAPIDHASH_VERSION:-v4}"

command -v git >/dev/null 2>&1 || { printf 'missing required build tool: git\n' >&2; exit 1; }

checkout_ref() {
  local name="$1" url="$2" ref="$3" sentinel="$4"
  local target="$RUNTIME_ROOT/c-bindings/$name/vendor/$name"
  local marker="$target/.clyth_vendor_ref"

  if [[ -f "$target/$sentinel" && -f "$marker" && "$(cat "$marker")" == "$ref" ]]; then
    printf 'Runtime vendor already present: %s %s\n' "$name" "$ref"
    return
  fi

  rm -rf "$target"
  mkdir -p "$(dirname "$target")"
  git clone --quiet --filter=blob:none --no-checkout "$url" "$target"
  if git -C "$target" ls-remote --exit-code --tags origin "refs/tags/$ref" >/dev/null 2>&1; then
    git -C "$target" fetch --quiet --depth 1 origin "refs/tags/$ref:refs/tags/$ref"
    git -C "$target" checkout --quiet "refs/tags/$ref^{}"
  else
    git -C "$target" fetch --quiet --depth 1 origin "$ref"
    git -C "$target" checkout --quiet FETCH_HEAD
  fi
  [[ -f "$target/$sentinel" ]] || { printf 'vendor sentinel missing after checkout: %s/%s\n' "$target" "$sentinel" >&2; exit 1; }
  printf '%s\n' "$ref" > "$marker"
  printf 'Fetched runtime vendor: %s %s\n' "$name" "$ref"
}

checkout_ref libuv https://github.com/libuv/libuv.git "$LIBUV_VERSION" include/uv.h
checkout_ref llhttp https://github.com/nodejs/llhttp.git "$LLHTTP_VERSION" include/llhttp.h
checkout_ref openssl https://github.com/openssl/openssl.git "$OPENSSL_VERSION" VERSION.dat
checkout_ref wslay https://github.com/tatsuhiro-t/wslay.git "$WSLAY_VERSION" lib/includes/wslay/wslay.h
checkout_ref yyjson https://github.com/ibireme/yyjson.git "$YYJSON_VERSION" src/yyjson.h
checkout_ref rapidhash https://github.com/Nicoshev/rapidhash.git "$RAPIDHASH_VERSION" rapidhash.h
