#!/bin/bash

# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# Run tests on all known toolchains.
# Expected to be run from the root of the project.
# Expected all configurations to be present in the CMakeLists.txt file and installed on the system.

set -e

# Toolchains to test.
TOOLCHAINS=(
    gcc-14
    gcc-13
    clang-19
    clang-18
    clang-17
)

# Build directory.
BUILD_DIR=".build"

# Print usage information.
function usage() {
    echo "Usage: $0"
    echo "  -h, --help: Display help."
    echo "  -v, --verbose: Display verbose output."
    echo "  -f, --fresh: Run tests on a fresh build directory."
}

# Parse command line arguments.
function parse_args() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                usage
                exit 0
                ;;
            -v|--verbose)
                VERBOSE=true
                shift
                ;;
            -f|--fresh)
                FRESH=true
                shift
                ;;
            *)
                echo "Unknown option: $1"
                usage
                exit 1
                ;;
        esac
    done

}

# Run tests with a specific toolchain.
function test_with_toolchain() {
    toolchain=$1
    echo "Testing with ${toolchain}..."

    cmd="cmake --workflow --preset ${toolchain}"
    if [ ! "$VERBOSE" ]; then
        cmd+="&> /dev/null"
    fi

    if ! eval "${cmd}"; then
        echo " -> failed"
        exit 1
    else
        echo " -> passed"
    fi
}

# Run tests with all toolchains.
function test_all_toolchains() {
    if [ "$FRESH" ]; then
        echo " -> cleaning build directory"
        rm -rf "${BUILD_DIR}"
    fi

    for toolchain in "${TOOLCHAINS[@]}"; do
        test_with_toolchain "${toolchain}"
    done

    echo "All tests passed on all toolchains."
}

parse_args "$@"
test_all_toolchains
