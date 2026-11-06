#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/003_Структуры данных/010_Дерево Фенвика/code.hpp"

std::vector<int> materialize(FenwickTree &tree) {
    std::vector<int> values(tree.n);
    int previous = 0;
    for (int i = 1; i <= tree.n; ++i) {
        int prefix = tree.get(i);
        values[i - 1] = prefix - previous;
        previous = prefix;
    }
    return values;
}

int brute_prefix(const std::vector<int> &values, int r) {
    int sum = 0;
    for (int i = 0; i < r && i < (int)values.size(); ++i) {
        sum += values[i];
    }
    return sum;
}

int brute_lower(const std::vector<int> &values, int sum) {
    int acc = 0;
    for (int i = 0; i < (int)values.size(); ++i) {
        acc += values[i];
        if (acc >= sum) {
            return i + 1;
        }
    }
    return (int)values.size() + 1;
}

}  // namespace

TEST(a_003_Struktury_dannyh_010_Derevo_Fenvika, EmptyAndSingle) {
    FenwickTree empty(0);
    EXPECT_EQ(empty.get(0), 0);
    EXPECT_EQ(empty.lower_bound(0), 1);
    EXPECT_EQ(empty.lower_bound(1), 1);

    FenwickTree one(1);
    EXPECT_EQ(one.get(1), 0);
    EXPECT_EQ(one.lower_bound(0), 1);
    EXPECT_EQ(one.lower_bound(1), 2);
    one.update(1, 7);
    EXPECT_EQ(one.get(1), 7);
    EXPECT_EQ(one.lower_bound(7), 1);
    EXPECT_EQ(one.lower_bound(8), 2);
    one.update(1, -7);
    EXPECT_EQ(one.get(1), 0);
}

TEST(a_003_Struktury_dannyh_010_Derevo_Fenvika, PrefixAndPointValues) {
    FenwickTree tree(6);
    std::vector<int> values(6, 0);
    std::vector<std::pair<int, int>> updates = {{1, 4}, {3, 5}, {6, 1}, {3, -2}, {2, 8}, {5, 3}};
    for (auto [index, delta] : updates) {
        tree.update(index, delta);
        values[index - 1] += delta;
    }
    EXPECT_EQ(materialize(tree), values);
    for (int r = 0; r <= 6; ++r) {
        EXPECT_EQ(tree.get(r), brute_prefix(values, r)) << r;
    }
}

TEST(a_003_Struktury_dannyh_010_Derevo_Fenvika, LowerBoundOnNonNegative) {
    FenwickTree tree(8);
    std::vector<int> values = {0, 2, 0, 3, 0, 0, 4, 1};
    for (int i = 0; i < 8; ++i) {
        tree.update(i + 1, values[i]);
    }
    for (int sum = 0; sum <= 12; ++sum) {
        EXPECT_EQ(tree.lower_bound(sum), brute_lower(values, sum)) << sum;
    }
    EXPECT_EQ(tree.lower_bound(0), 1);
    EXPECT_EQ(tree.lower_bound(2), 2);
    EXPECT_EQ(tree.lower_bound(5), 4);
    EXPECT_EQ(tree.lower_bound(9), 7);
    EXPECT_EQ(tree.lower_bound(10), 8);
    EXPECT_EQ(tree.lower_bound(11), 9);
}

TEST(a_003_Struktury_dannyh_010_Derevo_Fenvika, PowersOfTwoAndNegatives) {
    for (int n : {1, 2, 3, 4, 7, 8, 9, 16, 31, 32}) {
        FenwickTree tree(n);
        std::vector<int> values(n, 0);
        for (int i = 1; i <= n; ++i) {
            int delta = (i % 2 == 0 ? -3 : 5) * ((i % 5) + 1);
            tree.update(i, delta);
            values[i - 1] += delta;
        }
        EXPECT_EQ(materialize(tree), values);
        for (int r = 0; r <= n; ++r) {
            EXPECT_EQ(tree.get(r), brute_prefix(values, r));
        }
    }
}

TEST(a_003_Struktury_dannyh_010_Derevo_Fenvika, RandomPrefixesAndSearch) {
    std::mt19937 rng(1010);
    for (int n : {1, 5, 17, 64, 200}) {
        FenwickTree tree(n);
        std::vector<int> values(n, 0);
        for (int step = 0; step < n * 3; ++step) {
            int index = (int)(rng() % n) + 1;
            int delta = (int)(rng() % 21) - 5;
            tree.update(index, delta);
            values[index - 1] += delta;
            int r = (int)(rng() % (n + 1));
            EXPECT_EQ(tree.get(r), brute_prefix(values, r));
        }
        if (*std::min_element(values.begin(), values.end()) >= 0) {
            int total = std::accumulate(values.begin(), values.end(), 0);
            for (int sum = 0; sum <= total + 2; ++sum) {
                EXPECT_EQ(tree.lower_bound(sum), brute_lower(values, sum));
            }
        }
    }
}
