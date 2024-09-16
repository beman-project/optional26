// src/Beman/Optional26/tests/optional_constexpr.t.cpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Optional26/optional.hpp>
#include <Beman/Optional26/tests/test_types.hpp>
#include <Beman/Optional26/tests/test_utilities.hpp>

#include <functional>
#include <ranges>
#include <tuple>

#include <gtest/gtest.h>

TEST(OptionalConstexprTest, Constructors) {
    constexpr beman::optional26::optional<int> i1;
    constexpr beman::optional26::optional<int> i2{beman::optional26::nullopt};
    std::ignore = i1;
    std::ignore = i2;

    constexpr int                              i  = 0;
    constexpr beman::optional26::optional<int> i3 = i;
    std::ignore                                   = i3;

    constexpr beman::optional26::optional<beman::optional26::tests::empty> e1;
    constexpr beman::optional26::optional<int>                             e2{beman::optional26::nullopt};

    constexpr beman::optional26::tests::empty                              e{};
    constexpr beman::optional26::optional<beman::optional26::tests::empty> e3 = e;
    std::ignore                                                               = e1;
    std::ignore                                                               = e2;
    std::ignore                                                               = e;
    std::ignore                                                               = e3;
}

TEST(OptionalConstexprTest, Constructors2) {
    constexpr beman::optional26::optional<int> o1;
    EXPECT_TRUE(!o1);

    constexpr beman::optional26::optional<int> o2 = beman::optional26::nullopt;
    EXPECT_TRUE(!o2);

    constexpr beman::optional26::optional<int> o3 = 42;
    EXPECT_TRUE(*o3 == 42);

    constexpr beman::optional26::optional<int> o4 = o3;
    EXPECT_TRUE(*o4 == 42);

    constexpr beman::optional26::optional<int> o5 = o1;
    EXPECT_TRUE(!o5);

    constexpr beman::optional26::optional<int> o6 = std::move(o3);
    EXPECT_TRUE(*o6 == 42);

    constexpr beman::optional26::optional<short> o7 = 42;
    EXPECT_TRUE(*o7 == 42);

    constexpr beman::optional26::optional<int> o8 = o7;
    EXPECT_TRUE(*o8 == 42);

    constexpr beman::optional26::optional<int> o9 = std::move(o7);
    EXPECT_TRUE(*o9 == 42);

    {
        constexpr beman::optional26::optional<int&> o;
        EXPECT_TRUE(!o);

        constexpr beman::optional26::optional<int&> oo = o;
        EXPECT_TRUE(!oo);
    }

    {
        static constexpr auto                             i = 42;
        constexpr beman::optional26::optional<const int&> o = i;
        EXPECT_TRUE(o);
        EXPECT_TRUE(*o == 42);

        constexpr beman::optional26::optional<const int&> oo = o;
        EXPECT_TRUE(oo);
        EXPECT_TRUE(*oo == 42);
    }
}

TEST(OptionalConstexprTest, Constructors3) {
    constexpr beman::optional26::optional<int> ie;
    constexpr beman::optional26::optional<int> i4 = ie;
    EXPECT_FALSE(i4);

    using beman::optional26::tests::base;
    using beman::optional26::tests::derived;

    constexpr base                              b{1};
    constexpr derived                           d(1, 2);
    constexpr beman::optional26::optional<base> b1{b};
    constexpr beman::optional26::optional<base> b2{d};

    constexpr beman::optional26::optional<derived> d2{d};
    constexpr beman::optional26::optional<base>    b3 = d2;
    constexpr beman::optional26::optional<base>    b4{d2};
    std::ignore = b1;
    std::ignore = b2;
    std::ignore = b3;
    std::ignore = b4;
    std::ignore = d2;
}

namespace {
class NoDefault {
    int v_;

  public:
    constexpr NoDefault(int v) : v_(v) {}
    constexpr int value() const { return v_; }
};
} // namespace

TEST(OptionalConstexprTest, NonDefaultConstruct) {
    constexpr NoDefault                              i = 7;
    constexpr beman::optional26::optional<NoDefault> v1{};
    constexpr beman::optional26::optional<NoDefault> v2{i};
    std::ignore = v1;
    std::ignore = v2;
}

