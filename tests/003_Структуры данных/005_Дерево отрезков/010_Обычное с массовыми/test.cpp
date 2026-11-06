#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/003_Структуры данных/005_Дерево отрезков/010_Обычное с массовыми/code.hpp"

using Arr = std::vector<long long>;

Arr as_long(const vi &values) {
    return Arr(values.begin(), values.end());
}

void assign_range(Arr &values, int left, int right, int x) {
    for (int i = left; i < right; ++i) {
        values[i] = x;
    }
}

long long range_sum(const Arr &values, int left, int right) {
    long long answer = 0;
    for (int i = left; i < right; ++i) {
        answer += values[i];
    }
    return answer;
}

int first_right(const Arr &values, int left, long long limit) {
    long long acc = 0;
    for (int i = left; i < (int)values.size(); ++i) {
        acc += values[i];
        if (acc > limit) {
            return i;
        }
    }
    return (int)values.size();
}

int first_left(const Arr &values, int right, long long limit) {
    long long acc = 0;
    for (int i = right - 1; i >= 0; --i) {
        acc += values[i];
        if (acc > limit) {
            return i + 1;
        }
    }
    return 0;
}

void expect_sums(SegTree &tree, const Arr &values) {
    int n = (int)values.size();
    EXPECT_EQ(tree.get(0, 0), 0);
    for (int left = 0; left <= n; ++left) {
        for (int right = left; right <= n; ++right) {
            EXPECT_EQ(tree.get(left, right), range_sum(values, left, right))
                << "[" << left << ", " << right << ")";
        }
    }
}

