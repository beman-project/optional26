#include <smd/optional/optional.h>

#include <smd/optional/optional.h>

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
    smd::optional::optional<int&> i1;
    smd::optional::optional<int&> i2{smd::optional::nullopt};
    (void)i1;
    (void)i2;

    int                           i  = 0;
    smd::optional::optional<int&> i3 = i;
    (void)i3;

    smd::optional::optional<empty&> e1;
    smd::optional::optional<empty&> e2{smd::optional::nullopt};
    (void)e1;
    (void)e2;

    empty                           e{};
    smd::optional::optional<empty&> e3 = e;
    (void)e3;

    smd::optional::optional<no_default&> nd1;
    smd::optional::optional<no_default&> nd2{smd::optional::nullopt};
    (void)nd1;
    (void)nd2;

    no_default nd{e};

    smd::optional::optional<no_default&> nd3 = nd;
    (void)nd3;

    smd::optional::optional<int&> ie;
    smd::optional::optional<int&> i4 = ie;
    EXPECT_FALSE(i4);

    base                           b{1};
    derived                        d(1, 2);
    smd::optional::optional<base&> b1 = b;
    smd::optional::optional<base&> b2 = d;

    smd::optional::optional<derived&> d2 = d;
    smd::optional::optional<base&>    b3{d2};

    smd::optional::optional<derived&> empty;
    smd::optional::optional<base&> fromEmpty(empty);
}

TEST(OptionalRefTest, Assignment) {
    smd::optional::optional<int&> i1;
    EXPECT_FALSE(i1);
    int i = 5;
    i1    = i;
    i     = 7;
    EXPECT_TRUE(i1);
    EXPECT_TRUE(*i1 = 7);

    double d;
    // i1 = d;  // ill-formed by mandate
    smd::optional::optional<double&> d1 = d;
    // i1 = d1; // ill-formed by mandate
    smd::optional::optional<int&> i2 = i1;
    EXPECT_TRUE(i2);
    EXPECT_TRUE(*i2 = 7);

    smd::optional::optional<int&> empty;
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
    smd::optional::optional<int&> o1{i1};
    smd::optional::optional<int&> o2{i2};
    smd::optional::optional<int&> o3{};

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
        EXPECT_TRUE(!(o1 == smd::optional::nullopt));
        EXPECT_TRUE(!(smd::optional::nullopt == o1));
        EXPECT_TRUE(o1 != smd::optional::nullopt);
        EXPECT_TRUE(smd::optional::nullopt != o1);
        EXPECT_TRUE(!(o1 < smd::optional::nullopt));
        EXPECT_TRUE(smd::optional::nullopt < o1);
        EXPECT_TRUE(o1 > smd::optional::nullopt);
        EXPECT_TRUE(!(smd::optional::nullopt > o1));
        EXPECT_TRUE(!(o1 <= smd::optional::nullopt));
        EXPECT_TRUE(smd::optional::nullopt <= o1);
        EXPECT_TRUE(o1 >= smd::optional::nullopt);
        EXPECT_TRUE(!(smd::optional::nullopt >= o1));

        EXPECT_TRUE(o3 == smd::optional::nullopt);
        EXPECT_TRUE(smd::optional::nullopt == o3);
        EXPECT_TRUE(!(o3 != smd::optional::nullopt));
        EXPECT_TRUE(!(smd::optional::nullopt != o3));
        EXPECT_TRUE(!(o3 < smd::optional::nullopt));
        EXPECT_TRUE(!(smd::optional::nullopt < o3));
        EXPECT_TRUE(!(o3 > smd::optional::nullopt));
        EXPECT_TRUE(!(smd::optional::nullopt > o3));
        EXPECT_TRUE(o3 <= smd::optional::nullopt);
        EXPECT_TRUE(smd::optional::nullopt <= o3);
        EXPECT_TRUE(o3 >= smd::optional::nullopt);
        EXPECT_TRUE(smd::optional::nullopt >= o3);
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
    smd::optional::optional<std::string&> o4{s4};
    smd::optional::optional<std::string&> o5{s5};

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
        EXPECT_TRUE(!(o4 == smd::optional::nullopt));
        EXPECT_TRUE(!(smd::optional::nullopt == o4));
        EXPECT_TRUE(o4 != smd::optional::nullopt);
        EXPECT_TRUE(smd::optional::nullopt != o4);
        EXPECT_TRUE(!(o4 < smd::optional::nullopt));
        EXPECT_TRUE(smd::optional::nullopt < o4);
        EXPECT_TRUE(o4 > smd::optional::nullopt);
        EXPECT_TRUE(!(smd::optional::nullopt > o4));
        EXPECT_TRUE(!(o4 <= smd::optional::nullopt));
        EXPECT_TRUE(smd::optional::nullopt <= o4);
        EXPECT_TRUE(o4 >= smd::optional::nullopt);
        EXPECT_TRUE(!(smd::optional::nullopt >= o4));

        EXPECT_TRUE(o3 == smd::optional::nullopt);
        EXPECT_TRUE(smd::optional::nullopt == o3);
        EXPECT_TRUE(!(o3 != smd::optional::nullopt));
        EXPECT_TRUE(!(smd::optional::nullopt != o3));
        EXPECT_TRUE(!(o3 < smd::optional::nullopt));
        EXPECT_TRUE(!(smd::optional::nullopt < o3));
        EXPECT_TRUE(!(o3 > smd::optional::nullopt));
        EXPECT_TRUE(!(smd::optional::nullopt > o3));
        EXPECT_TRUE(o3 <= smd::optional::nullopt);
        EXPECT_TRUE(smd::optional::nullopt <= o3);
        EXPECT_TRUE(o3 >= smd::optional::nullopt);
        EXPECT_TRUE(smd::optional::nullopt >= o3);
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
    EXPECT_TRUE(std::is_trivially_copy_constructible<smd::optional::optional<int&>>::value);
    EXPECT_TRUE(std::is_trivially_copy_assignable<smd::optional::optional<int&>>::value);
    EXPECT_TRUE(std::is_trivially_move_constructible<smd::optional::optional<int&>>::value);
    EXPECT_TRUE(std::is_trivially_move_assignable<smd::optional::optional<int&>>::value);
    EXPECT_TRUE(std::is_trivially_destructible<smd::optional::optional<int&>>::value);

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = default;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = default;
            ~T()                   = default;
        };
        EXPECT_TRUE(std::is_trivially_copy_constructible<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_copy_assignable<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_move_constructible<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_move_assignable<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_destructible<smd::optional::optional<T&>>::value);
    }

    {
        struct T {
            T(const T&) {}
            T(T&&){};
            T& operator=(const T&) { return *this; }
            T& operator=(T&&) { return *this; };
            ~T() {}
        };
        EXPECT_TRUE(std::is_trivially_copy_constructible<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_copy_assignable<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_move_constructible<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_move_assignable<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_trivially_destructible<smd::optional::optional<T&>>::value);
    }
}

