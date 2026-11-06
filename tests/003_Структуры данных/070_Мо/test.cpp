#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/003_Структуры данных/070_Мо/code.hpp"

int blocks_for(int n) {
    int c = 1;
    while (c * 1ll * c < n) {
        ++c;
    }
    return c;
}

int brute(const std::vector<int> &values, int l, int r) {
    std::vector<char> seen(MAXN, 0);
    int count = 0;
    for (int i = l; i <= r; ++i) {
        if (!seen[values[i]]) {
            seen[values[i]] = 1;
            ++count;
        }
    }
    return count;
}

void load(const std::vector<int> &values) {
    std::fill(a, a + values.size(), 0);
    for (int i = 0; i < (int)values.size(); ++i) {
        a[i] = values[i];
    }
}

void expect_queries(const std::vector<int> &values, const std::vector<Query> &queries) {
    int n = (int)values.size();
    load(values);
    mo(blocks_for(n), queries);
    for (const Query &q : queries) {
        EXPECT_EQ(ans[q.idx], brute(values, q.l, q.r)) << q.l << " " << q.r;
    }
}

}  // namespace

TEST(a_003_Struktury_dannyh_070_Mo, EmptyAndSingle) {
    mo(1, {});
    expect_queries({7}, {{0, 0, 0}});
    expect_queries({1, 1, 2}, {{0, 2, 0}, {1, 1, 1}, {0, 0, 2}});
}

TEST(a_003_Struktury_dannyh_070_Mo, AllRangesOnSmallArrays) {
    std::vector<std::vector<int>> arrays = {
        {1},
        {1, 1},
        {1, 2, 1},
        {0, 0, 0, 0},
        {3, 1, 4, 1, 5, 9, 2},
        {1, 2, 3, 4, 5, 1, 2, 3},
    };
    for (const auto &values : arrays) {
        std::vector<Query> queries;
        int n = (int)values.size();
        int id = 0;
        for (int l = 0; l < n; ++l) {
            for (int r = l; r < n; ++r) {
                queries.push_back({l, r, id++});
            }
        }
        expect_queries(values, queries);
    }
}

TEST(a_003_Struktury_dannyh_070_Mo, RandomArrays) {
    std::mt19937 rng(7070);
    for (int n : {1, 8, 30, 80}) {
        std::vector<int> values(n);
        for (int i = 0; i < n; ++i) {
            values[i] = (int)(rng() % 12);
        }
        std::vector<Query> queries;
        int queries_count = n * 3;
        for (int i = 0; i < queries_count; ++i) {
            int l = (int)(rng() % n);
            int r = (int)(rng() % n);
            if (l > r) {
                std::swap(l, r);
            }
            queries.push_back({l, r, i});
        }
        expect_queries(values, queries);
    }
}
