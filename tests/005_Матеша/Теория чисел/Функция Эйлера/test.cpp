#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/005_Матеша/Теория чисел/Функция Эйлера/code.hpp"

int brute_phi(int n) {
    int count = 0;
    for (int i = 1; i <= n; ++i) {
        if (std::gcd(i, n) == 1) {
            ++count;
        }
    }
    return count;
}

}  // namespace

TEST(a_005_Matesha_Teoriya_chisel_Funktsiya_Eylera, SmallAndKnown) {
    EXPECT_EQ(phi(1), 1);
    EXPECT_EQ(phi(2), 1);
    EXPECT_EQ(phi(3), 2);
    EXPECT_EQ(phi(4), 2);
    EXPECT_EQ(phi(5), 4);
    EXPECT_EQ(phi(6), 2);
    EXPECT_EQ(phi(7), 6);
    EXPECT_EQ(phi(9), 6);
    EXPECT_EQ(phi(10), 4);
    EXPECT_EQ(phi(12), 4);
    EXPECT_EQ(phi(1), 1);
}

TEST(a_005_Matesha_Teoriya_chisel_Funktsiya_Eylera, PrimePowersAndProducts) {
    EXPECT_EQ(phi(8), 4);
    EXPECT_EQ(phi(16), 8);
    EXPECT_EQ(phi(27), 18);
    EXPECT_EQ(phi(25), 20);
    EXPECT_EQ(phi(49), 42);
    EXPECT_EQ(phi(97), 96);
    EXPECT_EQ(phi(100), 40);
    EXPECT_EQ(phi(210), phi(2) * phi(3) * phi(5) * phi(7));
}

TEST(a_005_Matesha_Teoriya_chisel_Funktsiya_Eylera, MatchesCoprimeCount) {
    for (int n = 1; n <= 2000; ++n) {
        EXPECT_EQ(phi(n), brute_phi(n)) << n;
    }
    EXPECT_EQ(phi(99991), 99990);
    EXPECT_EQ(phi(1000000), 400000);
}