TEST(OptionalRefTest, Deletion) {
    EXPECT_TRUE(std::is_copy_constructible<smd::optional::optional<int&>>::value);
    EXPECT_TRUE(std::is_copy_assignable<smd::optional::optional<int&>>::value);
    EXPECT_TRUE(std::is_move_constructible<smd::optional::optional<int&>>::value);
    EXPECT_TRUE(std::is_move_assignable<smd::optional::optional<int&>>::value);
    EXPECT_TRUE(std::is_destructible<smd::optional::optional<int&>>::value);

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = default;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = default;
            ~T()                   = default;
        };
        EXPECT_TRUE(std::is_copy_constructible<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_copy_assignable<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_move_constructible<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_move_assignable<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_destructible<smd::optional::optional<T&>>::value);
    }

    {
        struct T {
            T(const T&)            = delete;
            T(T&&)                 = delete;
            T& operator=(const T&) = delete;
            T& operator=(T&&)      = delete;
        };
        EXPECT_TRUE(std::is_copy_constructible<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_copy_assignable<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_move_constructible<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_move_assignable<smd::optional::optional<T&>>::value);
    }

    {
        struct T {
            T(const T&)            = delete;
            T(T&&)                 = default;
            T& operator=(const T&) = delete;
            T& operator=(T&&)      = default;
        };
        EXPECT_TRUE(std::is_copy_constructible<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_copy_assignable<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_move_constructible<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_move_assignable<smd::optional::optional<T&>>::value);
    }

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = delete;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = delete;
        };
        EXPECT_TRUE(std::is_copy_constructible<smd::optional::optional<T&>>::value);
        EXPECT_TRUE(std::is_copy_assignable<smd::optional::optional<T&>>::value);
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
    auto o1 = smd::optional::make_optional<int&>(var);
    auto o2 = smd::optional::optional<int&>(var);

    constexpr bool is_same = std::is_same<decltype(o1), smd::optional::optional<int>>::value;
    EXPECT_TRUE(is_same);
    EXPECT_TRUE(o1 == o2);

    std::tuple<int, int, int, int> tvar{0, 1, 2, 3};
    auto o3 = smd::optional::make_optional<std::tuple<int, int, int, int>&>(tvar);
    EXPECT_TRUE(std::get<0>(*o3) == 0);
    EXPECT_TRUE(std::get<1>(*o3) == 1);
    EXPECT_TRUE(std::get<2>(*o3) == 2);
    EXPECT_TRUE(std::get<3>(*o3) == 3);

    std::vector<int> ivec{0, 1, 2, 3};
    auto o4 = smd::optional::make_optional<std::vector<int>&>(ivec);
    EXPECT_TRUE(o4.value()[0] == 0);
    EXPECT_TRUE(o4.value()[1] == 1);
    EXPECT_TRUE(o4.value()[2] == 2);
    EXPECT_TRUE(o4.value()[3] == 3);

    takes_init_and_variadic tiv{{0, 1}, 2, 3};
    auto o5 = smd::optional::make_optional<takes_init_and_variadic&>(tiv);
    EXPECT_TRUE(o5->v[0] == 0);
    EXPECT_TRUE(o5->v[1] == 1);
    EXPECT_TRUE(std::get<0>(o5->t) == 2);
    EXPECT_TRUE(std::get<1>(o5->t) == 3);

    auto i  = 42;
    auto o6 = smd::optional::make_optional<int&>(i);
    static_assert(std::is_same_v<decltype(o6), smd::optional::optional<int>>);

    EXPECT_TRUE((std::is_same_v<decltype(o6), smd::optional::optional<int>>));
    EXPECT_TRUE(o6);
    EXPECT_TRUE(*o6 == 42);
}