consteval bool testConstexprAssignmentValue() {
    bool                             retval = true;
    beman::optional26::optional<int> o1     = 42;
    beman::optional26::optional<int> o2     = 12;
    beman::optional26::optional<int> o3;

    o1 = static_cast<beman::optional26::optional<int>&>(o1);
    retval &= (*o1 == 42);

    o1 = o2;
    retval &= (*o1 == 12);

    o1 = o3;
    retval &= (!o1);

    o1 = 42;
    retval &= (*o1 == 42);

    o1 = beman::optional26::nullopt;
    retval &= (!o1);

    o1 = std::move(o2);
    retval &= (*o1 == 12);

    beman::optional26::optional<short> o4 = 42;

    o1 = o4;
    retval &= (*o1 == 42);

    o1 = std::move(o4);
    retval &= (*o1 == 42);

    /*
      template <class U = T>
      constexpr optional& operator=(U&& u)
    */
    short s = 54;
    o1      = s;
    retval &= (*o1 == 54);

    struct not_trivial_copy_assignable {
        int i_;
        constexpr not_trivial_copy_assignable(int i) : i_(i) {}
        constexpr not_trivial_copy_assignable(const not_trivial_copy_assignable&) = default;
        constexpr not_trivial_copy_assignable& operator=(const not_trivial_copy_assignable& rhs) {
            i_ = rhs.i_;
            return *this;
        }
    };

    /*
      optional& operator=(const optional& rhs)
      requires std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> &&
      (!std::is_trivially_copy_assignable_v<T>)
    */
    beman::optional26::optional<not_trivial_copy_assignable> o5{5};
    beman::optional26::optional<not_trivial_copy_assignable> o6;
    o6 = o5;
    retval &= (o5->i_ == 5);
    return retval;
}

static_assert(testConstexprAssignmentValue());

struct takes_init_and_variadic {
    int                  v0;
    std::tuple<int, int> t;
    template <class... Args>
    constexpr takes_init_and_variadic(std::initializer_list<int>& l, Args&&... args)
        : v0{*std::begin(l)}, t(std::forward<Args>(args)...) {}
};

consteval bool testConstexprInPlace() {
    bool                                       retval = true;
    constexpr beman::optional26::optional<int> o1{beman::optional26::in_place};
    constexpr beman::optional26::optional<int> o2(beman::optional26::in_place);
    retval &= (bool(o1));
    retval &= (o1 == 0);
    retval &= (bool(o2));
    retval &= (o2 == 0);

    constexpr beman::optional26::optional<int> o3(beman::optional26::in_place, 42);
    retval &= (o3 == 42);

    constexpr beman::optional26::optional<std::tuple<int, int>> o4(beman::optional26::in_place, 0, 1);
    retval &= (bool(o4));
    retval &= (std::get<0>(*o4) == 0);
    retval &= (std::get<1>(*o4) == 1);

    constexpr beman::optional26::optional<takes_init_and_variadic> o6(beman::optional26::in_place, {0, 1}, 2, 3);
    retval &= (o6->v0 == 0);
    retval &= (std::get<0>(o6->t) == 2);
    retval &= (std::get<1>(o6->t) == 3);
    return retval;
}

using beman::optional26::tests::constify;

TEST(OptionalConstexprTest, InPlace) { EXPECT_TRUE(constify(testConstexprInPlace())); }

TEST(OptionalConstexprTest, MakeOptional) {
    constexpr auto o1 = beman::optional26::make_optional(42);
    constexpr auto o2 = beman::optional26::optional<int>(42);

    constexpr bool is_same = std::is_same<decltype(o1), const beman::optional26::optional<int>>::value;
    EXPECT_TRUE(is_same);
    EXPECT_TRUE(o1 == o2);

    constexpr auto o3 = beman::optional26::make_optional<std::tuple<int, int, int, int>>(0, 1, 2, 3);
    EXPECT_TRUE(std::get<0>(*o3) == 0);
    EXPECT_TRUE(std::get<1>(*o3) == 1);
    EXPECT_TRUE(std::get<2>(*o3) == 2);
    EXPECT_TRUE(std::get<3>(*o3) == 3);

    constexpr auto o5 = beman::optional26::make_optional<takes_init_and_variadic>({0, 1}, 2, 3);
    EXPECT_TRUE(o5->v0 == 0);
    EXPECT_TRUE(std::get<0>(o5->t) == 2);
    EXPECT_TRUE(std::get<1>(o5->t) == 3);

    static constexpr auto i  = 42;
    constexpr auto        o6 = beman::optional26::make_optional<const int&>(i);
    static_assert(std::is_same<decltype(o6), const beman::optional26::optional<int>>::value);

    EXPECT_TRUE((std::is_same<decltype(o6), const beman::optional26::optional<int>>::value));
    EXPECT_TRUE(o6);
    EXPECT_TRUE(*o6 == 42);
}

