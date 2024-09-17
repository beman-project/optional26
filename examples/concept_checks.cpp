// examples/concept_checks.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <optional>
#include <beman/optional26/optional.hpp>

#include <concepts>

namespace test {
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

} // namespace test

const auto test_concepts_disabled = [](auto&& opt) {
    // The optional type is the opt type without the reference.
    using optional = std::remove_reference_t<decltype(opt)>;

    // Check std::ranges concepts not enabled.
    static_assert(!std::ranges::range<optional>);
    static_assert(!std::ranges::view<optional>);
    static_assert(!std::ranges::input_range<optional>);
    static_assert(!std::ranges::forward_range<optional>);
    static_assert(!std::ranges::bidirectional_range<optional>);
    static_assert(!std::ranges::contiguous_range<optional>);
    static_assert(!std::ranges::common_range<optional>);
    static_assert(!std::ranges::viewable_range<optional>);
    static_assert(!std::ranges::random_access_range<optional>);
    static_assert(!std::ranges::sized_range<optional>);
};

const auto test_concepts_enabled = [](auto&& opt) {
    // The optional type is the opt type without the reference.
    using optional = std::remove_reference_t<decltype(opt)>;

    // Check std::ranges concepts enabled.
    static_assert(std::ranges::range<optional>);
    static_assert(std::ranges::view<optional>);
    static_assert(std::ranges::input_range<optional>);
    static_assert(std::ranges::forward_range<optional>);
    static_assert(std::ranges::bidirectional_range<optional>);
    static_assert(std::ranges::contiguous_range<optional>);
    static_assert(std::ranges::common_range<optional>);
    static_assert(std::ranges::viewable_range<optional>);
    static_assert(!std::ranges::borrowed_range<optional>);
    static_assert(std::ranges::random_access_range<optional>);
    static_assert(std::ranges::sized_range<optional>);
};

int main() {
    {
        // std::optional does not meet range concepts.
        test_concepts_disabled(std::optional<int>{});
        test_concepts_disabled(std::optional<int*>{});
        test_concepts_disabled(std::optional<test::empty>{});
        test_concepts_disabled(std::optional<test::no_default_ctor>{});
        test_concepts_disabled(std::optional<test::base>{});
        test_concepts_disabled(std::optional<test::derived>{});
    }

    {
        // beman::optional26::optional meets range concepts.
        test_concepts_enabled(beman::optional26::optional<int>{});
        test_concepts_enabled(beman::optional26::optional<int*>{});
        test_concepts_enabled(beman::optional26::optional<test::empty>{});
        test_concepts_enabled(beman::optional26::optional<test::no_default_ctor>{});
        test_concepts_enabled(beman::optional26::optional<test::base>{});
        test_concepts_enabled(beman::optional26::optional<test::derived>{});
    }

    return 0;
}

// # build example:
// $ cmake --workflow --preset gcc-14
//
// # run example:
// $ .build/gcc-14/examples/RelWithDebInfo/concept_checks
