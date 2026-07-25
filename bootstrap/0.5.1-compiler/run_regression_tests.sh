#!/usr/bin/env bash
# Convenience entry point for the Stage 1 regression suite.
# Delegating avoids maintaining duplicate test logic in the compiler directory.
set -Eeuo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
exec "$HERE/../scripts/test_stage1.sh" "$@"
