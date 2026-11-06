#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/Эйлеров путь/code.hpp"

struct Graph {
    int n = 0;
    std::vector<std::pair<int, int>> edges;
    vvi adj;

    explicit Graph(int n) : n(n), adj(n) {}

    void add(int u, int v) {
        edges.push_back({u, v});
        adj[u].push_back(v);
        if (u != v) {
            adj[v].push_back(u);
        }
    }
};

vi degrees(const Graph &g) {
    vi deg(g.n, 0);
    for (auto [u, v] : g.edges) {
        if (u == v) {
            deg[u] += 2;
        } else {
            deg[u]++;
            deg[v]++;
        }
    }
    return deg;
}

bool euler_exists(const Graph &g) {
    vi deg = degrees(g);
    int odd = 0;
    int positive = 0;
    int start = -1;
    for (int v = 0; v < g.n; ++v) {
        if (deg[v] % 2 != 0) {
            ++odd;
        }
        if (deg[v] > 0) {
            ++positive;
            if (start == -1) {
                start = v;
            }
        }
    }
    if (odd != 0 && odd != 2) {
        return false;
    }
    if (positive == 0) {
        return true;
    }
    std::vector<char> seen(g.n, 0);
    std::queue<int> queue;
    queue.push(start);
    seen[start] = 1;
    int reached = 0;
    while (!queue.empty()) {
        int v = queue.front();
        queue.pop();
        if (deg[v] > 0) {
            ++reached;
        }
        for (int to : g.adj[v]) {
            if (to < 0 || to >= g.n || seen[to]) {
                continue;
            }
            seen[to] = 1;
            queue.push(to);
        }
    }
    return reached == positive;
}

std::string show_path(const optional<vi> &got) {
    if (!got.has_value()) {
        return "nullopt";
    }
    std::ostringstream out;
    int shown = std::min<int>(got->size(), 12);
    for (int i = 0; i < shown; ++i) {
        if (i != 0) {
            out << ' ';
        }
        out << (*got)[i];
    }
    if ((int)got->size() > shown) {
        out << " ... (" << got->size() << ")";
    }
    return out.str();
}

bool path_covers(const Graph &g, const vi &path) {
    if (g.edges.empty()) {
        if (g.n == 0) {
            if (!path.empty()) {
                ADD_FAILURE() << "пустой граф, путь " << show_path(path);
                return false;
            }
            return true;
        }
        if (path.size() != 1 || path[0] < 0 || path[0] >= g.n) {
            ADD_FAILURE() << "граф без рёбер, путь " << show_path(path);
            return false;
        }
        return true;
    }
    if ((int)path.size() != (int)g.edges.size() + 1) {
        ADD_FAILURE() << "длина " << path.size() << ", рёбер " << g.edges.size();
        return false;
    }
    std::map<std::pair<int, int>, int> need;
    for (auto [u, v] : g.edges) {
        if (u > v) {
            std::swap(u, v);
        }
        need[{u, v}]++;
    }
    for (int i = 0; i + 1 < (int)path.size(); ++i) {
        int u = path[i];
        int v = path[i + 1];
        if (u < 0 || u >= g.n || v < 0 || v >= g.n) {
            ADD_FAILURE() << "вершина вне графа в пути " << show_path(path);
            return false;
        }
        if (u > v) {
            std::swap(u, v);
        }
        auto it = need.find({u, v});
        if (it == need.end() || it->second == 0) {
            ADD_FAILURE() << "лишнее ребро " << path[i] << "->" << path[i + 1];
            return false;
        }
        if (--it->second == 0) {
            need.erase(it);
        }
    }
    if (!need.empty()) {
        ADD_FAILURE() << "не пройдено рёбер " << need.size();
        return false;
    }
    vi deg = degrees(g);
    int odd = 0;
    int odds[2] = {-1, -1};
    for (int v = 0; v < g.n; ++v) {
        if (deg[v] % 2 != 0) {
            if (odd < 2) {
                odds[odd] = v;
            }
            ++odd;
        }
    }
    if (odd == 0 && path.front() != path.back()) {
        ADD_FAILURE() << "контур не замкнут: " << show_path(path);
        return false;
    }
    if (odd == 2) {
        int a = path.front();
        int b = path.back();
        if (!((a == odds[0] && b == odds[1]) || (a == odds[1] && b == odds[0]))) {
            ADD_FAILURE() << "концы " << a << " " << b << ", нечётные " << odds[0] << " " << odds[1];
            return false;
        }
    }
    return true;
}

bool check(Graph g) {
    vvi original = g.adj;
    auto got = find_euler_path(g.adj);
    EXPECT_EQ(g.adj, original);
    if (!euler_exists(g)) {
        if (got.has_value()) {
            ADD_FAILURE() << "эйлерова пути нет, вернулся " << show_path(got);
            return false;
        }
        return true;
    }
    if (!got.has_value()) {
        ADD_FAILURE() << "эйлеров путь есть, вернулся nullopt";
        return false;
    }
    return path_covers(g, *got);
}

Graph path_graph(int n) {
    Graph g(n);
    for (int i = 0; i + 1 < n; ++i) {
        g.add(i, i + 1);
    }
    return g;
}

Graph cycle_graph(int n) {
    Graph g(n);
    for (int i = 0; i < n; ++i) {
        g.add(i, (i + 1) % n);
    }
    return g;
}

}  // namespace

TEST(a_002_Grafy_Eylerov_put, NoEdges) {
    EXPECT_TRUE(check(Graph(0)));
    EXPECT_TRUE(check(Graph(1)));
    EXPECT_TRUE(check(Graph(4)));
}

TEST(a_002_Grafy_Eylerov_put, SingleEdge) {
    Graph g(2);
    g.add(0, 1);
    ASSERT_TRUE(check(g));
    Graph far(4);
    far.add(2, 3);
    ASSERT_TRUE(check(far));
}

