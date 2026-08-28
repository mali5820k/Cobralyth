#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
: "${CLYTH_CC:?}" "${CLYTH_AR:?}" "${CLYTH_ARTIFACT_ROOT:?}"
V="$($ROOT/fetch-vendors.sh)"
B="$ROOT/../build/vendor/yyjson"
rm -rf "$B"; mkdir -p "$B" "$CLYTH_ARTIFACT_ROOT"
"$CLYTH_CC" -std=c11 -O2 -fPIC -I"$V/src" -c "$V/src/yyjson.c" -o "$B/yyjson.o"
"$CLYTH_AR" rcs "$CLYTH_ARTIFACT_ROOT/libyyjson.a" "$B/yyjson.o"
