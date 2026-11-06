#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/Центроиды/code.hpp"

void add_edge(vvi &g, int u, int v) {
    g[u].push_back(v);
    g[v].push_back(u);
}

vvi path_graph(int n) {
    vvi g(n);
    for (int i = 0; i + 1 < n; ++i) {
        add_edge(g, i, i + 1);
    }
    return g;
}

vvi star_graph(int n, int center) {
    vvi g(n);
    for (int i = 0; i < n; ++i) {
        if (i != center) {
            add_edge(g, center, i);
        }
    }
    return g;
}

vvi heap_graph(int n) {
    vvi g(n);
    for (int i = 1; i < n; ++i) {
        add_edge(g, i, (i - 1) / 2);
    }
    return g;
}

vvi prufer_tree(const vi &prufer) {
    int n = (int)prufer.size() + 2;
    vi degree(n, 1);
    for (int vertex : prufer) {
        ++degree[vertex];
    }
    std::set<int> leaves;
    for (int i = 0; i < n; ++i) {
        if (degree[i] == 1) {
            leaves.insert(i);
        }
    }
    vvi g(n);
    for (int vertex : prufer) {
        int leaf = *leaves.begin();
        leaves.erase(leaves.begin());
        add_edge(g, leaf, vertex);
        if (--degree[vertex] == 1) {
            leaves.insert(vertex);
        }
    }
    int left = *leaves.begin();
    int right = *std::next(leaves.begin());
    add_edge(g, left, right);
    return g;
}

int floor_log(int value) {
    int power = 0;
    while ((1LL << (power + 1)) <= value) {
        ++power;
    }
    return power;
}

std::string check(vvi g) {
    int n = (int)g.size();
    vvi saved = g;
    CentroidDecomposition decomposition(g);
    if (g != saved) {
        return "исходный граф изменился";
    }
    const vi &parent = decomposition.centroid_parent;
    if ((int)parent.size() != n) {
        return "размер centroid_parent";
    }

    vi component(n, -1);
    std::vector<vi> members;
    for (int start = 0; start < n; ++start) {
        if (component[start] != -1) {
            continue;
        }
        vi group;
        std::queue<int> queue;
        queue.push(start);
        component[start] = (int)members.size();
        while (!queue.empty()) {
            int vertex = queue.front();
            queue.pop();
            group.push_back(vertex);
            for (int to : g[vertex]) {
                if (to == vertex || component[to] != -1) {
                    continue;
                }
                component[to] = component[start];
                queue.push(to);
            }
        }
        members.push_back(std::move(group));
    }

    vi depth(n, -1), subtree(n, 0);
    for (const vi &group : members) {
        int roots = 0;
        int root = -1;
        int id = component[group[0]];
        for (int vertex : group) {
            int up = parent[vertex];
            if (up == -1) {
                ++roots;
                root = vertex;
                continue;
            }
            if (up < 0 || up >= n || up == vertex || component[up] != id) {
                return "плохой родитель у вершины " + std::to_string(vertex);
            }
        }
        if (roots != 1) {
            return "в компоненте из " + std::to_string(group.size()) + " вершин корней " +
                   std::to_string(roots);
        }

        std::vector<vi> children(n);
        for (int vertex : group) {
            if (parent[vertex] != -1) {
                children[parent[vertex]].push_back(vertex);
            }
        }

        std::vector<std::pair<int, int>> stack{{root, 0}};
        vi postorder;
        depth[root] = 0;
        while (!stack.empty()) {
            auto [vertex, state] = stack.back();
            stack.pop_back();
            if (state == 1) {
                postorder.push_back(vertex);
                continue;
            }
            stack.push_back({vertex, 1});
            for (int child : children[vertex]) {
                if (depth[child] != -1) {
                    return "цикл в дереве центроидов";
                }
                depth[child] = depth[vertex] + 1;
                stack.push_back({child, 0});
            }
        }
        if ((int)postorder.size() != (int)group.size()) {
            return "центроиды не покрывают компоненту";
        }

        int limit = floor_log((int)group.size());
        for (int vertex : postorder) {
            subtree[vertex] = 1;
            for (int child : children[vertex]) {
                subtree[vertex] += subtree[child];
            }
            for (int child : children[vertex]) {
                if (subtree[child] > subtree[vertex] / 2) {
                    return "компонента центроида " + std::to_string(vertex) + " больше половины";
                }
            }
            if (depth[vertex] > limit) {
                return "глубина дерева центроидов больше log";
            }
        }
        if (subtree[root] != (int)group.size()) {
            return "размер поддерева корня не равен компоненте";
        }

        for (int vertex : group) {
            for (int to : g[vertex]) {
                if (to <= vertex) {
                    continue;
                }
                int upper = vertex;
                int lower = to;
                if (depth[upper] > depth[lower]) {
                    std::swap(upper, lower);
                }
                int cursor = lower;
                int steps = 0;
                while (cursor != upper && cursor != -1 && steps <= n) {
                    cursor = parent[cursor];
                    ++steps;
                }
                if (cursor != upper) {
                    return "ребро " + std::to_string(vertex) + "-" + std::to_string(to) +
                           " не соединяет предка с потомком";
                }
            }
        }
    }
    return "";
}

void expect_valid(const vvi &g) {
    std::string reason = check(g);
    if (!reason.empty()) {
        ADD_FAILURE() << "n=" << g.size() << ": " << reason;
    }
}

}  // namespace

