#include <Beman/Optional26/optional.hpp>

#include <format>
#include <functional>
#include <ranges>
#include <tuple>
#include <optional>
#include <gtest/gtest.h>

namespace test {
struct empty {};
struct no_default_ctor {
    no_default_ctor()                             = delete;
    no_default_ctor(const no_default_ctor&)            = default;
    no_default_ctor(no_default_ctor&&)                 = default;
    no_default_ctor& operator=(const no_default_ctor&) = default;
    no_default_ctor& operator=(no_default_ctor&&)      = default;
    no_default_ctor(empty){};
};

struct base {
    int i_;
    base() : i_(0) {}
    base(int i) : i_(i) {}
};

struct derived : public base {
    int j_;
    derived() : base(0), j_(0) {}
    derived(int i, int j) : base(i), j_(j) {}
};
}

TEST(RangeSupportTest, BeginOnEmptyOptional) {
    EXPECT_EQ(beman::optional::optional<int> {}.begin(), nullptr);

    EXPECT_EQ(beman::optional::optional<test::empty> {}.begin(), nullptr);
    
    EXPECT_EQ(beman::optional::optional<test::no_default_ctor> {}.begin(), nullptr);
    
    EXPECT_EQ(beman::optional::optional<test::base> {}.begin(), nullptr);
    
    EXPECT_EQ(beman::optional::optional<test::derived> {}.begin(), nullptr);
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
    EXPECT_EQ(beman::optional::optional<int> {}.end(), nullptr);

    EXPECT_EQ(beman::optional::optional<test::empty> {}.end(), nullptr);
    
    EXPECT_EQ(beman::optional::optional<test::no_default_ctor> {}.end(), nullptr);
    
    EXPECT_EQ(beman::optional::optional<test::base> {}.end(), nullptr);
    
    EXPECT_EQ(beman::optional::optional<test::derived> {}.end(), nullptr);
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

TEST(RangeSupportTest, LoopOverEmptyRange) {
    beman::optional::optional<int> empty;
    ASSERT_FALSE(empty.has_value());

    for(auto _ : empty) {
        ASSERT_TRUE(false) << "Should not be reached: expected not to loop over empty optional";
    }
}

TEST(RangeSupportTest, LoopOverNonEmptyRange) {
    const auto expected_value = 0xCAFEBABE;
    beman::optional::optional<int> empty{expected_value};
    ASSERT_TRUE(empty.has_value());

    bool entered_loop = false;
    for(auto i : empty) {
        EXPECT_EQ(i, expected_value);
        entered_loop = true;
    }
    EXPECT_TRUE(entered_loop);
}

TEST(RangeSupportTest, ConceptCheck) {
    // enable_view_for_optional
    static_assert(std::ranges::range<beman::optional::optional<int>>);
    static_assert(std::ranges::view<beman::optional::optional<int>>);
    static_assert(std::ranges::input_range<beman::optional::optional<int>>);
    static_assert(std::ranges::forward_range<beman::optional::optional<int>>);
    static_assert(std::ranges::bidirectional_range<beman::optional::optional<int>>);
    static_assert(std::ranges::contiguous_range<beman::optional::optional<int>>);
    static_assert(std::ranges::common_range<beman::optional::optional<int>>);
    static_assert(std::ranges::viewable_range<beman::optional::optional<int>>);
    static_assert(!std::ranges::borrowed_range<beman::optional::optional<int>>);
    static_assert(std::ranges::random_access_range<beman::optional::optional<int>>);
    static_assert(std::ranges::sized_range<beman::optional::optional<int>>);

    static_assert(std::ranges::range<beman::optional::optional<int*>>);
    static_assert(std::ranges::view<beman::optional::optional<int*>>);
    static_assert(std::ranges::input_range<beman::optional::optional<int*>>);
    static_assert(std::ranges::forward_range<beman::optional::optional<int*>>);
    static_assert(std::ranges::bidirectional_range<beman::optional::optional<int*>>);
    static_assert(std::ranges::contiguous_range<beman::optional::optional<int*>>);
    static_assert(std::ranges::common_range<beman::optional::optional<int*>>);
    static_assert(std::ranges::viewable_range<beman::optional::optional<int*>>);
    static_assert(std::ranges::borrowed_range<beman::optional::optional<int*>>);
    static_assert(std::ranges::random_access_range<beman::optional::optional<int*>>);

    using ref = std::reference_wrapper<int>;
    static_assert(std::ranges::range<beman::optional::optional<ref>>);
    static_assert(std::ranges::view<beman::optional::optional<ref>>);
    static_assert(std::ranges::input_range<beman::optional::optional<ref>>);
    static_assert(std::ranges::forward_range<beman::optional::optional<ref>>);
    static_assert(std::ranges::bidirectional_range<beman::optional::optional<ref>>);
    static_assert(std::ranges::contiguous_range<beman::optional::optional<ref>>);
    static_assert(std::ranges::common_range<beman::optional::optional<ref>>);
    static_assert(std::ranges::viewable_range<beman::optional::optional<ref>>);
    static_assert(std::ranges::borrowed_range<beman::optional::optional<ref>>);
    static_assert(std::ranges::random_access_range<beman::optional::optional<ref>>);
}

// TODO iterator tests

TEST(RangeSupportTest, FormatOptionalIsStillDisabled) {
// Since P3168R1: Give std::optional Range Support.
// Note: At 13.06.2024, gcc and msvc do not implement P2585R1: "Improve default container formatting".
// TODO @neatudarius: always enable once supported by other compilers.
#if defined(__clang__)
    EXPECT_EQ(std::format_kind<beman::optional::optional<int>>, std::range_format::disabled);

    // Next line shold not compile: formatter is not defined for std::optional and bemans::optional::optional.
    // EXPECT_EQ(std::format("{}", beman::optional::optional<int> {}), "<placeholder>");
#endif
}

