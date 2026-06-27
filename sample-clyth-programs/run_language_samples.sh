#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
COMPILER="$ROOT_DIR/build-compiler/clyth_compiler_bin"
SAMPLE_DIR="$ROOT_DIR/sample-clyth-programs"
BUILD_DIR="$ROOT_DIR/sample-clyth-programs/.sample-bin"
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
    local name="$1"
    local source="$2"
    local expected="$3"
    local bin="$BUILD_DIR/$name"
    local out="$BUILD_DIR/$name.out"
    local compile_log="$BUILD_DIR/$name.compile.log"
    local run_log="$BUILD_DIR/$name.run.log"

    TOTAL=$((TOTAL + 1))

    echo "$SEPARATOR"
    echo "[ TEST ] $source"
    echo "        Compiling..."

    if ! "$COMPILER" -c "$SAMPLE_DIR/$source" -o "$bin" >"$compile_log" 2>&1; then
        FAILED=$((FAILED + 1))
        echo "        [ FAILED ] Compile failed."
        echo "        Compiler output:"
        indent_file "$compile_log"
        echo "$SEPARATOR"
        return 1
    fi

    echo "        Running..."
    if ! "$bin" >"$out" 2>"$run_log"; then
        FAILED=$((FAILED + 1))
        echo "        [ FAILED ] Program exited with an error."
        echo "        Program stdout:"
        indent_file "$out"
        echo "        Program stderr:"
        indent_file "$run_log"
        echo "$SEPARATOR"
        return 1
    fi

    if ! grep -Fq "$expected" "$out"; then
        FAILED=$((FAILED + 1))
        echo "        [ FAILED ] Expected output was not found."
        echo "        Expected to find: $expected"
        echo "        Actual output:"
        indent_file "$out"
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

    run_case "printf_test" "printf_test.clyth" "Simplest extern-C function hookup" || true
    run_case "dynamic_array_test" "dynamic_array_test.clyth" "middle: 42" || true
    run_case "native_string_test" "native_string_test.clyth" "updated length: 22" || true
    run_case "list_container_test" "list_container_test.clyth" "middle after add: 42" || true
    run_case "set_container_test" "set_container_test.clyth" "contains 99: 0" || true
    run_case "map_container_test" "map_container_test.clyth" "value 4: 44" || true
    run_case "struct_release_test" "alpha_020_struct_release_test.clyth" "counter: 42" || true
    run_case "runtime_dma_include_test" "runtime_dma_include_test.clyth" "dma runtime include linked" || true
    run_case "generic_runtime_template_include_test" "generic_runtime_template_include_test.clyth" "generic runtime templates accepted" || true
    run_case "runtime_collection_constructor_test" "runtime_collection_constructor_test.clyth" "constructor map value 10: 101" || true
    run_case "generic_user_template_test" "generic_user_template_test.clyth" "generic pair value: generic" || true

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
