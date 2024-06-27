#!/bin/bash

# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# Lint all files in the project.
# Expected to be run from the root of the project. 
# Expected tools to be installed on the system.
# Usage: ./scripts/lint-all.sh

set -e
set -x

BASE_DIRS=(
    include
    examples
    scripts
    src
)
# Lint all C++ files in the project.
CPP_DIRS=(
    "${BASE_DIRS[@]}"
)
find -E "${CPP_DIRS[@]}" -regex '.*\.(h|hpp|hxx|cpp|c|cxx)' | xargs clang-format -i -style=file || echo "clang-format failed."
echo "All C++ files were linted."

# Lint all scripts in the project.
SHELL_DIRS=(
    "${BASE_DIRS[@]}"
)
find -E "${SHELL_DIRS[@]}" -regex '.*\.(sh)' | xargs shellcheck || echo "shellcheck failed."
echo "All scripts files were linted."

# Lint all CMake files in the project.
CMAKE_DIRS=(
    CMakeLists.txt
    "${BASE_DIRS[@]}"
    cmake
)
find -E "${CMAKE_DIRS[@]}" -regex '.*CMakeLists\.txt' | xargs cmake-format -i || echo "cmake-format failed."
echo "All CMake files were linted."

# Lint all Markdown files in the project.
MD_DIRS=(
    README.md
    "${BASE_DIRS[@]}"
    papers/P2988/README.md
)
find -E "${MD_DIRS[@]}" -regex '.*\.(md)' | xargs markdownlint -f || echo "markdownlint failed."
echo "All Markdown files were linted."

# Lint all YAML files in the project.
YAML_DIRS=(
    .github/
    "${BASE_DIRS[@]}"
)
find -E "${YAML_DIRS[@]}" -regex '.*\.(yml)' | xargs yamlfmt || echo "yamlfmt failed."
echo "All YAML files were linted."

echo "All linters finished."
