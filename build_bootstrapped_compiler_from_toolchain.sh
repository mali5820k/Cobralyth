#!/usr/bin/env bash
# Public entry point for building and validating the complete compiler chain.
# All implementation details remain under bootstrap/ so callers need only know
# this one command.
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

export CC="${CC:-$ROOT/bootstrap/scripts/zig-c.sh}"
export CXX="${CXX:-$ROOT/bootstrap/scripts/zig-c++.sh}"
export CLYTH_ZIG_TARGET="${CLYTH_ZIG_TARGET:-native-linux-musl}"
export CLYTH_ZIG_STATIC="${CLYTH_ZIG_STATIC:-1}"
export CLYTH_ZIG_CXX_STDLIB="${CLYTH_ZIG_CXX_STDLIB:-libc++}"

exec "$ROOT/bootstrap/build_driver.sh" "$@"
