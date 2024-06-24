// src/Beman/Optional26/tests/optional_ref.t.cpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <Beman/Optional26/optional.hpp>

#include <gtest/gtest.h>

TEST(OptionalRefTest, TestGTest) { ASSERT_EQ(1, 1); }

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

TEST(OptionalRefTest, Constructors) {
    beman::optional26::optional<int&> i1;
    beman::optional26::optional<int&> i2{beman::optional26::nullopt};
    (void)i1;
    (void)i2;

    int                           i  = 0;
    beman::optional26::optional<int&> i3 = i;
    (void)i3;

    beman::optional26::optional<empty&> e1;
    beman::optional26::optional<empty&> e2{beman::optional26::nullopt};
    (void)e1;
    (void)e2;

    empty                           e{};
    beman::optional26::optional<empty&> e3 = e;
    (void)e3;

    beman::optional26::optional<no_default&> nd1;
    beman::optional26::optional<no_default&> nd2{beman::optional26::nullopt};
    (void)nd1;
    (void)nd2;

    no_default nd{e};

    beman::optional26::optional<no_default&> nd3 = nd;
    (void)nd3;

    beman::optional26::optional<int&> ie;
    beman::optional26::optional<int&> i4 = ie;
    EXPECT_FALSE(i4);

    base                           b{1};
    derived                        d(1, 2);
    beman::optional26::optional<base&> b1 = b;
    beman::optional26::optional<base&> b2 = d;

    beman::optional26::optional<derived&> d2 = d;
    beman::optional26::optional<base&>    b3 = d2;
    beman::optional26::optional<base&>    b4{d2};

    beman::optional26::optional<derived&> empty;
    beman::optional26::optional<base&> fromEmpty(empty);
    beman::optional26::optional<base&> fromEmpty2 = empty;
}

TEST(OptionalRefTest, Assignment) {
    beman::optional26::optional<int&> i1;
    EXPECT_FALSE(i1);
    int i = 5;
    i1    = i;
    i     = 7;
    EXPECT_TRUE(i1);
    EXPECT_TRUE(*i1 = 7);

    double d;
    // i1 = d;  // ill-formed by mandate
    beman::optional26::optional<double&> d1 = d;
    // i1 = d1; // ill-formed by mandate
    beman::optional26::optional<int&> i2 = i1;
    EXPECT_TRUE(i2);
    EXPECT_TRUE(*i2 = 7);

    beman::optional26::optional<int&> empty;
    EXPECT_FALSE(empty);
    i2 = empty;
    EXPECT_FALSE(i2);
    int eight = 8;
    empty.emplace(eight);
    EXPECT_TRUE(empty);
    EXPECT_EQ(empty, 8);
}

TEST(OptionalRefTest, RelationalOps) {
    int                           i1 = 4;
    int                           i2 = 42;
    beman::optional26::optional<int&> o1{i1};
    beman::optional26::optional<int&> o2{i2};
    beman::optional26::optional<int&> o3{};

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
    std::string                           s4 = "hello";
    std::string                           s5 = "xyz";
    beman::optional26::optional<std::string&> o4{s4};
    beman::optional26::optional<std::string&> o5{s5};

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

TEST(OptionalRefTest, Triviality) {
    EXPECT_TRUE(std::is_trivially_copy_constructible<beman::optional26::optional<int&>>::value);
    EXPECT_TRUE(std::is_trivially_copy_assignable<beman::optional26::optional<int&>>::value);
    EXPECT_TRUE(std::is_trivially_move_constructible<beman::optional26::optional<int&>>::value);
    EXPECT_TRUE(std::is_trivially_move_assignable<beman::optional26::optional<int&>>::value);
    EXPECT_TRUE(std::is_trivially_destructible<beman::optional26::optional<int&>>::value);

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = default;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = default;
            ~T()                   = default;
        };
        EXPECT_TRUE(std::is_trivially_copy_constructible<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_copy_assignable<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_move_constructible<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_move_assignable<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_destructible<beman::optional26::optional<T&>>::value);
    }

    {
        struct T {
            T(const T&) {}
            T(T&&){};
            T& operator=(const T&) { return *this; }
            T& operator=(T&&) { return *this; };
            ~T() {}
        };
        EXPECT_TRUE(std::is_trivially_copy_constructible<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_copy_assignable<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_move_constructible<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_move_assignable<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_destructible<beman::optional26::optional<T&>>::value);
    }
}

