#include "preamble.hpp"

void inner_dfs(vvi &g, int v, int p, vc &used, vi &d, vi &h, vc &is_cutpoint) {
    used[v] = true;
    d[v] = h[v] = (p == -1 ? 0 : h[p] + 1);
    int children = 0;
    for (int u : g[v]) {
        if (u != p) {
            if (used[u]) {
                d[v] = min(d[v], h[u]);
            } else {
                inner_dfs(g, u, v, used, d, h, is_cutpoint);
                d[v] = min(d[v], d[u]);
                if (h[v] <= d[u] && p != -1) {
                    is_cutpoint[v] = true;
                }
                children++;
            }
        }
    }
    if (p == -1 && children > 1) {
        is_cutpoint[v] = true;
    }
}


vc find_cutpoints(vvi &g) {
    vc used(g.size(), false);
    vi d(g.size(), 0);
    vi h(g.size(), 0);
    vc is_cutpoint(g.size(), false);

    for (int v = 0; v < g.size(); v++) {
        if (!used[v]) {
            inner_dfs(g, v, -1, used, d, h, is_cutpoint);
        }
    }
    return is_cutpoint;
}
