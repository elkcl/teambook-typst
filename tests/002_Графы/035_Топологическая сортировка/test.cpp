#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/035_Топологическая сортировка/code.hpp"

bool has_cycle(const vvi &g) {
    int n = (int)g.size();
    std::vector<char> color(n, 0);
    std::vector<std::pair<int, int>> stack;
    for (int start = 0; start < n; ++start) {
        if (color[start] != 0) {
            continue;
        }
        color[start] = 1;
        stack.push_back({start, 0});
        while (!stack.empty()) {
            int v = stack.back().first;
            int i = stack.back().second;
            if (i == (int)g[v].size()) {
                color[v] = 2;
                stack.pop_back();
                continue;
            }
            int to = g[v][i];
            stack.back().second = i + 1;
            if (to < 0 || to >= n) {
                return true;
            }
            if (color[to] == 1) {
                return true;
            }
            if (color[to] == 0) {
                color[to] = 1;
                stack.push_back({to, 0});
            }
        }
    }
    return false;
}

bool expect_order(const vvi &g, const vi &order) {
    int n = (int)g.size();
    if ((int)order.size() != n) {
        ADD_FAILURE() << "длина " << order.size() << " вместо " << n;
        return false;
    }
    vi pos(n, -1);
    for (int i = 0; i < n; ++i) {
        int v = order[i];
        if (v < 0 || v >= n || pos[v] != -1) {
            ADD_FAILURE() << "порядок не перестановка";
            return false;
        }
        pos[v] = i;
    }
    for (int u = 0; u < n; ++u) {
        for (int v : g[u]) {
            if (pos[u] >= pos[v]) {
                ADD_FAILURE() << "ребро " << u << "->" << v << " идёт назад";
                return false;
            }
        }
    }
    return true;
}

bool check(vvi g) {
    vvi original = g;
    auto got = topological_sort(g);
    EXPECT_EQ(g, original);
    if (has_cycle(original)) {
        if (got.has_value()) {
            ADD_FAILURE() << "в графе цикл, но порядок вернулся";
            return false;
        }
        return true;
    }
    if (!got.has_value()) {
        ADD_FAILURE() << "ациклический граф, вернулся nullopt";
        return false;
    }
    return expect_order(original, *got);
}