TEST(OptionalConstexprTest, Nullopt) {
    constexpr beman::optional26::optional<int> o1 = beman::optional26::nullopt;
    constexpr beman::optional26::optional<int> o2{beman::optional26::nullopt};
    constexpr beman::optional26::optional<int> o3(beman::optional26::nullopt);
    constexpr beman::optional26::optional<int> o4 = {beman::optional26::nullopt};

    EXPECT_TRUE(constify(!o1));
    EXPECT_TRUE(constify(!o2));
    EXPECT_TRUE(constify(!o3));
    EXPECT_TRUE(constify(!o4));

    EXPECT_TRUE(!std::is_default_constructible<beman::optional26::nullopt_t>::value);
}

TEST(OptionalConstexprTest, Observers) {
    constexpr beman::optional26::optional<int> o1 = 42;
    constexpr beman::optional26::optional<int> o2;
    constexpr beman::optional26::optional<int> o3 = 42;

    EXPECT_TRUE(*o1 == 42);
    EXPECT_TRUE(*o1 == o1.value());
    EXPECT_TRUE(o2.value_or(42) == 42);
    EXPECT_TRUE(o3.value() == 42);
    constexpr auto success = std::is_same<decltype(o1.value()), const int&>::value;
    EXPECT_TRUE(success);
    constexpr auto success2 = std::is_same<decltype(o3.value()), const int&>::value;
    EXPECT_TRUE(success2);
    constexpr auto success3 = std::is_same<decltype(std::move(o1).value()), const int&>::value;
    EXPECT_TRUE(success3);
}

