#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/020_Кратчайшие пути/050_Флойд/code.hpp"

const long long K_UNREACH = (long long)4e18;

std::vector<std::vector<long long>> reference(const vector<vpii> &g) {
    int n = (int)g.size();
    std::vector<std::vector<long long>> d(n, std::vector<long long>(n, K_UNREACH));
    for (int i = 0; i < n; ++i) {
        d[i][i] = 0;
        for (auto [j, w] : g[i]) {
            d[i][j] = std::min(d[i][j], (long long)w);
        }
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            if (d[i][k] >= K_UNREACH / 2) {
                continue;
            }
            for (int j = 0; j < n; ++j) {
                if (d[k][j] >= K_UNREACH / 2) {
                    continue;
                }
                d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
            }
        }
    }
    return d;
}

bool has_negative_cycle(const std::vector<std::vector<long long>> &d) {
    for (int i = 0; i < (int)d.size(); ++i) {
        if (d[i][i] < 0) {
            return true;
        }
    }
    return false;
}

bool expect_matrix(const vvi &got, const std::vector<std::vector<long long>> &exp) {
    if (got.size() != exp.size()) {
        ADD_FAILURE() << "n = " << got.size() << ", ожидалось " << exp.size();
        return false;
    }
    for (int i = 0; i < (int)exp.size(); ++i) {
        if (got[i].size() != exp[i].size()) {
            ADD_FAILURE() << "строка " << i;
            return false;
        }
        for (int j = 0; j < (int)exp[i].size(); ++j) {
            long long want = exp[i][j] >= K_UNREACH / 2 ? INF : exp[i][j];
            if ((long long)got[i][j] != want) {
                ADD_FAILURE() << "dist[" << i << "][" << j << "] = " << got[i][j] << ", ожидалось " << want;
                return false;
            }
        }
    }
    return true;
}

void check(vector<vpii> g) {
    auto exp = reference(g);
    ASSERT_FALSE(has_negative_cycle(exp));
    expect_matrix(floyd(g), exp);
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

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, EmptyAndSingleVertex) {
    check({});
    check(vector<vpii>(1));
    vector<vpii> loop(1);
    loop[0].push_back({0, 5});
    loop[0].push_back({0, 0});
    check(loop);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, NoEdges) {
    check(vector<vpii>(4));
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, DirectedEdge) {
    vector<vpii> g(2);
    g[0].push_back({1, 4});
    check(g);
    g[1].push_back({0, 7});
    check(g);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, ParallelEdges) {
    vector<vpii> g(3);
    g[0].push_back({1, 9});
    g[0].push_back({1, 2});
    g[0].push_back({1, 2});
    g[0].push_back({1, 5});
    g[1].push_back({2, 0});
    g[1].push_back({2, 3});
    check(g);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, PathBeatsDirectEdge) {
    vector<vpii> g(3);
    g[0].push_back({2, 10});
    g[0].push_back({1, 2});
    g[1].push_back({2, 3});
    check(g);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, DirectEdgeBeatsPath) {
    vector<vpii> g(3);
    g[0].push_back({2, 1});
    g[0].push_back({1, 4});
    g[1].push_back({2, 4});
    check(g);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, ZeroCycleDoesNotChangeDiagonal) {
    vector<vpii> g(3);
    g[0].push_back({1, 0});
    g[1].push_back({2, 0});
    g[2].push_back({0, 0});
    check(g);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, PositiveCycleKeepsZeroDiagonal) {
    vector<vpii> g(4);
    for (int i = 0; i < 4; ++i) {
        g[i].push_back({(i + 1) % 4, i + 1});
    }
    check(g);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, DisconnectedComponents) {
    vector<vpii> g(5);
    g[0].push_back({1, 3});
    g[1].push_back({0, 4});
    g[3].push_back({4, 1});
    check(g);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, NegativeEdgesNoCycle) {
    vector<vpii> g(4);
    g[0].push_back({1, -5});
    g[1].push_back({2, 4});
    g[0].push_back({2, 100});
    g[2].push_back({3, -2});
    g[1].push_back({0, 9});
    check(g);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, LongNegativePathIsNotClamped) {
    vector<vpii> g(4);
    g[0].push_back({1, -(INF / 2 - 1)});
    g[1].push_back({2, -(INF / 2 - 1)});
    g[2].push_back({3, 1});
    check(g);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, NegativeSumJustInsideSentinel) {
    vector<vpii> g(3);
    g[0].push_back({1, -INF / 2});
    g[1].push_back({2, -(INF / 2 - 1)});
    check(g);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, HeavyDetourMustNotBeatShortEdge) {
    vector<vpii> g(3);
    g[0].push_back({2, 10});
    g[0].push_back({1, INF / 2 - 1});
    g[1].push_back({2, INF / 2 - 1});
    check(g);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, SumOfLargePositiveEdges) {
    vector<vpii> g(3);
    g[0].push_back({1, INF / 2});
    g[1].push_back({2, INF / 2 - 1});
    check(g);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, SelfLoopDoesNotSpoilOtherPairs) {
    vector<vpii> g(3);
    g[1].push_back({1, 8});
    g[0].push_back({1, 2});
    g[1].push_back({2, 2});
    check(g);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, StressNonNegative) {
    std::mt19937 rng(501);
    for (int it = 0; it < 25; ++it) {
        int n = 1 + (int)(rng() % 20);
        int m = (int)(rng() % 40);
        check(random_digraph(n, m, 0, 30, rng));
    }
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, StressSmallNegativeNoCycle) {
    std::mt19937 rng(502);
    int accepted = 0;
    for (int it = 0; it < 200 && accepted < 30; ++it) {
        int n = 2 + (int)(rng() % 12);
        int m = (int)(rng() % 25);
        auto g = random_digraph(n, m, -15, 20, rng);
        auto exp = reference(g);
        if (has_negative_cycle(exp)) {
            continue;
        }
        ++accepted;
        expect_matrix(floyd(g), exp);
    }
    EXPECT_GE(accepted, 30);
}

TEST(a_002_Grafy_020_Kratchayshie_puti_050_Floyd, Load) {
    std::mt19937 rng(503);
    check(random_digraph(80, 400, 0, 20, rng));
    check(random_digraph(60, 2000, 0, 9, rng));
    vector<vpii> path(40);
    for (int i = 0; i + 1 < 40; ++i) {
        path[i].push_back({i + 1, 3});
    }
    check(path);
}