TEST(OptionalRefTest, Nullopt) {
    smd::optional::optional<int&> o1 = smd::optional::nullopt;
    smd::optional::optional<int&> o2{smd::optional::nullopt};
    smd::optional::optional<int&> o3(smd::optional::nullopt);
    smd::optional::optional<int&> o4 = {smd::optional::nullopt};

    EXPECT_TRUE(!o1);
    EXPECT_TRUE(!o2);
    EXPECT_TRUE(!o3);
    EXPECT_TRUE(!o4);

    EXPECT_TRUE(!std::is_default_constructible<smd::optional::nullopt_t>::value);
}

struct move_detector {
    move_detector() = default;
    move_detector(move_detector&& rhs) { rhs.been_moved = true; }
    bool been_moved = false;
};

TEST(OptionalRefTest, Observers) {
    int                                 var = 42;
    smd::optional::optional<int&>       o1  = var;
    smd::optional::optional<int&>       o2;
    const smd::optional::optional<int&> o3 = var;

    EXPECT_TRUE(*o1 == 42);
    EXPECT_TRUE(*o1 == o1.value());
    EXPECT_TRUE(o2.value_or(42) == 42);
    EXPECT_TRUE(o3.value() == 42);
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
    smd::optional::optional<int_box&>       ob1  = i1;
    smd::optional::optional<int_box&>       ob2;
    const smd::optional::optional<int_box&> ob3 = i1;
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
    int& y =  std::move(smd::optional::optional<int&>(x)).value();
    EXPECT_EQ(&y, &x);

    int& z =  *std::move(smd::optional::optional<int&>(x));
    EXPECT_EQ(&z, &x);

}

TEST(OptionalRefTest, SwapValue) {
    int                           var    = 42;
    int                           twelve = 12;
    smd::optional::optional<int&> o1     = var;
    smd::optional::optional<int&> o2     = twelve;
    o1.swap(o2);
    EXPECT_EQ(o1.value(), 12);
    EXPECT_EQ(o2.value(), 42);
}

TEST(OptionalRefTest, SwapWNull) {
    int var = 42;

    smd::optional::optional<int&> o1 = var;
    smd::optional::optional<int&> o2 = smd::optional::nullopt;
    o1.swap(o2);
    EXPECT_TRUE(!o1.has_value());
    EXPECT_EQ(o2.value(), 42);
}

TEST(OptionalRefTest, SwapNullIntializedWithValue) {
    int                           var = 42;
    smd::optional::optional<int&> o1  = smd::optional::nullopt;
    smd::optional::optional<int&> o2  = var;
    o1.swap(o2);
    EXPECT_EQ(o1.value(), 42);
    EXPECT_TRUE(!o2.has_value());
}
