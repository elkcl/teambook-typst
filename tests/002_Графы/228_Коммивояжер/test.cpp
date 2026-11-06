#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/228_Коммивояжер/code.hpp"

vvll matrix(int n, ll fill = -1) { return vvll(n, vll(n, fill)); }

ll brute(const vvll &g) {
    int n = (int)g.size();
    if (n == 0) {
        return INF_LL;
    }
    vi order(n);
    iota(order.begin(), order.end(), 0);
    ll best = INF_LL;
    do {
        ll cost = 0;
        bool ok = true;
        for (int i = 0; i + 1 < n; ++i) {
            ll weight = g[order[i]][order[i + 1]];
            if (weight == -1) {
                ok = false;
                break;
            }
            cost += weight;
        }
        if (ok) {
            best = min(best, cost);
        }
    } while (next_permutation(order.begin(), order.end()));
    return best;
}

void expect_tsp(const vvll &g) { EXPECT_EQ(tsp(g), brute(g)); }

}  // namespace

TEST(a_002_Grafy_228_Kommivoyazher, EmptyAndSingleVertex) {
    EXPECT_EQ(tsp({}), INF_LL);
    EXPECT_EQ(tsp(matrix(1)), 0);
    vvll loop = matrix(1);
    loop[0][0] = -20;
    EXPECT_EQ(tsp(loop), 0);
}

TEST(a_002_Grafy_228_Kommivoyazher, DirectedEdge) {
    vvll g = matrix(2);
    g[0][1] = 5;
    EXPECT_EQ(tsp(g), 5);
    g[1][0] = 1;
    EXPECT_EQ(tsp(g), 1);
    g[0][1] = -4;
    EXPECT_EQ(tsp(g), -4);
}

TEST(a_002_Grafy_228_Kommivoyazher, OpenPathNotClosedTour) {
    vvll g = matrix(3);
    g[0][1] = 3;
    g[1][2] = 4;
    g[2][0] = 100;
    EXPECT_EQ(tsp(g), 7);
}

TEST(a_002_Grafy_228_Kommivoyazher, BestPathMayStartAwayFromZero) {
    vvll g = matrix(3);
    g[1][2] = 2;
    g[2][0] = 2;
    g[0][1] = 50;
    g[0][2] = 50;
    EXPECT_EQ(tsp(g), 4);
}

TEST(a_002_Grafy_228_Kommivoyazher, WeightMinusOneIsAMissingEdge) {
    vvll blocked = matrix(3);
    blocked[0][1] = -1;
    blocked[1][2] = 4;
    EXPECT_EQ(tsp(blocked), INF_LL);

    vvll real = matrix(3);
    real[0][1] = -2;
    real[1][2] = 3;
    EXPECT_EQ(tsp(real), 1);
}

TEST(a_002_Grafy_228_Kommivoyazher, ZeroWeightsAndSelfLoops) {
    vvll g = matrix(3, 0);
    g[0][0] = -100;
    g[1][1] = 100;
    g[2][2] = -100;
    EXPECT_EQ(tsp(g), 0);
}

TEST(a_002_Grafy_228_Kommivoyazher, NoHamiltonianPath) {
    EXPECT_EQ(tsp(matrix(4)), INF_LL);
    vvll split = matrix(4);
    split[0][1] = 1;
    split[1][0] = 1;
    split[2][3] = 1;
    split[3][2] = 1;
    EXPECT_EQ(tsp(split), INF_LL);
}

TEST(a_002_Grafy_228_Kommivoyazher, OnlyForwardPath) {
    vvll g = matrix(5);
    for (int i = 0; i + 1 < 5; ++i) {
        g[i][i + 1] = i + 1;
    }
    EXPECT_EQ(tsp(g), 10);
}

TEST(a_002_Grafy_228_Kommivoyazher, CompleteGraphMatchesBrute) {
    vvll g = matrix(4);
    g[0][1] = 8;
    g[0][2] = 3;
    g[0][3] = 6;
    g[1][0] = 1;
    g[1][2] = 4;
    g[1][3] = 9;
    g[2][0] = 2;
    g[2][1] = -3;
    g[2][3] = 5;
    g[3][0] = 7;
    g[3][1] = 1;
    g[3][2] = 2;
    expect_tsp(g);
}

TEST(a_002_Grafy_228_Kommivoyazher, RandomNonNegative) {
    std::mt19937 rng(2202);
    std::uniform_int_distribution<int> coin(0, 99);
    std::uniform_int_distribution<int> weight(0, 15);
    for (int n : {2, 3, 4, 5, 6, 7}) {
        for (int attempt = 0; attempt < 6; ++attempt) {
            vvll g = matrix(n);
            for (int u = 0; u < n; ++u) {
                for (int v = 0; v < n; ++v) {
                    if (u != v && coin(rng) < 70) {
                        g[u][v] = weight(rng);
                    }
                }
            }
            expect_tsp(g);
        }
    }
}

TEST(a_002_Grafy_228_Kommivoyazher, RandomCompleteWithNegatives) {
    std::mt19937 rng(11);
    std::uniform_int_distribution<int> weight(-6, 9);
    for (int attempt = 0; attempt < 8; ++attempt) {
        int n = 6;
        vvll g = matrix(n, 0);
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                if (u != v) {
                    g[u][v] = weight(rng);
                }
            }
        }
        expect_tsp(g);
    }
}

TEST(a_002_Grafy_228_Kommivoyazher, LoadPath) {
    const int n = 16;
    vvll g = matrix(n);
    ll want = 0;
    for (int i = 0; i + 1 < n; ++i) {
        g[i][i + 1] = 1;
        want += 1;
    }
    EXPECT_EQ(tsp(g), want);
}
