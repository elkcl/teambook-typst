#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/Компоненты сильной связности/code.hpp"

vvi normalize(vvi components) {
    for (auto &component : components) {
        std::sort(component.begin(), component.end());
    }
    std::sort(components.begin(), components.end());
    return components;
}

std::vector<std::vector<char>> reachability(const vvi &g) {
    int n = (int)g.size();
    std::vector<std::vector<char>> seen(n, std::vector<char>(n, 0));
    for (int source = 0; source < n; ++source) {
        std::queue<int> queue;
        queue.push(source);
        seen[source][source] = 1;
        while (!queue.empty()) {
            int v = queue.front();
            queue.pop();
            for (int to : g[v]) {
                if (to < 0 || to >= n || seen[source][to]) {
                    continue;
                }
                seen[source][to] = 1;
                queue.push(to);
            }
        }
    }
    return seen;
}

vvi oracle(const vvi &g) {
    int n = (int)g.size();
    auto seen = reachability(g);
    vi id(n, -1);
    int count = 0;
    for (int v = 0; v < n; ++v) {
        if (id[v] != -1) {
            continue;
        }
        for (int u = v; u < n; ++u) {
            if (id[u] == -1 && seen[v][u] && seen[u][v]) {
                id[u] = count;
            }
        }
        ++count;
    }
    vvi components(count);
    for (int v = 0; v < n; ++v) {
        components[id[v]].push_back(v);
    }
    return normalize(std::move(components));
}

void expect_partition(const vvi &g) {
    vvi snapshot = g;
    SCC solver(snapshot);
    vvi got = normalize(solver.find_scc());
    vvi exp = oracle(g);
    if (got != exp) {
        ADD_FAILURE() << "компонент " << got.size() << ", ожидалось " << exp.size();
    }
}

vvi random_digraph(int n, int m, std::mt19937 &rng) {
    vvi g(n);
    if (n == 0 || m <= 0) {
        return g;
    }
    std::uniform_int_distribution<int> vertex(0, n - 1);
    for (int i = 0; i < m; ++i) {
        g[vertex(rng)].push_back(vertex(rng));
    }
    return g;
}

}  // namespace

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, EmptyGraph) {
    vvi g;
    expect_partition(g);
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, InputGraphIsPreserved) {
    vvi g(3);
    g[0] = {1, 2};
    g[1] = {0};
    vvi copy = g;
    SCC solver(g);
    EXPECT_EQ(g, copy);
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, SingleVertex) {
    expect_partition(vvi(1));
    vvi loop(1);
    loop[0] = {0, 0};
    expect_partition(loop);
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, NoEdges) {
    for (int n = 0; n <= 8; ++n) {
        expect_partition(vvi(n));
    }
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, OneDirectionIsTwoComponents) {
    vvi g(2);
    g[0] = {1};
    expect_partition(g);
    g.assign(2, {});
    g[1] = {0};
    expect_partition(g);
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, MutualEdgeIsOneComponent) {
    vvi g(2);
    g[0] = {1};
    g[1] = {0};
    expect_partition(g);
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, ChainAndCycle) {
    for (int n = 1; n <= 12; ++n) {
        vvi chain(n);
        for (int i = 0; i + 1 < n; ++i) {
            chain[i].push_back(i + 1);
        }
        expect_partition(chain);

        vvi cycle(n);
        for (int i = 0; i < n; ++i) {
            cycle[i].push_back((i + 1) % n);
        }
        expect_partition(cycle);
    }
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, SelfLoopDoesNotAbsorbNeighbour) {
    vvi g(3);
    g[0] = {0, 1};
    g[1] = {1};
    g[2] = {2, 2};
    expect_partition(g);
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, CycleWithTail) {
    vvi g(4);
    g[0] = {1};
    g[1] = {2};
    g[2] = {1, 3};
    expect_partition(g);
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, TwoCyclesLinkedByEdge) {
    vvi g(6);
    g[0] = {1};
    g[1] = {2};
    g[2] = {0, 3};
    g[3] = {4};
    g[4] = {5};
    g[5] = {3};
    expect_partition(g);
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, CondensationDiamond) {
    vvi g(4);
    g[0] = {1, 2};
    g[1] = {3};
    g[2] = {3};
    g[3] = {};
    expect_partition(g);
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, DisconnectedMixture) {
    vvi g(7);
    g[0] = {1};
    g[1] = {0};
    g[2] = {3};
    g[4] = {5};
    g[5] = {6};
    g[6] = {4, 4};
    expect_partition(g);
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, BidirectionalClique) {
    for (int n = 1; n <= 6; ++n) {
        vvi g(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    g[i].push_back(j);
                }
            }
        }
        expect_partition(g);
    }
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, StarOutAndStarIn) {
    vvi out(6);
    vvi in(6);
    for (int i = 1; i < 6; ++i) {
        out[0].push_back(i);
        in[i].push_back(0);
    }
    expect_partition(out);
    expect_partition(in);
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, DuplicateEdges) {
    vvi g(3);
    g[0] = {1, 1, 1, 0};
    g[1] = {2, 0, 0};
    g[2] = {1, 1};
    expect_partition(g);
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, RepeatedFindReturnsSamePartition) {
    vvi g(2);
    g[0] = {1};
    vvi snapshot = g;
    SCC solver(g);
    vvi exp = oracle(snapshot);
    EXPECT_EQ(normalize(solver.find_scc()), exp);
    EXPECT_EQ(normalize(solver.find_scc()), exp);
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, StressRandomDigraphs) {
    std::mt19937 rng(1101);
    for (int n = 1; n <= 16; ++n) {
        for (int it = 0; it < 10; ++it) {
            int m = (int)(rng() % (n * n + 1));
            expect_partition(random_digraph(n, m, rng));
        }
    }
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, LoadKnownPartitions) {
    const int n = 4000;
    vvi chain(n);
    for (int i = 0; i + 1 < n; ++i) {
        chain[i].push_back(i + 1);
    }
    vvi chain_exp(n);
    for (int i = 0; i < n; ++i) {
        chain_exp[i] = {i};
    }
    SCC chain_solver(chain);
    EXPECT_EQ(normalize(chain_solver.find_scc()), normalize(chain_exp));

    vvi cycle(n);
    for (int i = 0; i < n; ++i) {
        cycle[i].push_back((i + 1) % n);
    }
    vi cycle_vertices(n);
    std::iota(cycle_vertices.begin(), cycle_vertices.end(), 0);
    SCC cycle_solver(cycle);
    EXPECT_EQ(normalize(cycle_solver.find_scc()), vvi{cycle_vertices});
}

TEST(a_002_Grafy_Komponenty_silnoy_svyaznosti, LoadTwoCyclesAndRandom) {
    vvi linked(3000);
    for (int i = 0; i < 1500; ++i) {
        linked[i].push_back((i + 1) % 1500);
    }
    for (int i = 1500; i < 3000; ++i) {
        linked[i].push_back(1500 + (i + 1 - 1500) % 1500);
    }
    linked[10].push_back(1600);
    vi left(1500), right(1500);
    std::iota(left.begin(), left.end(), 0);
    std::iota(right.begin(), right.end(), 1500);
    vvi exp{left, right};

    SCC solver(linked);
    EXPECT_EQ(normalize(solver.find_scc()), normalize(exp));

    std::mt19937 rng(1102);
    expect_partition(random_digraph(40, 200, rng));
    expect_partition(random_digraph(40, 20, rng));
}
