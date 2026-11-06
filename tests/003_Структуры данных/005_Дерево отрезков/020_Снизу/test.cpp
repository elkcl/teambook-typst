#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/003_Структуры данных/005_Дерево отрезков/020_Снизу/code.hpp"

using Arr = std::vector<long long>;

long long range_sum(const Arr &values, int left, int right) {
    long long answer = 0;
    for (int i = left; i < right; ++i) {
        answer += values[i];
    }
    return answer;
}

void expect_all(Segtree &tree, const Arr &values) {
    int n = (int)values.size();
    for (int left = 0; left <= n; ++left) {
        for (int right = left; right <= n; ++right) {
            EXPECT_EQ(tree.sum(left, right), range_sum(values, left, right))
                << "[" << left << ", " << right << ")";
        }
    }
}

}  // namespace

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_020_Snizu, EmptyAndSingle) {
    Segtree empty(vi{});
    EXPECT_EQ(empty.sum(0, 0), 0);

    Segtree one(vi{-7});
    EXPECT_EQ(one.sum(0, 1), -7);
    EXPECT_EQ(one.sum(0, 0), 0);
    EXPECT_EQ(one.sum(1, 1), 0);
    one.upd(0, 42);
    EXPECT_EQ(one.sum(0, 1), 42);
    one.upd(0, 0);
    EXPECT_EQ(one.sum(0, 1), 0);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_020_Snizu, BuildAndPointUpdate) {
    Segtree tree(vi{1, 2, 3, 4, 5});
    Arr values{1, 2, 3, 4, 5};
    expect_all(tree, values);
    tree.upd(2, 10);
    values[2] = 10;
    expect_all(tree, values);
    tree.upd(0, -4);
    tree.upd(4, -1);
    values[0] = -4;
    values[4] = -1;
    expect_all(tree, values);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_020_Snizu, AllLengthsUpToSixtyFour) {
    std::mt19937 rng(2020);
    for (int n = 1; n <= 64; ++n) {
        vi source(n);
        Arr values(n);
        for (int i = 0; i < n; ++i) {
            source[i] = (int)(rng() % 21) - 10;
            values[i] = source[i];
        }
        Segtree tree(source);
        expect_all(tree, values);
        for (int step = 0; step < 20; ++step) {
            int pos = (int)(rng() % n);
            int value = (int)(rng() % 41) - 20;
            tree.upd(pos, value);
            values[pos] = value;
        }
        expect_all(tree, values);
    }
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_020_Snizu, PowersOfTwoAndNeighbors) {
    for (int shift = 0; shift <= 12; ++shift) {
        for (int delta : {-1, 0, 1}) {
            int n = (1 << shift) + delta;
            if (n <= 0 || n > 5000) {
                continue;
            }
            vi source(n);
            Arr values(n);
            for (int i = 0; i < n; ++i) {
                source[i] = (i % 7) - 3;
                values[i] = source[i];
            }
            Segtree tree(source);
            EXPECT_EQ(tree.sum(0, n), range_sum(values, 0, n));
            for (int i = 0; i < n; ++i) {
                EXPECT_EQ(tree.sum(i, i + 1), values[i]);
            }
            tree.upd(0, 100);
            tree.upd(n - 1, -50);
            tree.upd(n / 2, 7);
            values[0] = 100;
            values[n - 1] = -50;
            values[n / 2] = 7;
            EXPECT_EQ(tree.sum(0, n), range_sum(values, 0, n));
            EXPECT_EQ(tree.sum(0, n / 2), range_sum(values, 0, n / 2));
            EXPECT_EQ(tree.sum(n / 2, n), range_sum(values, n / 2, n));
        }
    }
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_020_Snizu, NegativesZerosAndDuplicates) {
    Segtree tree(vi{0, 0, 0, -5, -5, 8, 8, 8});
    EXPECT_EQ(tree.sum(0, 3), 0);
    EXPECT_EQ(tree.sum(3, 5), -10);
    EXPECT_EQ(tree.sum(5, 8), 24);
    EXPECT_EQ(tree.sum(0, 8), 14);
    tree.upd(3, 0);
    tree.upd(4, 0);
    EXPECT_EQ(tree.sum(0, 8), 24);
    tree.upd(7, -24);
    EXPECT_EQ(tree.sum(0, 8), -8);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_020_Snizu, SumOfLargeIntsFits) {
    const int n = 50;
    vi source(n, 1000000000);
    Segtree tree(source);
    EXPECT_EQ(tree.sum(0, n), 50000000000LL);
    tree.upd(10, -1000000000);
    EXPECT_EQ(tree.sum(0, n), 48000000000LL);
    EXPECT_EQ(tree.sum(10, 11), -1000000000LL);
    EXPECT_EQ(tree.sum(9, 12), 1000000000LL);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_020_Snizu, ReversedQueryIsEmpty) {
    Segtree tree(vi{1, 2, 3, 4});
    EXPECT_EQ(tree.sum(3, 1), 0);
    EXPECT_EQ(tree.sum(2, 2), 0);
    EXPECT_EQ(tree.sum(4, 4), 0);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_020_Snizu, StressRandomPoints) {
    std::mt19937 rng(2021);
    for (int round = 0; round < 40; ++round) {
        int n = 1 + (int)(rng() % 70);
        vi source(n);
        Arr values(n);
        for (int i = 0; i < n; ++i) {
            source[i] = (int)(rng() % 101) - 50;
            values[i] = source[i];
        }
        Segtree tree(source);
        for (int step = 0; step < 40; ++step) {
            int pos = (int)(rng() % n);
            int value = (int)(rng() % 201) - 100;
            tree.upd(pos, value);
            values[pos] = value;
            int left = (int)(rng() % (n + 1));
            int right = (int)(rng() % (n + 1));
            if (left > right) {
                std::swap(left, right);
            }
            EXPECT_EQ(tree.sum(left, right), range_sum(values, left, right));
        }
        expect_all(tree, values);
    }
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_020_Snizu, LoadPointUpdates) {
    const int n = 200000;
    const int ops = 200000;
    std::mt19937 rng(2022);
    vi source(n);
    Arr values(n);
    for (int i = 0; i < n; ++i) {
        source[i] = (int)(rng() % 1000) - 500;
        values[i] = source[i];
    }
    Segtree tree(source);
    long long total = range_sum(values, 0, n);
    EXPECT_EQ(tree.sum(0, n), total);
    for (int step = 0; step < ops; ++step) {
        int pos = (int)(rng() % n);
        int value = (int)(rng() % 1000) - 500;
        total += value - values[pos];
        values[pos] = value;
        tree.upd(pos, value);
    }
    EXPECT_EQ(tree.sum(0, n), total);
    for (int i = 0; i < n; i += 1000) {
        EXPECT_EQ(tree.sum(i, i + 1), values[i]);
    }
    for (int step = 0; step < 300; ++step) {
        int left = (int)(rng() % n);
        int right = left + (int)(rng() % (n - left)) + 1;
        EXPECT_EQ(tree.sum(left, right), range_sum(values, left, right));
    }
}
