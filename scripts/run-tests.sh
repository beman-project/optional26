#!/bin/bash

# scripts/run-tests.sh -*-shell-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# Run tests on all known toolchains.
# Expected to be run from the root of the project.
# Expected all configurations to be present in the CMakeLists.txt file and installed on the system.

set -e

# Always run from the root of the project.
REPO_ROOT="$(dirname "$(realpath $0)")/.."
cd "${REPO_ROOT}"

# Toolchains to test.
ALL_TOOLCHAINS=(
    gcc-14
    gcc-13
    clang-19
    clang-18
    clang-17
)

# Build directory.
BUILD_DIR=".build"

# CMake configurations to test.
ALL_CONFIGURATIONS=(
    "Debug"
    "Release"
    "RelWithDebInfo"
    "Asan"
)

# Actual cmake configuration to use.
CMAKE_CONFIGURATION="RelWithDebInfo"

# C++ standards to test.
ALL_STANDARDS=(
    20
    23
    26
)
# Default standard to use.
CXX_STANDARD=23

# Print print_usage information.
function print_usage() {
    echo "Usage: $0"
    echo "  -h, --help: Display help."
    echo "  -v, --verbose: Display verbose output."
    echo "  -f, --fresh: Run tests on a fresh build directory."
    echo "  -p, --preset: Run tests on a specific toolchain. Default: all"
    echo "                Available presets: ${ALL_TOOLCHAINS[*]}"
    echo "  -c, --config: Run tests on a specific configuration. Default: ${CMAKE_CONFIGURATION}"
    echo "                Available configurations: ${ALL_CONFIGURATIONS[*]}"
    echo "  -s, --std: Run tests with a specific C++ standard. Default: ${CXX_STANDARD}"
    echo "                Available standards: ${ALL_STANDARDS[*]}"
}

# Parse command line arguments.
function parse_args() {
    # Ff no arguments are provided, print print_usage and exit.
    if [[ $# -eq 0 ]]; then
        print_usage
        exit 1
    fi

    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                print_usage
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
            -p|--preset)
                if [ "$2" == "all" ]; then
                    TOOLCHAINS=("${ALL_TOOLCHAINS[@]}")
                else
                    TOOLCHAINS=("$2")
                fi
                shift
                shift
                ;;
            -c|--config)
                CMAKE_CONFIGURATION="$2"
                shift
                shift
                ;;
            -s|--std)
                CXX_STANDARD="$2"
                shift
                shift
                ;;
            *)
                echo "Unknown option: $1"
                print_usage
                exit 1
                ;;
        esac
    done

}

# Run a command and print a message on success or failure.
function run_command() {
    # Command to run.
    cmd="$1"
    if [ ! "$VERBOSE" ]; then
        cmd+=" &> /dev/null"
    fi

    # Messages.
    status_message="$2"
    failure_message="$3"
    successful_message="$4"

    echo "Running: ${status_message}"
    if ! eval "${cmd}"; then
        echo " -> ${failure_message}"
        return 1;
    else
        echo " -> ${successful_message}"
        return 0;
    fi
}

# Run tests with a specific toolchain.
function test_with_toolchain() {
    # Always run from the root of the project.
    cd "${REPO_ROOT}"

    toolchain="etc/$1-toolchain.cmake"
    echo "Testing with ${toolchain}..."
    if [ ! -f "${toolchain}" ]; then
        echo "Toolchain file not found: ${toolchain}"
        exit 1
    fi

    # If FRESH is set or having multiple toolchain, remove the existing build directory.
    if [ "$FRESH" ] || [ "${#TOOLCHAINS[@]}" -gt 1 ] ; then
        echo "[WARNING] Removing existing build directory: ${BUILD_DIR} ..."
        rm -rf "${BUILD_DIR}"
        mkdir -p "${BUILD_DIR}"
    fi

    # Configure.
    cd "${BUILD_DIR}"
    cmd="cmake -G \"Ninja Multi-Config\"  -DCMAKE_CONFIGURATION_TYPES=\"RelWithDebInfo;Asan;Debug;Release\" -DCMAKE_TOOLCHAIN_FILE=\"${toolchain}\" -DCMAKE_CXX_STANDARD=\"${CXX_STANDARD}\" -B . -S .."
    run_command "${cmd}" "config"  "config failed" "config successful" || return 1

    # Build.
    cd ..
    cmd="cmake --build \"${BUILD_DIR}\" --config \"${CMAKE_CONFIGURATION}\" --target all -- -k 0"
    run_command "${cmd}" "build"  "build failed" "build successful" || return 1

    # Run tests.
    cd "${BUILD_DIR}"
    cmd="ctest --output-on-failure"
    run_command "${cmd}" "tests"  "tests failed" "tests passed" || return 1
    cd ..
}

# Run tests with all toolchains.
function test_all_toolchains() {
    for toolchain in "${TOOLCHAINS[@]}"; do
        cmd="test_with_toolchain \"${toolchain}\" \"${CMAKE_ARGS}\""
        # If multiple toolchains are provided, continue to the next toolchain.
        # Otherwise, stop the script on the first failure.
        if [ "${#TOOLCHAINS[@]}" -gt 1 ]; then
            cmd+=" || true"
        fi
        eval "${cmd}"

        echo "Done testing with ${toolchain}."
        echo ""
    done
}

parse_args "$@"
test_all_toolchains
echo "$0 all jobs are done."
