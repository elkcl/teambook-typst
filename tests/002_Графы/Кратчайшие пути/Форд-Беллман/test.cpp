#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/Кратчайшие пути/Форд-Беллман/code.hpp"

const long long K_UNREACH = (long long)4e18;

struct Reference {
    bool negative_cycle = false;
    std::vector<long long> dist;
};

Reference reference(const vector<vpii> &g, int s) {
    int n = (int)g.size();
    Reference out;
    out.dist.assign(n, K_UNREACH);
    if (s < 0 || s >= n) {
        return out;
    }
    out.dist[s] = 0;
    for (int iter = 0; iter < n; ++iter) {
        bool relaxed = false;
        for (int v = 0; v < n; ++v) {
            if (out.dist[v] >= K_UNREACH / 2) {
                continue;
            }
            for (auto [to, w] : g[v]) {
                long long nd = out.dist[v] + (long long)w;
                if (nd < out.dist[to]) {
                    out.dist[to] = nd;
                    relaxed = true;
                }
            }
        }
        if (!relaxed) {
            return out;
        }
    }
    out.negative_cycle = true;
    return out;
}

bool expect_result(const vi &got, const Reference &exp) {
    if (exp.negative_cycle) {
        if (got != vi(got.size(), -1)) {
            std::ostringstream text;
            text << "ожидался вектор из -1, получено";
            int shown = std::min<int>(got.size(), 8);
            for (int i = 0; i < shown; ++i) {
                text << (i == 0 ? " " : ", ") << got[i];
            }
            if ((int)got.size() > shown) {
                text << ", ...";
            }
            ADD_FAILURE() << text.str();
            return false;
        }
        return true;
    }
    if (got.size() != exp.dist.size()) {
        ADD_FAILURE() << "размер " << got.size() << " вместо " << exp.dist.size();
        return false;
    }
    for (int i = 0; i < (int)got.size(); ++i) {
        long long want = exp.dist[i] >= K_UNREACH / 2 ? INF : exp.dist[i];
        if ((long long)got[i] != want) {
            ADD_FAILURE() << "d[" << i << "] = " << got[i] << ", ожидалось " << want;
            return false;
        }
    }
    return true;
}

void check(vector<vpii> g, int s) {
    expect_result(ford_bellman(g, s), reference(g, s));
}

