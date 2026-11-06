#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/005_Матеша/Теория чисел/Ро-алгоритм Полларда/code.hpp"

ll mul_ref(ll a, ll b, ll m) {
    return (ll)((__int128)a * b % m);
}

}  // namespace

TEST(a_005_Matesha_Teoriya_chisel_Ro_algoritm_Pollarda, MulMatchesWideProduct) {
    EXPECT_EQ(mul(0, 0, 1), 0);
    EXPECT_EQ(mul(5, 7, 11), 2);
    EXPECT_EQ(mul(1, 8, 8), 0);
    EXPECT_EQ(mul(123456, 789, 1000003), mul_ref(123456, 789, 1000003));
    std::mt19937_64 rng(4242);
    for (int i = 0; i < 200; ++i) {
        ll m = (ll)(rng() % 1000000) + 1;
        ll a = (ll)(rng() % m);
        ll b = (ll)(rng() % m);
        EXPECT_EQ(mul(a, b, m), mul_ref(a, b, m)) << a << " " << b << " " << m;
    }
}

TEST(a_005_Matesha_Teoriya_chisel_Ro_algoritm_Pollarda, SmallTrialDivision) {
    EXPECT_EQ(pollard(0), -1);
    EXPECT_EQ(pollard(1), -1);
    EXPECT_EQ(pollard(2), -1);
    EXPECT_EQ(pollard(3), -1);
    EXPECT_EQ(pollard(4), 2);
    EXPECT_EQ(pollard(9), 3);
    EXPECT_EQ(pollard(49), 7);
    EXPECT_EQ(pollard(97), -1);
    EXPECT_EQ(pollard(100), 2);
    EXPECT_EQ(pollard(1000000), 2);
    EXPECT_EQ(pollard(999983), -1);
    for (ll value = 2; value <= 2000; ++value) {
        ll factor = pollard(value);
        if (factor == -1) {
            for (ll d = 2; d * d <= value; ++d) {
                EXPECT_NE(value % d, 0) << value;
            }
        } else {
            EXPECT_GT(factor, 1);
            EXPECT_LT(factor, value);
            EXPECT_EQ(value % factor, 0) << value;
        }
    }
}

TEST(a_005_Matesha_Teoriya_chisel_Ro_algoritm_Pollarda, RhoFindsAFactor) {
    ll value = 1000003LL * 1000033LL;
    ll factor = -1;
    for (int attempt = 0; attempt < 8 && factor == -1; ++attempt) {
        factor = pollard(value);
    }
    ASSERT_NE(factor, -1);
    EXPECT_GT(factor, 1);
    EXPECT_LT(factor, value);
    EXPECT_EQ(value % factor, 0);
}
