// src/Beman/Optional26/tests/optional.t.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Optional26/optional.hpp>

#include <Beman/Optional26/optional.hpp> // test 2nd include OK

#include <functional>
#include <ranges>
#include <tuple>

#include <gtest/gtest.h>

TEST(OptionalTest, TestGTest) { ASSERT_EQ(1, 1); }

namespace {
struct empty {};
struct no_default {
    no_default()                             = delete;
    no_default(const no_default&)            = default;
    no_default(no_default&&)                 = default;
    no_default& operator=(const no_default&) = default;
    no_default& operator=(no_default&&)      = default;
    no_default(empty){};
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
} // namespace

TEST(OptionalTest, Constructors) {
    beman::optional26::optional<int> i1;
    beman::optional26::optional<int> i2{beman::optional26::nullopt};

    int                          i  = 0;
    beman::optional26::optional<int> i3 = i;
    (void)i3;

    beman::optional26::optional<empty> e1;
    beman::optional26::optional<int>   e2{beman::optional26::nullopt};

    empty                          e{};
    beman::optional26::optional<empty> e3 = e;
    (void)e3;
}


TEST(OptionalTest, Constructors2) {
    beman::optional26::optional<int> o1;
    EXPECT_TRUE(!o1);

    beman::optional26::optional<int> o2 = beman::optional26::nullopt;
    EXPECT_TRUE(!o2);

    beman::optional26::optional<int> o3 = 42;
    EXPECT_TRUE(*o3 == 42);

    beman::optional26::optional<int> o4 = o3;
    EXPECT_TRUE(*o4 == 42);

    beman::optional26::optional<int> o5 = o1;
    EXPECT_TRUE(!o5);

    beman::optional26::optional<int> o6 = std::move(o3);
    EXPECT_TRUE(*o6 == 42);

    beman::optional26::optional<short> o7 = 42;
    EXPECT_TRUE(*o7 == 42);

    beman::optional26::optional<int> o8 = o7;
    EXPECT_TRUE(*o8 == 42);

    beman::optional26::optional<int> o9 = std::move(o7);
    EXPECT_TRUE(*o9 == 42);

    {
        beman::optional26::optional<int&> o;
        EXPECT_TRUE(!o);

        beman::optional26::optional<int&> oo = o;
        EXPECT_TRUE(!oo);
    }

    {
        auto                                    i = 42;
        beman::optional26::optional<int&> o = i;
        EXPECT_TRUE(o);
        EXPECT_TRUE(*o == 42);

        beman::optional26::optional<int&> oo = o;
        EXPECT_TRUE(oo);
        EXPECT_TRUE(*oo == 42);
    }

    std::vector<base> v;
    v.emplace_back();
    beman::optional26::optional<std::vector<base>> ov = std::move(v);
    EXPECT_TRUE(ov->size() == 1);
}

TEST(OptionalTest, Constructors3) {
    beman::optional26::optional<int> ie;
    beman::optional26::optional<int> i4 = ie;
    EXPECT_FALSE(i4);

    base                           b{1};
    derived                        d(1, 2);
    beman::optional26::optional<base> b1{b};
    beman::optional26::optional<base> b2{d};

    beman::optional26::optional<derived> d2{d};
    beman::optional26::optional<base>    b3 = d2;
    beman::optional26::optional<base>    b4{d2};
}

namespace {
class NoDefault {
    int v_;

  public:
    NoDefault(int v) : v_(v) {}
};
} // namespace

TEST(OptionalTest, NonDefaultConstruct) {
    NoDefault                          i = 7;
    beman::optional26::optional<NoDefault> v1{};
    beman::optional26::optional<NoDefault> v2{i};
}

TEST(OptionalTest, AssignmentValue) {
    beman::optional26::optional<int> o1 = 42;
    beman::optional26::optional<int> o2 = 12;
    beman::optional26::optional<int> o3;

    o1 = o1;
    EXPECT_TRUE(*o1 == 42);

    o1 = o2;
    EXPECT_TRUE(*o1 == 12);

    o1 = o3;
    EXPECT_TRUE(!o1);

    o1 = 42;
    EXPECT_TRUE(*o1 == 42);

    o1 = beman::optional26::nullopt;
    EXPECT_TRUE(!o1);

    o1 = std::move(o2);
    EXPECT_TRUE(*o1 == 12);

    beman::optional26::optional<short> o4 = 42;

    o1 = o4;
    EXPECT_TRUE(*o1 == 42);

    o1 = std::move(o4);
    EXPECT_TRUE(*o1 == 42);
}

TEST(OptionalTest, Triviality) {
    EXPECT_TRUE(std::is_trivially_copy_constructible<
                beman::optional26::optional<int>>::value);
    EXPECT_TRUE(std::is_trivially_copy_assignable<
                beman::optional26::optional<int>>::value);
    EXPECT_TRUE(std::is_trivially_move_constructible<
                beman::optional26::optional<int>>::value);
    EXPECT_TRUE(std::is_trivially_move_assignable<
                beman::optional26::optional<int>>::value);
    EXPECT_TRUE(
        std::is_trivially_destructible<beman::optional26::optional<int>>::value);

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = default;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = default;
            ~T()                   = default;
        };
        EXPECT_TRUE(std::is_trivially_copy_constructible<
                    beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_trivially_copy_assignable<
                    beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_trivially_move_constructible<
                    beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_trivially_move_assignable<
                    beman::optional26::optional<T>>::value);
        EXPECT_TRUE(
            std::is_trivially_destructible<beman::optional26::optional<T>>::value);
    }

    {
        struct T {
            T(const T&) {}
            T(T&&){};
            T& operator=(const T&) { return *this; }
            T& operator=(T&&) { return *this; };
            ~T() {}
        };
        EXPECT_TRUE(!std::is_trivially_copy_constructible<
                    beman::optional26::optional<T>>::value);
        EXPECT_TRUE(!std::is_trivially_copy_assignable<
                    beman::optional26::optional<T>>::value);
        EXPECT_TRUE(!std::is_trivially_move_constructible<
                    beman::optional26::optional<T>>::value);
        EXPECT_TRUE(!std::is_trivially_move_assignable<
                    beman::optional26::optional<T>>::value);
        EXPECT_TRUE(!std::is_trivially_destructible<
                    beman::optional26::optional<T>>::value);
    }
}