TEST(OptionalRefTest, Deletion) {
    EXPECT_TRUE(std::is_copy_constructible<beman::optional26::optional<int&>>::value);
    EXPECT_TRUE(std::is_copy_assignable<beman::optional26::optional<int&>>::value);
    EXPECT_TRUE(std::is_move_constructible<beman::optional26::optional<int&>>::value);
    EXPECT_TRUE(std::is_move_assignable<beman::optional26::optional<int&>>::value);
    EXPECT_TRUE(std::is_destructible<beman::optional26::optional<int&>>::value);

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = default;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = default;
            ~T()                   = default;
        };
        EXPECT_TRUE(std::is_copy_constructible<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_copy_assignable<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_move_constructible<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_move_assignable<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_destructible<beman::optional26::optional<T&>>::value);
    }

    {
        struct T {
            T(const T&)            = delete;
            T(T&&)                 = delete;
            T& operator=(const T&) = delete;
            T& operator=(T&&)      = delete;
        };
        EXPECT_TRUE(std::is_copy_constructible<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_copy_assignable<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_move_constructible<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_move_assignable<beman::optional26::optional<T&>>::value);
    }

    {
        struct T {
            T(const T&)            = delete;
            T(T&&)                 = default;
            T& operator=(const T&) = delete;
            T& operator=(T&&)      = default;
        };
        EXPECT_TRUE(std::is_copy_constructible<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_copy_assignable<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_move_constructible<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_move_assignable<beman::optional26::optional<T&>>::value);
    }

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = delete;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = delete;
        };
        EXPECT_TRUE(std::is_copy_constructible<beman::optional26::optional<T&>>::value);
        EXPECT_TRUE(std::is_copy_assignable<beman::optional26::optional<T&>>::value);
    }
}

struct takes_init_and_variadic {
    std::vector<int>     v;
    std::tuple<int, int> t;
    template <class... Args>
    takes_init_and_variadic(std::initializer_list<int> l, Args&&... args) : v(l), t(std::forward<Args>(args)...) {}
};

TEST(OptionalRefTest, MakeOptional) {
    int  var{42};
    auto o1 = beman::optional26::make_optional<int&>(var);
    auto o2 = beman::optional26::optional<int&>(var);

    constexpr bool is_same = std::is_same<decltype(o1), beman::optional26::optional<int>>::value;
    EXPECT_TRUE(is_same);
    EXPECT_TRUE(o1 == o2);

    std::tuple<int, int, int, int> tvar{0, 1, 2, 3};
    auto o3 = beman::optional26::make_optional<std::tuple<int, int, int, int>&>(tvar);
    EXPECT_TRUE(std::get<0>(*o3) == 0);
    EXPECT_TRUE(std::get<1>(*o3) == 1);
    EXPECT_TRUE(std::get<2>(*o3) == 2);
    EXPECT_TRUE(std::get<3>(*o3) == 3);

    std::vector<int> ivec{0, 1, 2, 3};
    auto o4 = beman::optional26::make_optional<std::vector<int>&>(ivec);
    EXPECT_TRUE(o4.value()[0] == 0);
    EXPECT_TRUE(o4.value()[1] == 1);
    EXPECT_TRUE(o4.value()[2] == 2);
    EXPECT_TRUE(o4.value()[3] == 3);

    takes_init_and_variadic tiv{{0, 1}, 2, 3};
    auto o5 = beman::optional26::make_optional<takes_init_and_variadic&>(tiv);
    EXPECT_TRUE(o5->v[0] == 0);
    EXPECT_TRUE(o5->v[1] == 1);
    EXPECT_TRUE(std::get<0>(o5->t) == 2);
    EXPECT_TRUE(std::get<1>(o5->t) == 3);

    auto i  = 42;
    auto o6 = beman::optional26::make_optional<int&>(i);
    static_assert(std::is_same_v<decltype(o6), beman::optional26::optional<int>>);

    EXPECT_TRUE((std::is_same_v<decltype(o6), beman::optional26::optional<int>>));
    EXPECT_TRUE(o6);
    EXPECT_TRUE(*o6 == 42);
}

