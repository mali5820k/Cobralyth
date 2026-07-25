#!/usr/bin/env bash
# Convenience entry point kept beside the final compiler sources.
# The canonical implementation lives under bootstrap/scripts.
set -Eeuo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
exec "$HERE/../bootstrap/scripts/build_stage2.sh" "$@"
