#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/003_Структуры данных/005_Дерево отрезков/011_Афинное на отрезке/code.hpp"

void prepare(int n) {
    t.assign(4 * n + 8, 0);
    need_push.assign(4 * n + 8, {});
}

void build(const std::vector<ll> &values, int v, int l, int r) {
    need_push[v] = {};
    if (r - l == 1) {
        t[v] = values[l] % M;
        return;
    }
    int mid = (l + r) / 2;
    build(values, v * 2, l, mid);
    build(values, v * 2 + 1, mid, r);
    t[v] = combine(t[v * 2], t[v * 2 + 1]);
}

void update(int v, int l, int r, int ql, int qr, Affine f) {
    push(v, l, r);
    if (qr <= l || r <= ql) {
        return;
    }
    if (ql <= l && r <= qr) {
        apply_push(v, l, r, f);
        return;
    }
    int mid = (l + r) / 2;
    update(v * 2, l, mid, ql, qr, f);
    update(v * 2 + 1, mid, r, ql, qr, f);
    t[v] = combine(t[v * 2], t[v * 2 + 1]);
}

ll query(int v, int l, int r, int ql, int qr) {
    if (qr <= l || r <= ql) {
        return 0;
    }
    push(v, l, r);
    if (ql <= l && r <= qr) {
        return t[v];
    }
    int mid = (l + r) / 2;
    return combine(query(v * 2, l, mid, ql, qr), query(v * 2 + 1, mid, r, ql, qr));
}

void apply_brute(std::vector<ll> &values, int l, int r, Affine f) {
    for (int i = l; i < r; ++i) {
        values[i] = (values[i] * f.k + f.b) % M;
    }
}

ll sum_brute(const std::vector<ll> &values, int l, int r) {
    ll sum = 0;
    for (int i = l; i < r; ++i) {
        sum = (sum + values[i]) % M;
    }
    return sum;
}

void expect_all(int n, const std::vector<ll> &values) {
    for (int l = 0; l <= n; ++l) {
        for (int r = l; r <= n; ++r) {
            EXPECT_EQ(query(1, 0, n, l, r), sum_brute(values, l, r)) << l << " " << r;
        }
    }
}

}  // namespace

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_011_Afinnoe_na_otrezke, CombineAndHandUpdates) {
    EXPECT_EQ(combine(3, 4), 7);
    EXPECT_EQ(combine(M - 1, 2), 1);

    std::vector<ll> values = {1, 2, 3, 4};
    prepare(4);
    build(values, 1, 0, 4);
    expect_all(4, values);

    Affine add{1, 5};
    update(1, 0, 4, 1, 4, add);
    apply_brute(values, 1, 4, add);
    expect_all(4, values);

    Affine scale{2, 0};
    update(1, 0, 4, 0, 2, scale);
    apply_brute(values, 0, 2, scale);
    expect_all(4, values);

    Affine set_zero{0, 0};
    update(1, 0, 4, 2, 3, set_zero);
    apply_brute(values, 2, 3, set_zero);
    expect_all(4, values);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_011_Afinnoe_na_otrezke, OverlapsAndSingleCells) {
    for (int n : {1, 2, 3, 5, 8}) {
        std::vector<ll> values(n);
        for (int i = 0; i < n; ++i) {
            values[i] = (i * 9 + 4) % 20;
        }
        prepare(n);
        build(values, 1, 0, n);
        Affine twice{2, 3};
        update(1, 0, n, 0, n, twice);
        apply_brute(values, 0, n, twice);
        Affine paint{0, 6};
        update(1, 0, n, n / 2, n, paint);
        apply_brute(values, n / 2, n, paint);
        expect_all(n, values);
    }
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_011_Afinnoe_na_otrezke, RandomSmall) {
    std::mt19937 rng(111);
    for (int n = 1; n <= 8; ++n) {
        std::vector<ll> values(n);
        for (int i = 0; i < n; ++i) {
            values[i] = rng() % 30;
        }
        prepare(n);
        build(values, 1, 0, n);
        for (int step = 0; step < 20; ++step) {
            int l = (int)(rng() % n);
            int r = l + 1 + (int)(rng() % (n - l));
            Affine f{(ll)(rng() % 5), (ll)(rng() % 11)};
            update(1, 0, n, l, r, f);
            apply_brute(values, l, r, f);
        }
        expect_all(n, values);
    }
}
