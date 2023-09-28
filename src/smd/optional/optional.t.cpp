#include <smd/optional/optional.h>

#include <smd/optional/optional.h>

#include <gtest/gtest.h>

TEST(OptionalTest, TestGTest) { ASSERT_EQ(1, 1); }

namespace {
struct empty {};
} // namespace

TEST(OptionalTest, Constructors) {
    smd::optional::optional<int> i1;
    smd::optional::optional<int> i2{smd::optional::nullopt};

    int                          i  = 0;
    smd::optional::optional<int> i3 = i;
    (void)i3;

    smd::optional::optional<empty> e1;
    smd::optional::optional<int>   e2{smd::optional::nullopt};

    empty                          e{};
    smd::optional::optional<empty> e3 = e;
    (void)e3;
}

namespace {
struct base {
    base()      = default;
    base(base&) = delete;
    base(base&&){};
};
} // namespace

TEST(OptionalTest, Constructors2) {
    smd::optional::optional<int> o1;
    EXPECT_TRUE(!o1);

    smd::optional::optional<int> o2 = smd::optional::nullopt;
    EXPECT_TRUE(!o2);

    smd::optional::optional<int> o3 = 42;
    EXPECT_TRUE(*o3 == 42);

    smd::optional::optional<int> o4 = o3;
    EXPECT_TRUE(*o4 == 42);

    smd::optional::optional<int> o5 = o1;
    EXPECT_TRUE(!o5);

    smd::optional::optional<int> o6 = std::move(o3);
    EXPECT_TRUE(*o6 == 42);

    smd::optional::optional<short> o7 = 42;
    EXPECT_TRUE(*o7 == 42);

    smd::optional::optional<int> o8 = o7;
    EXPECT_TRUE(*o8 == 42);

    smd::optional::optional<int> o9 = std::move(o7);
    EXPECT_TRUE(*o9 == 42);

    {
        smd::optional::optional<int&> o;
        EXPECT_TRUE(!o);

        smd::optional::optional<int&> oo = o;
        EXPECT_TRUE(!oo);
    }

    {
        auto                                    i = 42;
        smd::optional::optional<int&> o = i;
        EXPECT_TRUE(o);
        EXPECT_TRUE(*o == 42);

        smd::optional::optional<int&> oo = o;
        EXPECT_TRUE(oo);
        EXPECT_TRUE(*oo == 42);
    }

    std::vector<base> v;
    v.emplace_back();
    smd::optional::optional<std::vector<base>> ov = std::move(v);
    EXPECT_TRUE(ov->size() == 1);
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
    smd::optional::optional<NoDefault> v1{};
    smd::optional::optional<NoDefault> v2{i};
}

TEST(OptionalTest, AssignmentValue) {
    smd::optional::optional<int> o1 = 42;
    smd::optional::optional<int> o2 = 12;
    smd::optional::optional<int> o3;

    o1 = o1;
    EXPECT_TRUE(*o1 == 42);

    o1 = o2;
    EXPECT_TRUE(*o1 == 12);

    o1 = o3;
    EXPECT_TRUE(!o1);

    o1 = 42;
    EXPECT_TRUE(*o1 == 42);

    o1 = smd::optional::nullopt;
    EXPECT_TRUE(!o1);

    o1 = std::move(o2);
    EXPECT_TRUE(*o1 == 12);

    smd::optional::optional<short> o4 = 42;

    o1 = o4;
    EXPECT_TRUE(*o1 == 42);

    o1 = std::move(o4);
    EXPECT_TRUE(*o1 == 42);
}

TEST(OptionalTest, Triviality) {
    EXPECT_TRUE(std::is_trivially_copy_constructible<
                smd::optional::optional<int>>::value);
    EXPECT_TRUE(std::is_trivially_copy_assignable<
                smd::optional::optional<int>>::value);
    EXPECT_TRUE(std::is_trivially_move_constructible<
                smd::optional::optional<int>>::value);
    EXPECT_TRUE(std::is_trivially_move_assignable<
                smd::optional::optional<int>>::value);
    EXPECT_TRUE(
        std::is_trivially_destructible<smd::optional::optional<int>>::value);

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = default;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = default;
            ~T()                   = default;
        };
        EXPECT_TRUE(std::is_trivially_copy_constructible<
                    smd::optional::optional<T>>::value);
        EXPECT_TRUE(std::is_trivially_copy_assignable<
                    smd::optional::optional<T>>::value);
        EXPECT_TRUE(std::is_trivially_move_constructible<
                    smd::optional::optional<T>>::value);
        EXPECT_TRUE(std::is_trivially_move_assignable<
                    smd::optional::optional<T>>::value);
        EXPECT_TRUE(
            std::is_trivially_destructible<smd::optional::optional<T>>::value);
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
                    smd::optional::optional<T>>::value);
        EXPECT_TRUE(!std::is_trivially_copy_assignable<
                    smd::optional::optional<T>>::value);
        EXPECT_TRUE(!std::is_trivially_move_constructible<
                    smd::optional::optional<T>>::value);
        EXPECT_TRUE(!std::is_trivially_move_assignable<
                    smd::optional::optional<T>>::value);
        EXPECT_TRUE(!std::is_trivially_destructible<
                    smd::optional::optional<T>>::value);
    }
}

