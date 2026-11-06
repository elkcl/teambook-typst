#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/080_MST/001_Краскал/code.hpp"

void add_undirected(vector<vpii> &g, int u, int v, int w) {
    g[u].push_back({v, w});
    g[v].push_back({u, w});
}

struct RefDsu {
    vi parent;
    explicit RefDsu(int n) : parent(n) { iota(parent.begin(), parent.end(), 0); }
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

std::optional<ll> mst_weight(const vector<vpii> &g) {
    int n = (int)g.size();
    if (n == 0) {
        return std::nullopt;
    }
    std::map<pii, int> best;
    for (int u = 0; u < n; ++u) {
        for (auto [v, w] : g[u]) {
            if (u == v) {
                continue;
            }
            pii edge{std::min(u, v), std::max(u, v)};
            auto it = best.find(edge);
            if (it == best.end() || w < it->second) {
                best[edge] = w;
            }
        }
    }
    std::vector<std::tuple<int, int, int>> edges;
    for (auto [edge, w] : best) {
        edges.push_back({w, edge.first, edge.second});
    }
    std::sort(edges.begin(), edges.end());
    RefDsu dsu(n);
    ll weight = 0;
    int taken = 0;
    for (auto [w, u, v] : edges) {
        if (dsu.unite(u, v)) {
            weight += w;
            ++taken;
        }
    }
    if (n > 1 && taken != n - 1) {
        return std::nullopt;
    }
    return weight;
}

int min_directed(const vector<vpii> &g, int u, int v) {
    bool found = false;
    int best = 0;
    for (auto [to, w] : g[u]) {
        if (to != v) {
            continue;
        }
        if (!found || w < best) {
            best = w;
            found = true;
        }
    }
    EXPECT_TRUE(found) << u << " -> " << v;
    return best;
}

void expect_kruskal(vector<vpii> g) {
    vector<vpii> saved = g;
    auto got = kruskal(g);
    EXPECT_EQ(g, saved);
    auto want = mst_weight(saved);
    if (!want.has_value()) {
        EXPECT_FALSE(got.has_value());
        return;
    }
    ASSERT_TRUE(got.has_value());
    int n = (int)saved.size();
    if (n <= 1) {
        EXPECT_TRUE(got->empty());
        return;
    }
    ASSERT_EQ((int)got->size(), n - 1);
    RefDsu dsu(n);
    ll weight = 0;
    int previous = std::numeric_limits<int>::min();
    for (auto [u, v] : *got) {
        ASSERT_NE(u, v);
        int edge = min_directed(saved, u, v);
        EXPECT_GE(edge, previous);
        previous = edge;
        weight += edge;
        ASSERT_TRUE(dsu.unite(u, v)) << u << " " << v;
    }
    EXPECT_EQ(weight, *want);
    int head = dsu.root(0);
    for (int v = 1; v < n; ++v) {
        EXPECT_EQ(dsu.root(v), head);
    }
}

vector<vpii> random_undirected(int n, int extra, int lo, int hi, std::mt19937 &rng) {
    vector<vpii> g(n);
    std::uniform_int_distribution<int> weight(lo, hi);
    std::uniform_int_distribution<int> vertex(0, n - 1);
    for (int v = 1; v < n; ++v) {
        add_undirected(g, (int)(rng() % v), v, weight(rng));
    }
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

TEST(a_002_Grafy_080_MST_001_Kraskal, EmptyAndSingleVertex) {
    vector<vpii> empty;
    EXPECT_FALSE(kruskal(empty).has_value());
    vector<vpii> one(1);
    auto single = kruskal(one);
    ASSERT_TRUE(single.has_value());
    EXPECT_TRUE(single->empty());
    one[0].push_back({0, -5});
    single = kruskal(one);
    ASSERT_TRUE(single.has_value());
    EXPECT_TRUE(single->empty());
}

TEST(a_002_Grafy_080_MST_001_Kraskal, SingleEdge) {
    vector<vpii> g(2);
    g[0].push_back({1, 7});
    auto got = kruskal(g);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, (vpii{{0, 1}}));
}

TEST(a_002_Grafy_080_MST_001_Kraskal, BothDirectionsCountedOnce) {
    vector<vpii> g(3);
    add_undirected(g, 0, 1, 2);
    add_undirected(g, 1, 2, 5);
    auto got = kruskal(g);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, (vpii{{0, 1}, {1, 2}}));
}

