#include "preamble.hpp"

struct SCC {
    vvi &adj;
    vvi adj_rev;
    vc used;
    vi order, component;

    void dfs1(int v) {
        used[v] = true;
        for (auto u : adj[v]) {
            if (!used[u]) {
                dfs1(u);
            }
        }
        order.push_back(v);
    }

    void dfs2(int v) {
        used[v] = true;
        component.push_back(v);
        for (auto u : adj_rev[v]) {
            if (!used[u]) {
                dfs2(u);
            }
        }
    }

    SCC(vvi &g)
        : adj(g), adj_rev(adj.size()), used(adj.size(), false), order(),
          component() {
        for (int i = 0; i < adj.size(); i++) {
            for (auto to : adj[i]) {
                adj_rev[to].push_back(i);
            }
        }
    }

    vvi find_scc() {
        used.assign(adj.size(), false);
        order.clear();
        component.clear();
        vvi components{};
        for (int i = 0; i < adj.size(); i++) {
            if (!used[i]) {
                dfs1(i);
            }
        }
        used.assign(adj.size(), false);
        reverse(order.begin(), order.end());
        for (auto v : order) {
            if (!used[v]) {
                dfs2(v);
                components.push_back(component);
                component.clear();
            }
        }
        return components;
    }
};
