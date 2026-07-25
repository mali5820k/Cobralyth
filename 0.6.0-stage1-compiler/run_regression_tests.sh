#!/usr/bin/env bash
set -Eeuo pipefail

STAGE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
COMPILER="$STAGE_DIR/dist/bin/clyth-stage1"
SAMPLE_DIR="$STAGE_DIR/sample-clyth-programs-0.6"
BUILD_DIR="$STAGE_DIR/.regression-build"
SEPARATOR="------------------------------------------------------------"

PASSED=0
FAILED=0
TOTAL=0

indent_file() {
    sed 's/^/        /' "$1"
}

run_case() {
    local source="$1"
    local filename id stem binary compile_log run_log output expected diff_log
    filename="$(basename "$source")"
    id="${filename%%_*}"
    stem="${filename%.clyth}"
    binary="$BUILD_DIR/$stem"
    compile_log="$BUILD_DIR/$stem.compile.log"
    run_log="$BUILD_DIR/$stem.run.log"
    output="$BUILD_DIR/$stem.out"
    expected="$SAMPLE_DIR/$stem.expected"
    diff_log="$BUILD_DIR/$stem.diff"

    TOTAL=$((TOTAL + 1))
    echo "$SEPARATOR"
    echo "[ STAGE 1 TEST $id ] $filename"
    echo "        Compiling with: $COMPILER"

    if ! "$COMPILER" -c "$source" -o "$binary" >"$compile_log" 2>&1; then
        FAILED=$((FAILED + 1))
        echo "        [ FAILED ] Compile failed."
        indent_file "$compile_log"
        return
    fi

    if [[ ! -x "$binary" ]]; then
        FAILED=$((FAILED + 1))
        echo "        [ FAILED ] Compiler returned success without producing an executable."
        indent_file "$compile_log"
        return
    fi

    echo "        Running: $binary"
    set +e
    (cd "$(dirname "$source")" && "$binary") >"$output" 2>"$run_log"
    local rc=$?
    set -e

    if [[ "$rc" -ne 0 ]]; then
        FAILED=$((FAILED + 1))
        echo "        [ FAILED ] Program returned $rc."
        [[ ! -s "$output" ]] || { echo "        stdout:"; indent_file "$output"; }
        [[ ! -s "$run_log" ]] || { echo "        stderr:"; indent_file "$run_log"; }
        return
    fi

    if [[ -f "$expected" ]] && ! diff -u "$expected" "$output" >"$diff_log" 2>&1; then
        FAILED=$((FAILED + 1))
        echo "        [ FAILED ] Output differed from expected."
        indent_file "$diff_log"
        return
    fi

    PASSED=$((PASSED + 1))
    echo "        [ OK ] Test passed."
}

main() {
    [[ -x "$COMPILER" ]] || { echo "ERROR: missing Stage 1 compiler: $COMPILER" >&2; echo "Run ./build_stage1.sh first." >&2; exit 1; }
    [[ -d "$SAMPLE_DIR" ]] || { echo "ERROR: missing Stage 1 samples: $SAMPLE_DIR" >&2; exit 1; }

    rm -rf "$BUILD_DIR"
    mkdir -p "$BUILD_DIR"

    mapfile -t samples < <(find "$SAMPLE_DIR" -maxdepth 1 -type f -name '[0-9][0-9]_*.clyth' | sort)
    [[ "${#samples[@]}" -gt 0 ]] || { echo "ERROR: no Stage 1 samples found." >&2; exit 1; }

    local source
    for source in "${samples[@]}"; do
        run_case "$source"
    done

    echo "$SEPARATOR"
    echo "Stage 1 Test Summary"
    echo "        Compiler  : $COMPILER"
    echo "        Tests Run : $TOTAL"
    echo "        Passed    : $PASSED"
    echo "        Failed    : $FAILED"
    if [[ "$FAILED" -eq 0 ]]; then
        echo "        Overall   : SUCCESS"
        echo "$SEPARATOR"
        return 0
    fi
    echo "        Overall   : FAILED"
    echo "$SEPARATOR"
    return 1
}

main "$@"