TEST(OptionalConstexprTest, RelationalOps) {
    constexpr beman::optional26::optional<int> o1{4};
    constexpr beman::optional26::optional<int> o2{42};
    constexpr beman::optional26::optional<int> o3{};

    //  SECTION("self simple")
    {
        EXPECT_TRUE(constify(!(o1 == o2)));
        EXPECT_TRUE(constify((o1 == o1)));
        EXPECT_TRUE(constify(!(o1 == o2)));
        EXPECT_TRUE(constify(!(o1 != o1)));
        EXPECT_TRUE(constify((o1 < o2)));
        EXPECT_TRUE(constify(!(o1 < o1)));
        EXPECT_TRUE(constify(!(o1 > o2)));
        EXPECT_TRUE(constify(!(o1 > o1)));
        EXPECT_TRUE(constify((o1 <= o2)));
        EXPECT_TRUE(constify((o1 <= o1)));
        EXPECT_TRUE(constify(!(o1 >= o2)));
        EXPECT_TRUE(constify(o1 >= o1));
    }
    //  SECTION("nullopt simple")
    {
        {
            EXPECT_TRUE(constify((!(o1 == beman::optional26::nullopt))));
            EXPECT_TRUE(constify((!(beman::optional26::nullopt == o1))));
            EXPECT_TRUE(constify((o1 != beman::optional26::nullopt)));
            EXPECT_TRUE(constify((beman::optional26::nullopt != o1)));
            EXPECT_TRUE(constify((!(o1 < beman::optional26::nullopt))));
            EXPECT_TRUE(constify((beman::optional26::nullopt < o1)));
            EXPECT_TRUE(constify((o1 > beman::optional26::nullopt)));
            EXPECT_TRUE(constify((!(beman::optional26::nullopt > o1))));
            EXPECT_TRUE(constify((!(o1 <= beman::optional26::nullopt))));
            EXPECT_TRUE(constify((beman::optional26::nullopt <= o1)));
            EXPECT_TRUE(constify((o1 >= beman::optional26::nullopt)));
            EXPECT_TRUE(constify((!(beman::optional26::nullopt >= o1))));
            EXPECT_TRUE(constify((o3 == beman::optional26::nullopt)));
            EXPECT_TRUE(constify((beman::optional26::nullopt == o3)));
            EXPECT_TRUE(constify((!(o3 != beman::optional26::nullopt))));
            EXPECT_TRUE(constify((!(beman::optional26::nullopt != o3))));
            EXPECT_TRUE(constify((!(o3 < beman::optional26::nullopt))));
            EXPECT_TRUE(constify((!(beman::optional26::nullopt < o3))));
            EXPECT_TRUE(constify((!(o3 > beman::optional26::nullopt))));
            EXPECT_TRUE(constify((!(beman::optional26::nullopt > o3))));
            EXPECT_TRUE(constify((o3 <= beman::optional26::nullopt)));
            EXPECT_TRUE(constify((beman::optional26::nullopt <= o3)));
            EXPECT_TRUE(constify((o3 >= beman::optional26::nullopt)));
            EXPECT_TRUE(constify((beman::optional26::nullopt >= o3)));
        }
    }
    //  SECTION("with T simple")
    {
        {
            EXPECT_TRUE(constify((!(o1 == 1))));
            EXPECT_TRUE(constify((!(1 == o1))));
            EXPECT_TRUE(constify((o1 != 1)));
            EXPECT_TRUE(constify((1 != o1)));
            EXPECT_TRUE(constify((!(o1 < 1))));
            EXPECT_TRUE(constify((1 < o1)));
            EXPECT_TRUE(constify((o1 > 1)));
            EXPECT_TRUE(constify((!(1 > o1))));
            EXPECT_TRUE(constify((!(o1 <= 1))));
            EXPECT_TRUE(constify((1 <= o1)));
            EXPECT_TRUE(constify((o1 >= 1)));
            EXPECT_TRUE(constify((!(1 >= o1))));
        }

        {
            EXPECT_TRUE(constify((o1 == 4)));
            EXPECT_TRUE(constify((4 == o1)));
            EXPECT_TRUE(constify((!(o1 != 4))));
            EXPECT_TRUE(constify((!(4 != o1))));
            EXPECT_TRUE(constify((!(o1 < 4))));
            EXPECT_TRUE(constify((!(4 < o1))));
            EXPECT_TRUE(constify((!(o1 > 4))));
            EXPECT_TRUE(constify((!(4 > o1))));
            EXPECT_TRUE(constify((o1 <= 4)));
            EXPECT_TRUE(constify((4 <= o1)));
            EXPECT_TRUE(constify((o1 >= 4)));
            EXPECT_TRUE(constify((4 >= o1)));
        }
    }

    using beman::optional26::tests::Point;

    constexpr Point p4{2, 3};
    constexpr Point p5{3, 4};

    constexpr beman::optional26::optional<Point> o4{p4};
    constexpr beman::optional26::optional<Point> o5{p5};

    //  SECTION("self complex")
    {
        {
            EXPECT_TRUE(constify((!(o4 == o5))));
            EXPECT_TRUE(constify((o4 == o4)));
            EXPECT_TRUE(constify((o4 != o5)));
            EXPECT_TRUE(constify((!(o4 != o4))));
            EXPECT_TRUE(constify((o4 < o5)));
            EXPECT_TRUE(constify((!(o4 < o4))));
            EXPECT_TRUE(constify((!(o4 > o5))));
            EXPECT_TRUE(constify((!(o4 > o4))));
            EXPECT_TRUE(constify((o4 <= o5)));
            EXPECT_TRUE(constify((o4 <= o4)));
            EXPECT_TRUE(constify((!(o4 >= o5))));
            EXPECT_TRUE(constify((o4 >= o4)));
        }
    }
    //  SECTION("nullopt complex")
    {
        {
            EXPECT_TRUE(constify((!(o4 == beman::optional26::nullopt))));
            EXPECT_TRUE(constify((!(beman::optional26::nullopt == o4))));
            EXPECT_TRUE(constify((o4 != beman::optional26::nullopt)));
            EXPECT_TRUE(constify((beman::optional26::nullopt != o4)));
            EXPECT_TRUE(constify((!(o4 < beman::optional26::nullopt))));
            EXPECT_TRUE(constify((beman::optional26::nullopt < o4)));
            EXPECT_TRUE(constify((o4 > beman::optional26::nullopt)));
            EXPECT_TRUE(constify((!(beman::optional26::nullopt > o4))));
            EXPECT_TRUE(constify((!(o4 <= beman::optional26::nullopt))));
            EXPECT_TRUE(constify((beman::optional26::nullopt <= o4)));
            EXPECT_TRUE(constify((o4 >= beman::optional26::nullopt)));
            EXPECT_TRUE(constify((!(beman::optional26::nullopt >= o4))));
        }

        {
            EXPECT_TRUE(constify((o3 == beman::optional26::nullopt)));
            EXPECT_TRUE(constify((beman::optional26::nullopt == o3)));
            EXPECT_TRUE(constify((!(o3 != beman::optional26::nullopt))));
            EXPECT_TRUE(constify((!(beman::optional26::nullopt != o3))));
            EXPECT_TRUE(constify((!(o3 < beman::optional26::nullopt))));
            EXPECT_TRUE(constify((!(beman::optional26::nullopt < o3))));
            EXPECT_TRUE(constify((!(o3 > beman::optional26::nullopt))));
            EXPECT_TRUE(constify((!(beman::optional26::nullopt > o3))));
            EXPECT_TRUE(constify((o3 <= beman::optional26::nullopt)));
            EXPECT_TRUE(constify((beman::optional26::nullopt <= o3)));
            EXPECT_TRUE(constify((o3 >= beman::optional26::nullopt)));
            EXPECT_TRUE(constify((beman::optional26::nullopt >= o3)));
        }
    }

    //  SECTION("with T complex")
    {
        {
            EXPECT_TRUE(constify((!(o4 == Point{}))));
            EXPECT_TRUE(constify((!(Point{} == o4))));
            EXPECT_TRUE(constify((o4 != Point{})));
            EXPECT_TRUE(constify((Point{} != o4)));
            EXPECT_TRUE(constify((!(o4 < Point{}))));
            EXPECT_TRUE(constify((Point{} < o4)));
            EXPECT_TRUE(constify((o4 > Point{})));
            EXPECT_TRUE(constify((!(Point{} > o4))));
            EXPECT_TRUE(constify((!(o4 <= Point{}))));
            EXPECT_TRUE(constify((Point{} <= o4)));
            EXPECT_TRUE(constify((o4 >= Point{})));
            EXPECT_TRUE(constify((!(Point{} >= o4))));
        }

        {
            EXPECT_TRUE(constify((o4 == p4)));
            EXPECT_TRUE(constify((p4 == o4)));
            EXPECT_TRUE(constify((!(o4 != p4))));
            EXPECT_TRUE(constify((!(p4 != o4))));
            EXPECT_TRUE(constify((!(o4 < p4))));
            EXPECT_TRUE(constify((!(p4 < o4))));
            EXPECT_TRUE(constify((!(o4 > p4))));
            EXPECT_TRUE(constify((!(p4 > o4))));
            EXPECT_TRUE(constify((o4 <= p4)));
            EXPECT_TRUE(constify((p4 <= o4)));
            EXPECT_TRUE(constify((o4 >= p4)));
            EXPECT_TRUE(constify((p4 >= o4)));
        }
    }
}

