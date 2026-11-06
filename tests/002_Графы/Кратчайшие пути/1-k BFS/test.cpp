#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/Кратчайшие пути/1-k BFS/code.hpp"

const long long kUnreach = (long long)4e18;

std::vector<long long> reference(const vector<vpii> &g, int s) {
    int n = (int)g.size();
    std::vector<long long> d(n, kUnreach);
    if (s < 0 || s >= n) {
        return d;
    }
    d[s] = 0;
    using Item = std::pair<long long, int>;
    std::priority_queue<Item, std::vector<Item>, std::greater<Item>> pq;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [cd, v] = pq.top();
        pq.pop();
        if (cd != d[v]) {
            continue;
        }
        for (auto [u, w] : g[v]) {
            long long nd = cd + (long long)w;
            if (nd < d[u]) {
                d[u] = nd;
                pq.push({nd, u});
            }
        }
    }
    return d;
}

bool expect_dist(const vi &got, const std::vector<long long> &exp) {
    if (got.size() != exp.size()) {
        ADD_FAILURE() << "размер " << got.size() << " вместо " << exp.size();
        return false;
    }
    for (int i = 0; i < (int)exp.size(); ++i) {
        long long want = exp[i] >= kUnreach / 2 ? inf : exp[i];
        if ((long long)got[i] != want) {
            ADD_FAILURE() << "d[" << i << "] = " << got[i] << ", ожидалось " << want;
            return false;
        }
    }
    return true;
}

bool check(vector<vpii> g, int s, int k) {
    auto original = g;
    auto got = bfs1k(g, s, k);
    EXPECT_EQ(g, original);
    return expect_dist(got, reference(original, s));
}