TEST(a_002_Grafy_080_MST_001_Kraskal, NegativeAndParallelEdges) {
    vector<vpii> g(3);
    g[0].push_back({1, 10});
    g[0].push_back({1, 3});
    g[1].push_back({2, 4});
    g[0].push_back({2, 100});
    auto got = kruskal(g);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, (vpii{{0, 1}, {1, 2}}));

    vector<vpii> negative(4);
    negative[1].push_back({2, -4});
    negative[0].push_back({1, 1});
    negative[2].push_back({3, 2});
    got = kruskal(negative);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, (vpii{{1, 2}, {0, 1}, {2, 3}}));
}

TEST(a_002_Grafy_080_MST_001_Kraskal, SelfLoopAndCycleAreSkipped) {
    vector<vpii> g(3);
    g[1].push_back({1, -100});
    add_undirected(g, 0, 1, 1);
    add_undirected(g, 1, 2, 2);
    add_undirected(g, 0, 2, 10);
    auto got = kruskal(g);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, (vpii{{0, 1}, {1, 2}}));
}

TEST(a_002_Grafy_080_MST_001_Kraskal, Disconnected) {
    vector<vpii> separate(3);
    add_undirected(separate, 1, 2, 1);
    EXPECT_FALSE(kruskal(separate).has_value());

    vector<vpii> missing(3);
    add_undirected(missing, 0, 1, 4);
    EXPECT_FALSE(kruskal(missing).has_value());

    vector<vpii> only_loop(2);
    only_loop[0].push_back({0, 1});
    EXPECT_FALSE(kruskal(only_loop).has_value());
}

TEST(a_002_Grafy_080_MST_001_Kraskal, HeavyEdgeStillConnects) {
    vector<vpii> g(2);
    add_undirected(g, 0, 1, INF);
    auto got = kruskal(g);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, (vpii{{0, 1}}));
}

TEST(a_002_Grafy_080_MST_001_Kraskal, EqualWeightsPreferSmallerEndpoints) {
    vector<vpii> g(4);
    add_undirected(g, 0, 1, 1);
    add_undirected(g, 0, 2, 1);
    add_undirected(g, 1, 3, 1);
    add_undirected(g, 2, 3, 1);
    auto got = kruskal(g);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, (vpii{{0, 1}, {0, 2}, {1, 3}}));
}

TEST(a_002_Grafy_080_MST_001_Kraskal, GraphIsPreserved) {
    vector<vpii> g(4);
    add_undirected(g, 0, 1, 2);
    add_undirected(g, 1, 2, -1);
    add_undirected(g, 1, 3, 5);
    g[2].push_back({2, 7});
    vector<vpii> saved = g;
    ASSERT_TRUE(kruskal(g).has_value());
    EXPECT_EQ(g, saved);
}

TEST(a_002_Grafy_080_MST_001_Kraskal, RandomConnected) {
    std::mt19937 rng(2202);
    for (int n : {2, 3, 5, 10, 40}) {
        for (int attempt = 0; attempt < 8; ++attempt) {
            expect_kruskal(random_undirected(n, n, -5, 20, rng));
        }
    }
    expect_kruskal(random_undirected(300, 800, 0, 1000, rng));
}

TEST(a_002_Grafy_080_MST_001_Kraskal, Load) {
    std::mt19937 rng(11);
    expect_kruskal(random_undirected(2000, 6000, 0, 50, rng));
}