consteval bool testComparisons() {
    constexpr beman::optional26::optional<int> o1{4};
    constexpr beman::optional26::optional<int> o2{42};
    constexpr beman::optional26::optional<int> o3{};

    //  SECTION("self simple")
    {
        {
            constexpr auto b = !(o1 == o2);
            static_assert(b);
        }

        {
            constexpr auto b = (o1 == o1);
            static_assert(b);
        }

        {
            constexpr auto b = !(o1 == o2);
            static_assert(b);
        }

        {
            constexpr auto b = !(o1 != o1);
            static_assert(b);
        }

        {
            constexpr auto b = (o1 < o2);
            static_assert(b);
        }

        {
            constexpr auto b = !(o1 < o1);
            static_assert(b);
        }

        {
            constexpr auto b = !(o1 > o2);
            static_assert(b);
        }

        {
            constexpr auto b = !(o1 > o1);
            static_assert(b);
        }

        {
            constexpr auto b = (o1 <= o2);
            static_assert(b);
        }

        {
            constexpr auto b = (o1 <= o1);
            static_assert(b);
        }

        {
            constexpr auto b = !(o1 >= o2);
            static_assert(b);
        }

        {
            constexpr auto b = o1 >= o1;
            static_assert(b);
        }
    }
    //  SECTION("nullopt simple")
    {
        {
            constexpr auto b = (!(o1 == beman::optional26::nullopt));
            static_assert(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt == o1));
            static_assert(b);
        }

        {
            constexpr auto b = (o1 != beman::optional26::nullopt);
            static_assert(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt != o1);
            static_assert(b);
        }

        {
            constexpr auto b = (!(o1 < beman::optional26::nullopt));
            static_assert(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt < o1);
            static_assert(b);
        }

        {
            constexpr auto b = (o1 > beman::optional26::nullopt);
            static_assert(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt > o1));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o1 <= beman::optional26::nullopt));
            static_assert(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt <= o1);
            static_assert(b);
        }

        {
            constexpr auto b = (o1 >= beman::optional26::nullopt);
            static_assert(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt >= o1));
            static_assert(b);
        }

        {
            constexpr auto b = (o3 == beman::optional26::nullopt);
            static_assert(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt == o3);
            static_assert(b);
        }

        {
            constexpr auto b = (!(o3 != beman::optional26::nullopt));
            static_assert(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt != o3));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o3 < beman::optional26::nullopt));
            static_assert(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt < o3));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o3 > beman::optional26::nullopt));
            static_assert(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt > o3));
            static_assert(b);
        }

        {
            constexpr auto b = (o3 <= beman::optional26::nullopt);
            static_assert(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt <= o3);
            static_assert(b);
        }

        {
            constexpr auto b = (o3 >= beman::optional26::nullopt);
            static_assert(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt >= o3);
            static_assert(b);
        }
    }
    //  SECTION("with T simple")
    {
        {
            constexpr auto b = (!(o1 == 1));
            static_assert(b);
        }

        {
            constexpr auto b = (!(1 == o1));
            static_assert(b);
        }

        {
            constexpr auto b = (o1 != 1);
            static_assert(b);
        }

        {
            constexpr auto b = (1 != o1);
            static_assert(b);
        }

        {
            constexpr auto b = (!(o1 < 1));
            static_assert(b);
        }

        {
            constexpr auto b = (1 < o1);
            static_assert(b);
        }

        {
            constexpr auto b = (o1 > 1);
            static_assert(b);
        }

        {
            constexpr auto b = (!(1 > o1));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o1 <= 1));
            static_assert(b);
        }

        {
            constexpr auto b = (1 <= o1);
            static_assert(b);
        }

        {
            constexpr auto b = (o1 >= 1);
            static_assert(b);
        }

        {
            constexpr auto b = (!(1 >= o1));
            static_assert(b);
        }

        {
            constexpr auto b = (o1 == 4);
            static_assert(b);
        }

        {
            constexpr auto b = (4 == o1);
            static_assert(b);
        }

        {
            constexpr auto b = (!(o1 != 4));
            static_assert(b);
        }

        {
            constexpr auto b = (!(4 != o1));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o1 < 4));
            static_assert(b);
        }

        {
            constexpr auto b = (!(4 < o1));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o1 > 4));
            static_assert(b);
        }

        {
            constexpr auto b = (!(4 > o1));
            static_assert(b);
        }

        {
            constexpr auto b = (o1 <= 4);
            static_assert(b);
        }

        {
            constexpr auto b = (4 <= o1);
            static_assert(b);
        }

        {
            constexpr auto b = (o1 >= 4);
            static_assert(b);
        }

        {
            constexpr auto b = (4 >= o1);
            static_assert(b);
        }
    }

    using beman::optional26::tests::Point;

    constexpr Point p4{2, 3};
    constexpr Point p5{3, 4};

    constexpr beman::optional26::optional<Point> o4{p4};
    constexpr beman::optional26::optional<Point> o5{p5};

    //  SECTION("self complex")
    {
        {
            constexpr auto b = (!(o4 == o5));
            static_assert(b);
        }

        {
            constexpr auto b = (o4 == o4);
            static_assert(b);
        }

        {
            constexpr auto b = (o4 != o5);
            static_assert(b);
        }

        {
            constexpr auto b = (!(o4 != o4));
            static_assert(b);
        }

        {
            constexpr auto b = (o4 < o5);
            static_assert(b);
        }

        {
            constexpr auto b = (!(o4 < o4));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o4 > o5));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o4 > o4));
            static_assert(b);
        }

        {
            constexpr auto b = (o4 <= o5);
            static_assert(b);
        }

        {
            constexpr auto b = (o4 <= o4);
            static_assert(b);
        }

        {
            constexpr auto b = (!(o4 >= o5));
            static_assert(b);
        }

        {
            constexpr auto b = (o4 >= o4);
            static_assert(b);
        }
    }
    //  SECTION("nullopt complex")
    {
        {
            constexpr auto b = (!(o4 == beman::optional26::nullopt));
            static_assert(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt == o4));
            static_assert(b);
        }

        {
            constexpr auto b = (o4 != beman::optional26::nullopt);
            static_assert(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt != o4);
            static_assert(b);
        }

        {
            constexpr auto b = (!(o4 < beman::optional26::nullopt));
            static_assert(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt < o4);
            static_assert(b);
        }

        {
            constexpr auto b = (o4 > beman::optional26::nullopt);
            static_assert(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt > o4));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o4 <= beman::optional26::nullopt));
            static_assert(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt <= o4);
            static_assert(b);
        }

        {
            constexpr auto b = (o4 >= beman::optional26::nullopt);
            static_assert(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt >= o4));
            static_assert(b);
        }

        {
            constexpr auto b = (o3 == beman::optional26::nullopt);
            static_assert(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt == o3);
            static_assert(b);
        }

        {
            constexpr auto b = (!(o3 != beman::optional26::nullopt));
            static_assert(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt != o3));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o3 < beman::optional26::nullopt));
            static_assert(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt < o3));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o3 > beman::optional26::nullopt));
            static_assert(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt > o3));
            static_assert(b);
        }

        {
            constexpr auto b = (o3 <= beman::optional26::nullopt);
            static_assert(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt <= o3);
            static_assert(b);
        }

        {
            constexpr auto b = (o3 >= beman::optional26::nullopt);
            static_assert(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt >= o3);
            static_assert(b);
        }
    }

    //  SECTION("with T complex")
    {
        {
            constexpr auto b = (!(o4 == Point{}));
            static_assert(b);
        }

        {
            constexpr auto b = (!(Point{} == o4));
            static_assert(b);
        }

        {
            constexpr auto b = (o4 != Point{});
            static_assert(b);
        }

        {
            constexpr auto b = (Point{} != o4);
            static_assert(b);
        }

        {
            constexpr auto b = (!(o4 < Point{}));
            static_assert(b);
        }

        {
            constexpr auto b = (Point{} < o4);
            static_assert(b);
        }

        {
            constexpr auto b = (o4 > Point{});
            static_assert(b);
        }

        {
            constexpr auto b = (!(Point{} > o4));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o4 <= Point{}));
            static_assert(b);
        }

        {
            constexpr auto b = (Point{} <= o4);
            static_assert(b);
        }

        {
            constexpr auto b = (o4 >= Point{});
            static_assert(b);
        }

        {
            constexpr auto b = (!(Point{} >= o4));
            static_assert(b);
        }

        {
            constexpr auto b = (o4 == p4);
            static_assert(b);
        }

        {
            constexpr auto b = (p4 == o4);
            static_assert(b);
        }

        {
            constexpr auto b = (!(o4 != p4));
            static_assert(b);
        }

        {
            constexpr auto b = (!(p4 != o4));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o4 < p4));
            static_assert(b);
        }

        {
            constexpr auto b = (!(p4 < o4));
            static_assert(b);
        }

        {
            constexpr auto b = (!(o4 > p4));
            static_assert(b);
        }

        {
            constexpr auto b = (!(p4 > o4));
            static_assert(b);
        }

        {
            constexpr auto b = (o4 <= p4);
            static_assert(b);
        }

        {
            constexpr auto b = (p4 <= o4);
            static_assert(b);
        }

        {
            constexpr auto b = (o4 >= p4);
            static_assert(b);
        }

        {
            constexpr auto b = (p4 >= o4);
            static_assert(b);
        }
    }
    return true;
}

