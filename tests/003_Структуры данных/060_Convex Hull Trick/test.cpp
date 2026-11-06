#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/003_Структуры данных/060_Convex Hull Trick/code.hpp"

using i128 = __int128;
using Line = std::pair<ll, ll>;

i128 eval(ll k, ll b, ll x) { return (i128)k * x + (i128)b; }

std::string i128_text(i128 value) {
    if (value == 0) {
        return "0";
    }
    bool negative = value < 0;
    if (negative) {
        value = -value;
    }
    std::string text;
    while (value > 0) {
        text.push_back(char('0' + (int)(value % 10)));
        value /= 10;
    }
    if (negative) {
        text.push_back('-');
    }
    std::reverse(text.begin(), text.end());
    return text;
}

i128 minimum_at(const std::vector<Line> &lines, ll x) {
    i128 best = 0;
    bool any = false;
    for (auto [k, b] : lines) {
        i128 current = eval(k, b, x);
        if (!any || current < best) {
            best = current;
            any = true;
        }
    }
    return best;
}

void expect_value(ll got, i128 expected) {
    if ((i128)got != expected) {
        ADD_FAILURE() << "получено " << got << ", ожидалось " << i128_text(expected);
    }
}

void expect_queries(cht &hull, const std::vector<Line> &lines, const std::vector<int> &xs) {
    for (int x : xs) {
        expect_value(hull.get(x), minimum_at(lines, x));
    }
}

}  // namespace

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, SingleLine) {
    cht hull;
    hull.add({3, -4});
    for (int x : {-inf, -10, 0, 1, 10, inf}) {
        EXPECT_EQ(hull.get(x), 3LL * x - 4);
        EXPECT_EQ(hull.get_ordered(x), 3LL * x - 4);
    }
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, TwoLinesAndExactIntersection) {
    cht hull;
    std::vector<Line> lines{{0, 0}, {-1, 10}};
    for (auto [k, b] : lines) {
        hull.add({k, b});
    }
    EXPECT_EQ(hull.get(0), 0);
    EXPECT_EQ(hull.get(9), 0);
    EXPECT_EQ(hull.get(10), 0);
    EXPECT_EQ(hull.get(11), -1);
    EXPECT_EQ(hull.get(-inf), 0);
    EXPECT_EQ(hull.get(inf), (ll)-inf + 10);
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, DominatedMiddleLineIsRemoved) {
    cht hull;
    std::vector<Line> lines{{3, 0}, {1, 10}, {-1, 14}};
    for (auto [k, b] : lines) {
        hull.add({k, b});
    }
    std::vector<int> xs;
    for (int x = -20; x <= 20; ++x) {
        xs.push_back(x);
    }
    xs.push_back(-inf);
    xs.push_back(inf);
    expect_queries(hull, lines, xs);
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, FractionalCeilBoundary) {
    cht hull;
    std::vector<Line> lines{{3, 0}, {1, 11}};
    for (auto [k, b] : lines) {
        hull.add({k, b});
    }
    EXPECT_EQ(hull.get(5), 15);
    EXPECT_EQ(hull.get(6), 17);
    expect_queries(hull, lines, {-inf, -3, 0, 5, 6, 7, inf});
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, NegativeIntersection) {
    cht hull;
    std::vector<Line> lines{{0, 0}, {-3, -10}};
    for (auto [k, b] : lines) {
        hull.add({k, b});
    }
    EXPECT_EQ(hull.get(-4), 0);
    EXPECT_EQ(hull.get(-3), -1);
    expect_queries(hull, lines, {-inf, -5, -3, 0, 4, inf});
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, OrderedQueriesFollowIncreasingX) {
    cht hull;
    std::vector<Line> lines{{5, 1}, {2, 20}, {0, 40}, {-4, 100}};
    for (auto [k, b] : lines) {
        hull.add({k, b});
    }
    std::vector<int> xs{-inf, -20, -1, 0, 3, 10, 10, 25, inf};
    for (int x : xs) {
        expect_value(hull.get(x), minimum_at(lines, x));
        expect_value(hull.get_ordered(x), minimum_at(lines, x));
    }
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, RandomDecreasingSlopes) {
    std::mt19937 rng(6010);
    for (int round = 0; round < 80; ++round) {
        int count = 1 + (int)(rng() % 25);
        ll slope = 1000 + (ll)(rng() % 1000);
        std::vector<Line> lines;
        cht hull;
        for (int i = 0; i < count; ++i) {
            slope -= 1 + (ll)(rng() % 7);
            ll intercept = (ll)(rng() % 4001) - 2000;
            lines.push_back({slope, intercept});
            hull.add({slope, intercept});
        }
        for (int x = -30; x <= 30; ++x) {
            expect_value(hull.get(x), minimum_at(lines, x));
        }
        cht ordered;
        for (auto [k, b] : lines) {
            ordered.add({k, b});
        }
        std::vector<int> xs;
        for (int i = 0; i < 20; ++i) {
            xs.push_back((int)(rng() % 61) - 30);
        }
        std::sort(xs.begin(), xs.end());
        for (int x : xs) {
            expect_value(ordered.get_ordered(x), minimum_at(lines, x));
        }
    }
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, WideButFittingCoefficients) {
    std::mt19937 rng(6011);
    ll slope = 1000000;
    std::vector<Line> lines;
    cht hull;
    for (int i = 0; i < 40; ++i) {
        slope -= 1000 + (ll)(rng() % 5000);
        ll intercept = (ll)(rng() % 2000001) - 1000000;
        intercept *= 1000000;
        lines.push_back({slope, intercept});
        hull.add({slope, intercept});
    }
    std::vector<int> xs{-inf, -1000000, -12345, -1, 0, 1, 99991, 1000000, inf};
    expect_queries(hull, lines, xs);
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, LineBetterFarLeftStillCorrectInsideSentinel) {
    cht hull;
    std::vector<Line> lines{{0, 0}, {-1, -200000000000LL}};
    for (auto [k, b] : lines) {
        hull.add({k, b});
    }
    expect_queries(hull, lines, {-inf, -inf + 1, 0, 10, inf});
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, IncreasingSlopesDoNotBuildLowerHull) {
    cht hull;
    std::vector<Line> lines{{0, 0}, {1, -100}};
    for (auto [k, b] : lines) {
        hull.add({k, b});
    }
    expect_queries(hull, lines, {-20, 0, 50, 100, 150});
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, OrderedGetterDoesNotSearchBackward) {
    cht hull;
    std::vector<Line> lines{{0, 0}, {-1, 10}, {-2, 30}};
    for (auto [k, b] : lines) {
        hull.add({k, b});
    }
    EXPECT_EQ(hull.get_ordered(100), -170);
    expect_value(hull.get_ordered(0), minimum_at(lines, 0));
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, ProductOverflowsLongLong) {
    cht hull;
    ll slope = 1LL << 40;
    int x = 1 << 30;
    hull.add({slope, 0});
    expect_value(hull.get(x), eval(slope, 0, x));
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, IntersectionOverflowDropsBetterLine) {
    ll first = std::numeric_limits<ll>::max() - 10;
    ll second = std::numeric_limits<ll>::min() + 10;
    cht hull;
    hull.add({5, first});
    hull.add({4, second});
    EXPECT_EQ(hull.get(0), second);
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, LoadManyLines) {
    const int count = 100000;
    std::mt19937 rng(6012);
    ll slope = 500000;
    std::vector<Line> lines;
    lines.reserve(count);
    cht hull;
    for (int i = 0; i < count; ++i) {
        slope -= 1 + (ll)(rng() % 3);
        ll intercept = (ll)((int)rng() % 2001 - 1000);
        lines.push_back({slope, intercept});
        hull.add({slope, intercept});
    }
    std::vector<int> xs{-inf, -1000, 0, 1, 10, 1000, inf};
    expect_queries(hull, lines, xs);
    cht ordered;
    for (auto [k, b] : lines) {
        ordered.add({k, b});
    }
    std::sort(xs.begin(), xs.end());
    for (int x : xs) {
        expect_value(ordered.get_ordered(x), minimum_at(lines, x));
    }
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, ParallelSlopesDivideByZero) {
    cht hull;
    hull.add({2, 5});
    hull.add({2, -1});
    EXPECT_EQ(hull.get(0), -1);
    EXPECT_EQ(hull.get(10), -1);
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, QueryLeftOfMinusInf) {
    cht hull;
    hull.add({0, 7});
    EXPECT_EQ(hull.get(-inf - 1), 7);
    EXPECT_EQ(hull.get(INT_MIN), 7);
}

TEST(a_003_Struktury_dannyh_060_Convex_Hull_Trick, EmptyHullGet) {
    cht hull;
    EXPECT_EQ(hull.get(0), 0);
}
