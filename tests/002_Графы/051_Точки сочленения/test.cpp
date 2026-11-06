#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/051_Точки сочленения/code.hpp"

int components_without(const vvi &g, int banned) {
    int n = (int)g.size();
    std::vector<char> seen(n, 0);
    int components = 0;
    for (int start = 0; start < n; ++start) {
        if (start == banned || seen[start]) {
            continue;
        }
        ++components;
        std::queue<int> queue;
        queue.push(start);
        seen[start] = 1;
        while (!queue.empty()) {
            int v = queue.front();
            queue.pop();
            for (int to : g[v]) {
                if (to == banned || to < 0 || to >= n || seen[to]) {
                    continue;
                }
                seen[to] = 1;
                queue.push(to);
            }
        }
    }
    return components;
}

vc reference(const vvi &g) {
    int n = (int)g.size();
    int base = components_without(g, -1);
    vc answer(n, 0);
    for (int v = 0; v < n; ++v) {
        if (components_without(g, v) > base) {
            answer[v] = 1;
        }
    }
    return answer;
}

bool expect_cuts(const vc &got, const vc &exp) {
    if (got.size() != exp.size()) {
        ADD_FAILURE() << "размер " << got.size() << " вместо " << exp.size();
        return false;
    }
    for (int v = 0; v < (int)got.size(); ++v) {
        if ((bool)got[v] != (bool)exp[v]) {
            ADD_FAILURE() << "вершина " << v << ": " << (int)(bool)got[v] << ", ожидалось "
                          << (int)(bool)exp[v];
            return false;
        }
    }
    return true;
}

bool check(vvi g) {
    vvi original = g;
    vc got = find_cutpoints(g);
    EXPECT_EQ(g, original);
    return expect_cuts(got, reference(original));
}

void add_edge(vvi &g, int u, int v) {
    g[u].push_back(v);
    if (u != v) {
        g[v].push_back(u);
    }
}

vvi path_graph(int n) {
    vvi g(n);
    for (int i = 0; i + 1 < n; ++i) {
        add_edge(g, i, i + 1);
    }
    return g;
}

vvi cycle_graph(int n) {
    vvi g(n);
    for (int i = 0; i < n; ++i) {
        add_edge(g, i, (i + 1) % n);
    }
    return g;
}

vvi star_graph(int n) {
    vvi g(n);
    for (int i = 1; i < n; ++i) {
        add_edge(g, 0, i);
    }
    return g;
}

vc tree_cuts_by_degree(const vvi &g) {
    vc answer(g.size(), 0);
    for (int v = 0; v < (int)g.size(); ++v) {
        if ((int)g[v].size() >= 2) {
            answer[v] = 1;
        }
    }
    return answer;
}

}  // namespace

TEST(a_002_Grafy_051_Tochki_sochleneniya, EmptyAndSingleVertex) {
    ASSERT_TRUE(check({}));
    ASSERT_TRUE(check(vvi(1)));
    vvi loop(1);
    loop[0] = {0, 0};
    ASSERT_TRUE(check(loop));
}

TEST(a_002_Grafy_051_Tochki_sochleneniya, SingleEdgeAndIsolated) {
    ASSERT_TRUE(check(path_graph(2)));
    vvi isolated(5);
    ASSERT_TRUE(check(isolated));
    add_edge(isolated, 1, 3);
    ASSERT_TRUE(check(isolated));
}

TEST(a_002_Grafy_051_Tochki_sochleneniya, Path) {
    for (int n = 1; n <= 12; ++n) {
        auto g = path_graph(n);
        ASSERT_TRUE(check(g)) << "n=" << n;
        EXPECT_EQ(find_cutpoints(g), tree_cuts_by_degree(g));
    }
}

TEST(a_002_Grafy_051_Tochki_sochleneniya, CycleHasNoCutpoints) {
    for (int n = 3; n <= 12; ++n) {
        ASSERT_TRUE(check(cycle_graph(n))) << "n=" << n;
    }
    vvi chord = cycle_graph(4);
    add_edge(chord, 0, 2);
    ASSERT_TRUE(check(chord));
}

TEST(a_002_Grafy_051_Tochki_sochleneniya, StarAndClique) {
    for (int n = 1; n <= 10; ++n) {
        ASSERT_TRUE(check(star_graph(n))) << "star " << n;
        vvi clique(n);
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                add_edge(clique, u, v);
            }
        }
        ASSERT_TRUE(check(clique)) << "clique " << n;
    }
}

TEST(a_002_Grafy_051_Tochki_sochleneniya, SharedVertexAndBridge) {
    vvi shared(5);
    add_edge(shared, 0, 1);
    add_edge(shared, 1, 2);
    add_edge(shared, 2, 0);
    add_edge(shared, 0, 3);
    add_edge(shared, 3, 4);
    add_edge(shared, 4, 0);
    ASSERT_TRUE(check(shared));

    vvi barbell(6);
    for (int u = 0; u < 3; ++u) {
        for (int v = u + 1; v < 3; ++v) {
            add_edge(barbell, u, v);
            add_edge(barbell, u + 3, v + 3);
        }
    }
    add_edge(barbell, 2, 3);
    ASSERT_TRUE(check(barbell));
}

