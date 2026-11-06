#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/050_Мосты/code.hpp"

struct Graph {
    int n = 0;
    std::vector<std::pair<int, int>> edges;
    std::vector<std::vector<std::pair<int, int>>> adj;

    explicit Graph(int n) : n(n), adj(n) {}

    void add(int u, int v) {
        int id = (int)edges.size();
        edges.push_back({u, v});
        adj[u].push_back({v, id});
        if (u != v) {
            adj[v].push_back({u, id});
        }
    }

    vvi code_graph() const {
        vvi g(n);
        for (int v = 0; v < n; ++v) {
            for (auto [to, id] : adj[v]) {
                (void)id;
                g[v].push_back(to);
            }
        }
        return g;
    }
};

std::vector<std::pair<int, int>> normalize(std::vector<std::pair<int, int>> edges) {
    for (auto &e : edges) {
        if (e.first > e.second) {
            std::swap(e.first, e.second);
        }
    }
    std::sort(edges.begin(), edges.end());
    return edges;
}

std::vector<std::pair<int, int>> reference_bridges(const Graph &gr) {
    std::vector<std::pair<int, int>> ans;
    for (int id = 0; id < (int)gr.edges.size(); ++id) {
        auto [a, b] = gr.edges[id];
        if (a == b) {
            continue;
        }
        std::vector<char> seen(gr.n, 0);
        std::queue<int> q;
        q.push(a);
        seen[a] = 1;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto [to, eid] : gr.adj[v]) {
                if (eid == id || seen[to]) {
                    continue;
                }
                seen[to] = 1;
                q.push(to);
            }
        }
        if (!seen[b]) {
            if (a > b) {
                std::swap(a, b);
            }
            ans.push_back({a, b});
        }
    }
    std::sort(ans.begin(), ans.end());
    return ans;
}

std::vector<std::pair<int, int>> run(const Graph &gr) {
    vvi g = gr.code_graph();
    vi h(gr.n), d(gr.n);
    vc used(gr.n, false);
    vpii bridges;
    for (int v = 0; v < gr.n; ++v) {
        if (!used[v]) {
            find_bridges(g, h, d, used, bridges, v);
        }
    }
    return normalize(bridges);
}

void check(const Graph &gr) {
    auto got = run(gr);
    auto exp = reference_bridges(gr);
    if (got != exp) {
        ADD_FAILURE() << "мостов " << got.size() << ", ожидалось " << exp.size();
    }
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

Graph tree_from_parent(const vi &parent) {
    Graph g((int)parent.size());
    for (int v = 1; v < (int)parent.size(); ++v) {
        g.add(parent[v], v);
    }
    return g;
}

}  // namespace

TEST(a_002_Grafy_050_Mosty, EmptyAndIsolated) {
    check(Graph(0));
    check(Graph(1));
    check(Graph(5));
}

TEST(a_002_Grafy_050_Mosty, SelfLoopIsNotBridge) {
    Graph g(1);
    g.add(0, 0);
    check(g);
    Graph h(3);
    h.add(1, 1);
    h.add(1, 1);
    h.add(0, 1);
    h.add(1, 2);
    check(h);
}

TEST(a_002_Grafy_050_Mosty, SingleEdge) {
    Graph g(2);
    g.add(0, 1);
    check(g);
    Graph rev(2);
    rev.add(1, 0);
    check(rev);
}


TEST(a_002_Grafy_050_Mosty, PathChainAndStar) {
    for (int n = 2; n <= 12; ++n) {
        check(path_graph(n));
        Graph star(n);
        for (int i = 1; i < n; ++i) {
            star.add(0, i);
        }
        check(star);
    }
}

TEST(a_002_Grafy_050_Mosty, CyclesHaveNoBridges) {
    for (int n = 3; n <= 12; ++n) {
        check(cycle_graph(n));
    }
    Graph chord(4);
    for (int i = 0; i < 4; ++i) {
        chord.add(i, (i + 1) % 4);
    }
    chord.add(0, 2);
    check(chord);
}

