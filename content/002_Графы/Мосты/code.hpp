#include "preamble.hpp"

void find_bridges(vvi &g, vi &h, vi &d, vc &used, vpii &bridges, int v,
                  int p = -1) {
    used[v] = true;
    h[v] = (p == -1 ? 0 : h[p] + 1);
    d[v] = h[v];
    for (int u : g[v]) {
        if (u != p) {
            if (used[u]) {
                d[v] = min(d[v], h[u]);
            } else {
                find_bridges(g, h, d, used, bridges, u, v);
                d[v] = min(d[v], d[u]);
                if (h[v] < d[u]) {
                    bridges.emplace_back(v, u);
                }
            }
        }
    }
}