vector<vpii> random_digraph(int n, int m, int wmin, int wmax, std::mt19937 &rng) {
    vector<vpii> g(n);
    std::uniform_int_distribution<int> vertex(0, n - 1);
    std::uniform_int_distribution<int> weight(wmin, wmax);
    for (int i = 0; i < m; ++i) {
        g[vertex(rng)].push_back({vertex(rng), weight(rng)});
    }
    return g;
}

}  // namespace

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, SingleVertex) {
    ASSERT_TRUE(check(vector<vpii>(1), 0, 0));
    ASSERT_TRUE(check(vector<vpii>(1), 0, 4));
    vector<vpii> loop(1);
    loop[0].push_back({0, 0});
    loop[0].push_back({0, 4});
    ASSERT_TRUE(check(loop, 0, 4));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, NoEdges) {
    ASSERT_TRUE(check(vector<vpii>(5), 0, 1));
    ASSERT_TRUE(check(vector<vpii>(5), 3, 7));
    ASSERT_TRUE(check(vector<vpii>(5), 4, 0));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, DirectedEdgeAndSourceNotZero) {
    vector<vpii> g(4);
    g[2].push_back({0, 3});
    g[0].push_back({2, 1});
    g[2].push_back({3, 0});
    ASSERT_TRUE(check(g, 2, 3));
    ASSERT_TRUE(check(g, 0, 3));
    ASSERT_TRUE(check(g, 1, 3));
    ASSERT_TRUE(check(g, 3, 3));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, ParallelEdgesTakeMinimum) {
    vector<vpii> g(3);
    g[0].push_back({1, 5});
    g[0].push_back({1, 1});
    g[0].push_back({1, 4});
    g[1].push_back({2, 3});
    g[1].push_back({2, 2});
    ASSERT_TRUE(check(g, 0, 5));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, ZeroWeightsAndZeroCycle) {
    vector<vpii> g(4);
    g[0].push_back({1, 0});
    g[1].push_back({2, 0});
    g[2].push_back({1, 0});
    g[2].push_back({3, 0});
    ASSERT_TRUE(check(g, 0, 0));
    ASSERT_TRUE(check(g, 2, 5));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, MixedZeroAndPositive) {
    vector<vpii> g(5);
    g[0].push_back({1, 2});
    g[1].push_back({2, 0});
    g[2].push_back({3, 2});
    g[3].push_back({4, 0});
    g[1].push_back({4, 2});
    ASSERT_TRUE(check(g, 0, 2));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, LaterPathImprovesEarlierEstimate) {
    vector<vpii> g(3);
    g[0].push_back({2, 6});
    g[0].push_back({1, 1});
    g[1].push_back({2, 1});
    ASSERT_TRUE(check(g, 0, 6));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, BothDirections) {
    vector<vpii> g(4);
    auto add = [&](int u, int v, int w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    };
    add(0, 1, 1);
    add(1, 2, 0);
    add(2, 3, 3);
    add(0, 3, 2);
    ASSERT_TRUE(check(g, 0, 3));
    ASSERT_TRUE(check(g, 3, 3));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, DisconnectedHeavyComponent) {
    vector<vpii> g(5);
    g[0].push_back({1, 2});
    g[3].push_back({4, 9});
    g[4].push_back({3, 1});
    ASSERT_TRUE(check(g, 0, 9));
    ASSERT_TRUE(check(g, 4, 9));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, SelfLoopDoesNotChangeDistance) {
    vector<vpii> g(3);
    g[0].push_back({0, 4});
    g[0].push_back({1, 2});
    g[1].push_back({1, 0});
    g[1].push_back({1, 4});
    g[1].push_back({2, 4});
    ASSERT_TRUE(check(g, 0, 4));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, PathOfLengthNMinusOne) {
    const int n = 20;
    const int k = 9;
    vector<vpii> g(n);
    for (int i = 0; i < n - 1; ++i) {
        g[i].push_back({i + 1, k});
    }
    ASSERT_TRUE(check(g, 0, k));
    ASSERT_TRUE(check(g, n - 1, k));
    g[n - 1].push_back({0, 0});
    ASSERT_TRUE(check(g, 0, k));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, DirectEdgeHeavierThanKStillStored) {
    vector<vpii> one(2);
    one[0].push_back({1, 5});
    ASSERT_TRUE(check(one, 0, 1));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, EdgeJustBelowSentinel) {
    vector<vpii> g(2);
    g[0].push_back({1, inf - 1});
    ASSERT_TRUE(check(g, 0, 1));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, HeavierThanKStopsTheWalk) {
    {
        SCOPED_TRACE("k=1, веса 1,5,1");
        vector<vpii> g(4);
        g[0].push_back({1, 1});
        g[1].push_back({2, 5});
        g[2].push_back({3, 1});
        auto original = g;
        EXPECT_EQ(bfs1k(g, 0, 1), vi({0, 1, 6, inf}));
        EXPECT_EQ(g, original);
    }
    {
        SCOPED_TRACE("k=0, ребро веса 1");
        vector<vpii> g(3);
        g[0].push_back({1, 1});
        g[1].push_back({2, 1});
        auto original = g;
        EXPECT_EQ(bfs1k(g, 0, 0), vi({0, 1, inf}));
        EXPECT_EQ(g, original);
    }
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, LoopLimitOverflowSkipsSearch) {
    const int k = 100000;
    const int n = 21476;
    ASSERT_GT((long long)k * (n - 1), (long long)INT_MAX);
    vector<vpii> g(n);
    g[0].push_back({1, 1});
    g[1].push_back({2, 1});
    auto original = g;
    vi want(n, inf);
    want[0] = 0;
    EXPECT_EQ(bfs1k(g, 0, k), want);
    EXPECT_EQ(g, original);
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, EdgeSumOverflowCrashes) {
    EXPECT_DEATH(
        {
            vector<vpii> g(3);
            g[0].push_back({1, 5});
            g[1].push_back({2, INT_MAX});
            bfs1k(g, 0, 5);
        },
        "__n < this->size");
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, NegativeEdgeLeavesStaleSuccessor) {
    vector<vpii> g(4);
    g[0].push_back({1, 3});
    g[0].push_back({2, 4});
    g[2].push_back({1, -2});
    g[1].push_back({3, 1});
    auto original = g;
    auto got = bfs1k(g, 0, 4);
    EXPECT_EQ(g, original);
    EXPECT_EQ(got, vi({0, 2, 4, 4}));
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, NegativeDistanceCrashes) {
    EXPECT_DEATH(
        {
            vector<vpii> g(2);
            g[0].push_back({1, -1});
            bfs1k(g, 0, 1);
        },
        "__n < this->size");
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, StressWeightsWithinK) {
    std::mt19937 rng(1);
    for (int n = 1; n <= 25; ++n) {
        for (int k = 0; k <= 6; ++k) {
            int m = n * 4;
            auto g = random_digraph(n, m, 0, k, rng);
            for (int s = 0; s < n; s += std::max(1, n / 3)) {
                SCOPED_TRACE("n=" + std::to_string(n) + " k=" + std::to_string(k) + " s=" + std::to_string(s));
                ASSERT_TRUE(check(g, s, k));
            }
        }
    }
}

TEST(a_002_Grafy_Kratchayshie_puti_1_k_BFS, Load) {
    {
        SCOPED_TRACE("путь из 100000 рёбер веса 1");
        const int n = 100000;
        vector<vpii> g(n);
        for (int i = 0; i < n - 1; ++i) {
            g[i].push_back({i + 1, 1});
        }
        ASSERT_TRUE(check(g, 0, 1));
    }
    {
        SCOPED_TRACE("случайный ориентированный граф");
        std::mt19937 rng(2);
        const int n = 1500;
        const int k = 6;
        auto g = random_digraph(n, 20000, 0, k, rng);
        ASSERT_TRUE(check(g, 0, k));
        ASSERT_TRUE(check(g, n / 2, k));
        ASSERT_TRUE(check(g, n - 1, k));
    }
}
