#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/060_LCA (бинарные подъёмы)/code.hpp"

vvi tree_from_parent(const vi &parent) {
    int n = (int)parent.size();
    vvi g(n);
    for (int v = 0; v < n; ++v) {
        if (parent[v] == -1) {
            continue;
        }
        g[v].push_back(parent[v]);
        g[parent[v]].push_back(v);
    }
    return g;
}

int brute_lca(const vi &parent, int a, int b) {
    vc seen(parent.size(), false);
    for (int v = a; v != -1; v = parent[v]) {
        seen[v] = true;
    }
    for (int v = b; v != -1; v = parent[v]) {
        if (seen[v]) {
            return v;
        }
    }
    return -1;
}

struct FastLCA {
    vi parent;
    vi depth;
    vvi up;
    explicit FastLCA(const vi &p) : parent(p) {
        int n = (int)p.size();
        depth.assign(n, 0);
        for (int v = 1; v < n; ++v) {
            depth[v] = depth[parent[v]] + 1;
        }
        int lg = 1;
        while ((1 << lg) <= n) {
            ++lg;
        }
        up.assign(n, vi(lg, 0));
        for (int v = 0; v < n; ++v) {
            up[v][0] = parent[v] == -1 ? v : parent[v];
        }
        for (int j = 1; j < lg; ++j) {
            for (int v = 0; v < n; ++v) {
                up[v][j] = up[up[v][j - 1]][j - 1];
            }
        }
    }
    int lca(int a, int b) const {
        if (depth[a] < depth[b]) {
            std::swap(a, b);
        }
        int diff = depth[a] - depth[b];
        for (int j = 0; j < (int)up[a].size(); ++j) {
            if ((diff >> j) & 1) {
                a = up[a][j];
            }
        }
        if (a == b) {
            return a;
        }
        for (int j = (int)up[a].size() - 1; j >= 0; --j) {
            if (up[a][j] != up[b][j]) {
                a = up[a][j];
                b = up[b][j];
            }
        }
        return parent[a];
    }
};

void expect_tree(const vi &parent, const std::vector<std::pair<int, int>> &queries) {
    ASSERT_FALSE(parent.empty());
    ASSERT_EQ(parent[0], -1);
    int n = (int)parent.size();
    vvi g = tree_from_parent(parent);
    LCA solver(g, n);
    FastLCA fast(parent);
    if (n <= 150) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int walk = brute_lca(parent, i, j);
                int lifted = fast.lca(i, j);
                if (walk != lifted) {
                    ADD_FAILURE() << "эталон разошёлся на " << i << " " << j;
                    return;
                }
            }
        }
    }
    for (auto [a, b] : queries) {
        int exp = n <= 400 ? brute_lca(parent, a, b) : fast.lca(a, b);
        int got = solver.lca(a, b);
        if (got != exp) {
            ADD_FAILURE() << "lca(" << a << ", " << b << ") = " << got << ", ожидалось " << exp;
            return;
        }
        int rev = solver.lca(b, a);
        if (rev != exp) {
            ADD_FAILURE() << "lca(" << b << ", " << a << ") = " << rev << ", ожидалось " << exp;
            return;
        }
    }
}

void expect_all_pairs(const vi &parent) {
    int n = (int)parent.size();
    std::vector<std::pair<int, int>> queries;
    queries.reserve((size_t)n * n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            queries.push_back({i, j});
        }
    }
    expect_tree(parent, queries);
}

vi chain(int n) {
    vi parent(n, -1);
    for (int i = 1; i < n; ++i) {
        parent[i] = i - 1;
    }
    return parent;
}

vi star(int n) {
    vi parent(n, -1);
    for (int i = 1; i < n; ++i) {
        parent[i] = 0;
    }
    return parent;
}

vi heap_tree(int n) {
    vi parent(n, -1);
    for (int i = 1; i < n; ++i) {
        parent[i] = (i - 1) / 2;
    }
    return parent;
}

// Две длинные ветки из корня: ни один лист не предок другого, подъём реально используется.
vi two_arms(int n) {
    vi parent(n, -1);
    int mid = std::max(1, n / 2);
    for (int i = 1; i <= mid && i < n; ++i) {
        parent[i] = i - 1;
    }
    if (mid + 1 < n) {
        parent[mid + 1] = 0;
        for (int i = mid + 2; i < n; ++i) {
            parent[i] = i - 1;
        }
    }
    return parent;
}

vi random_tree(int n, std::mt19937 &rng) {
    vi parent(n, -1);
    for (int i = 1; i < n; ++i) {
        parent[i] = std::uniform_int_distribution<int>(0, i - 1)(rng);
    }
    return parent;
}

