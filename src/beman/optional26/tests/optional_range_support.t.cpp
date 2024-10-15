// src/beman/optional26/tests/optional_range_support.t.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

/**
 * This file contains tests for the range support. Check P3168R2: "Give std::optional Range Support".
 *
 * RangeSupportTest: test suite for the range support.
 *
 * Classes used in the tests: int, pointers, empty, no_default_ctor, base, derived.
 */
#include <beman/optional26/optional.hpp>

#include <gtest/gtest.h>

#include <beman/optional26/detail/iterator.hpp>
#include <beman/optional26/tests/test_types.hpp>

#include <algorithm>
#include <concepts>
#include <cstdlib>
#if defined(__cpp_lib_format_ranges)
#include <format>
#endif
#include <functional>
#include <ranges>
#include <tuple>
#include <optional>
#include <type_traits>
#include <unordered_set>
#include <vector>

#define CONSTEXPR_EXPECT_EQ(val1, val2)   \
    if (::std::is_constant_evaluated()) { \
        if (!((val1) == (val2))) {        \
            ::std::abort();               \
        }                                 \
    } else                                \
        EXPECT_EQ(val1, val2)

#define CONSTEXPR_EXPECT_TRUE(val)        \
    if (::std::is_constant_evaluated()) { \
        if (!(val)) {                     \
            ::std::abort();               \
        }                                 \
    } else                                \
        EXPECT_TRUE(val)

#define CONSTEXPR_ASSERT_TRUE(val)        \
    if (::std::is_constant_evaluated()) { \
        if (!(val)) {                     \
            ::std::abort();               \
        }                                 \
    } else                                \
        ASSERT_TRUE(val)

#define CONSTEXPR_ASSERT_FALSE(val)       \
    if (::std::is_constant_evaluated()) { \
        if (val) {                        \
            ::std::abort();               \
        }                                 \
    } else                                \
        ASSERT_FALSE(val)

using namespace beman::optional26::tests;

