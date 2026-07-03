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
    local id="$1"
    local source="$2"
    local expected="$3"
    local stem="${source%.clyth}"
    local bin="$BUILD_DIR/$stem"
    local out="$BUILD_DIR/$stem.out"
    local compile_log="$BUILD_DIR/$stem.compile.log"
    local run_log="$BUILD_DIR/$stem.run.log"

    TOTAL=$((TOTAL + 1))

    echo "$SEPARATOR"
    echo "[ TEST $id ] $source"
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

    run_case "01" "01_0_1_printf_extern_c.clyth" "Simplest extern-C function hookup" || true
    run_case "02" "02_0_2_dynamic_array.clyth" "middle: 42" || true
    run_case "03" "03_0_2_native_string.clyth" "updated length: 22" || true
    run_case "04" "04_0_4_runtime_list_collection.clyth" "middle after add: 42" || true
    run_case "05" "05_0_4_runtime_set_collection.clyth" "contains 99: 0" || true
    run_case "06" "06_0_4_runtime_map_collection.clyth" "value 4: 44" || true
    run_case "07" "07_0_2_struct_methods_release.clyth" "counter: 42" || true
    run_case "08" "08_0_4_runtime_dma_include.clyth" "dma runtime include linked" || true
    run_case "09" "09_0_4_generic_runtime_package_include.clyth" "generic runtime templates accepted" || true
    run_case "10" "10_0_4_runtime_collection_constructors.clyth" "constructor map value 10: 101" || true
    run_case "11" "11_0_4_generic_user_templates.clyth" "generic pair value: generic" || true

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
