#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/005_Матеша/Теория чисел/Дискретный логарифм/code.hpp"

ll ask(ll a, ll b) { return dlog(a, b); }

}  // namespace

TEST(a_005_Matesha_Teoriya_chisel_Diskretnyy_logarifm, KnownPowersAndNoSolution) {
    auto expect_log = [](ll a, ll b) {
        ll got = ask(a, b);
        ASSERT_GE(got, 0) << a << " " << b;
        EXPECT_EQ(binpow(a, got, MOD), b % MOD);
    };
    EXPECT_EQ(ask(2, 1), MOD - 1);
    EXPECT_EQ(binpow(2, MOD - 1, MOD), 1);
    expect_log(2, 1024);
    EXPECT_EQ(ask(3, binpow(3, 20, MOD)), 20);
    EXPECT_EQ(ask(5, binpow(5, BLOCK, MOD)), BLOCK);
    EXPECT_EQ(ask(2, 0), -1);
}

TEST(a_005_Matesha_Teoriya_chisel_Diskretnyy_logarifm, ZeroBaseDoesNotSolveOne) {
    ll got = ask(0, 1);
    EXPECT_EQ(got, 499);
    EXPECT_EQ(binpow(0, got, MOD), 0);
    EXPECT_EQ(ask(0, 0), 500);
    EXPECT_EQ(binpow(0, 500, MOD), 0);
}

TEST(a_005_Matesha_Teoriya_chisel_Diskretnyy_logarifm, RandomExponents) {
    std::mt19937 rng(505);
    ll base = 2;
    for (int i = 0; i < 8; ++i) {
        ll exponent = (ll)(rng() % 5000);
        ll value = binpow(base, exponent, MOD);
        ll got = dlog(base, value);
        ASSERT_GE(got, 0);
        EXPECT_EQ(binpow(base, got, MOD), value) << exponent;
    }
    ll other = 3;
    ll fresh = ask(other, binpow(other, 100, MOD));
    EXPECT_EQ(binpow(other, fresh, MOD), binpow(other, 100, MOD));
}
