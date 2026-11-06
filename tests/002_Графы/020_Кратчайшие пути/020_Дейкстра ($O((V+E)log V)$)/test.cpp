#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/020_Кратчайшие пути/020_Дейкстра ($O((V+E)log V)$)/code.hpp"

const long long K_UNREACH = (long long)4e18;

std::vector<long long> reference(const vector<vpii> &g, int s) {
    int n = (int)g.size();
    std::vector<long long> d(n, K_UNREACH);
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
        long long want = exp[i] >= K_UNREACH / 2 ? INF : exp[i];
        if ((long long)got[i] != want) {
            ADD_FAILURE() << "d[" << i << "] = " << got[i] << ", ожидалось " << want;
            return false;
        }
    }
    return true;
}

void check(vector<vpii> g, int s) {
    auto exp = reference(g, s);
    {
        SCOPED_TRACE("priority_queue");
        auto got = dijkstra(g, (int)g.size(), s);
        expect_dist(got, exp);
    }
    {
        SCOPED_TRACE("set");
        auto got = set_dijkstra(g, (int)g.size(), s);
        expect_dist(got, exp);
    }
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

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, SingleVertex) {
    check(vector<vpii>(1), 0);
    vector<vpii> loop(1);
    loop[0].push_back({0, 0});
    loop[0].push_back({0, 5});
    check(loop, 0);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, NoEdges) {
    check(vector<vpii>(5), 0);
    check(vector<vpii>(5), 3);
    check(vector<vpii>(5), 4);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, SingleEdgeBothDirections) {
    vector<vpii> g(2);
    g[0].push_back({1, 3});
    g[1].push_back({0, 100});
    check(g, 0);
    check(g, 1);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, DisconnectedAndSourceNotZero) {
    vector<vpii> g(6);
    g[1].push_back({2, 4});
    g[2].push_back({4, 1});
    g[5].push_back({5, 0});
    g[0].push_back({0, 7});
    check(g, 1);
    check(g, 0);
    check(g, 5);
    check(g, 3);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, ParallelEdgesTakeMinimum) {
    vector<vpii> g(3);
    g[0].push_back({1, 10});
    g[0].push_back({1, 3});
    g[0].push_back({1, 3});
    g[0].push_back({1, 8});
    g[1].push_back({2, 0});
    g[1].push_back({2, 4});
    check(g, 0);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, ZeroWeightsAndZeroCycle) {
    vector<vpii> g(4);
    g[0].push_back({1, 0});
    g[1].push_back({2, 0});
    g[2].push_back({1, 0});
    g[2].push_back({3, 0});
    g[3].push_back({3, 0});
    check(g, 0);
    check(g, 2);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, DecreaseKey) {
    vector<vpii> g(4);
    g[0].push_back({1, 5});
    g[0].push_back({2, 2});
    g[2].push_back({1, 1});
    g[1].push_back({3, 10});
    g[2].push_back({3, 4});
    g[0].push_back({3, 100});
    check(g, 0);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, EqualDistances) {
    vector<vpii> g(4);
    g[0].push_back({1, 2});
    g[0].push_back({2, 2});
    g[1].push_back({3, 2});
    g[2].push_back({3, 2});
    check(g, 0);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, UnreachableVertexWithOutgoingEdges) {
    vector<vpii> g(4);
    g[0].push_back({1, 5});
    g[2].push_back({1, 1});
    g[2].push_back({3, 1});
    g[3].push_back({1, 1});
    check(g, 0);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, ChainStarAndCycle) {
    vector<vpii> chain(10);
    for (int i = 0; i < 9; ++i) {
        chain[i].push_back({i + 1, i == 0 ? 0 : i});
    }
    check(chain, 0);
    check(chain, 9);
    check(chain, 4);

    vector<vpii> star(12);
    for (int i = 1; i < 12; ++i) {
        star[0].push_back({i, i});
        star[i].push_back({0, 100});
    }
    check(star, 0);
    check(star, 7);

    vector<vpii> cycle(6);
    for (int i = 0; i < 6; ++i) {
        cycle[i].push_back({(i + 1) % 6, 1});
    }
    check(cycle, 0);
    check(cycle, 3);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, NegativeEdgeNoCycle) {
    vector<vpii> g(4);
    g[0].push_back({1, 5});
    g[0].push_back({2, 6});
    g[2].push_back({1, -4});
    g[1].push_back({3, 0});
    check(g, 0);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, HeavyDetourMustNotBeatShortEdge) {
    vector<vpii> g(3);
    g[0].push_back({2, 10});
    g[0].push_back({1, INF / 2 - 1});
    g[1].push_back({2, INF / 2 - 1});
    check(g, 0);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, DistanceNearSentinel) {
    vector<vpii> g(3);
    g[0].push_back({1, INF - 5});
    g[1].push_back({2, 0});
    check(g, 0);
    g[1].back().second = 4;
    check(g, 0);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, StressSmallWeights) {
    std::mt19937 rng(101);
    for (int it = 0; it < 40; ++it) {
        int n = 2 + (int)(rng() % 40);
        int m = (int)(rng() % 80);
        auto g = random_digraph(n, m, 0, 20, rng);
        int sources = 1 + (int)(rng() % std::min(n, 4));
        for (int k = 0; k < sources; ++k) {
            check(g, (int)(rng() % n));
        }
    }
}

TEST(a_002_Grafy_020_Kratchayshie_puti_020_Deykstra_O_V_E_log_V, Load) {
    std::mt19937 rng(202);
    auto sparse = random_digraph(5000, 30000, 0, 100, rng);
    check(sparse, 0);
    check(sparse, 1234);
    check(sparse, 4999);

    auto dense = random_digraph(400, 400 * 30, 0, 50, rng);
    check(dense, 0);
    check(dense, 200);

    vector<vpii> path(100000);
    for (int i = 0; i + 1 < 100000; ++i) {
        path[i].push_back({i + 1, 1});
    }
    check(path, 0);
}
