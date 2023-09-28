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
}
