#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/003_Структуры данных/015_Disjoint Sparse Table/code.hpp"

struct Add {
    int operator()(int a, int b) const { return a + b; }
};

struct MinOp {
    ll operator()(ll a, ll b) const { return std::min(a, b); }
};

constexpr ll kMinIdentity = 4'000'000'000'000'000'000LL;

int range_sum(const std::vector<int> &values, int left, int right) {
    int sum = 0;
    for (int i = left; i < right; ++i) {
        sum += values[i];
    }
    return sum;
}

ll range_min(const std::vector<ll> &values, int left, int right) {
    ll answer = values[left];
    for (int i = left + 1; i < right; ++i) {
        answer = std::min(answer, values[i]);
    }
    return answer;
}

void expect_sums(const std::vector<int> &values) {
    DisjointSparseTable<int, Add, 0> table(values);
    int n = (int)values.size();
    for (int left = 0; left < n; ++left) {
        for (int right = left + 1; right <= n; ++right) {
            EXPECT_EQ(table.query(left, right), range_sum(values, left, right))
                << "[" << left << ", " << right << ")";
        }
    }
}

void expect_mins(const std::vector<ll> &values) {
    DisjointSparseTable<ll, MinOp, kMinIdentity> table(values);
    int n = (int)values.size();
    for (int left = 0; left < n; ++left) {
        for (int right = left + 1; right <= n; ++right) {
            EXPECT_EQ(table.query(left, right), range_min(values, left, right))
                << "[" << left << ", " << right << ")";
        }
    }
}

}  // namespace

TEST(a_003_Struktury_dannyh_015_Disjoint_Sparse_Table, EmptyConstructsAndPair) {
    DisjointSparseTable<int, Add, 0> empty({});
    expect_sums({4, -5});
    EXPECT_EQ((DisjointSparseTable<int, Add, 0>({4, -5}).query(0, 1)), 4);
    EXPECT_EQ((DisjointSparseTable<int, Add, 0>({4, -5}).query(1, 2)), -5);
    EXPECT_EQ((DisjointSparseTable<int, Add, 0>({4, -5}).query(0, 2)), -1);
}

TEST(a_003_Struktury_dannyh_015_Disjoint_Sparse_Table, SumsAroundPowersOfTwo) {
    for (int n = 2; n <= 33; ++n) {
        std::vector<int> values(n);
        for (int i = 0; i < n; ++i) {
            values[i] = (i * 5) % 9 - 3;
        }
        expect_sums(values);
    }
}

TEST(a_003_Struktury_dannyh_015_Disjoint_Sparse_Table, MinimumIdentityDoesNotLeak) {
    expect_mins({kMinIdentity, -kMinIdentity / 4, 0, 7});
    expect_mins({5, 5, 5, 5, 4});
    expect_mins({-8, -1, -8, -3});
}

TEST(a_003_Struktury_dannyh_015_Disjoint_Sparse_Table, RandomSumsAndMins) {
    std::mt19937 rng(1515);
    for (int n : {2, 3, 6, 17, 40}) {
        std::vector<int> sums(n);
        std::vector<ll> mins(n);
        for (int i = 0; i < n; ++i) {
            sums[i] = (int)(rng() % 51) - 25;
            mins[i] = (ll)((int)(rng() % 1001) - 500);
        }
        expect_sums(sums);
        expect_mins(mins);
    }
}