TEST(a_002_Grafy_051_Tochki_sochleneniya, SecondComponentAndPendant) {
    vvi g(6);
    add_edge(g, 3, 4);
    add_edge(g, 4, 5);
    ASSERT_TRUE(check(g));

    vvi pendant(4);
    add_edge(pendant, 0, 1);
    add_edge(pendant, 1, 2);
    add_edge(pendant, 0, 2);
    add_edge(pendant, 2, 3);
    ASSERT_TRUE(check(pendant));
}

TEST(a_002_Grafy_051_Tochki_sochleneniya, ParallelEdgesAndSelfLoop) {
    vvi parallel(3);
    add_edge(parallel, 0, 1);
    add_edge(parallel, 0, 1);
    add_edge(parallel, 1, 2);
    add_edge(parallel, 1, 2);
    ASSERT_TRUE(check(parallel));

    vvi loop_on_leaf = path_graph(3);
    add_edge(loop_on_leaf, 0, 0);
    add_edge(loop_on_leaf, 2, 2);
    ASSERT_TRUE(check(loop_on_leaf));

    vvi only_parallel(2);
    add_edge(only_parallel, 0, 1);
    add_edge(only_parallel, 0, 1);
    add_edge(only_parallel, 0, 1);
    ASSERT_TRUE(check(only_parallel));
}

TEST(a_002_Grafy_051_Tochki_sochleneniya, AdjacencyOrderDoesNotMatter) {
    vvi g(6);
    add_edge(g, 0, 1);
    add_edge(g, 1, 2);
    add_edge(g, 2, 3);
    add_edge(g, 1, 4);
    add_edge(g, 4, 5);
    add_edge(g, 5, 1);
    vc exp = reference(g);
    std::mt19937 rng(1);
    for (int it = 0; it < 40; ++it) {
        for (auto &row : g) {
            std::shuffle(row.begin(), row.end(), rng);
        }
        EXPECT_EQ(find_cutpoints(g), exp);
    }
}

TEST(a_002_Grafy_051_Tochki_sochleneniya, GraphIsPreserved) {
    vvi g = cycle_graph(5);
    add_edge(g, 0, 2);
    vvi copy = g;
    find_cutpoints(g);
    EXPECT_EQ(g, copy);
}

TEST(a_002_Grafy_051_Tochki_sochleneniya, ExhaustiveSimpleGraphs) {
    for (int n = 0; n <= 6; ++n) {
        std::vector<std::pair<int, int>> pairs;
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                pairs.push_back({u, v});
            }
        }
        int bits = (int)pairs.size();
        for (int mask = 0; mask < (1 << bits); ++mask) {
            vvi g(n);
            for (int bit = 0; bit < bits; ++bit) {
                if ((mask >> bit) & 1) {
                    add_edge(g, pairs[bit].first, pairs[bit].second);
                }
            }
            ASSERT_TRUE(check(g)) << "n=" << n << " mask=" << mask;
        }
    }
}

TEST(a_002_Grafy_051_Tochki_sochleneniya, StressRandom) {
    std::mt19937 rng(2);
    for (int n = 1; n <= 20; ++n) {
        for (int it = 0; it < 20; ++it) {
            vvi g(n);
            int m = (int)(rng() % (n * 3 + 1));
            std::uniform_int_distribution<int> vertex(0, n - 1);
            for (int edge = 0; edge < m; ++edge) {
                add_edge(g, vertex(rng), vertex(rng));
            }
            ASSERT_TRUE(check(g)) << "n=" << n;
        }
    }
}

TEST(a_002_Grafy_051_Tochki_sochleneniya, LoadShallow) {
    {
        SCOPED_TRACE("звезда");
        const int n = 100000;
        auto g = star_graph(n);
        EXPECT_EQ(find_cutpoints(g), tree_cuts_by_degree(g));
    }
    {
        SCOPED_TRACE("куча-дерево");
        const int n = 100000;
        vvi g(n);
        for (int v = 1; v < n; ++v) {
            add_edge(g, (v - 1) / 2, v);
        }
        EXPECT_EQ(find_cutpoints(g), tree_cuts_by_degree(g));
    }
    {
        SCOPED_TRACE("путь из 50000");
        const int n = 50000;
        auto g = path_graph(n);
        EXPECT_EQ(find_cutpoints(g), tree_cuts_by_degree(g));
    }
}

TEST(a_002_Grafy_051_Tochki_sochleneniya, LoadLongPath) {
    const int n = 100000;
    auto g = path_graph(n);
    EXPECT_EQ(find_cutpoints(g), tree_cuts_by_degree(g));
}
