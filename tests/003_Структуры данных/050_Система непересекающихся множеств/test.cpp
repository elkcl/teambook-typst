#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/003_Структуры данных/050_Система непересекающихся множеств/code.hpp"

struct Components {
    vi parent;

    explicit Components(int n) : parent(n) { iota(parent.begin(), parent.end(), 0); }

    int root(int v) const {
        while (parent[v] != v) {
            v = parent[v];
        }
        return v;
    }

    bool unite(int a, int b) {
        a = root(a);
        b = root(b);
        if (a == b) {
            return false;
        }
        parent[b] = a;
        return true;
    }

    bool same(int a, int b) const { return root(a) == root(b); }

    int size_of(int v) const {
        int head = root(v);
        int count = 0;
        for (int i = 0; i < (int)parent.size(); ++i) {
            if (root(i) == head) {
                ++count;
            }
        }
        return count;
    }
};

void expect_same(DSU &dsu, const Components &ref) {
    int n = (int)ref.parent.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            EXPECT_EQ(dsu.root(i) == dsu.root(j), ref.same(i, j)) << i << " " << j;
        }
        EXPECT_EQ(dsu.sz[dsu.root(i)], ref.size_of(i)) << i;
    }
}

}  // namespace

TEST(a_003_Struktury_dannyh_050_Sistema_neperesekayuschihsya_mnozhestv, EmptyAndSingletons) {
    DSU empty(0);
    EXPECT_TRUE(empty.p.empty());
    EXPECT_TRUE(empty.sz.empty());

    DSU one(1);
    EXPECT_EQ(one.root(0), 0);
    EXPECT_EQ(one.sz[0], 1);
    EXPECT_FALSE(one.unite(0, 0));
    EXPECT_EQ(one.root(0), 0);
    EXPECT_EQ(one.sz[0], 1);

    DSU many(5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(many.root(i), i);
        EXPECT_EQ(many.sz[i], 1);
        EXPECT_FALSE(many.unite(i, i));
    }
}

TEST(a_003_Struktury_dannyh_050_Sistema_neperesekayuschihsya_mnozhestv, UniteLinksLargerRoot) {
    DSU dsu(4);
    EXPECT_TRUE(dsu.unite(0, 1));
    EXPECT_EQ(dsu.root(0), 0);
    EXPECT_EQ(dsu.root(1), 0);
    EXPECT_EQ(dsu.sz[0], 2);
    EXPECT_FALSE(dsu.unite(1, 0));
    EXPECT_FALSE(dsu.unite(0, 1));

    DSU other(2);
    EXPECT_TRUE(other.unite(1, 0));
    EXPECT_EQ(other.root(0), 1);
    EXPECT_EQ(other.root(1), 1);
    EXPECT_EQ(other.sz[1], 2);
}

TEST(a_003_Struktury_dannyh_050_Sistema_neperesekayuschihsya_mnozhestv, SmallerComponentJoinsLarger) {
    DSU dsu(6);
    EXPECT_TRUE(dsu.unite(0, 1));
    EXPECT_TRUE(dsu.unite(0, 2));
    EXPECT_EQ(dsu.sz[0], 3);

    EXPECT_TRUE(dsu.unite(3, 4));
    EXPECT_EQ(dsu.root(3), 3);
    EXPECT_EQ(dsu.sz[3], 2);

    EXPECT_TRUE(dsu.unite(3, 0));
    EXPECT_EQ(dsu.root(3), 0);
    EXPECT_EQ(dsu.root(4), 0);
    EXPECT_EQ(dsu.root(2), 0);
    EXPECT_EQ(dsu.sz[0], 5);
    EXPECT_FALSE(dsu.unite(4, 1));

    EXPECT_TRUE(dsu.unite(5, 4));
    EXPECT_EQ(dsu.root(5), 0);
    EXPECT_EQ(dsu.sz[0], 6);
}

TEST(a_003_Struktury_dannyh_050_Sistema_neperesekayuschihsya_mnozhestv, PathCompressionRewritesParent) {
    DSU dsu(4);
    EXPECT_TRUE(dsu.unite(0, 1));
    EXPECT_TRUE(dsu.unite(2, 3));
    EXPECT_TRUE(dsu.unite(0, 2));
    EXPECT_EQ(dsu.p[2], 0);
    EXPECT_EQ(dsu.p[3], 2);
    EXPECT_EQ(dsu.root(3), 0);
    EXPECT_EQ(dsu.p[3], 0);
    EXPECT_EQ(dsu.p[2], 0);
    EXPECT_EQ(dsu.sz[0], 4);
}

TEST(a_003_Struktury_dannyh_050_Sistema_neperesekayuschihsya_mnozhestv, SeparateComponentsStaySeparate) {
    DSU dsu(6);
    Components ref(6);
    auto merge = [&](int a, int b) {
        EXPECT_EQ(dsu.unite(a, b), ref.unite(a, b));
    };
    merge(0, 1);
    merge(2, 3);
    merge(1, 0);
    merge(4, 5);
    merge(3, 2);
    expect_same(dsu, ref);
    EXPECT_NE(dsu.root(0), dsu.root(2));
    EXPECT_NE(dsu.root(2), dsu.root(4));
}

TEST(a_003_Struktury_dannyh_050_Sistema_neperesekayuschihsya_mnozhestv, RandomUnionsMatchComponents) {
    std::mt19937 rng(2202);
    for (int n : {1, 2, 3, 8, 30}) {
        DSU dsu(n);
        Components ref(n);
        std::uniform_int_distribution<int> vertex(0, n - 1);
        int steps = n * 4;
        for (int step = 0; step < steps; ++step) {
            int a = vertex(rng);
            int b = vertex(rng);
            EXPECT_EQ(dsu.unite(a, b), ref.unite(a, b)) << n << " " << a << " " << b;
        }
        expect_same(dsu, ref);
    }
}

TEST(a_003_Struktury_dannyh_050_Sistema_neperesekayuschihsya_mnozhestv, LoadChain) {
    const int n = 200000;
    DSU dsu(n);
    for (int i = 1; i < n; ++i) {
        ASSERT_TRUE(dsu.unite(0, i));
    }
    EXPECT_EQ(dsu.sz[dsu.root(0)], n);
    EXPECT_FALSE(dsu.unite(n - 1, 1));
    for (int i = 0; i < n; i += 1000) {
        EXPECT_EQ(dsu.root(i), dsu.root(0));
    }
}