TEST(OptionalTest, Deletion) {
    EXPECT_TRUE(
        std::is_copy_constructible<beman::optional26::optional<int>>::value);
    EXPECT_TRUE(std::is_copy_assignable<beman::optional26::optional<int>>::value);
    EXPECT_TRUE(
        std::is_move_constructible<beman::optional26::optional<int>>::value);
    EXPECT_TRUE(std::is_move_assignable<beman::optional26::optional<int>>::value);
    EXPECT_TRUE(std::is_destructible<beman::optional26::optional<int>>::value);

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = default;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = default;
            ~T()                   = default;
        };
        EXPECT_TRUE(
            std::is_copy_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(
            std::is_copy_assignable<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(
            std::is_move_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(
            std::is_move_assignable<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_destructible<beman::optional26::optional<T>>::value);
    }

    {
        struct T {
            T(const T&)            = delete;
            T(T&&)                 = delete;
            T& operator=(const T&) = delete;
            T& operator=(T&&)      = delete;
        };
        EXPECT_TRUE(
            !std::is_copy_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(
            !std::is_copy_assignable<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(
            !std::is_move_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(
            !std::is_move_assignable<beman::optional26::optional<T>>::value);
    }

    {
        struct T {
            T(const T&)            = delete;
            T(T&&)                 = default;
            T& operator=(const T&) = delete;
            T& operator=(T&&)      = default;
        };
        EXPECT_TRUE(
            !std::is_copy_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(
            !std::is_copy_assignable<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(
            std::is_move_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(
            std::is_move_assignable<beman::optional26::optional<T>>::value);
    }

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = delete;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = delete;
        };
        EXPECT_TRUE(
            std::is_copy_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(
            std::is_copy_assignable<beman::optional26::optional<T>>::value);
    }
}

struct takes_init_and_variadic {
    std::vector<int>     v;
    std::tuple<int, int> t;
    template <class... Args>
    takes_init_and_variadic(std::initializer_list<int> l, Args&&... args)
        : v(l), t(std::forward<Args>(args)...) {}
};

TEST(OptionalTest, InPlace) {
    beman::optional26::optional<int> o1{beman::optional26::in_place};
    beman::optional26::optional<int> o2(beman::optional26::in_place);
    EXPECT_TRUE(o1);
    EXPECT_TRUE(o1 == 0);
    EXPECT_TRUE(o2);
    EXPECT_TRUE(o2 == 0);

    beman::optional26::optional<int> o3(beman::optional26::in_place, 42);
    EXPECT_TRUE(o3 == 42);

    beman::optional26::optional<std::tuple<int, int>> o4(
        beman::optional26::in_place, 0, 1);
    EXPECT_TRUE(o4);
    EXPECT_TRUE(std::get<0>(*o4) == 0);
    EXPECT_TRUE(std::get<1>(*o4) == 1);

    beman::optional26::optional<std::vector<int>> o5(beman::optional26::in_place,
                                                 {0, 1});
    EXPECT_TRUE(o5);
    EXPECT_TRUE((*o5)[0] == 0);
    EXPECT_TRUE((*o5)[1] == 1);

    beman::optional26::optional<takes_init_and_variadic> o6(
        beman::optional26::in_place, {0, 1}, 2, 3);
    EXPECT_TRUE(o6->v[0] == 0);
    EXPECT_TRUE(o6->v[1] == 1);
    EXPECT_TRUE(std::get<0>(o6->t) == 2);
    EXPECT_TRUE(std::get<1>(o6->t) == 3);
}

TEST(OptionalTest, MakeOptional) {
    auto o1 = beman::optional26::make_optional(42);
    auto o2 = beman::optional26::optional<int>(42);

    constexpr bool is_same =
        std::is_same<decltype(o1), beman::optional26::optional<int>>::value;
    EXPECT_TRUE(is_same);
    EXPECT_TRUE(o1 == o2);

    auto o3 = beman::optional26::make_optional<std::tuple<int, int, int, int>>(
        0, 1, 2, 3);
    EXPECT_TRUE(std::get<0>(*o3) == 0);
    EXPECT_TRUE(std::get<1>(*o3) == 1);
    EXPECT_TRUE(std::get<2>(*o3) == 2);
    EXPECT_TRUE(std::get<3>(*o3) == 3);

    auto o4 = beman::optional26::make_optional<std::vector<int>>({0, 1, 2, 3});
    EXPECT_TRUE(o4.value()[0] == 0);
    EXPECT_TRUE(o4.value()[1] == 1);
    EXPECT_TRUE(o4.value()[2] == 2);
    EXPECT_TRUE(o4.value()[3] == 3);

    auto o5 =
        beman::optional26::make_optional<takes_init_and_variadic>({0, 1}, 2, 3);
    EXPECT_TRUE(o5->v[0] == 0);
    EXPECT_TRUE(o5->v[1] == 1);
    EXPECT_TRUE(std::get<0>(o5->t) == 2);
    EXPECT_TRUE(std::get<1>(o5->t) == 3);

    auto i  = 42;
    auto o6 = beman::optional26::make_optional<int&>(i);
    static_assert(std::is_same<decltype(o6), beman::optional26::optional<int>>::value);

    EXPECT_TRUE(
        (std::is_same<decltype(o6), beman::optional26::optional<int>>::value));
    EXPECT_TRUE(o6);
    EXPECT_TRUE(*o6 == 42);
}

TEST(OptionalTest, Nullopt) {
    beman::optional26::optional<int> o1 = beman::optional26::nullopt;
    beman::optional26::optional<int> o2{beman::optional26::nullopt};
    beman::optional26::optional<int> o3(beman::optional26::nullopt);
    beman::optional26::optional<int> o4 = {beman::optional26::nullopt};

    EXPECT_TRUE(!o1);
    EXPECT_TRUE(!o2);
    EXPECT_TRUE(!o3);
    EXPECT_TRUE(!o4);

    EXPECT_TRUE(
        !std::is_default_constructible<beman::optional26::nullopt_t>::value);
}

struct move_detector {
    move_detector() = default;
    move_detector(move_detector&& rhs) { rhs.been_moved = true; }
    bool been_moved = false;
};

TEST(OptionalTest, Observers) {
    beman::optional26::optional<int>       o1 = 42;
    beman::optional26::optional<int>       o2;
    const beman::optional26::optional<int> o3 = 42;

    EXPECT_TRUE(*o1 == 42);
    EXPECT_TRUE(*o1 == o1.value());
    EXPECT_TRUE(o2.value_or(42) == 42);
    EXPECT_TRUE(o3.value() == 42);
    auto success = std::is_same<decltype(o1.value()), int&>::value;
    EXPECT_TRUE(success);
    success = std::is_same<decltype(o3.value()), const int&>::value;
    EXPECT_TRUE(success);
    success = std::is_same<decltype(std::move(o1).value()), int&&>::value;
    EXPECT_TRUE(success);

    beman::optional26::optional<move_detector> o4{beman::optional26::in_place};
    move_detector                          o5 = std::move(o4).value();
    EXPECT_TRUE(o4->been_moved);
    EXPECT_TRUE(!o5.been_moved);
}

TEST(OptionalTest, RelationalOps) {
    beman::optional26::optional<int> o1{4};
    beman::optional26::optional<int> o2{42};
    beman::optional26::optional<int> o3{};

    //  SECTION("self simple")
    {
        EXPECT_TRUE(!(o1 == o2));
        EXPECT_TRUE(o1 == o1);
        EXPECT_TRUE(o1 != o2);
        EXPECT_TRUE(!(o1 != o1));
        EXPECT_TRUE(o1 < o2);
        EXPECT_TRUE(!(o1 < o1));
        EXPECT_TRUE(!(o1 > o2));
        EXPECT_TRUE(!(o1 > o1));
        EXPECT_TRUE(o1 <= o2);
        EXPECT_TRUE(o1 <= o1);
        EXPECT_TRUE(!(o1 >= o2));
        EXPECT_TRUE(o1 >= o1);
    }
    //  SECTION("nullopt simple")
    {
        EXPECT_TRUE(!(o1 == beman::optional26::nullopt));
        EXPECT_TRUE(!(beman::optional26::nullopt == o1));
        EXPECT_TRUE(o1 != beman::optional26::nullopt);
        EXPECT_TRUE(beman::optional26::nullopt != o1);
        EXPECT_TRUE(!(o1 < beman::optional26::nullopt));
        EXPECT_TRUE(beman::optional26::nullopt < o1);
        EXPECT_TRUE(o1 > beman::optional26::nullopt);
        EXPECT_TRUE(!(beman::optional26::nullopt > o1));
        EXPECT_TRUE(!(o1 <= beman::optional26::nullopt));
        EXPECT_TRUE(beman::optional26::nullopt <= o1);
        EXPECT_TRUE(o1 >= beman::optional26::nullopt);
        EXPECT_TRUE(!(beman::optional26::nullopt >= o1));

        EXPECT_TRUE(o3 == beman::optional26::nullopt);
        EXPECT_TRUE(beman::optional26::nullopt == o3);
        EXPECT_TRUE(!(o3 != beman::optional26::nullopt));
        EXPECT_TRUE(!(beman::optional26::nullopt != o3));
        EXPECT_TRUE(!(o3 < beman::optional26::nullopt));
        EXPECT_TRUE(!(beman::optional26::nullopt < o3));
        EXPECT_TRUE(!(o3 > beman::optional26::nullopt));
        EXPECT_TRUE(!(beman::optional26::nullopt > o3));
        EXPECT_TRUE(o3 <= beman::optional26::nullopt);
        EXPECT_TRUE(beman::optional26::nullopt <= o3);
        EXPECT_TRUE(o3 >= beman::optional26::nullopt);
        EXPECT_TRUE(beman::optional26::nullopt >= o3);
    }
    //  SECTION("with T simple")
    {
        EXPECT_TRUE(!(o1 == 1));
        EXPECT_TRUE(!(1 == o1));
        EXPECT_TRUE(o1 != 1);
        EXPECT_TRUE(1 != o1);
        EXPECT_TRUE(!(o1 < 1));
        EXPECT_TRUE(1 < o1);
        EXPECT_TRUE(o1 > 1);
        EXPECT_TRUE(!(1 > o1));
        EXPECT_TRUE(!(o1 <= 1));
        EXPECT_TRUE(1 <= o1);
        EXPECT_TRUE(o1 >= 1);
        EXPECT_TRUE(!(1 >= o1));

        EXPECT_TRUE(o1 == 4);
        EXPECT_TRUE(4 == o1);
        EXPECT_TRUE(!(o1 != 4));
        EXPECT_TRUE(!(4 != o1));
        EXPECT_TRUE(!(o1 < 4));
        EXPECT_TRUE(!(4 < o1));
        EXPECT_TRUE(!(o1 > 4));
        EXPECT_TRUE(!(4 > o1));
        EXPECT_TRUE(o1 <= 4);
        EXPECT_TRUE(4 <= o1);
        EXPECT_TRUE(o1 >= 4);
        EXPECT_TRUE(4 >= o1);
    }
    beman::optional26::optional<std::string> o4{"hello"};
    beman::optional26::optional<std::string> o5{"xyz"};

    //  SECTION("self complex")
    {
        EXPECT_TRUE(!(o4 == o5));
        EXPECT_TRUE(o4 == o4);
        EXPECT_TRUE(o4 != o5);
        EXPECT_TRUE(!(o4 != o4));
        EXPECT_TRUE(o4 < o5);
        EXPECT_TRUE(!(o4 < o4));
        EXPECT_TRUE(!(o4 > o5));
        EXPECT_TRUE(!(o4 > o4));
        EXPECT_TRUE(o4 <= o5);
        EXPECT_TRUE(o4 <= o4);
        EXPECT_TRUE(!(o4 >= o5));
        EXPECT_TRUE(o4 >= o4);
    }
    //  SECTION("nullopt complex")
    {
        EXPECT_TRUE(!(o4 == beman::optional26::nullopt));
        EXPECT_TRUE(!(beman::optional26::nullopt == o4));
        EXPECT_TRUE(o4 != beman::optional26::nullopt);
        EXPECT_TRUE(beman::optional26::nullopt != o4);
        EXPECT_TRUE(!(o4 < beman::optional26::nullopt));
        EXPECT_TRUE(beman::optional26::nullopt < o4);
        EXPECT_TRUE(o4 > beman::optional26::nullopt);
        EXPECT_TRUE(!(beman::optional26::nullopt > o4));
        EXPECT_TRUE(!(o4 <= beman::optional26::nullopt));
        EXPECT_TRUE(beman::optional26::nullopt <= o4);
        EXPECT_TRUE(o4 >= beman::optional26::nullopt);
        EXPECT_TRUE(!(beman::optional26::nullopt >= o4));

        EXPECT_TRUE(o3 == beman::optional26::nullopt);
        EXPECT_TRUE(beman::optional26::nullopt == o3);
        EXPECT_TRUE(!(o3 != beman::optional26::nullopt));
        EXPECT_TRUE(!(beman::optional26::nullopt != o3));
        EXPECT_TRUE(!(o3 < beman::optional26::nullopt));
        EXPECT_TRUE(!(beman::optional26::nullopt < o3));
        EXPECT_TRUE(!(o3 > beman::optional26::nullopt));
        EXPECT_TRUE(!(beman::optional26::nullopt > o3));
        EXPECT_TRUE(o3 <= beman::optional26::nullopt);
        EXPECT_TRUE(beman::optional26::nullopt <= o3);
        EXPECT_TRUE(o3 >= beman::optional26::nullopt);
        EXPECT_TRUE(beman::optional26::nullopt >= o3);
    }

    //  SECTION("with T complex")
    {
        EXPECT_TRUE(!(o4 == "a"));
        EXPECT_TRUE(!("a" == o4));
        EXPECT_TRUE(o4 != "a");
        EXPECT_TRUE("a" != o4);
        EXPECT_TRUE(!(o4 < "a"));
        EXPECT_TRUE("a" < o4);
        EXPECT_TRUE(o4 > "a");
        EXPECT_TRUE(!("a" > o4));
        EXPECT_TRUE(!(o4 <= "a"));
        EXPECT_TRUE("a" <= o4);
        EXPECT_TRUE(o4 >= "a");
        EXPECT_TRUE(!("a" >= o4));

        EXPECT_TRUE(o4 == "hello");
        EXPECT_TRUE("hello" == o4);
        EXPECT_TRUE(!(o4 != "hello"));
        EXPECT_TRUE(!("hello" != o4));
        EXPECT_TRUE(!(o4 < "hello"));
        EXPECT_TRUE(!("hello" < o4));
        EXPECT_TRUE(!(o4 > "hello"));
        EXPECT_TRUE(!("hello" > o4));
        EXPECT_TRUE(o4 <= "hello");
        EXPECT_TRUE("hello" <= o4);
        EXPECT_TRUE(o4 >= "hello");
        EXPECT_TRUE("hello" >= o4);
    }
}

TEST(OptionalTest, SwapValue) {
    beman::optional26::optional<int> o1 = 42;
    beman::optional26::optional<int> o2 = 12;
    o1.swap(o2);
    EXPECT_EQ(o1.value(), 12);
    EXPECT_EQ(o2.value(), 42);
}

TEST(OptionalTest, SwapWNull) {
    beman::optional26::optional<int> o1 = 42;
    beman::optional26::optional<int> o2 = beman::optional26::nullopt;
    o1.swap(o2);
    EXPECT_TRUE(!o1.has_value());
    EXPECT_EQ(o2.value(), 42);
}

TEST(OptionalTest, SwapNullIntializedWithValue) {
    beman::optional26::optional<int> o1 = beman::optional26::nullopt;
    beman::optional26::optional<int> o2 = 42;
    o1.swap(o2);
    EXPECT_EQ(o1.value(), 42);
    EXPECT_TRUE(!o2.has_value());
}

TEST(OptionalTest, Emplace) {
    beman::optional26::optional<
        std::pair<std::pair<int, int>, std::pair<double, double>>>
        i;
    i.emplace(std::piecewise_construct,
              std::make_tuple(0, 2),
              std::make_tuple(3, 4));
    EXPECT_TRUE(i->first.first == 0);
    EXPECT_TRUE(i->first.second == 2);
    EXPECT_TRUE(i->second.first == 3);
    EXPECT_TRUE(i->second.second == 4);
}

struct A {
    A() { throw std::exception(); }
};

TEST(OptionalTest, EmplaceWithExceptionThrown) {
    beman::optional26::optional<A> a;
    EXPECT_ANY_THROW(a.emplace());
}

TEST(OptionalTest, RangeTest) {
    beman::optional26::optional<int> o1 = beman::optional26::nullopt;
    beman::optional26::optional<int> o2 = 42;
    EXPECT_EQ(*o2, 42);
    for(auto k : o1) {
        (void)k;
        EXPECT_TRUE(false);
    }
    for(auto k : o2) {
        EXPECT_EQ(k, 42);
    }
}

TEST(ViewMaybeTest, Constructors) {
    std::ranges::single_view<std::optional<int>> s;
    std::ranges::single_view<std::optional<int>> s2{s};
    std::ranges::single_view<std::optional<int>> s3{std::optional<int>{}};

    beman::optional26::optional<std::optional<int>> n;
    beman::optional26::optional<std::optional<int>> n2{n};
    beman::optional26::optional<std::optional<int>> n3{std::optional<int>{}};
}


TEST(ViewMaybeTest, ConceptCheckRef) {
    static_assert(std::ranges::range<beman::optional26::optional<int&>>);
    static_assert(std::ranges::view<beman::optional26::optional<int&>>);
    static_assert(std::ranges::input_range<beman::optional26::optional<int&>>);
    static_assert(std::ranges::forward_range<beman::optional26::optional<int&>>);
    static_assert(std::ranges::bidirectional_range<beman::optional26::optional<int&>>);
    static_assert(std::ranges::contiguous_range<beman::optional26::optional<int&>>);
    static_assert(std::ranges::common_range<beman::optional26::optional<int&>>);
    static_assert(std::ranges::viewable_range<beman::optional26::optional<int&>>);
    static_assert(std::ranges::borrowed_range<beman::optional26::optional<int&>>);
    static_assert(std::ranges::random_access_range<beman::optional26::optional<int&>>);

    static_assert(std::ranges::range<beman::optional26::optional<int*&>>);
    static_assert(std::ranges::view<beman::optional26::optional<int*&>>);
    static_assert(std::ranges::input_range<beman::optional26::optional<int*&>>);
    static_assert(std::ranges::forward_range<beman::optional26::optional<int*&>>);
    static_assert(std::ranges::bidirectional_range<beman::optional26::optional<int*&>>);
    static_assert(std::ranges::contiguous_range<beman::optional26::optional<int*&>>);
    static_assert(std::ranges::common_range<beman::optional26::optional<int*&>>);
    static_assert(std::ranges::viewable_range<beman::optional26::optional<int*&>>);
    static_assert(std::ranges::borrowed_range<beman::optional26::optional<int*&>>);
    static_assert(std::ranges::random_access_range<beman::optional26::optional<int*&>>);

    using ref = std::reference_wrapper<int>&;
    static_assert(std::ranges::range<beman::optional26::optional<ref>>);
    static_assert(std::ranges::view<beman::optional26::optional<ref>>);
    static_assert(std::ranges::input_range<beman::optional26::optional<ref>>);
    static_assert(std::ranges::forward_range<beman::optional26::optional<ref>>);
    static_assert(std::ranges::bidirectional_range<beman::optional26::optional<ref>>);
    static_assert(std::ranges::contiguous_range<beman::optional26::optional<ref>>);
    static_assert(std::ranges::common_range<beman::optional26::optional<ref>>);
    static_assert(std::ranges::viewable_range<beman::optional26::optional<ref>>);
    static_assert(std::ranges::borrowed_range<beman::optional26::optional<ref>>);
    static_assert(std::ranges::random_access_range<beman::optional26::optional<ref>>);
}

TEST(ViewMaybeTest, BreathingTest) {
    beman::optional26::optional<int> m;
    // ASSERT_TRUE(m.empty());
    // ASSERT_TRUE(m.size() == 0);
    // ASSERT_TRUE(m.data() == nullptr);

    beman::optional26::optional<int> m1{1};
    // ASSERT_TRUE(!m1.empty());
    // ASSERT_TRUE(m1.size() == 1);
    // ASSERT_TRUE(m1.data() != nullptr);
    // ASSERT_TRUE(*(m1.data()) == 1);

    m = m1;
    ASSERT_EQ(*std::begin(m), 1);

    m = {};
    // ASSERT_TRUE(m.size() == 0);
    // ASSERT_TRUE(m1.size() == 1);

    beman::optional26::optional<double> d0{0};
    // ASSERT_TRUE(!d0.empty());

    beman::optional26::optional<double> d1{1};
    // ASSERT_TRUE(!d1.empty());

    d0 = d1;
    ASSERT_EQ(*std::begin(d0), 1.0);
}

TEST(ViewMaybeTest, BreathingTestRef) {
    beman::optional26::optional<int&> m;
    // ASSERT_TRUE(m.empty());
    // ASSERT_TRUE(m.size() == 0);
    // ASSERT_TRUE(m.data() == nullptr);

    int              one = 1;
    beman::optional26::optional<int&> m1{one};
    // ASSERT_TRUE(!m1.empty());
    // ASSERT_TRUE(m1.size() == 1);
    // ASSERT_TRUE(m1.data() != nullptr);
    // ASSERT_TRUE(*(m1.data()) == 1);

    m = m1;
    ASSERT_EQ(*std::begin(m), 1);

    // m = {};
    // ASSERT_TRUE(m.size() == 0);
    // ASSERT_TRUE(m1.size() == 1);

    double              zero = 0.0;
    beman::optional26::optional<double&> d0{zero};
    // ASSERT_TRUE(!d0.empty());

    double              one_d = 1.0;
    beman::optional26::optional<double&> d1{one_d};
    // ASSERT_TRUE(!d1.empty());

    d0 = d1;
    ASSERT_EQ(*std::begin(d0), 1.0);
}

TEST(ViewMaybe, CompTest) {
    beman::optional26::optional<int> m;
    beman::optional26::optional<int> m0{0};
    beman::optional26::optional<int> m1{1};
    beman::optional26::optional<int> m1a{1};

    ASSERT_EQ(m, m);
    ASSERT_EQ(m0, m0);
    ASSERT_EQ(m1, m1);
    ASSERT_EQ(m1a, m1a);
    ASSERT_EQ(m1, m1a);

    ASSERT_NE(m, m0);
    ASSERT_NE(m0, m1);

    ASSERT_TRUE(m < m0);
    ASSERT_TRUE(m0 < m1);
    ASSERT_TRUE(m1 <= m1a);
}

TEST(ViewMaybe, CompTestRef) {
    beman::optional26::optional<int&> m;
    int              zero  = 0;
    int              one   = 1;
    int              one_a = 1;
    beman::optional26::optional<int&> m0{zero};
    beman::optional26::optional<int&> m1{one};
    beman::optional26::optional<int&> m1a{one_a};

    ASSERT_EQ(m, m);
    ASSERT_EQ(m0, m0);
    ASSERT_EQ(m1, m1);
    ASSERT_EQ(m1a, m1a);
    ASSERT_EQ(m1, m1a);

    ASSERT_NE(m, m0);
    ASSERT_NE(m0, m1);

    ASSERT_TRUE(m < m0);
    ASSERT_TRUE(m0 > m);
    ASSERT_TRUE(m0 < m1);
    ASSERT_TRUE(m1a <= m1);
}

// "and_then" creates a new view by applying a
// transformation to each element in an input
// range, and flattening the resulting range of
// ranges. A.k.a. bind
// (This uses one syntax for constrained lambdas
// in C++20.)
inline constexpr auto and_then = [](auto&& r, auto fun) {
    return decltype(r)(r) | std::views::transform(std::move(fun)) |
           std::views::join;
};

// "yield_if" takes a bool and a value and
// returns a view of zero or one elements.
inline constexpr auto yield_if = []<class T>(bool b, T x) {
    return b ?
           beman::optional26::optional<T>{move(x)} :
           beman::optional26::nullopt;
};


TEST(ViewMaybeTest, PythTripleTest) {
    using std::views::iota;
    auto triples = and_then(iota(1), [](int z) {
        return and_then(iota(1, z + 1), [=](int x) {
            return and_then(iota(x, z + 1), [=](int y) {
                return yield_if(x * x + y * y == z * z,
                                std::make_tuple(x, y, z));
            });
        });
    });

    ASSERT_EQ(*std::ranges::begin(triples), std::make_tuple(3, 4, 5));
}

using namespace beman;

TEST(ViewMaybeTest, ValueBase) {
    int             i = 7;
    beman::optional26::optional<int> v1{};
    // ASSERT_TRUE(v1.size() == 0);

    beman::optional26::optional<int> v2{i};
    // ASSERT_TRUE(v2.size() == 1);
    for (auto i : v1)
        ASSERT_TRUE(i != i); // tautology so i is used and not warned


    for (auto i : v2)
        ASSERT_EQ(i, 7);

//     ASSERT_EQ(v2[0], 7);  // no match for operator[]
//    auto x = v2[1000];

    // int s = 4;
    // for (auto&& i : views::maybe(s)) {
    //     ASSERT_EQ(i, 4);
    //     i = 9;
    //     ASSERT_EQ(i, 9);
    // }
    // ASSERT_EQ(s, 4);
}

TEST(ViewMaybeTest, RefWrapper) {
    int i = 7;

    beman::optional26::optional<int> v2{std::ref(i)};

    for (auto i : v2)
        ASSERT_EQ(i, 7);

    // int s = 4;
    // for (auto&& i : views::maybe(std::ref(s))) {
    //     ASSERT_EQ(i, 4);
    //     i.get() = 9;
    //     ASSERT_EQ(i, 9);
    // }
    // ASSERT_EQ(s, 9);
}


TEST(ViewMaybeTest, ValueNonDefaultConstruct) {
    NoDefault             i = 7;
    beman::optional26::optional<NoDefault> v1{};
    beman::optional26::optional<NoDefault> v2{i};
}

TEST(ViewMaybeTest, RefBase) {
    int              i = 7;
    beman::optional26::optional<int&> v1{};
    // ASSERT_TRUE(v1.size() == 0);

    beman::optional26::optional<int&> v2{i};
    // ASSERT_TRUE(v2.size() == 1);
    for (auto i : v1)
        ASSERT_TRUE(i != i); // tautology so i is used and not warned

    for (auto i : v2) {
        ASSERT_EQ(i, 7);
        i = 9;
        ASSERT_EQ(i, 9);
    }
    ASSERT_EQ(i, 7);

    for (auto&& i : v2) {
        ASSERT_EQ(i, 7);
        i = 9;
        ASSERT_EQ(i, 9);
    }
    ASSERT_EQ(i, 9);

    int s = 4;
    // for (auto&& i : views::maybe(s)) {
    //     ASSERT_EQ(i, 4);
    //     i = 9;
    //     ASSERT_EQ(i, 9);
    // }
    // ASSERT_EQ(s, 4);

    for (auto&& i : beman::optional26::optional<int&>(s)) {
        ASSERT_EQ(i, 4);
        i = 9;
        ASSERT_EQ(i, 9);
    }
    ASSERT_EQ(s, 9);
}

// TEST(ViewMaybeTest, MonadicAndThen) {
//     beman::optional26::optional<int> mv{40};
//     auto            r = mv.and_then([](int i) { return beman::optional26::optional{i + 2}; });
//     ASSERT_TRUE(!r.empty());
//     ASSERT_TRUE(r.size() == 1);
//     ASSERT_TRUE(r.data() != nullptr);
//     ASSERT_TRUE(*(r.data()) == 42);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 40);

//     auto r2 = mv.and_then([](int) { return beman::optional26::optional<int>{}; });
//     ASSERT_TRUE(r2.empty());
//     ASSERT_TRUE(r2.size() == 0);
//     ASSERT_TRUE(r2.data() == nullptr);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 40);

//     beman::optional26::optional<int> empty{};

//     auto r3 = empty.and_then([](int i) { return beman::optional26::optional{i + 2}; });
//     ASSERT_TRUE(r3.empty());
//     ASSERT_TRUE(r3.size() == 0);
//     ASSERT_TRUE(r3.data() == nullptr);
//     ASSERT_TRUE(empty.empty());

//     auto r4 = mv.and_then([](double d) { return beman::optional26::optional{d + 2}; });
//     ASSERT_TRUE(!r4.empty());
//     ASSERT_TRUE(r4.size() == 1);
//     ASSERT_TRUE(*(r4.data()) == 42.0);
//     static_assert(std::is_same_v<decltype(r4), beman::optional26::optional<double>>);

//     auto r5 = std::move(mv).and_then([](int i) { return beman::optional26::optional{i + 2}; });
//     ASSERT_TRUE(!r5.empty());
//     ASSERT_TRUE(r5.size() == 1);
//     ASSERT_TRUE(r5.data() != nullptr);
//     ASSERT_TRUE(*(r5.data()) == 42);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 40);

//     auto r6 = std::move(mv).and_then([](int&& i) {
//         int k = i;
//         i     = 0;
//         return beman::optional26::optional{k + 2};
//     });
//     ASSERT_TRUE(!r6.empty());
//     ASSERT_TRUE(r6.size() == 1);
//     ASSERT_TRUE(r6.data() != nullptr);
//     ASSERT_TRUE(*(r6.data()) == 42);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 0);

//     const beman::optional26::optional<int> cmv{40};
//     auto r7 = cmv.and_then([](int i) { return beman::optional26::optional{i + 2}; });
//     ASSERT_TRUE(!r7.empty());
//     ASSERT_TRUE(r7.size() == 1);
//     ASSERT_TRUE(r7.data() != nullptr);
//     ASSERT_TRUE(*(r7.data()) == 42);
//     ASSERT_TRUE(!cmv.empty());
//     ASSERT_TRUE(*(cmv.data()) == 40);

//     auto r8 = std::move(cmv).and_then([](int i) { return beman::optional26::optional{i + 2}; });
//     ASSERT_TRUE(!r8.empty());
//     ASSERT_TRUE(r8.size() == 1);
//     ASSERT_TRUE(r8.data() != nullptr);
//     ASSERT_TRUE(*(r8.data()) == 42);
//     ASSERT_TRUE(!cmv.empty());
//     ASSERT_TRUE(*(cmv.data()) == 40);
// }

// TEST(MaybeView, MonadicTransform) {
//     beman::optional26::optional<int> mv{40};
//     auto            r = mv.transform([](int i) { return i + 2; });
//     ASSERT_TRUE(!r.empty());
//     ASSERT_TRUE(r.size() == 1);
//     ASSERT_TRUE(r.data() != nullptr);
//     ASSERT_TRUE(*(r.data()) == 42);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 40);

//     auto r2 = mv.transform([](int& i) {
//         i += 2;
//         return i;
//     });
//     ASSERT_TRUE(!r2.empty());
//     ASSERT_TRUE(r2.size() == 1);
//     ASSERT_TRUE(r2.data() != nullptr);
//     ASSERT_TRUE(*(r2.data()) == 42);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 42);

//     beman::optional26::optional<int> empty{};

//     auto r3 = empty.transform([](int i) { return i + 2; });
//     ASSERT_TRUE(r3.empty());
//     ASSERT_TRUE(r3.size() == 0);
//     ASSERT_TRUE(r3.data() == nullptr);
//     ASSERT_TRUE(empty.empty());

//     auto r4 = mv.transform([](double d) { return d + 2; });
//     ASSERT_TRUE(!r4.empty());
//     ASSERT_TRUE(r4.size() == 1);
//     ASSERT_TRUE(*(r4.data()) == 44.0);
//     static_assert(std::is_same_v<decltype(r4), beman::optional26::optional<double>>);

//     auto r5 = std::move(mv).transform([](int i) { return i + 2; });
//     ASSERT_TRUE(!r5.empty());
//     ASSERT_TRUE(r5.size() == 1);
//     ASSERT_TRUE(r5.data() != nullptr);
//     ASSERT_TRUE(*(r5.data()) == 44);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 42);

//     auto r6 = std::move(mv).transform([](int&& i) {
//         int k = i;
//         i     = 0;
//         return k + 2;
//     });
//     ASSERT_TRUE(!r6.empty());
//     ASSERT_TRUE(r6.size() == 1);
//     ASSERT_TRUE(r6.data() != nullptr);
//     ASSERT_TRUE(*(r6.data()) == 44);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 0);

//     const beman::optional26::optional<int> cmv{40};
//     auto                  r7 = cmv.transform([](int i) { return i + 2; });
//     ASSERT_TRUE(!r7.empty());
//     ASSERT_TRUE(r7.size() == 1);
//     ASSERT_TRUE(r7.data() != nullptr);
//     ASSERT_TRUE(*(r7.data()) == 42);
//     ASSERT_TRUE(!cmv.empty());
//     ASSERT_TRUE(*(cmv.data()) == 40);

//     auto r8 = std::move(cmv).transform([](int i) { return i + 2; });
//     ASSERT_TRUE(!r8.empty());
//     ASSERT_TRUE(r8.size() == 1);
//     ASSERT_TRUE(r8.data() != nullptr);
//     ASSERT_TRUE(*(r8.data()) == 42);
//     ASSERT_TRUE(!cmv.empty());
//     ASSERT_TRUE(*(cmv.data()) == 40);
// }

// TEST(MaybeView, MonadicOrElse) {
//     beman::optional26::optional<int> o1(42);
//     auto            r = o1.or_else([] { return beman::optional26::optional<int>(13); });
//     ASSERT_TRUE(*(r.data()) == 42);

//     beman::optional26::optional<int> o2;
//     ASSERT_TRUE(*(o2.or_else([] { return beman::optional26::optional<int>(13); })).data() ==
//                 13);

//     auto r2 = std::move(o1).or_else([] { return beman::optional26::optional<int>(13); });
//     ASSERT_TRUE(*(r2.data()) == 42);

//     auto r3 = std::move(o2).or_else([] { return beman::optional26::optional<int>(13); });
//     ASSERT_TRUE(*(r3.data()) == 13);
// }

// TEST(ViewMaybeTest, MonadicAndThenRef) {
//     int              forty{40};
//     beman::optional26::optional<int&> mv{forty};
//     auto             r = mv.and_then([](int i) { return beman::optional26::optional{i + 2}; });
//     ASSERT_TRUE(!r.empty());
//     ASSERT_TRUE(r.size() == 1);
//     ASSERT_TRUE(r.data() != nullptr);
//     ASSERT_TRUE(*(r.data()) == 42);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 40);

//     auto r2 = mv.and_then([](int) { return beman::optional26::optional<int&>{}; });
//     ASSERT_TRUE(r2.empty());
//     ASSERT_TRUE(r2.size() == 0);
//     ASSERT_TRUE(r2.data() == nullptr);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 40);

//     beman::optional26::optional<int&> empty{};

//     auto r3 = empty.and_then([](int i) { return beman::optional26::optional{i + 2}; });
//     ASSERT_TRUE(r3.empty());
//     ASSERT_TRUE(r3.size() == 0);
//     ASSERT_TRUE(r3.data() == nullptr);
//     ASSERT_TRUE(empty.empty());

//     auto r4 = mv.and_then([](double d) { return beman::optional26::optional{d + 2}; });
//     ASSERT_TRUE(!r4.empty());
//     ASSERT_TRUE(r4.size() == 1);
//     ASSERT_TRUE(*(r4.data()) == 42.0);
//     static_assert(std::is_same_v<decltype(r4), beman::optional26::optional<double>>);

//     auto r5 = std::move(mv).and_then([](int i) { return beman::optional26::optional{i + 2}; });
//     ASSERT_TRUE(!r5.empty());
//     ASSERT_TRUE(r5.size() == 1);
//     ASSERT_TRUE(r5.data() != nullptr);
//     ASSERT_TRUE(*(r5.data()) == 42);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 40);

//     auto r6 = std::move(mv).and_then([](int&& i) {
//         int k = i;
//         i     = 0;
//         return beman::optional26::optional{k + 2};
//     });
//     ASSERT_TRUE(!r6.empty());
//     ASSERT_TRUE(r6.size() == 1);
//     ASSERT_TRUE(r6.data() != nullptr);
//     ASSERT_TRUE(*(r6.data()) == 42);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 0);
//     ASSERT_EQ(forty, 0);
//     forty = 40;

//     const beman::optional26::optional<int&> cmv{forty};
//     auto r7 = cmv.and_then([](int i) { return beman::optional26::optional{i + 2}; });
//     ASSERT_TRUE(!r7.empty());
//     ASSERT_TRUE(r7.size() == 1);
//     ASSERT_TRUE(r7.data() != nullptr);
//     ASSERT_EQ(*(r7.data()), 42);
//     ASSERT_TRUE(!cmv.empty());
//     ASSERT_TRUE(*(cmv.data()) == 40);

//     auto r8 = std::move(cmv).and_then([](int i) { return beman::optional26::optional{i + 2}; });
//     ASSERT_TRUE(!r8.empty());
//     ASSERT_TRUE(r8.size() == 1);
//     ASSERT_TRUE(r8.data() != nullptr);
//     ASSERT_EQ(*(r8.data()), 42);
//     ASSERT_TRUE(!cmv.empty());
//     ASSERT_TRUE(*(cmv.data()) == 40);
// }

// TEST(MaybeView, MonadicTransformRef) {
//     int              forty{40};
//     beman::optional26::optional<int&> mv{forty};
//     auto             r = mv.transform([](int i) { return i + 2; });
//     ASSERT_TRUE(!r.empty());
//     ASSERT_TRUE(r.size() == 1);
//     ASSERT_TRUE(r.data() != nullptr);
//     ASSERT_EQ(*(r.data()), 42);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 40);

//     beman::optional26::optional<int&> empty{};

//     auto r3 = empty.transform([](int i) { return i + 2; });
//     ASSERT_TRUE(r3.empty());
//     ASSERT_TRUE(r3.size() == 0);
//     ASSERT_TRUE(r3.data() == nullptr);
//     ASSERT_TRUE(empty.empty());

//     auto r4 = mv.transform([](double d) { return d + 2; });
//     ASSERT_TRUE(!r4.empty());
//     ASSERT_TRUE(r4.size() == 1);
//     ASSERT_TRUE(*(r4.data()) == 42.0);
//     static_assert(std::is_same_v<decltype(r4), beman::optional26::optional<double>>);

//     auto r5 = std::move(mv).transform([](int i) { return i + 2; });
//     ASSERT_TRUE(!r5.empty());
//     ASSERT_TRUE(r5.size() == 1);
//     ASSERT_TRUE(r5.data() != nullptr);
//     ASSERT_TRUE(*(r5.data()) == 42);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 40);

//     auto r6 = std::move(mv).transform([](int&& i) {
//         int k = i;
//         i     = 0;
//         return k + 2;
//     });
//     ASSERT_TRUE(!r6.empty());
//     ASSERT_TRUE(r6.size() == 1);
//     ASSERT_TRUE(r6.data() != nullptr);
//     ASSERT_TRUE(*(r6.data()) == 42);
//     ASSERT_TRUE(!mv.empty());
//     ASSERT_TRUE(*(mv.data()) == 0);
//     ASSERT_EQ(forty, 0);
//     forty = 40;

//     const beman::optional26::optional<int&> cmv{forty};
//     ASSERT_EQ(*(cmv.data()), 40);
//     auto r7 = cmv.transform([](int i) { return i + 2; });
//     ASSERT_TRUE(!r7.empty());
//     ASSERT_TRUE(r7.size() == 1);
//     ASSERT_TRUE(r7.data() != nullptr);
//     ASSERT_TRUE(*(r7.data()) == 42);
//     ASSERT_TRUE(!cmv.empty());
//     ASSERT_TRUE(*(cmv.data()) == 40);

//     auto r8 = std::move(cmv).transform([](int i) { return i + 2; });
//     ASSERT_TRUE(!r8.empty());
//     ASSERT_TRUE(r8.size() == 1);
//     ASSERT_TRUE(r8.data() != nullptr);
//     ASSERT_TRUE(*(r8.data()) == 42);
//     ASSERT_TRUE(!cmv.empty());
//     ASSERT_TRUE(*(cmv.data()) == 40);

//     auto r9 = mv.transform([](int& i) {
//         int k = i;
//         i     = 56;
//         return k * 2;
//     });
//     ASSERT_TRUE(!r9.empty());
//     ASSERT_EQ(r9.size(), 1);
//     ASSERT_TRUE(r9.data() != nullptr);
//     for (auto r: r9) {
//       ASSERT_EQ(r, 80);
//     }
//     ASSERT_TRUE(!mv.empty());
//     for (auto v: mv) {
//       ASSERT_EQ(v, 56);
//     }
//     ASSERT_EQ(forty, 56);
//     forty = 40;
// }

// TEST(MaybeView, MonadicOrElseRef) {
//     int              fortytwo{42};
//     int              thirteen{13};
//     beman::optional26::optional<int&> o1(fortytwo);
//     auto r = o1.or_else([&thirteen] { return beman::optional26::optional<int&>(thirteen); });
//     ASSERT_TRUE(*(r.data()) == 42);

//     beman::optional26::optional<int&> o2;
//     ASSERT_TRUE(*(o2.or_else([&thirteen] {
//                      return beman::optional26::optional<int&>(thirteen);
//                  })).data() == 13);

//     auto r2 = std::move(o1).or_else(
//         [&thirteen] { return beman::optional26::optional<int&>(thirteen); });
//     ASSERT_TRUE(*(r2.data()) == 42);

//     auto r3 = std::move(o2).or_else(
//         [&thirteen] { return beman::optional26::optional<int&>(thirteen); });
//     ASSERT_TRUE(*(r3.data()) == 13);
// }
