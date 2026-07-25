#!/usr/bin/env bash
# Convenience entry point for the final compiler regression suite.
# Delegating keeps bootstrap orchestration and path handling in one location.
set -Eeuo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
exec "$HERE/../bootstrap/scripts/test_stage2.sh" "$@"
