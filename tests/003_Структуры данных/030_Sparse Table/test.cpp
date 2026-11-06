#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/003_Структуры данных/030_Sparse Table/code.hpp"

int brute(const vi &values, int left, int right) {
    int answer = values[left];
    for (int i = left + 1; i < right; ++i) {
        answer = std::min(answer, values[i]);
    }
    return answer;
}

void expect_all(const vi &values) {
    sparse_table table(values);
    int n = (int)values.size();
    for (int left = 0; left < n; ++left) {
        for (int right = left + 1; right <= n; ++right) {
            EXPECT_EQ(table.get(left, right), brute(values, left, right))
                << "[" << left << ", " << right << ") n=" << n;
        }
    }
}

vi pattern(int n, int kind, std::mt19937 &rng) {
    vi values(n);
    for (int i = 0; i < n; ++i) {
        if (kind == 0) {
            values[i] = i;
        } else if (kind == 1) {
            values[i] = n - i;
        } else if (kind == 2) {
            values[i] = 7;
        } else if (kind == 3) {
            values[i] = (i % 2 == 0) ? inf + 5 : -(inf + 3);
        } else if (kind == 4) {
            values[i] = (i * 17) % 9 - 4;
        } else {
            values[i] = (int)rng() % 2001 - 1000;
        }
    }
    if (n > 0) {
        values[0] = INT_MIN;
        values[n - 1] = INT_MAX;
        if (n > 2) {
            values[n / 2] = inf;
        }
    }
    return values;
}

}  // namespace

TEST(a_003_Struktury_dannyh_030_Sparse_Table, EmptyAndSingle) {
    sparse_table empty(vi{});
    EXPECT_EQ(empty.n, 0);

    sparse_table one(vi{INT_MIN});
    EXPECT_EQ(one.get(0, 1), INT_MIN);
    sparse_table other(vi{inf + 1});
    EXPECT_EQ(other.get(0, 1), inf + 1);
}

TEST(a_003_Struktury_dannyh_030_Sparse_Table, HandRanges) {
    vi values{5, 1, 4, 1, 8, -3, 2};
    sparse_table table(values);
    EXPECT_EQ(table.get(0, 1), 5);
    EXPECT_EQ(table.get(0, 7), -3);
    EXPECT_EQ(table.get(1, 4), 1);
    EXPECT_EQ(table.get(4, 7), -3);
    EXPECT_EQ(table.get(6, 7), 2);
    EXPECT_EQ(table.get(2, 5), 1);
    expect_all(values);
}

TEST(a_003_Struktury_dannyh_030_Sparse_Table, AllSmallLengthsAndPatterns) {
    std::mt19937 rng(3010);
    for (int n = 1; n <= 48; ++n) {
        for (int kind = 0; kind < 6; ++kind) {
            expect_all(pattern(n, kind, rng));
        }
    }
}

TEST(a_003_Struktury_dannyh_030_Sparse_Table, PowersOfTwoLengths) {
    std::mt19937 rng(3011);
    for (int shift = 0; shift <= 10; ++shift) {
        for (int delta : {-1, 0, 1}) {
            int n = (1 << shift) + delta;
            if (n <= 0 || n > 80) {
                continue;
            }
            expect_all(pattern(n, 5, rng));
        }
    }
}

TEST(a_003_Struktury_dannyh_030_Sparse_Table, SentinelDoesNotReplaceRealValues) {
    vi values{inf, inf, inf + 1, INT_MAX, inf - 1};
    expect_all(values);
    vi negatives{INT_MIN, -1, INT_MIN, 0};
    expect_all(negatives);
}

TEST(a_003_Struktury_dannyh_030_Sparse_Table, MinimumAtEndsAndDuplicates) {
    for (int n = 4; n <= 64; ++n) {
        vi values(n, 5);
        values[0] = -10;
        values[n - 1] = -10;
        values[n / 2] = -3;
        sparse_table table(values);
        EXPECT_EQ(table.get(0, n), -10);
        EXPECT_EQ(table.get(1, n - 1), -3);
        EXPECT_EQ(table.get(1, n / 2), 5);
        EXPECT_EQ(table.get(n - 1, n), -10);
        for (int k = 0; (1 << k) <= n; ++k) {
            int len = 1 << k;
            EXPECT_EQ(table.get(0, len), brute(values, 0, len));
            EXPECT_EQ(table.get(n - len, n), brute(values, n - len, n));
        }
    }
}

TEST(a_003_Struktury_dannyh_030_Sparse_Table, RandomQueries) {
    std::mt19937 rng(3012);
    for (int n : {100, 257, 1000, 4095, 4096, 4097}) {
        vi values = pattern(n, 5, rng);
        sparse_table table(values);
        for (int step = 0; step < 400; ++step) {
            int left = (int)(rng() % n);
            int right = left + 1 + (int)(rng() % (n - left));
            EXPECT_EQ(table.get(left, right), brute(values, left, right))
                << "n=" << n << " [" << left << ", " << right << ")";
        }
        EXPECT_EQ(table.get(0, n), brute(values, 0, n));
    }
}

TEST(a_003_Struktury_dannyh_030_Sparse_Table, LoadBuildAndQueries) {
    const int n = 200000;
    const int queries = 200000;
    std::mt19937 rng(3013);
    vi values(n);
    for (int i = 0; i < n; ++i) {
        values[i] = (int)rng();
    }
    values[0] = INT_MAX;
    values[n - 1] = INT_MIN;
    values[n / 3] = inf;
    values[2 * n / 3] = inf + 7;
    sparse_table table(values);
    EXPECT_EQ(table.get(0, n), INT_MIN);
    for (int i = 0; i < n; i += 1000) {
        EXPECT_EQ(table.get(i, i + 1), values[i]);
    }
    for (int step = 0; step < 200; ++step) {
        int left = (int)(rng() % n);
        int right = left + 1 + (int)(rng() % (n - left));
        EXPECT_EQ(table.get(left, right), brute(values, left, right));
    }
    long long checksum = 0;
    for (int step = 0; step < queries; ++step) {
        int left = (int)(rng() % n);
        int len = 1 + (int)(rng() % std::min(n - left, 64));
        checksum += table.get(left, left + len);
    }
    EXPECT_EQ(table.get(n - 1, n), INT_MIN);
    (void)checksum;
}
