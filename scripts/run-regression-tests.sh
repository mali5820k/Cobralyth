#!/usr/bin/env bash
set -Eeuo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
COMPILER="${CLYTH_TEST_COMPILER:-}"
[[ -n "$COMPILER" ]] || { printf 'error: CLYTH_TEST_COMPILER must explicitly identify the compiler under test\n' >&2; exit 2; }
SAMPLES="$ROOT/sample-clyth-programs"
STAGE1_REGRESSIONS="$ROOT/compiler/clyth/stage1-tests/regression"
[[ -x "$COMPILER" ]] || { printf 'error: compiler not found: %s\n' "$COMPILER" >&2; exit 1; }
printf 'Testing compiler: %s\n' "$COMPILER"

run_set() {
  local label="$1" directory="$2"
  local passed=0 failed=0
  local case_log case_ir status
  case_log="$(mktemp "${TMPDIR:-/tmp}/clyth-regression.XXXXXX.log")"
  case_ir="$(mktemp "${TMPDIR:-/tmp}/clyth-regression.XXXXXX.ll")"
  trap 'rm -f "$case_log" "$case_ir"' RETURN
  while IFS= read -r -d '' source; do
    rel="${source#$ROOT/}"
    : > "$case_log"
    rm -f "$case_ir"
    set +e
    "$COMPILER" build "$source" --emit-llvm-ir -o "$case_ir" >"$case_log" 2>&1
    status=$?
    set -e
    if ((status == 0)); then
      printf 'PASS %s\n' "$rel"; passed=$((passed+1))
    else
      printf 'FAIL %s (rc=%d)\n' "$rel" "$status"; failed=$((failed+1))
      if [[ -s "$case_log" ]]; then
        printf '%s\n' '  --- compiler diagnostic (tail) ---'
        tail -n 30 "$case_log" | sed 's/^/  /'
        printf '%s\n' '  --- end diagnostic ---'
      fi
    fi
  done < <(find "$directory" -maxdepth 1 -type f -name '*.clyth' -print0 | sort -z)
  rm -f "$case_log" "$case_ir"
  trap - RETURN
  printf '%s: %d passed, %d failed\n' "$label" "$passed" "$failed"
  ((failed==0))
}

overall_status=0
if ! run_set 'Regression summary' "$SAMPLES"; then
  overall_status=1
fi
if [[ -d "$STAGE1_REGRESSIONS" ]]; then
  if ! run_set 'Stage-1 targeted regressions' "$STAGE1_REGRESSIONS"; then
    overall_status=1
  fi
fi
exit "$overall_status"
