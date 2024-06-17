/**
 * This file contains tests for the range support. Check P3168R1: "Give std::optional Range Support".
 *
 * RangeSupportTest: test suite for the range support.
 *
 * Classes used in the tests: int, pointers, empty, no_default_ctor, base, derived.
 */
#include <Beman/Optional26/optional.hpp>

#include <algorithm>
#include <format>
#include <functional>
#include <ranges>
#include <tuple>
#include <optional>
#include <unordered_set>
#include <vector>
#include <gtest/gtest.h>

namespace test {
/**
 * Empty class helper.
 **/
struct empty {};

/**
 * No default constructor class helper.
 **/
struct no_default_ctor {
    no_default_ctor()                                  = delete;
    no_default_ctor(const no_default_ctor&)            = default;
    no_default_ctor(no_default_ctor&&)                 = default;
    no_default_ctor& operator=(const no_default_ctor&) = default;
    no_default_ctor& operator=(no_default_ctor&&)      = default;
    no_default_ctor(empty) {};
};

/**
 * Base class helper.
 **/
struct base {
    int i_;
    base() : i_(0) {}
    base(int i) : i_(i) {}
};

/**
 * Derived class helper.
 **/
struct derived : public base {
    int j_;
    derived() : base(0), j_(0) {}
    derived(int i, int j) : base(i), j_(j) {}
};
} // namespace test

TEST(RangeSupportTest, BeginOnEmptyOptional) {
    EXPECT_EQ(beman::optional::optional<int>{}.begin(), nullptr);

    EXPECT_EQ(beman::optional::optional<test::empty>{}.begin(), nullptr);

    EXPECT_EQ(beman::optional::optional<test::no_default_ctor>{}.begin(), nullptr);

    EXPECT_EQ(beman::optional::optional<test::base>{}.begin(), nullptr);

    EXPECT_EQ(beman::optional::optional<test::derived>{}.begin(), nullptr);
}

TEST(RangeSupportTest, BeginOnNonEmptyOptional) {
    beman::optional::optional<int> opt_int = 26;
    EXPECT_EQ(opt_int.begin(), &*opt_int);

    beman::optional::optional<test::empty> opt_empty = test::empty{};
    EXPECT_EQ(opt_empty.begin(), &*opt_empty);

    beman::optional::optional<test::no_default_ctor> opt_no_default_ctor = test::no_default_ctor{test::empty{}};
    EXPECT_EQ(opt_no_default_ctor.begin(), &*opt_no_default_ctor);

    beman::optional::optional<test::base> opt_base = test::base{};
    EXPECT_EQ(opt_base.begin(), &*opt_base);

    beman::optional::optional<test::derived> opt_derived = test::derived{};
    EXPECT_EQ(opt_derived.begin(), &*opt_derived);
}

TEST(RangeSupportTest, EndOnEmptyOptional) {
    EXPECT_EQ(beman::optional::optional<int>{}.end(), nullptr);

    EXPECT_EQ(beman::optional::optional<test::empty>{}.end(), nullptr);

    EXPECT_EQ(beman::optional::optional<test::no_default_ctor>{}.end(), nullptr);

    EXPECT_EQ(beman::optional::optional<test::base>{}.end(), nullptr);

    EXPECT_EQ(beman::optional::optional<test::derived>{}.end(), nullptr);
}

TEST(RangeSupportTest, EndOnNonEmptyOptional) {
    beman::optional::optional<int> opt_int = 0XCAFEBABE;
    EXPECT_EQ(opt_int.end(), opt_int.begin() + 1);

    beman::optional::optional<test::empty> opt_empty = test::empty{};
    EXPECT_EQ(opt_empty.end(), opt_empty.begin() + 1);

    beman::optional::optional<test::no_default_ctor> opt_no_default_ctor = test::no_default_ctor{test::empty{}};
    EXPECT_EQ(opt_no_default_ctor.end(), opt_no_default_ctor.begin() + 1);

    beman::optional::optional<test::base> opt_base = test::base{};
    EXPECT_EQ(opt_base.end(), opt_base.begin() + 1);

    beman::optional::optional<test::derived> opt_derived = test::derived{};
    EXPECT_EQ(opt_derived.end(), opt_derived.begin() + 1);
}