void expect_walks(SegTree &tree, const Arr &values, long long limit) {
    int n = (int)values.size();
    for (int left = 0; left <= n; ++left) {
        EXPECT_EQ(tree.max_right(left, [limit](ll sum) { return sum <= limit; }),
                  first_right(values, left, limit))
            << "max_right l=" << left << " limit=" << limit;
    }
    for (int right = 0; right <= n; ++right) {
        EXPECT_EQ(tree.min_left(right, [limit](ll sum) { return sum <= limit; }),
                  first_left(values, right, limit))
            << "min_left r=" << right << " limit=" << limit;
    }
}

}  // namespace

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_010_Obychnoe_s_massovymi, EmptyAndSingle) {
    SegTree empty(0);
    EXPECT_EQ(empty.get(0, 0), 0);
    EXPECT_EQ(empty.max_right(0, [](ll sum) { return sum <= 0; }), 0);
    EXPECT_EQ(empty.min_left(0, [](ll sum) { return sum <= 0; }), 0);

    SegTree one(vi{5});
    Arr values{5};
    expect_sums(one, values);
    expect_walks(one, values, 4);
    expect_walks(one, values, 5);
    one.upd(0, 1, -3);
    values[0] = -3;
    expect_sums(one, values);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_010_Obychnoe_s_massovymi, BuildAllSubarrays) {
    for (int n = 1; n <= 24; ++n) {
        vi source(n);
        for (int i = 0; i < n; ++i) {
            source[i] = i * i - 3 * i + (i % 2 ? -1 : 4);
        }
        SegTree tree(source);
        expect_sums(tree, as_long(source));
    }
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_010_Obychnoe_s_massovymi, AssignOverwritesAndTouchesEnds) {
    SegTree tree(vi{1, 2, 3, 4, 5, 6, 7, 8});
    Arr values{1, 2, 3, 4, 5, 6, 7, 8};
    tree.upd(2, 6, 10);
    assign_range(values, 2, 6, 10);
    expect_sums(tree, values);

    tree.upd(0, 1, 0);
    tree.upd(7, 8, -2);
    tree.upd(4, 8, -3);
    tree.upd(3, 3, 99);
    assign_range(values, 0, 1, 0);
    assign_range(values, 7, 8, -2);
    assign_range(values, 4, 8, -3);
    expect_sums(tree, values);
    EXPECT_EQ(tree.get(3, 4), 10);
    EXPECT_EQ(tree.get(4, 5), -3);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_010_Obychnoe_s_massovymi, NonPowerOfTwoPadding) {
    for (int n : {2, 3, 5, 6, 7, 9, 15, 17, 31, 33}) {
        vi source(n, 1);
        SegTree tree(source);
        tree.upd(0, n, 4);
        tree.upd(n / 2, n, 7);
        if (n > 1) {
            tree.upd(0, 1, -5);
        }
        Arr values(n, 4);
        assign_range(values, n / 2, n, 7);
        if (n > 1) {
            values[0] = -5;
        }
        expect_sums(tree, values);
        EXPECT_EQ(tree.get(0, n), range_sum(values, 0, n));
    }
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_010_Obychnoe_s_massovymi, WalksAfterAssign) {
    SegTree tree(vi{1, 2, 3, 4, 5, 6, 7});
    Arr values{1, 2, 3, 4, 5, 6, 7};
    expect_walks(tree, values, 0);
    expect_walks(tree, values, 6);
    expect_walks(tree, values, 100);
    tree.upd(1, 5, 3);
    assign_range(values, 1, 5, 3);
    expect_walks(tree, values, 9);
    expect_sums(tree, values);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_010_Obychnoe_s_massovymi, ZeroAssignAndOverlapping) {
    const int n = 20;
    Arr values(n, 0);
    SegTree tree(n);
    expect_sums(tree, values);
    tree.upd(0, n, 0);
    tree.upd(5, 15, 2);
    tree.upd(0, 8, 1);
    tree.upd(12, 20, -4);
    tree.upd(8, 12, 9);
    assign_range(values, 0, n, 0);
    assign_range(values, 5, 15, 2);
    assign_range(values, 0, 8, 1);
    assign_range(values, 12, 20, -4);
    assign_range(values, 8, 12, 9);
    expect_sums(tree, values);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_010_Obychnoe_s_massovymi, HeavyValuesStayIn64Bit) {
    const int n = 20;
    vi source(n, 1000000000);
    SegTree tree(source);
    EXPECT_EQ(tree.get(0, n), 20000000000LL);
    tree.upd(0, n, -1000000000);
    EXPECT_EQ(tree.get(0, n), -20000000000LL);
    EXPECT_EQ(tree.get(0, 1), -1000000000LL);
    tree.upd(3, 4, 1000000000);
    EXPECT_EQ(tree.get(0, n), -18000000000LL);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_010_Obychnoe_s_massovymi, StressRandomAssign) {
    std::mt19937 rng(2010);
    for (int n = 1; n <= 18; ++n) {
        for (int round = 0; round < 15; ++round) {
            vi source(n);
            for (int i = 0; i < n; ++i) {
                source[i] = (int)(rng() % 21) - 10;
            }
            SegTree tree(source);
            Arr values = as_long(source);
            for (int step = 0; step < 25; ++step) {
                int left = (int)(rng() % (n + 1));
                int right = (int)(rng() % (n + 1));
                if (left > right) {
                    std::swap(left, right);
                }
                int x = (int)(rng() % 15) - 7;
                tree.upd(left, right, x);
                assign_range(values, left, right, x);
                expect_sums(tree, values);
            }
        }
    }
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_010_Obychnoe_s_massovymi, BoundariesAroundPowersOfTwo) {
    for (int shift = 1; shift <= 10; ++shift) {
        for (int delta : {-1, 0, 1}) {
            int n = (1 << shift) + delta;
            if (n <= 0) {
                continue;
            }
            vi source(n);
            for (int i = 0; i < n; ++i) {
                source[i] = (i % 5) - 2;
            }
            SegTree tree(source);
            Arr values = as_long(source);
            tree.upd(0, n, 3);
            assign_range(values, 0, n, 3);
            tree.upd(n / 3, (2 * n) / 3, -1);
            assign_range(values, n / 3, (2 * n) / 3, -1);
            EXPECT_EQ(tree.get(0, n), range_sum(values, 0, n));
            for (int i = 0; i < n; ++i) {
                EXPECT_EQ(tree.get(i, i + 1), values[i]) << "n=" << n << " i=" << i;
            }
            EXPECT_EQ(tree.max_right(0, [](ll sum) { return sum <= 1000000; }), n);
        }
    }
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_010_Obychnoe_s_massovymi, LoadRangeAssign) {
    const int n = 100000;
    const int ops = 400;
    std::mt19937 rng(2011);
    vi source(n);
    Arr values(n);
    for (int i = 0; i < n; ++i) {
        source[i] = (int)(rng() % 1000) - 500;
        values[i] = source[i];
    }
    SegTree tree(source);
    for (int step = 0; step < ops; ++step) {
        int left = (int)(rng() % n);
        int right = left + (int)(rng() % (n - left)) + 1;
        int x = (int)(rng() % 2001) - 1000;
        tree.upd(left, right, x);
        assign_range(values, left, right, x);
        int ql = (int)(rng() % n);
        int qr = ql + (int)(rng() % (n - ql)) + 1;
        EXPECT_EQ(tree.get(ql, qr), range_sum(values, ql, qr));
        EXPECT_EQ(tree.get(0, n), range_sum(values, 0, n));
    }
    EXPECT_EQ(tree.max_right(0, [](ll sum) { return sum <= (ll)4e18; }), n);
}