TEST(OptionalTest, Deletion) {
    EXPECT_TRUE(
        std::is_copy_constructible<smd::optional::optional<int>>::value);
    EXPECT_TRUE(std::is_copy_assignable<smd::optional::optional<int>>::value);
    EXPECT_TRUE(
        std::is_move_constructible<smd::optional::optional<int>>::value);
    EXPECT_TRUE(std::is_move_assignable<smd::optional::optional<int>>::value);
    EXPECT_TRUE(std::is_destructible<smd::optional::optional<int>>::value);

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = default;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = default;
            ~T()                   = default;
        };
        EXPECT_TRUE(
            std::is_copy_constructible<smd::optional::optional<T>>::value);
        EXPECT_TRUE(
            std::is_copy_assignable<smd::optional::optional<T>>::value);
        EXPECT_TRUE(
            std::is_move_constructible<smd::optional::optional<T>>::value);
        EXPECT_TRUE(
            std::is_move_assignable<smd::optional::optional<T>>::value);
        EXPECT_TRUE(std::is_destructible<smd::optional::optional<T>>::value);
    }

    {
        struct T {
            T(const T&)            = delete;
            T(T&&)                 = delete;
            T& operator=(const T&) = delete;
            T& operator=(T&&)      = delete;
        };
        EXPECT_TRUE(
            !std::is_copy_constructible<smd::optional::optional<T>>::value);
        EXPECT_TRUE(
            !std::is_copy_assignable<smd::optional::optional<T>>::value);
        EXPECT_TRUE(
            !std::is_move_constructible<smd::optional::optional<T>>::value);
        EXPECT_TRUE(
            !std::is_move_assignable<smd::optional::optional<T>>::value);
    }

    {
        struct T {
            T(const T&)            = delete;
            T(T&&)                 = default;
            T& operator=(const T&) = delete;
            T& operator=(T&&)      = default;
        };
        EXPECT_TRUE(
            !std::is_copy_constructible<smd::optional::optional<T>>::value);
        EXPECT_TRUE(
            !std::is_copy_assignable<smd::optional::optional<T>>::value);
        EXPECT_TRUE(
            std::is_move_constructible<smd::optional::optional<T>>::value);
        EXPECT_TRUE(
            std::is_move_assignable<smd::optional::optional<T>>::value);
    }

    {
        struct T {
            T(const T&)            = default;
            T(T&&)                 = delete;
            T& operator=(const T&) = default;
            T& operator=(T&&)      = delete;
        };
        EXPECT_TRUE(
            std::is_copy_constructible<smd::optional::optional<T>>::value);
        EXPECT_TRUE(
            std::is_copy_assignable<smd::optional::optional<T>>::value);
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
    smd::optional::optional<int> o1{smd::optional::in_place};
    smd::optional::optional<int> o2(smd::optional::in_place);
    EXPECT_TRUE(o1);
    EXPECT_TRUE(o1 == 0);
    EXPECT_TRUE(o2);
    EXPECT_TRUE(o2 == 0);

    smd::optional::optional<int> o3(smd::optional::in_place, 42);
    EXPECT_TRUE(o3 == 42);

    smd::optional::optional<std::tuple<int, int>> o4(
        smd::optional::in_place, 0, 1);
    EXPECT_TRUE(o4);
    EXPECT_TRUE(std::get<0>(*o4) == 0);
    EXPECT_TRUE(std::get<1>(*o4) == 1);

    smd::optional::optional<std::vector<int>> o5(smd::optional::in_place,
                                                 {0, 1});
    EXPECT_TRUE(o5);
    EXPECT_TRUE((*o5)[0] == 0);
    EXPECT_TRUE((*o5)[1] == 1);

    smd::optional::optional<takes_init_and_variadic> o6(
        smd::optional::in_place, {0, 1}, 2, 3);
    EXPECT_TRUE(o6->v[0] == 0);
    EXPECT_TRUE(o6->v[1] == 1);
    EXPECT_TRUE(std::get<0>(o6->t) == 2);
    EXPECT_TRUE(std::get<1>(o6->t) == 3);
}