TEST(a_002_Grafy_Tsentroidy, SingleVertex) {
    vvi g(1);
    expect_valid(g);
    CentroidDecomposition decomposition(g);
    EXPECT_EQ(decomposition.centroid_parent, (vi{-1}));
    EXPECT_TRUE(decomposition.dead[0]);
}

TEST(a_002_Grafy_Tsentroidy, EdgeAndTriangleFreePath) {
    for (int n = 2; n <= 64; ++n) {
        vvi g = path_graph(n);
        expect_valid(g);
        CentroidDecomposition decomposition(g);
        EXPECT_EQ(decomposition.centroid_parent[(n - 1) / 2], -1) << n;
    }
}

TEST(a_002_Grafy_Tsentroidy, StarCenterIsTheOnlyCentroid) {
    for (int n = 3; n <= 30; ++n) {
        for (int center : {0, n / 2, n - 1}) {
            vvi g = star_graph(n, center);
            expect_valid(g);
            CentroidDecomposition decomposition(g);
            ASSERT_EQ(decomposition.centroid_parent[center], -1) << n << " center " << center;
            for (int vertex = 0; vertex < n; ++vertex) {
                if (vertex == center) {
                    continue;
                }
                EXPECT_EQ(decomposition.centroid_parent[vertex], center)
                    << n << " leaf " << vertex;
            }
        }
    }
    expect_valid(path_graph(2));
}

TEST(a_002_Grafy_Tsentroidy, HandBranch) {
    vvi g(5);
    add_edge(g, 0, 1);
    add_edge(g, 1, 2);
    add_edge(g, 2, 3);
    add_edge(g, 1, 4);
    expect_valid(g);
    CentroidDecomposition decomposition(g);
    EXPECT_EQ(decomposition.centroid_parent, (vi{1, -1, 1, 2, 1}));
}

TEST(a_002_Grafy_Tsentroidy, HeapShapes) {
    for (int n = 1; n <= 128; ++n) {
        expect_valid(heap_graph(n));
    }
}

TEST(a_002_Grafy_Tsentroidy, GraphIsPreserved) {
    vvi g = heap_graph(20);
    vvi saved = g;
    CentroidDecomposition decomposition(g);
    EXPECT_EQ(g, saved);
    CentroidDecomposition again(g);
    EXPECT_EQ(again.centroid_parent, decomposition.centroid_parent);
}

TEST(a_002_Grafy_Tsentroidy, ParallelEdgeBetweenTwoVertices) {
    vvi g(2);
    add_edge(g, 0, 1);
    add_edge(g, 0, 1);
    expect_valid(g);
}

TEST(a_002_Grafy_Tsentroidy, AllLabeledTreesUpToSeven) {
    expect_valid(vvi(1));
    for (int n = 2; n <= 7; ++n) {
        vi code(n - 2, 0);
        while (true) {
            std::string reason = check(prufer_tree(code));
            if (!reason.empty()) {
                ADD_FAILURE() << "n=" << n << " prufer: " << reason;
                return;
            }
            int index = n - 3;
            while (index >= 0 && code[index] == n - 1) {
                code[index] = 0;
                --index;
            }
            if (index < 0) {
                break;
            }
            ++code[index];
        }
    }
}

TEST(a_002_Grafy_Tsentroidy, RandomTrees) {
    std::mt19937 rng(2201);
    for (int n = 2; n <= 40; ++n) {
        for (int round = 0; round < 25; ++round) {
            vi code(n - 2);
            for (int &vertex : code) {
                vertex = (int)(rng() % n);
            }
            std::string reason = check(prufer_tree(code));
            if (!reason.empty()) {
                ADD_FAILURE() << "n=" << n << ": " << reason;
                return;
            }
        }
    }
    for (int n : {100, 300, 1000}) {
        for (int round = 0; round < 5; ++round) {
            vi code(n - 2);
            for (int &vertex : code) {
                vertex = (int)(rng() % n);
            }
            std::string reason = check(prufer_tree(code));
            if (!reason.empty()) {
                ADD_FAILURE() << "n=" << n << ": " << reason;
                return;
            }
        }
    }
}

TEST(a_002_Grafy_Tsentroidy, SecondComponentIsNotDecomposed) {
    vvi g(4);
    add_edge(g, 0, 1);
    add_edge(g, 2, 3);
    expect_valid(g);
}

TEST(a_002_Grafy_Tsentroidy, ComponentAwayFromZero) {
    vvi g(5);
    add_edge(g, 1, 2);
    add_edge(g, 2, 3);
    add_edge(g, 2, 4);
    expect_valid(g);
}

TEST(a_002_Grafy_Tsentroidy, IsolatedVertexBesidesTheMainTree) {
    vvi g = path_graph(4);
    g.push_back({});
    expect_valid(g);
}

TEST(a_002_Grafy_Tsentroidy, LoadStarHeapAndPath) {
    expect_valid(star_graph(100000, 0));
    expect_valid(star_graph(100000, 50000));
    expect_valid(heap_graph(100000));
    expect_valid(path_graph(20000));
}

TEST(a_002_Grafy_Tsentroidy, LoadRandomTree) {
    const int n = 100000;
    std::mt19937 rng(2202);
    vi code(n - 2);
    for (int &vertex : code) {
        vertex = (int)(rng() % n);
    }
    expect_valid(prufer_tree(code));
}

TEST(a_002_Grafy_Tsentroidy, LoadLongPath) {
    expect_valid(path_graph(100000));
}

TEST(a_002_Grafy_Tsentroidy, EmptyGraph) {
    vvi g;
    expect_valid(g);
}