TEST(OptionalRefTest, Nullopt) {
    beman::optional26::optional<int&> o1 = beman::optional26::nullopt;
    beman::optional26::optional<int&> o2{beman::optional26::nullopt};
    beman::optional26::optional<int&> o3(beman::optional26::nullopt);
    beman::optional26::optional<int&> o4 = {beman::optional26::nullopt};

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

TEST(OptionalRefTest, Observers) {
    int                                 var = 42;
    beman::optional26::optional<int&>       o1  = var;
    beman::optional26::optional<int&>       o2;
    const beman::optional26::optional<int&> o3 = var;
    const beman::optional26::optional<int&> o4;
    int var2 = 42;
    int var3 = 6*9;
    EXPECT_TRUE(*o1 == 42);
    EXPECT_TRUE(*o1 == o1.value());
    EXPECT_TRUE(o2.value_or(var2) == 42);
    EXPECT_TRUE(o3.value() == 42);
    EXPECT_TRUE(o3.value_or(var3) == 42);
    EXPECT_TRUE(o4.value_or(var3) == 54);
    int j = 99;
    EXPECT_TRUE(o4.value_or(j) == 99);
    // o4.value_or(j) = 88;
    // EXPECT_TRUE(j == 88);
    int var99 = 99;
    j = 88;
    EXPECT_TRUE([&](){beman::optional26::optional<int&> o(j);return o; }().value_or(var99) == 88);

    EXPECT_TRUE([&](){beman::optional26::optional<int&> o;return o; }().value_or(var99) == 99);


    EXPECT_TRUE(o3.value_or([&]()->int&{return var3;}()) == 42);
    EXPECT_TRUE(o4.value_or([&]()->int&{return var3;}()) == 54);

    std::string meow{"meow"};
    std::string bark{"bark"};
    beman::optional26::optional<std::string&> so1;
    beman::optional26::optional<std::string&> so2{meow};
    auto t1 = so1.value_or(bark);
    auto t2 = so2.value_or(bark);
    // auto t3 = so1.value_or("bark");
    // auto t4 = so2.value_or("bark");
    // std::tuple<const std::string&> t("meow");

    auto success = std::is_same<decltype(o1.value()), int&>::value;
    static_assert(std::is_same<decltype(o1.value()), int&>::value);
    EXPECT_TRUE(success);
    success = std::is_same<decltype(o2.value()), int&>::value;
    static_assert(std::is_same<decltype(o2.value()), int&>::value);
    EXPECT_TRUE(success);
    success = std::is_same<decltype(o3.value()), int&>::value;
    static_assert(std::is_same<decltype(o3.value()), int&>::value);
    EXPECT_TRUE(success);
    success = std::is_same<decltype(std::move(o1).value()), int&>::value;
    static_assert(std::is_same<decltype(std::move(o1).value()), int&>::value);
    EXPECT_TRUE(success);

    success = std::is_same<decltype(*o1), int&>::value;
    static_assert(std::is_same<decltype(*o1), int&>::value);
    EXPECT_TRUE(success);
    success = std::is_same<decltype(*o2), int&>::value;
    static_assert(std::is_same<decltype(*o2), int&>::value);
    EXPECT_TRUE(success);
    success = std::is_same<decltype(*o3), int&>::value;
    static_assert(std::is_same<decltype(*o3), int&>::value);
    EXPECT_TRUE(success);
    success = std::is_same<decltype(*std::move(o1)), int&>::value;
    static_assert(std::is_same<decltype(*std::move(o1)), int&>::value);
    EXPECT_TRUE(success);

    success = std::is_same<decltype(o1.operator->()), int*>::value;
    static_assert(std::is_same<decltype(o1.operator->()), int*>::value);
    EXPECT_TRUE(success);
    success = std::is_same<decltype(o2.operator->()), int*>::value;
    static_assert(std::is_same<decltype(o2.operator->()), int*>::value);
    EXPECT_TRUE(success);
    success = std::is_same<decltype(o3.operator->()), int*>::value;
    static_assert(std::is_same<decltype(o3.operator->()), int*>::value);
    EXPECT_TRUE(success);
    success = std::is_same<decltype(std::move(o1).operator->()), int*>::value;
    static_assert(std::is_same<decltype(std::move(o1).operator->()), int*>::value);
    EXPECT_TRUE(success);

    struct int_box {int i_;};
    int_box i1{3};
    beman::optional26::optional<int_box&>       ob1  = i1;
    beman::optional26::optional<int_box&>       ob2;
    const beman::optional26::optional<int_box&> ob3 = i1;
    success = std::is_same<decltype(ob1->i_), int>::value;
    static_assert(std::is_same<decltype(ob1->i_), int>::value);
    EXPECT_TRUE(success);
    success = std::is_same<decltype(ob2->i_), int>::value;
    static_assert(std::is_same<decltype(ob2->i_), int>::value);
    EXPECT_TRUE(success);
    success = std::is_same<decltype(ob3->i_), int>::value;
    static_assert(std::is_same<decltype(ob3->i_), int>::value);
    EXPECT_TRUE(success);
    success = std::is_same<decltype(std::move(ob1)->i_), int>::value;
    static_assert(std::is_same<decltype(std::move(ob1)->i_), int>::value);
    EXPECT_TRUE(success);

}

TEST(OptionalRefTest, MoveCheck) {
    int x = 0;
    int& y =  std::move(beman::optional26::optional<int&>(x)).value();
    EXPECT_EQ(&y, &x);

    int& z =  *std::move(beman::optional26::optional<int&>(x));
    EXPECT_EQ(&z, &x);

}

TEST(OptionalRefTest, SwapValue) {
    int                           var    = 42;
    int                           twelve = 12;
    beman::optional26::optional<int&> o1     = var;
    beman::optional26::optional<int&> o2     = twelve;
    o1.swap(o2);
    EXPECT_EQ(o1.value(), 12);
    EXPECT_EQ(o2.value(), 42);
}

TEST(OptionalRefTest, SwapWNull) {
    int var = 42;

    beman::optional26::optional<int&> o1 = var;
    beman::optional26::optional<int&> o2 = beman::optional26::nullopt;
    o1.swap(o2);
    EXPECT_TRUE(!o1.has_value());
    EXPECT_EQ(o2.value(), 42);
}

TEST(OptionalRefTest, SwapNullIntializedWithValue) {
    int                           var = 42;
    beman::optional26::optional<int&> o1  = beman::optional26::nullopt;
    beman::optional26::optional<int&> o2  = var;
    o1.swap(o2);
    EXPECT_EQ(o1.value(), 42);
    EXPECT_TRUE(!o2.has_value());
}
