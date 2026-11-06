#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/007_Другое/IntPairHash/code.hpp"

std::uint64_t mixed(int first, int second) {
    return (static_cast<std::uint64_t>(first) << 32) | static_cast<std::uint64_t>(second);
}

}  // namespace

TEST(a_007_Drugoe_IntPairHash, EqualPairsMatchAndPositivesRoundTrip) {
    IntPairHash hash;
    EXPECT_EQ(hash({0, 0}), hash({0, 0}));
    EXPECT_EQ(hash({-5, 9}), hash({-5, 9}));
    EXPECT_EQ(hash({1, 2}), std::hash<std::uint64_t>{}(mixed(1, 2)));

    std::unordered_map<std::pair<int, int>, int, IntPairHash> values;
    for (int x = -20; x <= 20; ++x) {
        for (int y = 0; y <= 40; ++y) {
            values[{x, y}] = x * 100 + y;
        }
    }
    for (int x = -20; x <= 20; ++x) {
        for (int y = 0; y <= 40; ++y) {
            std::pair<int, int> key{x, y};
            EXPECT_EQ(values[key], x * 100 + y);
        }
    }
}

TEST(a_007_Drugoe_IntPairHash, NegativeSecondCollidesButBothStayStored) {
    IntPairHash hash;
    EXPECT_EQ(hash({0, -1}), hash({-1, -1}));
    EXPECT_EQ(mixed(0, -1), mixed(-1, -1));

    std::unordered_set<std::pair<int, int>, IntPairHash> pairs;
    pairs.insert({0, -1});
    pairs.insert({-1, -1});
    EXPECT_EQ(pairs.count({0, -1}), 1u);
    EXPECT_EQ(pairs.count({-1, -1}), 1u);
    EXPECT_EQ(pairs.size(), 2u);
}