vvi random_dag(int n, int m, std::mt19937 &rng) {
    vi perm(n);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), rng);
    vvi g(n);
    if (n <= 1 || m <= 0) {
        return g;
    }
    std::uniform_int_distribution<int> pos(0, n - 1);
    for (int edge = 0; edge < m; ++edge) {
        int a = pos(rng);
        int b = pos(rng);
        if (a == b) {
            continue;
        }
        if (a > b) {
            std::swap(a, b);
        }
        g[perm[a]].push_back(perm[b]);
    }
    return g;
}

}  // namespace

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, EmptyGraph) {
    vvi g;
    ASSERT_TRUE(check(g));
    auto got = topological_sort(g);
    ASSERT_TRUE(got.has_value());
    EXPECT_TRUE(got->empty());
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, SingleVertex) {
    ASSERT_TRUE(check(vvi(1)));
    vvi loop(1);
    loop[0] = {0};
    ASSERT_TRUE(check(loop));
    loop[0] = {0, 0};
    ASSERT_TRUE(check(loop));
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, NoEdges) {
    for (int n = 0; n <= 8; ++n) {
        ASSERT_TRUE(check(vvi(n)));
    }
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, SingleEdge) {
    vvi forward(2);
    forward[0] = {1};
    ASSERT_TRUE(check(forward));
    auto got = topological_sort(forward);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, vi({0, 1}));

    vvi backward(2);
    backward[1] = {0};
    got = topological_sort(backward);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, vi({1, 0}));
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, MutualEdge) {
    vvi g(2);
    g[0] = {1};
    g[1] = {0};
    ASSERT_TRUE(check(g));
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, Chain) {
    for (int n = 1; n <= 20; ++n) {
        vvi g(n);
        for (int i = 0; i + 1 < n; ++i) {
            g[i].push_back(i + 1);
        }
        auto got = topological_sort(g);
        ASSERT_TRUE(got.has_value()) << "n=" << n;
        vi want(n);
        std::iota(want.begin(), want.end(), 0);
        EXPECT_EQ(*got, want);
    }
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, ReverseChain) {
    for (int n = 1; n <= 20; ++n) {
        vvi g(n);
        for (int i = n - 1; i > 0; --i) {
            g[i].push_back(i - 1);
        }
        auto got = topological_sort(g);
        ASSERT_TRUE(got.has_value()) << "n=" << n;
        vi want(n);
        std::iota(want.begin(), want.end(), 0);
        std::reverse(want.begin(), want.end());
        EXPECT_EQ(*got, want);
    }
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, DiamondAndSkipEdge) {
    vvi g(4);
    g[0] = {1, 2};
    g[1] = {3};
    g[2] = {3};
    ASSERT_TRUE(check(g));

    g[0].push_back(3);
    ASSERT_TRUE(check(g));
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, Disconnected) {
    vvi g(6);
    g[0] = {1};
    g[1] = {2};
    g[4] = {3};
    ASSERT_TRUE(check(g));
    g[5] = {5};
    ASSERT_TRUE(check(g));
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, CycleShapes) {
    vvi two(3);
    two[0] = {1};
    two[1] = {0};
    ASSERT_TRUE(check(two));

    vvi triangle(3);
    triangle[0] = {1};
    triangle[1] = {2};
    triangle[2] = {0};
    ASSERT_TRUE(check(triangle));

    vvi tail(4);
    tail[0] = {1};
    tail[1] = {2};
    tail[2] = {1, 3};
    ASSERT_TRUE(check(tail));

    vvi into(4);
    into[0] = {1};
    into[1] = {2};
    into[2] = {3};
    into[3] = {1};
    ASSERT_TRUE(check(into));
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, CycleAwayFromZero) {
    vvi g(5);
    g[0] = {1};
    g[1] = {0};
    g[3] = {4};
    g[4] = {3};
    ASSERT_TRUE(check(g));

    g[1].clear();
    ASSERT_TRUE(check(g));
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, ParallelEdges) {
    vvi g(3);
    g[0] = {1, 1, 1, 2};
    g[1] = {2, 2};
    ASSERT_TRUE(check(g));
    auto got = topological_sort(g);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(*got, vi({0, 1, 2}));
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, GraphIsPreserved) {
    vvi g(3);
    g[0] = {1, 2};
    g[1] = {2};
    g[2] = {0};
    vvi copy = g;
    auto got = topological_sort(g);
    EXPECT_EQ(g, copy);
    EXPECT_FALSE(got.has_value());
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, RepeatedCall) {
    vvi g(3);
    g[0] = {1};
    g[1] = {2};
    auto first = topological_sort(g);
    auto second = topological_sort(g);
    ASSERT_TRUE(first.has_value());
    ASSERT_TRUE(second.has_value());
    EXPECT_EQ(*first, *second);
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, ExhaustiveUpToFourVertices) {
    for (int n = 1; n <= 4; ++n) {
        int bits = n * n;
        for (int mask = 0; mask < (1 << bits); ++mask) {
            vvi g(n);
            for (int bit = 0; bit < bits; ++bit) {
                if ((mask >> bit) & 1) {
                    g[bit / n].push_back(bit % n);
                }
            }
            ASSERT_TRUE(check(g)) << "n=" << n << " mask=" << mask;
        }
    }
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, StressRandomDags) {
    std::mt19937 rng(1);
    for (int n = 1; n <= 30; ++n) {
        for (int it = 0; it < 15; ++it) {
            auto g = random_dag(n, n * 3, rng);
            ASSERT_TRUE(check(g)) << "n=" << n;
            if (n >= 2) {
                g[n - 1].push_back(0);
                g[0].push_back(n - 1);
                ASSERT_TRUE(check(g)) << "cycle n=" << n;
            }
        }
    }
}

TEST(a_002_Grafy_035_Topologicheskaya_sortirovka, Load) {
    {
        SCOPED_TRACE("цепь из 100000");
        const int n = 100000;
        vvi g(n);
        for (int i = 0; i + 1 < n; ++i) {
            g[i].push_back(i + 1);
        }
        auto got = topological_sort(g);
        ASSERT_TRUE(got.has_value());
        EXPECT_EQ(got->front(), 0);
        EXPECT_EQ(got->back(), n - 1);
        EXPECT_EQ((int)got->size(), n);
        for (int i = 0; i + 1 < n; ++i) {
            ASSERT_EQ((*got)[i], i);
        }
    }
    {
        SCOPED_TRACE("плотный ациклический граф");
        const int n = 400;
        vvi g(n);
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                g[u].push_back(v);
            }
        }
        auto got = topological_sort(g);
        ASSERT_TRUE(got.has_value());
        vi want(n);
        std::iota(want.begin(), want.end(), 0);
        EXPECT_EQ(*got, want);
    }
    {
        SCOPED_TRACE("цикл в конце длинной цепи");
        const int n = 100000;
        vvi g(n);
        for (int i = 0; i + 1 < n; ++i) {
            g[i].push_back(i + 1);
        }
        g[n - 1].push_back(n - 2);
        auto got = topological_sort(g);
        EXPECT_FALSE(got.has_value());
    }
}
