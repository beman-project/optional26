// src/Beman/Optional26/tests/test_types.h -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_OPTIONAL26_TESTS_TEST_TYPES_HPP
#define BEMAN_OPTIONAL26_TESTS_TEST_TYPES_HPP

namespace beman::optional26::test {
// Classes used in the tests.

// Empty class helper.
struct empty {};

// No default constructor class helper.
struct no_default_ctor {
    no_default_ctor()                                  = delete;
    no_default_ctor(const no_default_ctor&)            = default;
    no_default_ctor(no_default_ctor&&)                 = default;
    no_default_ctor& operator=(const no_default_ctor&) = default;
    no_default_ctor& operator=(no_default_ctor&&)      = default;
    no_default_ctor(empty) {};
};

// Base class helper.
struct base {
    int m_i;
    base() : m_i(0) {}
    base(int i) : m_i(i) {}
};

// Derived class helper.
struct derived : public base {
    int m_j;
    derived() : base(), m_j(0) {}
    derived(int i, int j) : base(i), m_j(j) {}
};

} // namespace beman::optional26::test

#endif // BEMAN_OPTIONAL26_TESTS_TEST_TYPES_HPP
