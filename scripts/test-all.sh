#!/bin/bash

# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

# Test all configurations: gcc-14, gcc-13, clang-18, clang-17.
# Expected to be run from the root of the project.
# Expected all configurations to be present in the CMakeLists.txt file and installed on the system.
# Usage: ./scripts/test-all.sh

set -e
set -x

# rm -rf .build

cmake --workflow --preset gcc-14   &> /dev/null && echo "passed" || echo "gcc-14 test failed."
cmake --workflow --preset gcc-13   &> /dev/null && echo "passed" || echo "gcc-13 test failed."
cmake --workflow --preset clang-18 &> /dev/null && echo "passed" || echo "clang-18 test failed."
cmake --workflow --preset clang-17 &> /dev/null && echo "passed" || echo "clang-17 test failed."

echo "All tests passed."
