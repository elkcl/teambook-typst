#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/007_Другое/TOF/code.hpp"

ll leftmost_min(ll left, ll right, const std::function<ll(ll)> &f) {
    ll best = left;
    for (ll i = left; i <= right; ++i) {
        if (f(i) < f(best)) {
            best = i;
        }
    }
    return best;
}

ll guarded(ll left, ll right, ll value, const std::function<ll(ll)> &inside) {
    if (value < left || value > right) {
        return (ll)4e17;
    }
    return inside(value);
}

}  // namespace

TEST(a_007_Drugoe_TOF, LooksLeftOfTheRangeWhenValuesKeepFalling) {
    auto identity = [](ll i) { return i; };
    EXPECT_EQ(tof(5, 12, identity), 0);
    EXPECT_EQ(tof(5, 5, identity), 0);
    EXPECT_LT(tof(30, 80, [](ll i) { return -(i - 50) * (i - 50); }), 30);
}

TEST(a_007_Drugoe_TOF, LeftmostMinimumWhenOutsideIsWorse) {
    auto check = [](ll left, ll right, ll block, const std::function<ll(ll)> &inside) {
        auto f = [&](ll i) { return guarded(left, right, i, inside); };
        ll got = tof(left, right, f, block);
        ASSERT_GE(got, left);
        ASSERT_LE(got, right);
        EXPECT_EQ(got, leftmost_min(left, right, f));
    };

    check(0, 100, 20, [](ll i) { return (i - 50) * (i - 50); });
    check(3, 7, 20, [](ll i) { return i; });
    check(0, 40, 1, [](ll i) { return -i; });
    check(0, 40, 5, [](ll i) { return llabs(i - 17); });
    check(4, 4, 3, [](ll) { return 8; });
    check(2, 9, 4, [](ll i) { return (i < 5 ? 5 - i : i - 5); });
}

TEST(a_007_Drugoe_TOF, RandomConvexAndAbsolute) {
    std::mt19937 rng(7072);
    for (int test = 0; test < 80; ++test) {
        ll left = (ll)(rng() % 30);
        ll right = left + (ll)(rng() % 25);
        ll block = 1 + (ll)(rng() % 7);
        ll center = (ll)(rng() % 40) - 5;
        int kind = (int)(rng() % 3);
        auto inside = [&](ll i) -> ll {
            if (kind == 0) {
                return (i - center) * (i - center);
            }
            if (kind == 1) {
                return llabs(i - center);
            }
            return i;
        };
        auto f = [&](ll i) { return guarded(left, right, i, inside); };
        ll got = tof(left, right, f, block);
        EXPECT_EQ(got, leftmost_min(left, right, f)) << left << " " << right << " " << block;
    }
}