TEST(a_002_Grafy_050_Mosty, TwoCyclesSharingVertexOrEdge) {
    Graph vertex(5);
    vertex.add(0, 1);
    vertex.add(1, 2);
    vertex.add(2, 0);
    vertex.add(0, 3);
    vertex.add(3, 4);
    vertex.add(4, 0);
    check(vertex);

    Graph edge(4);
    edge.add(0, 1);
    edge.add(1, 2);
    edge.add(2, 0);
    edge.add(1, 3);
    edge.add(3, 2);
    check(edge);
}

TEST(a_002_Grafy_050_Mosty, TwoCyclesLinkedByPath) {
    Graph g(7);
    g.add(0, 1);
    g.add(1, 2);
    g.add(2, 0);
    g.add(2, 3);
    g.add(3, 4);
    g.add(4, 5);
    g.add(5, 6);
    g.add(6, 4);
    check(g);
}

TEST(a_002_Grafy_050_Mosty, PendantOnCycle) {
    Graph g(5);
    for (int i = 0; i < 4; ++i) {
        g.add(i, (i + 1) % 4);
    }
    g.add(2, 4);
    check(g);
}

TEST(a_002_Grafy_050_Mosty, DisconnectedComponents) {
    Graph g(6);
    g.add(0, 1);
    g.add(2, 3);
    g.add(3, 4);
    g.add(4, 2);
    check(g);
}

TEST(a_002_Grafy_050_Mosty, TriangleWithDoubledEdge) {
    Graph g(3);
    g.add(0, 1);
    g.add(1, 0);
    g.add(1, 2);
    g.add(2, 0);
    check(g);
}

TEST(a_002_Grafy_050_Mosty, RandomTrees) {
    std::mt19937 rng(601);
    for (int it = 0; it < 30; ++it) {
        int n = 2 + (int)(rng() % 25);
        vi parent(n, -1);
        for (int v = 1; v < n; ++v) {
            parent[v] = std::uniform_int_distribution<int>(0, v - 1)(rng);
        }
        check(tree_from_parent(parent));
    }
}

TEST(a_002_Grafy_050_Mosty, RandomSimpleGraphs) {
    std::mt19937 rng(602);
    for (int it = 0; it < 40; ++it) {
        int n = 1 + (int)(rng() % 18);
        Graph g(n);
        std::set<std::pair<int, int>> used;
        int m = (int)(rng() % 30);
        for (int i = 0; i < m && n >= 2; ++i) {
            int a = (int)(rng() % n);
            int b = (int)(rng() % n);
            if (a == b) {
                continue;
            }
            if (a > b) {
                std::swap(a, b);
            }
            if (!used.insert({a, b}).second) {
                continue;
            }
            g.add(a, b);
        }
        check(g);
    }
}


TEST(a_002_Grafy_050_Mosty, LoadKnownBridges) {
    auto chain = path_graph(20000);
    auto chain_got = run(chain);
    ASSERT_EQ(chain_got.size(), chain.edges.size());
    EXPECT_EQ(chain_got, normalize(chain.edges));

    auto cycle = cycle_graph(20000);
    EXPECT_TRUE(run(cycle).empty());

    vi parent(100000, -1);
    for (int v = 1; v < (int)parent.size(); ++v) {
        parent[v] = (v - 1) / 2;
    }
    auto wide = tree_from_parent(parent);
    EXPECT_EQ(run(wide), normalize(wide.edges));

    Graph pendants(10000);
    for (int i = 0; i < 5000; ++i) {
        pendants.add(i, (i + 1) % 5000);
    }
    for (int i = 0; i < 5000; ++i) {
        pendants.add(i, 5000 + i);
    }
    auto got = run(pendants);
    std::vector<std::pair<int, int>> exp;
    for (int i = 0; i < 5000; ++i) {
        exp.push_back({i, 5000 + i});
    }
    EXPECT_EQ(got, normalize(exp));
}
