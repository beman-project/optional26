// src/Beman/Optional26/tests/test_utilities.hpp                      -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_OPTIONAL26_TESTS_TEST_UTILITIES_HPP
#define BEMAN_OPTIONAL26_TESTS_TEST_UTILITIES_HPP

namespace beman::optional26::tests {
/***
 * Evaluate and return an expression in a consteval context for testing
 * constexpr correctness.
 */
auto consteval constify(auto expr) { return (expr); }
} // namespace beman::optional26::tests

#endif