constexpr bool checkTestComparison = testComparisons();
static_assert(checkTestComparison);

TEST(OptionalConstexprTest, RangeTest) {
    constexpr beman::optional26::optional<int> o1 = beman::optional26::nullopt;
    constexpr beman::optional26::optional<int> o2 = 42;
    EXPECT_EQ(*o2, 42);
    for (auto k : o1) {
        std::ignore = k;
        EXPECT_TRUE(false);
    }
    for (auto k : o2) {
        EXPECT_EQ(k, 42);
    }
}

consteval bool testSwap() {
    beman::optional26::optional<int> o1 = 42;
    beman::optional26::optional<int> o2 = 12;
    o1.swap(o2);
    return (o1.value() == 12) && (o2.value() == 42);
}
static_assert(testSwap());

consteval bool testSwapWNull() {
    beman::optional26::optional<int> o1 = 42;
    beman::optional26::optional<int> o2 = beman::optional26::nullopt;
    o1.swap(o2);
    return (!o1.has_value()) && (o2.value(), 42);
}
static_assert(testSwapWNull());

consteval bool testSwapNullIntializedWithValue() {
    beman::optional26::optional<int> o1 = beman::optional26::nullopt;
    beman::optional26::optional<int> o2 = 42;
    o1.swap(o2);
    return (o1.value() == 42) && (!o2.has_value());
}
static_assert(testSwapNullIntializedWithValue());