TEST(a_002_Grafy_Eylerov_put, PathAndCycle) {
    for (int n = 2; n <= 12; ++n) {
        ASSERT_TRUE(check(path_graph(n))) << "path " << n;
    }
    for (int n = 3; n <= 12; ++n) {
        ASSERT_TRUE(check(cycle_graph(n))) << "cycle " << n;
    }
}

TEST(a_002_Grafy_Eylerov_put, IsolatesDoNotBreakAComponent) {
    Graph g(5);
    g.add(1, 2);
    g.add(2, 4);
    ASSERT_TRUE(check(g));
}

TEST(a_002_Grafy_Eylerov_put, DisconnectedEdges) {
    Graph two_edges(4);
    two_edges.add(0, 1);
    two_edges.add(2, 3);
    ASSERT_TRUE(check(two_edges));

    Graph two_cycles(6);
    for (int i = 0; i < 3; ++i) {
        two_cycles.add(i, (i + 1) % 3);
        two_cycles.add(3 + i, 3 + (i + 1) % 3);
    }
    ASSERT_TRUE(check(two_cycles));
}

TEST(a_002_Grafy_Eylerov_put, CycleWithPendantAndSharedVertex) {
    Graph pendant(4);
    pendant.add(0, 1);
    pendant.add(1, 2);
    pendant.add(2, 0);
    pendant.add(2, 3);
    ASSERT_TRUE(check(pendant));

    Graph bowtie(5);
    bowtie.add(0, 1);
    bowtie.add(1, 2);
    bowtie.add(2, 0);
    bowtie.add(0, 3);
    bowtie.add(3, 4);
    bowtie.add(4, 0);
    ASSERT_TRUE(check(bowtie));
}

TEST(a_002_Grafy_Eylerov_put, StarAndCliques) {
    for (int n = 2; n <= 8; ++n) {
        Graph star(n);
        for (int v = 1; v < n; ++v) {
            star.add(0, v);
        }
        ASSERT_TRUE(check(star)) << "star " << n;
    }
    for (int n = 2; n <= 7; ++n) {
        Graph clique(n);
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                clique.add(u, v);
            }
        }
        ASSERT_TRUE(check(clique)) << "K" << n;
    }
}

TEST(a_002_Grafy_Eylerov_put, SelfLoop) {
    Graph one(1);
    one.add(0, 0);
    EXPECT_TRUE(check(one));

    Graph with_edge(2);
    with_edge.add(0, 0);
    with_edge.add(0, 1);
    EXPECT_TRUE(check(with_edge));
}

TEST(a_002_Grafy_Eylerov_put, TwoSelfLoops) {
    Graph separate(2);
    separate.add(0, 0);
    separate.add(1, 1);
    EXPECT_TRUE(check(separate));

    Graph same(1);
    same.add(0, 0);
    same.add(0, 0);
    EXPECT_TRUE(check(same));
}

TEST(a_002_Grafy_Eylerov_put, ParallelEdges) {
    Graph two(2);
    two.add(0, 1);
    two.add(0, 1);
    EXPECT_TRUE(check(two));

    Graph three(2);
    three.add(0, 1);
    three.add(0, 1);
    three.add(0, 1);
    EXPECT_TRUE(check(three));

    Graph on_path(3);
    on_path.add(0, 1);
    on_path.add(0, 1);
    on_path.add(1, 2);
    EXPECT_TRUE(check(on_path));
}

TEST(a_002_Grafy_Eylerov_put, RepeatedCallAndShuffle) {
    Graph g(4);
    g.add(0, 1);
    g.add(1, 2);
    g.add(2, 3);
    g.add(3, 1);
    ASSERT_TRUE(check(g));
    ASSERT_TRUE(check(g));
    std::mt19937 rng(1);
    for (int it = 0; it < 20; ++it) {
        for (auto &row : g.adj) {
            std::shuffle(row.begin(), row.end(), rng);
        }
        ASSERT_TRUE(check(g));
    }
}

TEST(a_002_Grafy_Eylerov_put, ExhaustiveSimpleGraphs) {
    for (int n = 1; n <= 6; ++n) {
        std::vector<std::pair<int, int>> pairs;
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                pairs.push_back({u, v});
            }
        }
        int bits = (int)pairs.size();
        for (int mask = 1; mask < (1 << bits); ++mask) {
            Graph g(n);
            for (int bit = 0; bit < bits; ++bit) {
                if ((mask >> bit) & 1) {
                    g.add(pairs[bit].first, pairs[bit].second);
                }
            }
            ASSERT_TRUE(check(std::move(g))) << "n=" << n << " mask=" << mask;
        }
    }
}

TEST(a_002_Grafy_Eylerov_put, StressRandomSimple) {
    std::mt19937 rng(2);
    for (int n = 1; n <= 15; ++n) {
        for (int it = 0; it < 20; ++it) {
            Graph g(n);
            std::set<std::pair<int, int>> used;
            int m = (int)(rng() % (n * 2 + 1));
            std::uniform_int_distribution<int> vertex(0, n - 1);
            for (int edge = 0; edge < m; ++edge) {
                int u = vertex(rng);
                int v = vertex(rng);
                if (u > v) {
                    std::swap(u, v);
                }
                if (u == v || !used.insert({u, v}).second) {
                    continue;
                }
                g.add(u, v);
            }
            if (g.edges.empty()) {
                continue;
            }
            ASSERT_TRUE(check(std::move(g))) << "n=" << n;
        }
    }
}

TEST(a_002_Grafy_Eylerov_put, Load) {
    ASSERT_TRUE(check(path_graph(100000)));
    ASSERT_TRUE(check(cycle_graph(100000)));
}
