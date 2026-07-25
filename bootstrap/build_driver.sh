#!/usr/bin/env bash
# Orchestrate the complete 0.5.0 -> 0.5.1 -> 0.6.0 bootstrap lifecycle.
# Each stage is built before it is used to compile the next generation, and
# both self-hosted generations are regression-tested before continuing.
set -Eeuo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Stage-specific scripts create their own build and distribution directories.
# Keeping directory ownership local prevents cleanup from needing placeholders.
"$HERE/scripts/build_stage0.sh"
"$HERE/scripts/build_stage1.sh"
"$HERE/scripts/test_stage1.sh"
"$HERE/scripts/build_stage2.sh"
"$HERE/scripts/test_stage2.sh"

printf 'Bootstrap chain completed: 0.5.0 -> 0.5.1 -> 0.6.0\n'
