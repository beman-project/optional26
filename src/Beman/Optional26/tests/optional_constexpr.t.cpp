// src/Beman/Optional26/tests/optional_constexpr.t.cpp                -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Optional26/optional.hpp>

#include <functional>
#include <ranges>
#include <tuple>

#include <gtest/gtest.h>

TEST(OptionalConstexprTest, TestGTest) { ASSERT_EQ(1, 1); }

namespace {
struct empty {};
struct no_default {
    no_default()                             = delete;
    no_default(const no_default&)            = default;
    no_default(no_default&&)                 = default;
    no_default& operator=(const no_default&) = default;
    no_default& operator=(no_default&&)      = default;
    no_default(empty) {};
};

struct base {
    int i_;
    constexpr base() : i_(0) {}
    constexpr base(int i) : i_(i) {}
};

struct derived : public base {
    int j_;
    constexpr derived() : base(0), j_(0) {}
    constexpr derived(int i, int j) : base(i), j_(j) {}
};
} // namespace

TEST(OptionalConstexprTest, Constructors) {
    constexpr beman::optional26::optional<int> i1;
    constexpr beman::optional26::optional<int> i2{beman::optional26::nullopt};

    constexpr int                              i  = 0;
    constexpr beman::optional26::optional<int> i3 = i;
    (void)i3;

    constexpr beman::optional26::optional<empty> e1;
    constexpr beman::optional26::optional<int>   e2{beman::optional26::nullopt};

    constexpr empty                              e{};
    constexpr beman::optional26::optional<empty> e3 = e;
    (void)e3;
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
        constexpr static auto                              i = 42;
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

    constexpr base                              b{1};
    constexpr derived                           d(1, 2);
    constexpr beman::optional26::optional<base> b1{b};
    constexpr beman::optional26::optional<base> b2{d};

    constexpr beman::optional26::optional<derived> d2{d};
    constexpr beman::optional26::optional<base>    b3 = d2;
    constexpr beman::optional26::optional<base>    b4{d2};
}

namespace {
class NoDefault {
    int v_;

  public:
    constexpr NoDefault(int v) : v_(v) {}
};
} // namespace

TEST(OptionalConstexprTest, NonDefaultConstruct) {
    constexpr NoDefault                              i = 7;
    constexpr beman::optional26::optional<NoDefault> v1{};
    constexpr beman::optional26::optional<NoDefault> v2{i};
}

