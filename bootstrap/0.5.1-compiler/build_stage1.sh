#!/usr/bin/env bash
# Convenience entry point kept beside the Stage 1 sources.
# The canonical implementation lives in bootstrap/scripts to centralize paths.
set -Eeuo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
exec "$HERE/../scripts/build_stage1.sh" "$@"
