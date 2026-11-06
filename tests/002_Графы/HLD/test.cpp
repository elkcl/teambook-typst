#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/HLD/code.hpp"

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

vvi random_tree(int n, std::mt19937 &rng) {
    vvi g(n);
    for (int i = 1; i < n; ++i) {
        add_edge(g, i, (int)(rng() % i));
    }
    return g;
}

struct Reference {
    int n;
    vvi g;
    vi parent, order, sub;
    vi values;

    explicit Reference(vvi graph, vi vals)
        : n((int)graph.size()), g(std::move(graph)), parent(n, -1), sub(n), values(std::move(vals)) {
        std::vector<int> stack{0};
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            order.push_back(v);
            for (int u : g[v]) {
                if (u == parent[v]) {
                    continue;
                }
                parent[u] = v;
                stack.push_back(u);
            }
        }
        for (int i = n - 1; i >= 0; --i) {
            int v = order[i];
            sub[v] = values[v];
            for (int u : g[v]) {
                if (parent[u] == v) {
                    sub[v] += sub[u];
                }
            }
        }
    }

    int path(int v, int u) const {
        std::vector<char> seen(n);
        for (int x = v; x != -1; x = parent[x]) {
            seen[x] = 1;
        }
        int sum = 0;
        int y = u;
        for (; !seen[y]; y = parent[y]) {
            sum += values[y];
        }
        for (int x = v; x != y; x = parent[x]) {
            sum += values[x];
        }
        sum += values[y];
        return sum;
    }
};

void assign_values(HLD &hld, const vi &values) {
    for (int v = 0; v < hld.n; ++v) {
        hld.st.upd(hld.tin[v], hld.tin[v] + 1, values[v]);
    }
}

void expect_tree(const vvi &g, const vi &values, std::mt19937 *rng, int samples) {
    vvi saved = g;
    HLD hld((int)g.size(), g);
    ASSERT_EQ(g, saved);
    int n = (int)g.size();
    ASSERT_EQ(hld.siz[0], n);
    std::vector<char> seen(n);
    for (int v = 0; v < n; ++v) {
        ASSERT_GE(hld.tin[v], 0);
        ASSERT_LT(hld.tin[v], n);
        ASSERT_FALSE(seen[hld.tin[v]]) << v;
        seen[hld.tin[v]] = 1;
    }
    assign_values(hld, values);
    Reference ref(saved, values);
    for (int v = 0; v < n; ++v) {
        ASSERT_EQ(hld.get_subtree(v), ref.sub[v]) << "subtree " << v;
    }
    auto check_path = [&](int v, int u) {
        int forward = hld.get_path(v, u);
        int backward = hld.get_path(u, v);
        int want = ref.path(v, u);
        if (forward != want || backward != ref.path(u, v)) {
            ADD_FAILURE() << v << " -> " << u << " got " << forward << " / " << backward
                          << " want " << want;
            return false;
        }
        return true;
    };
    if (n <= 40) {
        for (int v = 0; v < n; ++v) {
            for (int u = v; u < n; ++u) {
                if (!check_path(v, u)) {
                    return;
                }
            }
        }
        return;
    }
    for (int v = 0; v < n; ++v) {
        if (!check_path(v, v) || !check_path(0, v)) {
            return;
        }
    }
    if (rng == nullptr || samples <= 0) {
        return;
    }
    for (int i = 0; i < samples; ++i) {
        int v = (int)((*rng)() % n);
        int u = (int)((*rng)() % n);
        if (!check_path(v, u)) {
            return;
        }
    }
}

vi ones(int n) { return vi(n, 1); }

vi ramp(int n) {
    vi values(n);
    for (int i = 0; i < n; ++i) {
        values[i] = i - n / 3;
    }
    return values;
}

}  // namespace

TEST(a_002_Grafy_HLD, SingleVertex) {
    vvi g(1);
    expect_tree(g, vi{0}, nullptr, 0);
    expect_tree(g, vi{-7}, nullptr, 0);
    expect_tree(g, vi{15}, nullptr, 0);
}

TEST(a_002_Grafy_HLD, ZerosBeforeUpdate) {
    vvi g = path_graph(6);
    HLD hld(6, g);
    for (int v = 0; v < 6; ++v) {
        EXPECT_EQ(hld.get_subtree(v), 0);
        for (int u = 0; u < 6; ++u) {
            EXPECT_EQ(hld.get_path(v, u), 0);
        }
    }
}

TEST(a_002_Grafy_HLD, EdgeAndPath) {
    expect_tree(path_graph(2), vi{4, -3}, nullptr, 0);
    expect_tree(path_graph(8), ones(8), nullptr, 0);
    expect_tree(path_graph(8), ramp(8), nullptr, 0);
}

TEST(a_002_Grafy_HLD, StarAtRootAndAwayFromRoot) {
    expect_tree(star_graph(7, 0), ones(7), nullptr, 0);
    expect_tree(star_graph(7, 3), ramp(7), nullptr, 0);
    expect_tree(star_graph(6, 5), vi{5, -1, 0, 8, -4, 2}, nullptr, 0);
}

TEST(a_002_Grafy_HLD, HeapAndBranchOffHeavyChain) {
    expect_tree(heap_graph(15), ramp(15), nullptr, 0);
    vvi g(9);
    for (int i = 0; i + 1 < 6; ++i) {
        add_edge(g, i, i + 1);
    }
    add_edge(g, 2, 6);
    add_edge(g, 6, 7);
    add_edge(g, 4, 8);
    expect_tree(g, vi{1, 2, 3, 4, 5, 6, -2, 9, 0}, nullptr, 0);
}

TEST(a_002_Grafy_HLD, AssignOverwritesPreviousValue) {
    vvi g = path_graph(5);
    HLD hld(5, g);
    assign_values(hld, ones(5));
    EXPECT_EQ(hld.get_path(0, 4), 5);
    EXPECT_EQ(hld.get_subtree(2), 3);
    hld.st.upd(hld.tin[2], hld.tin[2] + 1, 10);
    EXPECT_EQ(hld.get_path(0, 4), 14);
    EXPECT_EQ(hld.get_path(2, 2), 10);
    EXPECT_EQ(hld.get_subtree(2), 12);
    EXPECT_EQ(hld.get_subtree(3), 2);
    EXPECT_EQ(hld.get_subtree(0), 14);
}

TEST(a_002_Grafy_HLD, InputGraphIsPreserved) {
    vvi g = star_graph(5, 1);
    vvi saved = g;
    HLD hld(5, g);
    EXPECT_EQ(g, saved);
    assign_values(hld, ones(5));
    EXPECT_EQ(g, saved);
}

TEST(a_002_Grafy_HLD, RandomTrees) {
    std::mt19937 rng(2202);
    for (int n : {1, 2, 3, 5, 17, 40, 80}) {
        for (int attempt = 0; attempt < 5; ++attempt) {
            vvi g = n == 1 ? vvi(1) : random_tree(n, rng);
            vi values(n);
            for (int &value : values) {
                value = (int)(rng() % 21) - 10;
            }
            expect_tree(g, values, &rng, 30);
        }
    }
}

TEST(a_002_Grafy_HLD, LoadStarHeapAndPath) {
    std::mt19937 rng(11);
    expect_tree(star_graph(100000, 0), ones(100000), &rng, 20);
    expect_tree(star_graph(100000, 50000), ones(100000), &rng, 20);
    expect_tree(heap_graph(100000), ones(100000), &rng, 20);
    expect_tree(path_graph(20000), ones(20000), &rng, 20);
}
