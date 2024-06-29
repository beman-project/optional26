#!/bin/bash

# scripts/lint.sh -*-shell-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# Lint all files in the project.
# Expected to be run from the root of the project. 
# Expected tools to be installed on the system. A pre-check is done to ensure all tools are installed.

set -e
set -x

# Base directories to lint.
BASE_DIRS=(
    include
    examples
    scripts
    src
)

# Linter to run.
# e.g. lint_cpp_files, lint_shell_files, lint_markdown_files, lint_yaml_files, lint_cmake_files
#      or
#      lint_all_files
LINTER=

# Print usage information.
function print_usage() {
    echo "Usage: $0"
    echo "  -h, --help: Display help."
    echo "  -f, --fix: Fix all linting issues. Default: false."
    echo "  -v, --verbose: Enable verbose output."
    echo "  -a, --all: Lint all files."
    echo "  -c, --cpp: Lint C++ files."
    echo "  -s, --shell: Lint shell scripts."
    echo "  -m, --markdown: Lint Markdown files."
    echo "  -y, --yaml: Lint YAML files."
    echo "  -C, --cmake: Lint CMake files."

    exit 1
}

# Parse command line arguments.
function parse_args() {
    # If no arguments are provided, print usage and exit.
    if [[ $# -eq 0 ]]; then
        print_usage
        exit 1
    fi

    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                print_usage
                exit 1
                ;;
            -f|--fix)
                FIX_INPLACE=true
                shift
                ;;
            -v|--verbose)
                set -x
                shift
                ;;
            -a|--all)
                LINTER=lint_all_files
                shift
                ;;
            -c|--cpp)
                LINTER=lint_cpp_files
                shift
                ;;
            -s|--shell)
                LINTER=lint_shell_files
                shift
                ;;
            -m|--markdown)
                LINTER=lint_markdown_files
                shift
                ;;
            -y|--yaml)
                LINTER=lint_yaml_files
                shift
                ;;
            -C|--cmake)
                LINTER=lint_cmake_files
                shift
                ;;
            *)
                echo "Unknown option: $1"
                print_usage
                exit 1
                ;;
        esac
    done

    # If no linter is specified, exit.
    [ ! -z "${LINTER}" ] || print_usage
    return 0
}

# Lint all C++ files in the project.
function lint_cpp_files() {
    echo "Linting C++ files..."
    clang-format --version &> /dev/null

    CPP_DIRS=(
        "${BASE_DIRS[@]}"
    )
    
    CLANG_FORMAT_FLAGS=""
    if [ "${FIX_INPLACE}" = true ]; then
        CLANG_FORMAT_FLAGS="-i"
    else
        CLANG_FORMAT_FLAGS="--dry-run"
    fi

    find "${CPP_DIRS[@]}" -regex '.*\.\(hpp\|cpp\)$' | xargs clang-format --Werror --style=file ${CLANG_FORMAT_FLAGS} --verbose

    echo "Done."
    echo ""

    return 0;
}

# Lint all shell files in the project.
function lint_shell_files() {
    echo "Linting shell files..."
    shellcheck --version &> /dev/null

    SHELL_DIRS=(
        "${BASE_DIRS[@]}"
    )

    find "${SHELL_DIRS[@]}" -regex '.*\.\(sh\)$'
    find "${SHELL_DIRS[@]}" -regex '.*\.\(sh\)$' | xargs shellcheck
    
    echo "Done."
    echo ""

    return 0
}

# Lint all Markdown files in the project.
function lint_markdown_files() {
    echo "Linting Markdown files..."
    markdownlint --version &> /dev/null

    MD_DIRS=(
        README.md
        "${BASE_DIRS[@]}"
        papers/P2988/README.md
    )

    MARKDOWNLINT_FLAGS=
    if [ "${FIX_INPLACE}" = true ]; then
        MARKDOWNLINT_FLAGS="-f"
    fi

    find ${MD_DIRS[@]} -regex '.*\.\(md\)$'
    find "${MD_DIRS[@]}" -regex '.*\.\(md\)$' | xargs markdownlint --config .markdownlint.yml "${MARKDOWNLINT_FLAGS}"

    echo "Done."
    echo ""

    return 0;
}

# Lint all YAML files in the project.
function lint_yaml_files() {
    echo "Linting YAML files..."
    yamllint --version &> /dev/null

    YAML_DIRS=(
        .github/
        "${BASE_DIRS[@]}"
    )

    find "${YAML_DIRS[@]}" -regex '.*\.\(yml\)$'
    find "${YAML_DIRS[@]}" -regex '.*\.\(yml\)$' | xargs yamllint -c .yamllint

    echo "Done."
    echo ""

    return 0;
}

# Lint all CMake files in the project.
function lint_cmake_files() {
    echo "Linting CMake files..."
    cmake-format --version &> /dev/null

    CMAKE_DIRS=(
        CMakeLists.txt
        "${BASE_DIRS[@]}"
        cmake/
        etc
    )

    CMAKE_FORMAT_FLAGS=
    if [ "${FIX_INPLACE}" = true ]; then
        CMAKE_FORMAT_FLAGS="-i"
    else
        CMAKE_FORMAT_FLAGS="--check"
    fi

    find "${CMAKE_DIRS[@]}" -regex '.*\(CMakeLists\.txt\|cmake\)$'
    find "${CMAKE_DIRS[@]}" -regex '.*\(CMakeLists\.txt\|cmake\)$' | xargs cmake-format -c .cmake-format ${CMAKE_FORMAT_FLAGS}

    echo "Done."
    echo ""

    return 0;
}

# Lint all files in the project.
function lint_all_files() {
    lint_cpp_files          || exit 1
    lint_shell_files        || exit 1
    lint_markdown_files     || exit 1
    lint_yaml_files         || exit 1
    lint_cmake_files        || exit 1

    echo "All linters finished."
    return 0;
}

parse_args "$@"
eval "${LINTER}"
