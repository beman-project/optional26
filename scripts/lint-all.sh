#!/bin/bash

# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# Lint all files in the project.
# Expected to be run from the root of the project. 
# Expected tools to be installed on the system.
# Usage: ./scripts/lint-all.sh

set -e
set -x

# Lint all C++ files in the project.
find include/ examples/ src/ -name "*.h" -o -name "*.cpp" | xargs clang-format -i -style=file || echo "clang-format failed."
echo "All C++ files were linted."

# Lint all scripts in the project.
find scripts/ -name "*.sh" | xargs shellcheck || echo "shellcheck failed."
echo "All scripts files were linted."

# TODO: Lint all CMake files in the project.
# echo "All CMake files were linted."

# Lint all Markdown files in the project.
find README.md examples/ -name "*.md" | xargs markdownlint -f || echo "markdownlint failed."
echo "All Markdown files were linted."

# Lint all YAML files in the project.
find . -name "*.yaml" -o -name "*.yml" | xargs yamlfmt || echo "yamlfmt failed."
echo "All YAML files were linted."

# TODO: Lint all JSON files in the project.
# echo "All JSON files were linted."

echo "All linters finished."
