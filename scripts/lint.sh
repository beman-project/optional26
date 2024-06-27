#!/bin/bash

# scripts/lint.sh -*-shell-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# Lint all files in the project.
# Expected to be run from the root of the project. 
# Expected tools to be installed on the system. A pre-check is done to ensure all tools are installed.

set -e

# Base directories to lint.
BASE_DIRS=(
    include
    examples
    scripts
    src
)

# Print usage information.
function print_usage() {
    echo "Usage: $0"
    echo "  -h, --help: Display help."
    echo "  -a, --all: Lint all files."
    echo "  -c, --cpp: Lint C++ files."
    echo "  -s, --shell: Lint shell scripts."
    echo "  -m, --markdown: Lint Markdown files."
    echo "  -y, --yaml: Lint YAML files."
    echo "  -C, --cmake: Lint CMake files."
}

# Parse command line arguments.
function parse_args() {
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                print_usage
                exit 0
                ;;
            -a|--all)
                lint_all_files
                exit 0
                ;;
            -c|--cpp)
                lint_cpp_files
                exit 0
                ;;
            -s|--shell)
                lint_shell_files
                exit 0
                ;;
            -m|--markdown)
                lint_markdown_files
                exit 0
                ;;
            -y|--yaml)
                lint_yaml_files
                exit 0
                ;;
            -C|--cmake)
                lint_cmake_files
                exit 0
                ;;
            *)
                echo "Unknown option: $1"
                print_usage
                exit 1
                ;;
        esac
    done
}

# Check if all linters are installed.
function check_linters_installed() {
    echo "Checking linters..."
    linters=(
        clang-format
        shellcheck
        cmake-format
        markdownlint
        yamlfmt
    )

    for linter in "${linters[@]}"; do
        echo " -> ${linter}..."
        if ! command -v "${linter}" &> /dev/null; then
            echo "${linter} could not be found."
            exit 1
        fi
    done
    echo "Checking linters finished: all linters installed."
    echo ""
}

# Lint all C++ files in the project.
function lint_cpp_files() {
    echo "Linting C++ files..."

    CPP_DIRS=(
        "${BASE_DIRS[@]}"
    )
    find -E "${CPP_DIRS[@]}" -regex '.*\.(h|hpp|hxx|cpp|c|cxx)' | xargs clang-format -i -style=file || echo "clang-format failed."

    echo "Done."
    echo ""
}

# Lint all shell files in the project.
function lint_shell_files() {
    echo "Linting shell files..."

    SHELL_DIRS=(
        "${BASE_DIRS[@]}"
    )
    find -E "${SHELL_DIRS[@]}" -regex '.*\.(sh)' | xargs shellcheck || echo "shellcheck failed."
    
    echo "Done."
    echo ""
}

# Lint all CMake files in the project.
function lint_cmake_files() {
    echo "Linting CMake files..."

    CMAKE_DIRS=(
        CMakeLists.txt
        "${BASE_DIRS[@]}"
        cmake
    )
    find -E "${CMAKE_DIRS[@]}" -regex '.*CMakeLists\.txt' | xargs cmake-format -i || echo "cmake-format failed."

    echo "Done."
    echo ""
}

# Lint all Markdown files in the project.
function lint_markdown_files() {
    echo "Linting Markdown files..."

    MD_DIRS=(
        README.md
        "${BASE_DIRS[@]}"
        papers/P2988/README.md
    )
    find -E "${MD_DIRS[@]}" -regex '.*\.(md)' | xargs markdownlint -f || echo "markdownlint failed."

    echo "Done."
    echo ""
}

# Lint all YAML files in the project.
function lint_yaml_files() {
    echo "Linting YAML files..."

    YAML_DIRS=(
        .github/
        "${BASE_DIRS[@]}"
    )
    find -E "${YAML_DIRS[@]}" -regex '.*\.(yml)' | xargs yamlfmt || echo "yamlfmt failed."

    echo "Done."
    echo ""
}

# Lint all files in the project.
function lint_all_files() {
    check_linters_installed || exit 1

    lint_cpp_files          || exit 1
    lint_shell_files        || exit 1
    lint_cmake_files        || exit 1
    lint_markdown_files     || exit 1
    lint_yaml_files         || exit 1

    echo "All linters finished."
}

parse_args "$@"