consteval bool testEmplace() {
    beman::optional26::optional<std::pair<std::pair<int, int>, std::pair<double, double>>> i;
    i.emplace(std::piecewise_construct, std::make_tuple(0, 2), std::make_tuple(3, 4));
    return (i->first.first == 0) && (i->first.second == 2) && (i->second.first == 3) && (i->second.second == 4);
}
static_assert(testEmplace());

consteval bool testEmplaceInitList() {
    beman::optional26::optional<takes_init_and_variadic> o;
    o.emplace({0, 1}, 2, 3);
    return (o->v0 == 0) && (std::get<0>(o->t) == 2) && (std::get<1>(o->t) == 3);
}
static_assert(testEmplaceInitList());

consteval bool testAssignment() {
    beman::optional26::optional<int> o1 = 42;
    beman::optional26::optional<int> o2 = 12;

    bool retval = true;

    o2 = std::move(o1);
    retval &= (*o1 == 42);

    o1 = {};

    return retval;
}
static_assert(testAssignment());

consteval bool testAssignmentValue() {
    beman::optional26::optional<int> o1 = 42;
    beman::optional26::optional<int> o2 = 12;
    beman::optional26::optional<int> o3;

    bool retval = true;

    retval &= (*o1 == 42);

    o1 = o2;
    retval &= (*o1 == 12);

    o1 = o3;
    retval &= (!o1);

    o1 = 42;
    retval &= (*o1 == 42);

    o1 = beman::optional26::nullopt;
    retval &= (!o1);

    o1 = std::move(o2);
    retval &= (*o1 == 12);

    beman::optional26::optional<short> o4 = 42;

    o1 = o4;
    retval &= (*o1 == 42);

    o1 = std::move(o4);
    retval &= (*o1 == 42);

    /*
      template <class U = T>
      constexpr optional& operator=(U&& u)
    */

    short s = 54;
    o1      = s;
    retval &= (*o1 == 54);

    struct not_trivial_copy_assignable {
        int i_;
        constexpr not_trivial_copy_assignable(int i) : i_(i) {}
        constexpr not_trivial_copy_assignable(const not_trivial_copy_assignable&) = default;
        constexpr not_trivial_copy_assignable& operator=(const not_trivial_copy_assignable& rhs) {
            i_ = rhs.i_;
            return *this;
        }
    };
    static_assert(!std::is_trivially_copy_assignable_v<not_trivial_copy_assignable>);

    /*
      optional& operator=(const optional& rhs)
        requires std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> &&
        (!std::is_trivially_copy_assignable_v<T>)
    */
    beman::optional26::optional<not_trivial_copy_assignable> o5{5};
    beman::optional26::optional<not_trivial_copy_assignable> o6;
    o6 = o5;
    retval &= (o5->i_ == 5);

    return retval;
}

static_assert(testAssignmentValue());
