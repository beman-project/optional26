// src/Beman/Optional26/tests/test_types.h -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_OPTIONAL26_TESTS_TEST_TYPES_HPP
#define BEMAN_OPTIONAL26_TESTS_TEST_TYPES_HPP

#include <compare>

namespace beman::optional26::tests {
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

struct int_ctor {
    int i_;
    int_ctor() = delete;
    int_ctor(int i) : i_(i) {}
};

// Base class helper.
struct base {
    int m_i;
    constexpr base() : m_i(0) {}
    constexpr base(int i) : m_i(i) {}

    bool operator!=(const base&) const = default;
    auto operator<=>(const base&) const = default;

};

// Derived class helper.
struct derived : public base {
    int m_j;
    constexpr derived() : base(), m_j(0) {}
    constexpr derived(int i, int j) : base(i), m_j(j) {}

    bool operator!=(const derived&) const = default;
    auto operator<=>(const derived&) const = default;
};

struct move_detector {
    move_detector() = default;
    move_detector(move_detector&& rhs) { rhs.been_moved = true; }
    bool been_moved = false;
};

class Point {
    int x_;
    int y_;

  public:
    constexpr Point() : x_(0), y_(0) {}
    constexpr Point(int x, int y) : x_(x), y_(y) {}
    auto operator<=>(const Point&) const = default;
    bool operator==(const Point&) const  = default;
};

} // namespace beman::optional26::tests

#endif // BEMAN_OPTIONAL26_TESTS_TEST_TYPES_HPP
