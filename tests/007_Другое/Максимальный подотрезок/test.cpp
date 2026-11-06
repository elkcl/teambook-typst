#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
using ii = std::pair<int, int>;
int n;

#include "content/007_Другое/Максимальный подотрезок/code.hpp"

ll best_sum(const vi &values) {
    ll best = values[0];
    for (int right = 0; right < (int)values.size(); ++right) {
        ll sum = 0;
        for (int left = right; left >= 0; --left) {
            sum += values[left];
            best = std::max(best, sum);
        }
    }
    return best;
}

ll segment_sum(const vi &values, ii segment) {
    ll sum = 0;
    for (int i = segment.first; i <= segment.second; ++i) {
        sum += values[i];
    }
    return sum;
}

ii run(vi values) {
    n = (int)values.size();
    return max_seg(values);
}

}  // namespace

TEST(a_007_Drugoe_Maksimalnyy_podotrezok, SingleAndHandExamples) {
    EXPECT_EQ(run({5}), (ii{0, 0}));
    EXPECT_EQ(run({-3}), (ii{0, 0}));
    EXPECT_EQ(run({-2, -1}), (ii{1, 1}));
    EXPECT_EQ(run({1, -2, 3}), (ii{2, 2}));
    EXPECT_EQ(run({5, -1, 3}), (ii{0, 2}));
    EXPECT_EQ(run({1, 2, 3}), (ii{0, 2}));
    EXPECT_EQ(run({-5, 1, 2, -1, 3, -2}), (ii{1, 4}));
}

TEST(a_007_Drugoe_Maksimalnyy_podotrezok, TiesKeepTheEarliestSegment) {
    EXPECT_EQ(run({1, -1, 1}), (ii{0, 0}));
    EXPECT_EQ(run({2, -3, 2}), (ii{0, 0}));
    EXPECT_EQ(run({4, -1, 2, -1, 4, -10, 5}), (ii{0, 4}));
}

TEST(a_007_Drugoe_Maksimalnyy_podotrezok, RandomMatchesBrute) {
    std::mt19937 rng(7071);
    for (int test = 0; test < 300; ++test) {
        int length = (int)(rng() % 25) + 1;
        vi values(length);
        for (int i = 0; i < length; ++i) {
            values[i] = (int)(rng() % 21) - 10;
        }
        ii got = run(values);
        ASSERT_LE(0, got.first);
        ASSERT_LE(got.first, got.second);
        ASSERT_LT(got.second, (int)values.size());
        EXPECT_EQ(segment_sum(values, got), best_sum(values)) << testing::PrintToString(values);
    }
}