TEST(RangeSupportTest, RangeConcepts) {
    const auto test = [](auto&& opt) {
        // The optional type is the opt type without the reference.
        using optional = std::remove_reference_t<decltype(opt)>;

        // Check std::ranges::range concept.
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

    test(beman::optional26::optional<int>{});
    test(beman::optional26::optional<int*>{});
    test(beman::optional26::optional<empty>{});
    test(beman::optional26::optional<no_default_ctor>{});
    test(beman::optional26::optional<base>{});
    test(beman::optional26::optional<derived>{});
}

TEST(RangeSupportTest, IteratorConcepts) {
    const auto test = [](auto&& opt) {
        // The iterator type is the same as the iterator of the optional.
        // e.g. iterator = optional<T>::iterator if opt is optional<T>
        using iterator = typename std::remove_reference_t<decltype(opt)>::iterator;
        // e.g. const_iterator = optional<T>::const_iterator if opt is optional<T>
        using const_iterator = typename std::remove_reference_t<decltype(opt)>::const_iterator;

        // Check std::contiguous_iterator concept.
        // Note: Check each sub-concept to get the less verbose error message first!
        static_assert(std::input_iterator<iterator>);
        static_assert(std::forward_iterator<iterator>);
        static_assert(std::bidirectional_iterator<iterator>);
        static_assert(std::random_access_iterator<iterator>);
        static_assert(std::contiguous_iterator<iterator>);

        // Check std::contiguous_iterator concept.
        // Note: Check each sub-concept to get the less verbose error message first!
        static_assert(std::input_iterator<const_iterator>);
        static_assert(std::forward_iterator<const_iterator>);
        static_assert(std::bidirectional_iterator<const_iterator>);
        static_assert(std::random_access_iterator<const_iterator>);
        static_assert(std::contiguous_iterator<const_iterator>);
    };

    test(beman::optional26::optional<int>{});
    test(beman::optional26::optional<int*>{});
    test(beman::optional26::optional<empty>{});
    test(beman::optional26::optional<no_default_ctor>{});
    test(beman::optional26::optional<base>{});
    test(beman::optional26::optional<derived>{});
}

TEST(RangeSupportTest, BeginOnEmptyOptional) {
    auto lambda = [&] {
        const auto test = [](auto&& opt) {
            // The iterator type is the same as the iterator of the optional.
            // e.g. iterator = optional<T>::iterator if opt is optional<T>
            using iterator = typename std::remove_reference_t<decltype(opt)>::iterator;
            // e.g. const_iterator = optional<T>::const_iterator if opt is optional<T>
            using const_iterator = typename std::remove_reference_t<decltype(opt)>::const_iterator;

            CONSTEXPR_EXPECT_EQ(opt.begin(), iterator());

            const auto& const_opt = opt;
            CONSTEXPR_EXPECT_EQ(const_opt.begin(), const_iterator());
        };

        test(beman::optional26::optional<int>{});
        test(beman::optional26::optional<int*>{});
        test(beman::optional26::optional<empty>{});
        test(beman::optional26::optional<no_default_ctor>{});
        test(beman::optional26::optional<base>{});
        test(beman::optional26::optional<derived>{});
    };
    static_assert((lambda(), true));
    lambda();
}

TEST(RangeSupportTest, BeginOnNonEmptyOptional) {
    auto lambda = [&] {
        const auto test = [](auto&& opt) {
            // The iterator type is the same as the iterator of the optional.
            // e.g. iterator = optional<T>::iterator if opt is optional<T>
            using iterator = typename std::remove_reference_t<decltype(opt)>::iterator;
            // e.g. const_iterator = optional<T>::const_iterator if opt is optional<T>
            using const_iterator = typename std::remove_reference_t<decltype(opt)>::const_iterator;

            CONSTEXPR_EXPECT_EQ(opt.begin(), iterator(&*opt));

            const auto& const_opt = opt;
            CONSTEXPR_EXPECT_EQ(const_opt.begin(), const_iterator(&*opt));
        };

        test(beman::optional26::optional<int>{26});
        if (!std::is_constant_evaluated()) {
            test(beman::optional26::optional<int*>{reinterpret_cast<int*>(0XCAFEBABE)});
        }
        test(beman::optional26::optional<empty>{empty{}});
        test(beman::optional26::optional<no_default_ctor>{no_default_ctor{empty{}}});
        test(beman::optional26::optional<base>{base{}});
        test(beman::optional26::optional<derived>{derived{}});
    };
    static_assert((lambda(), true));
    lambda();
}

TEST(RangeSupportTest, EndOnEmptyOptional) {
    auto lambda = [&] {
        const auto test = [](auto&& opt) {
            // The iterator type is the same as the iterator of the optional.
            // e.g. iterator = optional<T>::iterator if opt is optional<T>
            using iterator = typename std::remove_reference_t<decltype(opt)>::iterator;
            // e.g. const_iterator = optional<T>::const_iterator if opt is optional<T>
            using const_iterator = typename std::remove_reference_t<decltype(opt)>::const_iterator;

            CONSTEXPR_EXPECT_EQ(opt.end(), iterator());

            const auto& const_opt = opt;
            CONSTEXPR_EXPECT_EQ(const_opt.end(), const_iterator());
        };

        test(beman::optional26::optional<int>{});
        test(beman::optional26::optional<int*>{});
        test(beman::optional26::optional<empty>{});
        test(beman::optional26::optional<no_default_ctor>{});
        test(beman::optional26::optional<base>{});
        test(beman::optional26::optional<derived>{});
    };
    static_assert((lambda(), true));
    lambda();
}

TEST(RangeSupportTest, EndOnNonEmptyOptional) {
    auto lambda = [&] {
        const auto test = [](auto&& opt) {
            // The iterator type is the same as the iterator of the optional.
            // e.g. iterator = optional<T>::iterator if opt is optional<T>
            using iterator = typename std::remove_reference_t<decltype(opt)>::iterator;
            // e.g. const_iterator = optional<T>::const_iterator if opt is optional<T>
            using const_iterator = typename std::remove_reference_t<decltype(opt)>::const_iterator;

            CONSTEXPR_EXPECT_EQ(opt.end(), iterator(&*opt + 1));

            const auto& const_opt = opt;
            CONSTEXPR_EXPECT_EQ(const_opt.end(), const_iterator(&*opt + 1));
        };

        test(beman::optional26::optional<int>{26});
        if (!std::is_constant_evaluated()) {
            test(beman::optional26::optional<int*>{reinterpret_cast<int*>(0XCAFEBABE)});
        }
        test(beman::optional26::optional<empty>{empty{}});
        test(beman::optional26::optional<no_default_ctor>{no_default_ctor{empty{}}});
        test(beman::optional26::optional<base>{base{}});
        test(beman::optional26::optional<derived>{derived{}});
    };
    static_assert((lambda(), true));
    lambda();
}

TEST(RangeSupportTest, FormatOptionalIsStillDisabled) {
// TODO: Always enable when all major compilers implement P2585R1: "Improve default container formatting".
#if defined(__cpp_lib_format_ranges)
    static_assert(std::format_kind<beman::optional26::optional<int>> == std::range_format::disabled);
    EXPECT_EQ(std::format_kind<beman::optional26::optional<int>>, std::range_format::disabled);

    // Next line should not compile: formatter is not defined for optional.
    // EXPECT_EQ(std::format("{}", beman::optional26::optional<int> {}), "[]");
#endif
}

TEST(RangeSupportTest, LoopOverEmptyRange) {
    auto lambda = [&] {
        beman::optional26::optional<int> empty;
        CONSTEXPR_ASSERT_FALSE(empty.has_value());

        for ([[maybe_unused]] auto _ : empty) {
            CONSTEXPR_ASSERT_TRUE(false) << "Should not be reached: expected not to loop over empty optional";
        }
    };
    static_assert((lambda(), true));
    lambda();
}

TEST(RangeSupportTest, LoopOverNonEmptyRange) {
    auto lambda = [&] {
        const auto                       expected_value = 0xCAFEBABE;
        beman::optional26::optional<int> empty{expected_value};
        CONSTEXPR_ASSERT_TRUE(empty.has_value());

        bool entered_loop = false;
        for (auto i : empty) {
            CONSTEXPR_EXPECT_EQ(i, expected_value);
            entered_loop = true;
        }
        CONSTEXPR_EXPECT_TRUE(entered_loop);
    };
    static_assert((lambda(), true));
    lambda();
}

TEST(RangeSupportTest, LoopOptionalAccess) {
    auto lambda = [&] {
        // Example from P3168R2: should access the value from an optional object.
        const int  expected_value = 0xCAFEBABE;
        const auto get_optional   = [&]() -> beman::optional26::optional<int> { return expected_value; };
        CONSTEXPR_ASSERT_TRUE(get_optional().has_value());

        for (auto&& opt : get_optional()) {
            CONSTEXPR_EXPECT_EQ(opt, expected_value); // usage of opt here is safe
        }
    };
    static_assert((lambda(), true));
    lambda();
}

TEST(RangeSupportTest, LoopOptionalAssignment) {
    auto lambda = [&] {
        // Example from P3168R2: should mutate the value from an optional object.
        const int  initial_expected_value = 0xCAFEBABE;
        const int  expected_value         = 0xDEADBEEF;
        const auto get_optional = [&]() -> beman::optional26::optional<int> { return initial_expected_value; };
        CONSTEXPR_ASSERT_TRUE(get_optional().has_value());
        CONSTEXPR_ASSERT_TRUE(get_optional().value() == initial_expected_value);

        auto opt_int = get_optional();
        for (auto&& opt : opt_int) {
            CONSTEXPR_EXPECT_EQ(opt, initial_expected_value);
            opt = expected_value; // usage of opt here is safe
        }
        CONSTEXPR_ASSERT_TRUE(opt_int.has_value());
        CONSTEXPR_EXPECT_EQ(opt_int.value(), expected_value);
    };
    static_assert((lambda(), true));
    lambda();
}

TEST(RangeSupportTest, RangeChainExample) {
    // Example from P3168R2: start from a set of values, apply multiple range operations involving optional values.
    std::unordered_set<int> s{1, 3, 7, 9};
    const auto              flt = [&](int i) -> beman::optional26::optional<int> {
        if (s.contains(i)) {
            return i;
        } else {
            return {};
        }
    };

    for (auto i : std::views::iota(1, 10) | std::views::transform(flt)) {
        for (auto j : i) { // no need to transform
            for (auto k : std::views::iota(0, j)) {
                // std::cout << '\a'; // do not actually log in tests
                std::ignore = k;
            }
            // std::cout << '\n'; // do not actually log in tests
        }
    }
}

TEST(RangeSupportTest, PythagoreanTriples) {
    auto lambda = [&] {
        // Example from P3168R2: generate an infinite sequence of Pythagorean triples.
        // (x, y, z) is a Pythagorean triple if 1 <= x <= y <= z and x^2 + y^2 = z^2.
        constexpr auto yield_if = []<class T>(bool b, T x) -> beman::optional26::optional<T> {
            return b ? beman::optional26::optional<T>{std::move(x)} : beman::optional26::nullopt;
        };
        constexpr auto and_then = []<class T>(T&& r, auto fun) {
            return decltype(r)(r) | std::views::transform(std::move(fun)) | std::views::join;
        };
        auto triples = and_then(std::views::iota(1), [=](int z) {
            return and_then(std::views::iota(1, z + 1), [=](int x) {
                return and_then(std::views::iota(x, z + 1),
                                [=](int y) { return yield_if(x * x + y * y == z * z, std::make_tuple(x, y, z)); });
            });
        });

        {
            // Generate first 10 Pythagorean triples.
            // https://mathworld.wolfram.com/PythagoreanTriple.html
            auto&& r = triples | std::views::take(10);
            CONSTEXPR_EXPECT_TRUE(std::ranges::equal(r,
                                                     std::vector{
                                                         std::tuple{3, 4, 5},
                                                         std::tuple{6, 8, 10},
                                                         std::tuple{5, 12, 13},
                                                         std::tuple{9, 12, 15},
                                                         std::tuple{8, 15, 17},
                                                         std::tuple{12, 16, 20},
                                                         std::tuple{7, 24, 25},
                                                         std::tuple{15, 20, 25},
                                                         std::tuple{10, 24, 26},
                                                         std::tuple{20, 21, 29},
                                                     }));
        }

        if (!std::is_constant_evaluated()) { // too many steps for the default constant evaluation limit
            // Generate the nth Pythagorean triple (x, y, z) where:
            // 1 <= x <= y <= z <= max_value and x^2 + y^2 = z^2

            // Generate the nth Pythagorean triple with brute force.
            auto bruteforce_generate_nth = [](int n, int max_value) -> std::tuple<int, int, int> {
                int count = 0; // Count the number of Pythagorean triples.
                for (int z = 1; z <= max_value; ++z) {
                    for (int x = 1; x <= max_value; ++x) {
                        for (int y = 1; y <= max_value; ++y) {
                            if (!(x <= y && y <= z) || x * x + y * y != z * z) {
                                continue;
                            }

                            if (++count == n) {
                                return std::make_tuple(x, y, z);
                            }
                        }
                    }
                }

                return {}; // Should not reach here.
            };
            constexpr const std::tuple k100th_triple = {
                26, 168, 170}; // The 100th Pythagorean triple with x, y, z <= 200.
            ASSERT_EQ(bruteforce_generate_nth(100, 200), k100th_triple);

            // Generate the 100th Pythagorean triple with ranges.
            auto&& r = triples | std::views::drop(99) | std::views::take(1);
            EXPECT_TRUE(std::ranges::equal(r, std::vector{k100th_triple}));
        }
    };
    static_assert((lambda(), true));
    lambda();
}
