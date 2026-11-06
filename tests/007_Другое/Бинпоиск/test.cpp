#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/007_Другое/Бинпоиск/code.hpp"
}  // namespace

TEST(a_007_Drugoe_Binpoisk, EmptyAndSingle) {
    EXPECT_EQ(bin_search(0, 0, [](int) { return true; }), 0);
    EXPECT_EQ(bin_search(5, 5, [](int) { return false; }), 5);
    EXPECT_EQ(bin_search(3, 4, [](int x) { return x == 3; }), 4);
    EXPECT_EQ(bin_search(3, 4, [](int) { return false; }), 3);
}

TEST(a_007_Drugoe_Binpoisk, FirstFalseOnAPrefix) {
    EXPECT_EQ(bin_search(0, 10, [](int x) { return x < 5; }), 5);
    EXPECT_EQ(bin_search(0, 10, [](int) { return true; }), 10);
    EXPECT_EQ(bin_search(0, 10, [](int) { return false; }), 0);
    EXPECT_EQ(bin_search(-4, 6, [](int x) { return x < 0; }), 0);
    EXPECT_EQ(bin_search(2, 9, [](int x) { return x <= 2; }), 3);
}

TEST(a_007_Drugoe_Binpoisk, RandomThresholds) {
    std::mt19937 rng(707);
    for (int test = 0; test < 200; ++test) {
        int left = (int)(rng() % 50) - 20;
        int right = left + (int)(rng() % 40);
        int cut = left + (int)(rng() % (right - left + 1));
        int got = bin_search(left, right, [&](int x) { return x < cut; });
        EXPECT_EQ(got, std::min(std::max(cut, left), right));
    }
}
