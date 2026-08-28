#!/usr/bin/env bash
set -Eeuo pipefail
: "${CLYTH_ZIG:=zig}"
exec "$CLYTH_ZIG" ar "$@"