std::vector<std::pair<int, int>> sample_pairs(int n, int count, std::mt19937 &rng) {
    std::uniform_int_distribution<int> dist(0, n - 1);
    std::vector<std::pair<int, int>> queries;
    queries.reserve(count + 2 * n);
    for (int i = 0; i < n; ++i) {
        queries.push_back({i, i});
        queries.push_back({0, i});
        queries.push_back({i, n - 1});
    }
    for (int i = 0; i < count; ++i) {
        queries.push_back({dist(rng), dist(rng)});
    }
    return queries;
}

}  // namespace

TEST(a_002_Grafy_060_LCA_binarnye_podyomy, SingleVertex) {
    expect_all_pairs(chain(1));
}

TEST(a_002_Grafy_060_LCA_binarnye_podyomy, TwoVerticesBothOrders) {
    expect_all_pairs(chain(2));
}

TEST(a_002_Grafy_060_LCA_binarnye_podyomy, ChainAllPairs) {
    for (int n = 2; n <= 80; ++n) {
        expect_all_pairs(chain(n));
    }
}

TEST(a_002_Grafy_060_LCA_binarnye_podyomy, StarAllPairs) {
    for (int n = 2; n <= 40; ++n) {
        expect_all_pairs(star(n));
    }
}

TEST(a_002_Grafy_060_LCA_binarnye_podyomy, HeapShapeAllPairs) {
    for (int n = 1; n <= 63; ++n) {
        expect_all_pairs(heap_tree(n));
    }
}

TEST(a_002_Grafy_060_LCA_binarnye_podyomy, TwoArmsUsesBinaryLifting) {
    for (int n : {4, 5, 7, 8, 9, 15, 16, 17, 31, 32, 33, 63, 64, 65, 127, 128, 129}) {
        expect_all_pairs(two_arms(n));
    }
}

TEST(a_002_Grafy_060_LCA_binarnye_podyomy, SplitBelowRoot) {
    // Корни веток — 1 и 2. LCA листьев разных веток не корень.
    vi parent = {-1, 0, 0, 1, 1, 2, 2, 6};
    expect_all_pairs(parent);
    vvi g = tree_from_parent(parent);
    LCA solver(g, (int)g.size());
    EXPECT_EQ(solver.lca(3, 4), 1);
    EXPECT_EQ(solver.lca(4, 3), 1);
    EXPECT_EQ(solver.lca(5, 7), 2);
    EXPECT_EQ(solver.lca(7, 6), 6);
    EXPECT_EQ(solver.lca(3, 7), 0);
    EXPECT_EQ(solver.lca(4, 5), 0);
    EXPECT_EQ(solver.lca(1, 7), 0);
    EXPECT_EQ(solver.lca(solver.lca(3, 4), 4), 1);
}

TEST(a_002_Grafy_060_LCA_binarnye_podyomy, RepeatedQueriesDoNotMutate) {
    vi parent = two_arms(64);
    vvi g = tree_from_parent(parent);
    LCA solver(g, (int)g.size());
    for (int rep = 0; rep < 5; ++rep) {
        EXPECT_EQ(solver.lca(31, 63), brute_lca(parent, 31, 63));
        EXPECT_EQ(solver.lca(63, 31), brute_lca(parent, 63, 31));
        EXPECT_EQ(solver.lca(10, 10), 10);
    }
}

TEST(a_002_Grafy_060_LCA_binarnye_podyomy, PowersOfTwoBoundaries) {
    std::mt19937 rng(7);
    for (int k = 1; k <= 14; ++k) {
        for (int n : {(1 << k) - 1, 1 << k, (1 << k) + 1}) {
            if (n < 2) {
                continue;
            }
            expect_tree(two_arms(n), sample_pairs(n, 200, rng));
            expect_tree(heap_tree(n), sample_pairs(n, 200, rng));
            expect_tree(chain(n), sample_pairs(n, 50, rng));
        }
    }
}

TEST(a_002_Grafy_060_LCA_binarnye_podyomy, RandomTreesAllPairs) {
    std::mt19937 rng(11);
    for (int n = 1; n <= 25; ++n) {
        for (int it = 0; it < 15; ++it) {
            expect_all_pairs(random_tree(n, rng));
        }
    }
}

TEST(a_002_Grafy_060_LCA_binarnye_podyomy, StressRandomQueries) {
    std::mt19937 rng(13);
    for (int n : {100, 500, 2000, 8000}) {
        expect_tree(random_tree(n, rng), sample_pairs(n, 2000, rng));
        expect_tree(two_arms(n), sample_pairs(n, 300, rng));
        expect_tree(heap_tree(n), sample_pairs(n, 2000, rng));
    }
}

TEST(a_002_Grafy_060_LCA_binarnye_podyomy, LoadWideAndDeep) {
    std::mt19937 rng(17);
    expect_tree(heap_tree(100000), sample_pairs(100000, 20000, rng));
    expect_tree(star(100000), sample_pairs(100000, 5000, rng));
    expect_tree(two_arms(20000), sample_pairs(20000, 200, rng));
    expect_tree(chain(20000), sample_pairs(20000, 100, rng));
}
