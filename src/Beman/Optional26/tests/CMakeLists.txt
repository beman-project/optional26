# cmake-format: off
# src/Beman/Optional26/tests/CMakeLists.txt -*-makefile-*-
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# cmake-format: on

include(GoogleTest)

# Tests
add_executable(optional_test
  optional.t.cpp
  optional_ref.t.cpp
  optional_monadic.t.cpp
  optional_range_support.t.cpp
  optional_ref_monadic.t.cpp
  detail/iterator.t.cpp
)

target_link_libraries(optional_test beman_optional26 GTest::gtest GTest::gtest_main)

# Note: clang-19 + gtest_discover_tests + Asan setup causes errors on some platforms.
# Temporary switch to gtest_add_tests and skip some Asan checks.
gtest_add_tests(optional_test "" AUTO)