TEST(OptionalConstexprTest, Triviality) {
    EXPECT_TRUE(std::is_trivially_copy_constructible<beman::optional26::optional<int>>::value);
    EXPECT_TRUE(std::is_trivially_copy_assignable<beman::optional26::optional<int>>::value);
    EXPECT_TRUE(std::is_trivially_move_constructible<beman::optional26::optional<int>>::value);
    EXPECT_TRUE(std::is_trivially_move_assignable<beman::optional26::optional<int>>::value);
    EXPECT_TRUE(std::is_trivially_destructible<beman::optional26::optional<int>>::value);

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = default;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = default;
            ~T()                   = default;
        };
        EXPECT_TRUE(std::is_trivially_copy_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_trivially_copy_assignable<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_trivially_move_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_trivially_move_assignable<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_trivially_destructible<beman::optional26::optional<T>>::value);
    }

    {
        struct T {
            T(const T&) {}
            T(T&&) {};
            T& operator=(const T&) { return *this; }
            T& operator=(T&&) { return *this; };
            ~T() {}
        };
        EXPECT_TRUE(!std::is_trivially_copy_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(!std::is_trivially_copy_assignable<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(!std::is_trivially_move_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(!std::is_trivially_move_assignable<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(!std::is_trivially_destructible<beman::optional26::optional<T>>::value);
    }
}

TEST(OptionalConstexprTest, Deletion) {
    EXPECT_TRUE(std::is_copy_constructible<beman::optional26::optional<int>>::value);
    EXPECT_TRUE(std::is_copy_assignable<beman::optional26::optional<int>>::value);
    EXPECT_TRUE(std::is_move_constructible<beman::optional26::optional<int>>::value);
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
        EXPECT_TRUE(std::is_copy_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_copy_assignable<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_move_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_move_assignable<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_destructible<beman::optional26::optional<T>>::value);
    }

    {
        struct T {
            T(const T&)            = delete;
            T(T&&)                 = delete;
            T& operator=(const T&) = delete;
            T& operator=(T&&)      = delete;
        };
        EXPECT_TRUE(!std::is_copy_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(!std::is_copy_assignable<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(!std::is_move_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(!std::is_move_assignable<beman::optional26::optional<T>>::value);
    }

    {
        struct T {
            T(const T&)            = delete;
            T(T&&)                 = default;
            T& operator=(const T&) = delete;
            T& operator=(T&&)      = default;
        };
        EXPECT_TRUE(!std::is_copy_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(!std::is_copy_assignable<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_move_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_move_assignable<beman::optional26::optional<T>>::value);
    }

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = delete;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = delete;
        };
        EXPECT_TRUE(std::is_copy_constructible<beman::optional26::optional<T>>::value);
        EXPECT_TRUE(std::is_copy_assignable<beman::optional26::optional<T>>::value);
    }
}

struct takes_init_and_variadic {
    int     v0;
    std::tuple<int, int> t;
    template <class... Args>
    constexpr takes_init_and_variadic(std::initializer_list<int>& l, Args&&... args) : v0{*std::begin(l)}, t(std::forward<Args>(args)...) {}
};

TEST(OptionalConstexprTest, InPlace) {
    constexpr beman::optional26::optional<int> o1{beman::optional26::in_place};
    constexpr beman::optional26::optional<int> o2(beman::optional26::in_place);
    EXPECT_TRUE(o1);
    EXPECT_TRUE(o1 == 0);
    EXPECT_TRUE(o2);
    EXPECT_TRUE(o2 == 0);

    constexpr beman::optional26::optional<int> o3(beman::optional26::in_place, 42);
    EXPECT_TRUE(o3 == 42);

    constexpr beman::optional26::optional<std::tuple<int, int>> o4(beman::optional26::in_place, 0, 1);
    EXPECT_TRUE(o4);
    EXPECT_TRUE(std::get<0>(*o4) == 0);
    EXPECT_TRUE(std::get<1>(*o4) == 1);

    constexpr beman::optional26::optional<takes_init_and_variadic> o6(beman::optional26::in_place, {0, 1}, 2, 3);
    EXPECT_TRUE(o6->v0 == 0);
    EXPECT_TRUE(std::get<0>(o6->t) == 2);
    EXPECT_TRUE(std::get<1>(o6->t) == 3);
}

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

    constexpr static auto i  = 42;
    constexpr auto o6 = beman::optional26::make_optional<const int&>(i);
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

    EXPECT_TRUE(!o1);
    EXPECT_TRUE(!o2);
    EXPECT_TRUE(!o3);
    EXPECT_TRUE(!o4);

    EXPECT_TRUE(!std::is_default_constructible<beman::optional26::nullopt_t>::value);
}

struct move_detector {
    move_detector() = default;
    move_detector(move_detector&& rhs) { rhs.been_moved = true; }
    bool been_moved = false;
};

TEST(OptionalConstexprTest, Observers) {
    constexpr beman::optional26::optional<int>       o1 = 42;
    constexpr beman::optional26::optional<int>       o2;
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

namespace {
class Point
{
    int x_;
    int y_;
  public:
    constexpr Point() : x_(0), y_(0) {}
    constexpr Point(int x, int y) : x_(x), y_(y) {}
    auto operator<=>(const Point&) const = default;
    bool operator==(const Point&) const = default;
};

}
TEST(OptionalConstexprTest, RelationalOps) {
    constexpr beman::optional26::optional<int> o1{4};
    constexpr beman::optional26::optional<int> o2{42};
    constexpr beman::optional26::optional<int> o3{};

    //  SECTION("self simple")
    {
        {
        constexpr auto b = !(o1 == o2);
        EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o1 == o1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = !(o1 == o2);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = !(o1 != o1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o1 < o2);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = !(o1 < o1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = !(o1 > o2);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = !(o1 > o1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o1 <= o2);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o1 <= o1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = !(o1 >= o2);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = o1 >= o1;
            EXPECT_TRUE(b);
        }
    }
    //  SECTION("nullopt simple")
    {
        {
            constexpr auto b =(!(o1 == beman::optional26::nullopt));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt == o1));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o1 != beman::optional26::nullopt);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt != o1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o1 < beman::optional26::nullopt));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt < o1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o1 > beman::optional26::nullopt);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt > o1));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o1 <= beman::optional26::nullopt));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt <= o1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o1 >= beman::optional26::nullopt);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt >= o1));
            EXPECT_TRUE(b);
        }


        {
            constexpr auto b = (o3 == beman::optional26::nullopt);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt == o3);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o3 != beman::optional26::nullopt));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt != o3));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o3 < beman::optional26::nullopt));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt < o3));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o3 > beman::optional26::nullopt));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt > o3));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o3 <= beman::optional26::nullopt);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt <= o3);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o3 >= beman::optional26::nullopt);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt >= o3);
            EXPECT_TRUE(b);
        }

    }
    //  SECTION("with T simple")
    {
        {
            constexpr auto b = (!(o1 == 1));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(1 == o1));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o1 != 1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (1 != o1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o1 < 1));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (1 < o1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o1 > 1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(1 > o1));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o1 <= 1));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (1 <= o1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o1 >= 1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(1 >= o1));
            EXPECT_TRUE(b);
        }


        {
            constexpr auto b = (o1 == 4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (4 == o1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o1 != 4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(4 != o1));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o1 < 4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(4 < o1));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o1 > 4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(4 > o1));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o1 <= 4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (4 <= o1);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o1 >= 4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (4 >= o1);
            EXPECT_TRUE(b);
        }
    }

    constexpr Point p4{2, 3};
    constexpr Point p5{3, 4};

    constexpr beman::optional26::optional<Point> o4{p4};
    constexpr beman::optional26::optional<Point> o5{p5};

    //  SECTION("self complex")
    {
        {
            constexpr auto b = (!(o4 == o5));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 == o4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 != o5);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o4 != o4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 < o5);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o4 < o4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o4 > o5));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o4 > o4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 <= o5);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 <= o4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o4 >= o5));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 >= o4);
            EXPECT_TRUE(b);
        }

    }
    //  SECTION("nullopt complex")
    {
        {
            constexpr auto b = (!(o4 == beman::optional26::nullopt));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt == o4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 != beman::optional26::nullopt);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt != o4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o4 < beman::optional26::nullopt));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt < o4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 > beman::optional26::nullopt);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt > o4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o4 <= beman::optional26::nullopt));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt <= o4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 >= beman::optional26::nullopt);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt >= o4));
            EXPECT_TRUE(b);
        }


        {
            constexpr auto b = (o3 == beman::optional26::nullopt);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt == o3);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o3 != beman::optional26::nullopt));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt != o3));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o3 < beman::optional26::nullopt));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt < o3));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o3 > beman::optional26::nullopt));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(beman::optional26::nullopt > o3));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o3 <= beman::optional26::nullopt);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt <= o3);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o3 >= beman::optional26::nullopt);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (beman::optional26::nullopt >= o3);
            EXPECT_TRUE(b);
        }

    }

    //  SECTION("with T complex")
    {
        {
            constexpr auto b = (!(o4 == Point{}));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(Point{} == o4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 != Point{});
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (Point{} != o4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o4 < Point{}));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (Point{} < o4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 > Point{});
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(Point{} > o4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o4 <= Point{}));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (Point{} <= o4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 >= Point{});
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(Point{} >= o4));
            EXPECT_TRUE(b);
        }


        {
            constexpr auto b = (o4 == p4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (p4 == o4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o4 != p4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(p4 != o4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o4 < p4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(p4 < o4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(o4 > p4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (!(p4 > o4));
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 <= p4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (p4 <= o4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (o4 >= p4);
            EXPECT_TRUE(b);
        }

        {
            constexpr auto b = (p4 >= o4);
            EXPECT_TRUE(b);
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
        (void)k;
        EXPECT_TRUE(false);
    }
    for (auto k : o2) {
        EXPECT_EQ(k, 42);
    }
}