vector<vpii> random_digraph(int n, int m, int wmin, int wmax, std::mt19937 &rng) {
    vector<vpii> g(n);
    if (n == 0) {
        return g;
    }
    std::uniform_int_distribution<int> vertex(0, n - 1);
    std::uniform_int_distribution<int> weight(wmin, wmax);
    for (int i = 0; i < m; ++i) {
        g[vertex(rng)].push_back({vertex(rng), weight(rng)});
    }
    return g;
}

}  // namespace

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, SingleVertex) {
    check(vector<vpii>(1), 0);
    vector<vpii> positive(1);
    positive[0].push_back({0, 5});
    positive[0].push_back({0, 0});
    check(positive, 0);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, NegativeSelfLoop) {
    vector<vpii> g(1);
    g[0].push_back({0, -1});
    check(g, 0);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, NoEdges) {
    check(vector<vpii>(5), 0);
    check(vector<vpii>(5), 4);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, DirectedEdgeAndSourceNotZero) {
    vector<vpii> g(3);
    g[2].push_back({0, 4});
    g[0].push_back({2, 100});
    check(g, 2);
    check(g, 0);
    check(g, 1);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, ParallelEdgesTakeMinimum) {
    vector<vpii> g(3);
    g[0].push_back({1, 9});
    g[0].push_back({1, 2});
    g[0].push_back({1, 4});
    g[1].push_back({2, 0});
    g[1].push_back({2, 3});
    check(g, 0);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, ZeroWeightsAndZeroCycle) {
    vector<vpii> g(3);
    g[0].push_back({1, 0});
    g[1].push_back({2, 0});
    g[2].push_back({1, 0});
    check(g, 0);
    check(g, 2);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, PositiveCycleIsNotNegative) {
    vector<vpii> g(4);
    for (int i = 0; i < 4; ++i) {
        g[i].push_back({(i + 1) % 4, i + 1});
    }
    check(g, 0);
    check(g, 2);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, NegativeEdgesNoCycle) {
    vector<vpii> g(4);
    g[0].push_back({1, -5});
    g[1].push_back({2, 4});
    g[0].push_back({2, 100});
    g[2].push_back({3, -2});
    g[1].push_back({0, 9});
    check(g, 0);
    check(g, 1);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, LongNegativePathIsExact) {
    vector<vpii> g(4);
    g[0].push_back({1, -600000000});
    g[1].push_back({2, -600000000});
    g[2].push_back({3, 1});
    check(g, 0);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, PathOfLengthNMinusOne) {
    for (int n : {2, 3, 5, 17, 64}) {
        vector<vpii> g(n);
        for (int i = 0; i + 1 < n; ++i) {
            g[i].push_back({i + 1, -3});
        }
        check(g, 0);
        check(g, n - 1);
    }
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, NegativeCycleShapes) {
    vector<vpii> loop(4);
    loop[1].push_back({1, -1});
    loop[0].push_back({1, 5});
    loop[1].push_back({2, 3});
    check(loop, 0);

    vector<vpii> pair(3);
    pair[0].push_back({1, 1});
    pair[1].push_back({2, -4});
    pair[2].push_back({1, 1});
    check(pair, 0);

    vector<vpii> full(6);
    for (int i = 0; i < 6; ++i) {
        full[i].push_back({(i + 1) % 6, -1});
    }
    check(full, 3);

    vector<vpii> behind(5);
    behind[0].push_back({1, 2});
    behind[1].push_back({2, 2});
    behind[2].push_back({3, -3});
    behind[3].push_back({2, 1});
    behind[4].push_back({4, -100});
    check(behind, 0);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, UnreachableNegativeCycleStaysFinite) {
    vector<vpii> g(4);
    g[0].push_back({1, -2});
    g[2].push_back({3, 1});
    g[3].push_back({2, -3});
    check(g, 0);
    check(g, 1);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, UnreachableHeavyEdgeDoesNotCorrupt) {
    vector<vpii> g(4);
    g[0].push_back({1, 7});
    g[2].push_back({1, 2000000000});
    g[3].push_back({1, -2000000000});
    check(g, 0);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, EdgeJustBelowSentinel) {
    vector<vpii> g(2);
    g[0].push_back({1, INF - 1});
    check(g, 0);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, PathSumJustBelowSentinel) {
    vector<vpii> g(3);
    g[0].push_back({1, INF / 2});
    g[1].push_back({2, INF / 2 - 1});
    check(g, 0);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, PathSumBelowSentinelStillExact) {
    vector<vpii> g(3);
    g[0].push_back({1, 400000000});
    g[1].push_back({2, 400000000});
    check(g, 0);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, HeavyNegativeCycleMustBeReported) {
    vector<vpii> loop(2);
    loop[0].push_back({0, -INF / 2});
    check(loop, 0);

    vector<vpii> pair(2);
    pair[0].push_back({1, -INF / 2});
    pair[1].push_back({0, -INF / 2});
    check(pair, 0);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, StressMixedWeights) {
    std::mt19937 rng(801);
    int cycles = 0;
    int finite = 0;
    for (int it = 0; it < 60; ++it) {
        int n = 1 + (int)(rng() % 15);
        int m = (int)(rng() % 30);
        auto g = random_digraph(n, m, -8, 12, rng);
        int s = (int)(rng() % n);
        auto exp = reference(g, s);
        cycles += exp.negative_cycle;
        finite += !exp.negative_cycle;
        expect_result(ford_bellman(g, s), exp);
    }
    EXPECT_GT(cycles, 0);
    EXPECT_GT(finite, 0);
}

TEST(a_002_Grafy_Kratchayshie_puti_Ford_Bellman, Load) {
    std::mt19937 rng(802);
    auto sparse = random_digraph(300, 2500, 0, 20, rng);
    check(sparse, 0);
    check(sparse, 150);
    check(sparse, 299);

    vector<vpii> path(2000);
    for (int i = 0; i + 1 < 2000; ++i) {
        path[i].push_back({i + 1, (i % 2 == 0) ? 1 : -1});
    }
    check(path, 0);

    vector<vpii> cycled(250);
    for (int i = 0; i < 250; ++i) {
        cycled[i].push_back({(i + 1) % 250, 2});
    }
    cycled[10].push_back({10, -1});
    check(cycled, 0);
    check(cycled, 200);
}