TEST(RangeSupportTest, RangeConceptsCheck) {
    {
        // std::optional is not (yet) a range.
        static_assert(!std::ranges::range<std::optional<int>>);
    }

    {
        // beman::optional::optional<int> is a range.
        static_assert(std::ranges::range<beman::optional::optional<int>>);
        static_assert(std::ranges::view<beman::optional::optional<int>>);
        static_assert(std::ranges::input_range<beman::optional::optional<int>>);
        static_assert(std::ranges::forward_range<beman::optional::optional<int>>);
        static_assert(std::ranges::bidirectional_range<beman::optional::optional<int>>);
        static_assert(std::ranges::contiguous_range<beman::optional::optional<int>>);
        static_assert(std::ranges::common_range<beman::optional::optional<int>>);
        static_assert(std::ranges::viewable_range<beman::optional::optional<int>>);
        static_assert(!std::ranges::borrowed_range<beman::optional::optional<int>>); // borrowed_range is disabled for
                                                                                     // non-pointers
        static_assert(std::ranges::random_access_range<beman::optional::optional<int>>);
        static_assert(std::ranges::sized_range<beman::optional::optional<int>>);
    }

    {
        // beman::optional::optional<int*> is a range.
        static_assert(std::ranges::range<beman::optional::optional<int*>>);
        static_assert(std::ranges::view<beman::optional::optional<int*>>);
        static_assert(std::ranges::input_range<beman::optional::optional<int*>>);
        static_assert(std::ranges::forward_range<beman::optional::optional<int*>>);
        static_assert(std::ranges::bidirectional_range<beman::optional::optional<int*>>);
        static_assert(std::ranges::contiguous_range<beman::optional::optional<int*>>);
        static_assert(std::ranges::common_range<beman::optional::optional<int*>>);
        static_assert(std::ranges::viewable_range<beman::optional::optional<int*>>);
        static_assert(
            std::ranges::borrowed_range<beman::optional::optional<int*>>); // borrowed_range is enabled for pointers
        static_assert(std::ranges::random_access_range<beman::optional::optional<int*>>);
    }

    {
        using ref = std::reference_wrapper<int>;
        // beman::optional::optional<ref> is a range.
        static_assert(std::ranges::range<beman::optional::optional<ref>>);
        static_assert(std::ranges::view<beman::optional::optional<ref>>);
        static_assert(std::ranges::input_range<beman::optional::optional<ref>>);
        static_assert(std::ranges::forward_range<beman::optional::optional<ref>>);
        static_assert(std::ranges::bidirectional_range<beman::optional::optional<ref>>);
        static_assert(std::ranges::contiguous_range<beman::optional::optional<ref>>);
        static_assert(std::ranges::common_range<beman::optional::optional<ref>>);
        static_assert(std::ranges::viewable_range<beman::optional::optional<ref>>);
        static_assert(std::ranges::borrowed_range<beman::optional::optional<ref>>); // borrowed_range is
                                                                                    // enabled for
                                                                                    // reference_wrapper
        static_assert(std::ranges::random_access_range<beman::optional::optional<ref>>);
    }
}

TEST(RangeSupportTest, IteratorConceptsCheck) {
    {
        // beman::optional::optional<int>::iterator is an iterator type.
        static_assert(std::input_iterator<beman::optional::optional<int>::iterator>);
        static_assert(std::forward_iterator<beman::optional::optional<int>::iterator>);
        static_assert(std::bidirectional_iterator<beman::optional::optional<int>::iterator>);
        static_assert(std::random_access_iterator<beman::optional::optional<int>::iterator>);
        static_assert(std::contiguous_iterator<beman::optional::optional<int>::iterator>);

        // beman::optional::optional<int>::const_iterator is an iterator type.
        static_assert(std::input_iterator<beman::optional::optional<int>::const_iterator>);
        static_assert(std::forward_iterator<beman::optional::optional<int>::const_iterator>);
        static_assert(std::bidirectional_iterator<beman::optional::optional<int>::const_iterator>);
        static_assert(std::random_access_iterator<beman::optional::optional<int>::const_iterator>);
        static_assert(std::contiguous_iterator<beman::optional::optional<int>::const_iterator>);
    }

    {
        // beman::optional::optional<int*>::iterator is an iterator type.
        static_assert(std::input_iterator<beman::optional::optional<int*>::iterator>);
        static_assert(std::forward_iterator<beman::optional::optional<int*>::iterator>);
        static_assert(std::bidirectional_iterator<beman::optional::optional<int*>::iterator>);
        static_assert(std::random_access_iterator<beman::optional::optional<int*>::iterator>);
        static_assert(std::contiguous_iterator<beman::optional::optional<int*>::iterator>);

        // beman::optional::optional<int*>::const_iterator is an iterator type.
        static_assert(std::input_iterator<beman::optional::optional<int*>::const_iterator>);
        static_assert(std::forward_iterator<beman::optional::optional<int*>::const_iterator>);
        static_assert(std::bidirectional_iterator<beman::optional::optional<int*>::const_iterator>);
        static_assert(std::random_access_iterator<beman::optional::optional<int*>::const_iterator>);
        static_assert(std::contiguous_iterator<beman::optional::optional<int*>::const_iterator>);
    }
}

