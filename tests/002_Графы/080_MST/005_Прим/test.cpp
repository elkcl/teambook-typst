#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/080_MST/005_Прим/code.hpp"

void add_undirected(vector<vpii> &g, int u, int v, int w) {
    g[u].push_back({v, w});
    g[v].push_back({u, w});
}

struct Dsu {
    vi parent;
    explicit Dsu(int n) : parent(n) { iota(parent.begin(), parent.end(), 0); }
    int root(int v) {
        if (parent[v] != v) {
            parent[v] = root(parent[v]);
        }
        return parent[v];
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
};

int min_weight(const vector<vpii> &g, int u, int v) {
    int best = INF;
    for (auto [to, w] : g[u]) {
        if (to == v) {
            best = min(best, w);
        }
    }
    return best;
}

std::optional<ll> kruskal(const vector<vpii> &g) {
    int n = (int)g.size();
    map<pii, int> best;
    for (int u = 0; u < n; ++u) {
        for (auto [v, w] : g[u]) {
            if (u >= v || w >= INF) {
                continue;
            }
            int back = min_weight(g, v, u);
            if (back >= INF || back != w) {
                continue;
            }
            pii edge{u, v};
            auto it = best.find(edge);
            if (it == best.end() || w < it->second) {
                best[edge] = w;
            }
        }
    }
    vector<tuple<int, int, int>> edges;
    for (auto [edge, w] : best) {
        edges.push_back({w, edge.first, edge.second});
    }
    sort(edges.begin(), edges.end());
    Dsu dsu(n);
    ll weight = 0;
    int taken = 0;
    for (auto [w, u, v] : edges) {
        if (dsu.unite(u, v)) {
            weight += w;
            ++taken;
        }
    }
    if (n <= 1) {
        return 0;
    }
    if (taken != n - 1) {
        return nullopt;
    }
    return weight;
}

void expect_tree(const vector<vpii> &g, const vpii &edges) {
    int n = (int)g.size();
    ASSERT_EQ((int)edges.size(), max(n - 1, 0));
    Dsu dsu(n);
    ll weight = 0;
    for (auto [v, parent] : edges) {
        ASSERT_GE(v, 0);
        ASSERT_LT(v, n);
        ASSERT_GE(parent, 0);
        ASSERT_LT(parent, n);
        ASSERT_NE(v, parent);
        int forward = min_weight(g, parent, v);
        int back = min_weight(g, v, parent);
        ASSERT_LT(forward, INF);
        ASSERT_EQ(forward, back);
        weight += forward;
        ASSERT_TRUE(dsu.unite(v, parent)) << v << " " << parent;
    }
    if (n > 0) {
        int root = dsu.root(0);
        for (int v = 1; v < n; ++v) {
            ASSERT_EQ(dsu.root(v), root);
        }
    }
    auto want = kruskal(g);
    ASSERT_TRUE(want.has_value());
    EXPECT_EQ(weight, *want);
}

void expect_prim(vector<vpii> g) {
    vector<vpii> saved = g;
    auto got = prim(g);
    EXPECT_EQ(g, saved);
    auto want = kruskal(saved);
    if (!want.has_value()) {
        EXPECT_FALSE(got.has_value());
        return;
    }
    ASSERT_TRUE(got.has_value());
    expect_tree(saved, *got);
}

vector<vpii> random_undirected(int n, int extra, int lo, int hi, std::mt19937 &rng) {
    vector<vpii> g(n);
    std::uniform_int_distribution<int> weight(lo, hi);
    for (int v = 1; v < n; ++v) {
        add_undirected(g, v, (int)(rng() % v), weight(rng));
    }
    std::uniform_int_distribution<int> vertex(0, n - 1);
    for (int i = 0; i < extra; ++i) {
        int u = vertex(rng);
        int v = vertex(rng);
        if (u != v) {
            add_undirected(g, u, v, weight(rng));
        }
    }
    return g;
}

}  // namespace

TEST(a_002_Grafy_080_MST_005_Prim, SingleVertex) {
    expect_prim(vector<vpii>(1));
    vector<vpii> loop(1);
    loop[0].push_back({0, -5});
    expect_prim(loop);
}

TEST(a_002_Grafy_080_MST_005_Prim, SingleEdge) {
    vector<vpii> g(2);
    add_undirected(g, 0, 1, 7);
    auto got = prim(g);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, (vpii{{1, 0}}));
}

