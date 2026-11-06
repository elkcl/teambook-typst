#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/005_Матеша/Бинарное возведение (без рекурсии)/code.hpp"

int brute(int a, int n) {
    int result = 1;
    for (int i = 0; i < n; ++i) {
        result *= a;
    }
    return result;
}

}  // namespace

TEST(a_005_Matesha_Binarnoe_vozvedenie_bez_rekursii, ZeroExponentAndBase) {
    EXPECT_EQ(binpow(2, 0), 1);
    EXPECT_EQ(binpow(0, 0), 1);
    EXPECT_EQ(binpow(-7, 0), 1);
    EXPECT_EQ(binpow(0, 1), 0);
    EXPECT_EQ(binpow(0, 5), 0);
    EXPECT_EQ(binpow(1, 20), 1);
    EXPECT_EQ(binpow(-1, 8), 1);
    EXPECT_EQ(binpow(-1, 9), -1);
}

TEST(a_005_Matesha_Binarnoe_vozvedenie_bez_rekursii, PowersThatFitInInt) {
    EXPECT_EQ(binpow(2, 10), 1024);
    EXPECT_EQ(binpow(2, 30), 1073741824);
    EXPECT_EQ(binpow(3, 10), 59049);
    EXPECT_EQ(binpow(-3, 5), -243);
    EXPECT_EQ(binpow(-3, 4), 81);
    EXPECT_EQ(binpow(10, 6), 1000000);
    for (int a = -8; a <= 8; ++a) {
        for (int n = 0; n <= 8; ++n) {
            EXPECT_EQ(binpow(a, n), brute(a, n)) << a << "^" << n;
        }
    }
}