TEST(RangeSupportTest, FormatOptionalIsStillDisabled) {
// TODO: Always enable when all major compilers implement P2585R1: "Improve default container formatting".
#if defined(__cpp_lib_format_ranges)
    EXPECT_EQ(std::format_kind<beman::optional::optional<int>>, std::range_format::disabled);

    // Next line shold not compile: formatter is not defined for optional.
    // EXPECT_EQ(std::format("{}", beman::optional::optional<int> {}), "[]");
#endif
}

TEST(RangeSupportTest, LoopOverEmptyRange) {
    beman::optional::optional<int> empty;
    ASSERT_FALSE(empty.has_value());

    for (auto _ : empty) {
        ASSERT_TRUE(false) << "Should not be reached: expected not to loop over empty optional";
    }
}

TEST(RangeSupportTest, LoopOverNonEmptyRange) {
    const auto                     expected_value = 0xCAFEBABE;
    beman::optional::optional<int> empty{expected_value};
    ASSERT_TRUE(empty.has_value());

    bool entered_loop = false;
    for (auto i : empty) {
        EXPECT_EQ(i, expected_value);
        entered_loop = true;
    }
    EXPECT_TRUE(entered_loop);
}

TEST(RangeSupportTest, LoopOptionalAccess) {
    // Example from P3168R1: should access the value from an optional object.
    const int  expected_value = 0xCAFEBABE;
    const auto get_optional   = [&]() -> beman::optional::optional<int> { return expected_value; };
    ASSERT_TRUE(get_optional().has_value());

    for (auto&& opt : get_optional()) {
        EXPECT_EQ(opt, expected_value); // usage of opt here is safe
    }
}

TEST(RangeSupportTest, LoopOptionalAssignment) {
    // Example from P3168R1: should mutate the value from an optional object.
    const int  initial_expected_value = 0xCAFEBABE;
    const int  expected_value         = 0xDEADBEEF;
    const auto get_optional           = [&]() -> beman::optional::optional<int> { return initial_expected_value; };
    ASSERT_TRUE(get_optional().has_value());
    ASSERT_TRUE(get_optional().value() == initial_expected_value);

    auto opt_int = get_optional();
    for (auto&& opt : opt_int) {
        EXPECT_EQ(opt, initial_expected_value);
        opt = expected_value; // usage of opt here is safe
    }
    ASSERT_TRUE(opt_int.has_value());
    EXPECT_EQ(opt_int.value(), expected_value);
}

TEST(RangeSupportTest, RangeChainExampleWithVector) {
    // Example from P3168R1: start from a vector of values, apply multiple range operations involving optional values.
    std::vector<int> v{2, 3, 4, 5, 6, 7, 8, 9, 1};
    const auto       test = [](int i) -> beman::optional::optional<int> {
        switch (i) {
        case 1:
        case 3:
        case 7:
        case 9:
            return i;
        default:
            return {};
        }
    };

    auto&& r = v                             // starting vector
               | std::views::transform(test) // generates {nullopt, 3, nullopt, 7, nullopt, nullopt, nullopt, 9, 1}
               | std::views::filter([](auto x) {
                     return bool(x);
                 }) // generates {optional<int>{3}, optional<int>{7}, optional<int>{9}, optional<int>{1}}
               | std::views::transform([](auto x) { return *x; }) // generates {3, 5, 7, 9}
               | std::views::transform([](int i) {
                     // std::cout << i << "\n"; // do not actually do it in tests
                     return i;
                 }) // print + identity transform
        ;

    ASSERT_TRUE(std::ranges::equal(r, std::vector<int>{3, 7, 9, 1}));
}

TEST(RangeSupportTest, RangeChainExampleWithSets) {
    // Example from P3168R1: start from a set of values, apply multiple range operations involving optional values.
    std::unordered_set<int> s{1, 3, 7, 9};
    const auto              flt = [&](int i) -> beman::optional::optional<int> {
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
    // Example from P3168R1: generate an infinite sequence of Pythagorean triples.
    // (x, y, z) is a Pythagorean triple if 1 <= x <= y <= z and x^2 + y^2 = z^2.
    constexpr auto yield_if = []<class T>(bool b, T x) -> beman::optional::optional<T> {
        return b ? beman::optional::optional<T>{std::move(x)} : beman::optional::nullopt;
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
        EXPECT_TRUE(std::ranges::equal(r,
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

    {
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
        constexpr const std::tuple k100th_triple = {26, 168, 170}; // The 100th Pythagorean triple with x, y, z <= 200.
        ASSERT_EQ(bruteforce_generate_nth(100, 200), k100th_triple);

        // Generate the 100th Pythagorean triple with ranges.
        auto&& r = triples | std::views::drop(99) | std::views::take(1);
        EXPECT_TRUE(std::ranges::equal(r, std::vector{k100th_triple}));
    }
}