TEST(a_002_Grafy_080_MST_005_Prim, OutgoingDirectionOnly) {
    vector<vpii> missing(2);
    missing[1].push_back({0, 1});
    EXPECT_FALSE(prim(missing).has_value());

    vector<vpii> forward(3);
    forward[0].push_back({1, 4});
    forward[1].push_back({0, 4});
    forward[2].push_back({1, 5});
    EXPECT_FALSE(prim(forward).has_value());

    forward[1].push_back({2, 5});
    auto got = prim(forward);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, (vpii{{1, 0}, {2, 1}}));
}

TEST(a_002_Grafy_080_MST_005_Prim, ChainStarAndNegative) {
    vector<vpii> chain(4);
    add_undirected(chain, 0, 1, 1);
    add_undirected(chain, 1, 2, -4);
    add_undirected(chain, 2, 3, 2);
    auto chain_mst = prim(chain);
    ASSERT_TRUE(chain_mst.has_value());
    EXPECT_EQ(*chain_mst, (vpii{{1, 0}, {2, 1}, {3, 2}}));

    vector<vpii> star(5);
    for (int v = 1; v < 5; ++v) {
        add_undirected(star, 0, v, 3);
    }
    auto star_mst = prim(star);
    ASSERT_TRUE(star_mst.has_value());
    EXPECT_EQ(*star_mst, (vpii{{1, 0}, {2, 0}, {3, 0}, {4, 0}}));

    vector<vpii> choice(3);
    add_undirected(choice, 0, 1, 10);
    add_undirected(choice, 0, 2, 10);
    add_undirected(choice, 1, 2, -2);
    auto choice_mst = prim(choice);
    ASSERT_TRUE(choice_mst.has_value());
    EXPECT_EQ(*choice_mst, (vpii{{1, 0}, {2, 1}}));
}

TEST(a_002_Grafy_080_MST_005_Prim, ParallelEdgesTakeMinimum) {
    vector<vpii> g(3);
    add_undirected(g, 0, 1, 10);
    add_undirected(g, 0, 1, 3);
    add_undirected(g, 1, 2, 4);
    add_undirected(g, 0, 2, 100);
    expect_prim(g);
    auto got = prim(g);
    ASSERT_TRUE(got.has_value());
    ll weight = 0;
    for (auto [v, parent] : *got) {
        weight += min_weight(g, parent, v);
    }
    EXPECT_EQ(weight, 7);
}

TEST(a_002_Grafy_080_MST_005_Prim, SelfLoopDoesNotReplaceTreeEdge) {
    vector<vpii> g(3);
    add_undirected(g, 0, 1, 5);
    add_undirected(g, 1, 2, 6);
    g[1].push_back({1, -100});
    expect_prim(g);
}

TEST(a_002_Grafy_080_MST_005_Prim, Disconnected) {
    vector<vpii> separate(3);
    add_undirected(separate, 1, 2, 1);
    EXPECT_FALSE(prim(separate).has_value());

    vector<vpii> missing_leaf(3);
    add_undirected(missing_leaf, 0, 1, 1);
    EXPECT_FALSE(prim(missing_leaf).has_value());

    vector<vpii> heavy(2);
    add_undirected(heavy, 0, 1, INF);
    EXPECT_FALSE(prim(heavy).has_value());

    add_undirected(heavy, 0, 1, INF - 1);
    expect_prim(heavy);
}

TEST(a_002_Grafy_080_MST_005_Prim, TieKeepsEarlierVertex) {
    vector<vpii> g(4);
    add_undirected(g, 0, 1, 1);
    add_undirected(g, 0, 2, 1);
    add_undirected(g, 1, 3, 1);
    add_undirected(g, 2, 3, 1);
    auto got = prim(g);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, (vpii{{1, 0}, {2, 0}, {3, 1}}));
}

TEST(a_002_Grafy_080_MST_005_Prim, GraphIsPreserved) {
    vector<vpii> g(4);
    add_undirected(g, 0, 1, 2);
    add_undirected(g, 1, 2, 2);
    add_undirected(g, 1, 3, 2);
    g[2].push_back({2, 1});
    vector<vpii> saved = g;
    ASSERT_TRUE(prim(g).has_value());
    EXPECT_EQ(g, saved);
}

TEST(a_002_Grafy_080_MST_005_Prim, RandomConnected) {
    std::mt19937 rng(2202);
    for (int n : {2, 3, 5, 10, 30}) {
        for (int attempt = 0; attempt < 8; ++attempt) {
            expect_prim(random_undirected(n, n, -5, 20, rng));
        }
    }
    expect_prim(random_undirected(200, 400, 0, 1000, rng));
}

TEST(a_002_Grafy_080_MST_005_Prim, Load) {
    std::mt19937 rng(11);
    expect_prim(random_undirected(800, 2000, 0, 30, rng));
}
