#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
COMPILER="$ROOT_DIR/build-compiler/clyth_compiler_bin"
SAMPLE_DIR="$ROOT_DIR/sample-clyth-programs"
BUILD_DIR="$SAMPLE_DIR/.sample-bin"
SEPARATOR="------------------------------------------------------------"

PASSED=0
FAILED=0
TOTAL=0

mkdir -p "$BUILD_DIR"
trap 'rm -rf "$BUILD_DIR"' EXIT

indent_file() {
    sed 's/^/        /' "$1"
}

run_case() {
    local source="$1"
    local filename
    filename="$(basename "$source")"
    local id="${filename%%_*}"
    local stem="${filename%.clyth}"
    local bin="$BUILD_DIR/$stem"
    local compile_log="$BUILD_DIR/$stem.compile.log"
    local run_log="$BUILD_DIR/$stem.run.log"
    local out="$BUILD_DIR/$stem.out"

    TOTAL=$((TOTAL + 1))

    echo "$SEPARATOR"
    echo "[ TEST $id ] $filename"
    echo "        Compiling..."

    if ! "$COMPILER" -c "$source" -o "$bin" >"$compile_log" 2>&1; then
        FAILED=$((FAILED + 1))
        echo "        [ FAILED ] Compile failed."
        echo "        Compiler output:"
        indent_file "$compile_log"
        echo "$SEPARATOR"
        return 1
    fi

    echo "        Running..."
    set +e
    "$bin" >"$out" 2>"$run_log"
    local rc=$?
    set -e

    if [[ "$rc" -ne 0 ]]; then
        FAILED=$((FAILED + 1))
        echo "        [ FAILED ] Program returned non-zero exit code: $rc"
        echo "        Program stdout:"
        indent_file "$out"
        echo "        Program stderr:"
        indent_file "$run_log"
        echo "$SEPARATOR"
        return 1
    fi

    rm -f "$bin" "$out" "$compile_log" "$run_log" "$bin.ll" "$bin.o"
    PASSED=$((PASSED + 1))
    echo "        [ OK ] Test passed."
    echo "$SEPARATOR"
}

main() {
    if [[ ! -x "$COMPILER" ]]; then
        echo "ERROR: compiler binary not found at $COMPILER"
        echo "Build first with: ./build_compiler.sh --skip-dist"
        exit 1
    fi

    mapfile -t samples < <(find "$SAMPLE_DIR" -maxdepth 1 -type f -name '[0-9][0-9]_*.clyth' | sort)
    if [[ "${#samples[@]}" -eq 0 ]]; then
        echo "ERROR: no numbered sample programs found in $SAMPLE_DIR"
        exit 1
    fi

    local source
    for source in "${samples[@]}"; do
        run_case "$source" || true
    done

    echo "$SEPARATOR"
    echo "Test Summary"
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
