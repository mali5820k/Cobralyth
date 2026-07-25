#!/usr/bin/env bash
# Public entry point for building and validating the complete compiler chain.
# All implementation details remain under bootstrap/ so callers need only know
# this one command.
set -Eeuo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
exec "$ROOT/bootstrap/build_driver.sh" "$@"
