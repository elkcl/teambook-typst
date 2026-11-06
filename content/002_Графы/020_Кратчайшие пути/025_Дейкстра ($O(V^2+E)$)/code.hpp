#include "preamble.hpp"

vi dijkstra(vector<vpii> &g, int n, int s) {
    vi d(n, INF);
    vc used(n, false);
    d[s] = 0;
    for (int i = 0; i < n; i++) {
        int v = -1;
        for (int u = 0; u < n; u++) {
            if (!used[u] && (v == -1 || d[u] < d[v])) {
                v = u;
            }
        }
        if (d[v] == INF) {
            break;
        }
        used[v] = true;
        for (auto [u, w] : g[v]) {
            d[u] = min(d[u], d[v] + w);
        }
    }
    return d;
}
