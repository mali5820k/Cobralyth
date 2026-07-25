#!/usr/bin/env bash
# C compiler shim used by CMake or helper scripts. ZIG may override the default
# executable while all remaining arguments are forwarded unchanged.
set -Eeuo pipefail
exec "${ZIG:-zig}" cc "$@"