TEST(OptionalTest, MakeOptional) {
    auto o1 = smd::optional::make_optional(42);
    auto o2 = smd::optional::optional<int>(42);

    constexpr bool is_same =
        std::is_same<decltype(o1), smd::optional::optional<int>>::value;
    EXPECT_TRUE(is_same);
    EXPECT_TRUE(o1 == o2);

    auto o3 = smd::optional::make_optional<std::tuple<int, int, int, int>>(
        0, 1, 2, 3);
    EXPECT_TRUE(std::get<0>(*o3) == 0);
    EXPECT_TRUE(std::get<1>(*o3) == 1);
    EXPECT_TRUE(std::get<2>(*o3) == 2);
    EXPECT_TRUE(std::get<3>(*o3) == 3);

    auto o4 = smd::optional::make_optional<std::vector<int>>({0, 1, 2, 3});
    EXPECT_TRUE(o4.value()[0] == 0);
    EXPECT_TRUE(o4.value()[1] == 1);
    EXPECT_TRUE(o4.value()[2] == 2);
    EXPECT_TRUE(o4.value()[3] == 3);

    auto o5 =
        smd::optional::make_optional<takes_init_and_variadic>({0, 1}, 2, 3);
    EXPECT_TRUE(o5->v[0] == 0);
    EXPECT_TRUE(o5->v[1] == 1);
    EXPECT_TRUE(std::get<0>(o5->t) == 2);
    EXPECT_TRUE(std::get<1>(o5->t) == 3);

    auto i  = 42;
    auto o6 = smd::optional::make_optional<int&>(i);
    static_assert(std::is_same<decltype(o6), smd::optional::optional<int&>>::value);

    EXPECT_TRUE(
        (std::is_same<decltype(o6), smd::optional::optional<int&>>::value));
    EXPECT_TRUE(o6);
    EXPECT_TRUE(*o6 == 42);
}

TEST(OptionalTest, Nullopt) {
    smd::optional::optional<int> o1 = smd::optional::nullopt;
    smd::optional::optional<int> o2{smd::optional::nullopt};
    smd::optional::optional<int> o3(smd::optional::nullopt);
    smd::optional::optional<int> o4 = {smd::optional::nullopt};

    EXPECT_TRUE(!o1);
    EXPECT_TRUE(!o2);
    EXPECT_TRUE(!o3);
    EXPECT_TRUE(!o4);

    EXPECT_TRUE(
        !std::is_default_constructible<smd::optional::nullopt_t>::value);
}

struct move_detector {
    move_detector() = default;
    move_detector(move_detector&& rhs) { rhs.been_moved = true; }
    bool been_moved = false;
};

TEST(OptionalTest, Observers) {
    smd::optional::optional<int>       o1 = 42;
    smd::optional::optional<int>       o2;
    const smd::optional::optional<int> o3 = 42;

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

    smd::optional::optional<move_detector> o4{smd::optional::in_place};
    move_detector                          o5 = std::move(o4).value();
    EXPECT_TRUE(o4->been_moved);
    EXPECT_TRUE(!o5.been_moved);
}

TEST(OptionalTest, RelationalOps) {
    smd::optional::optional<int> o1{4};
    smd::optional::optional<int> o2{42};
    smd::optional::optional<int> o3{};

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
    smd::optional::optional<std::string> o4{"hello"};
    smd::optional::optional<std::string> o5{"xyz"};

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

TEST(OptionalTest, SwapValue) {
    smd::optional::optional<int> o1 = 42;
    smd::optional::optional<int> o2 = 12;
    o1.swap(o2);
    EXPECT_EQ(o1.value(), 12);
    EXPECT_EQ(o2.value(), 42);
}

TEST(OptionalTest, SwapWNull) {
    smd::optional::optional<int> o1 = 42;
    smd::optional::optional<int> o2 = smd::optional::nullopt;
    o1.swap(o2);
    EXPECT_TRUE(!o1.has_value());
    EXPECT_EQ(o2.value(), 42);
}

TEST(OptionalTest, SwapNullIntializedWithValue) {
    smd::optional::optional<int> o1 = smd::optional::nullopt;
    smd::optional::optional<int> o2 = 42;
    o1.swap(o2);
    EXPECT_EQ(o1.value(), 42);
    EXPECT_TRUE(!o2.has_value());
}

TEST(OptionalTest, Emplace) {
    smd::optional::optional<
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
    smd::optional::optional<A> a;
    EXPECT_ANY_THROW(a.emplace());
}
