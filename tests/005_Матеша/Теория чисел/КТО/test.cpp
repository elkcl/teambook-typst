#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/005_Матеша/Теория чисел/КТО/code.hpp"

ll brute_crt(ll a, ll m, ll b, ll n) {
    ll g = std::gcd(m, n);
    if ((a - b) % g) {
        return -1;
    }
    ll mod = m / g * n;
    for (ll x = 0; x < mod; ++x) {
        if (x % m == a % m && x % n == b % n) {
            return x;
        }
    }
    return -1;
}

}  // namespace

TEST(a_005_Matesha_Teoriya_chisel_KTO, EuclidBezout) {
    for (ll a = 0; a <= 40; ++a) {
        for (ll b = 0; b <= 40; ++b) {
            ll x = 7;
            ll y = 9;
            ll g = euclid(a, b, x, y);
            EXPECT_EQ(g, std::gcd(a, b));
            EXPECT_EQ(a * x + b * y, g) << a << " " << b;
        }
    }
    ll x, y;
    EXPECT_EQ(euclid(0, 0, x, y), 0);
    EXPECT_EQ(euclid(15, 0, x, y), 15);
    EXPECT_EQ(15 * x + 0 * y, 15);
}

TEST(a_005_Matesha_Teoriya_chisel_KTO, HandSystems) {
    EXPECT_EQ(crt(1, 2, 0, 3), brute_crt(1, 2, 0, 3));
    EXPECT_EQ(crt(2, 3, 3, 5), 8);
    EXPECT_EQ(crt(1, 4, 3, 6), brute_crt(1, 4, 3, 6));
    EXPECT_EQ(crt(1, 2, 2, 4), -1);
    EXPECT_EQ(crt(0, 1, 5, 7), 5);
    EXPECT_EQ(crt(4, 6, 4, 6), 4);
}

TEST(a_005_Matesha_Teoriya_chisel_KTO, SmallModuliMatchBrute) {
    for (ll m = 1; m <= 18; ++m) {
        for (ll n = 1; n <= 18; ++n) {
            for (ll a = 0; a < m; ++a) {
                for (ll b = 0; b < n; ++b) {
                    ll got = crt(a, m, b, n);
                    ll expected = brute_crt(a, m, b, n);
                    if (expected == -1) {
                        EXPECT_EQ(got, -1) << a << " " << m << " " << b << " " << n;
                    } else {
                        ASSERT_NE(got, -1);
                        EXPECT_EQ(got % m, a);
                        EXPECT_EQ(got % n, b);
                        EXPECT_GE(got, 0);
                        EXPECT_LT(got, m / std::gcd(m, n) * n);
                    }
                }
            }
        }
    }
}
