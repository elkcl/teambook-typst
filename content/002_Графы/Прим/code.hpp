#include "preamble.hpp"

optional<vpii> prim(vector<vpii> &g) {
    int n = g.size();
    vc used(n, false);
    vi min_edge(n, INF);
    vi best_edge(n, -1);
    min_edge[0] = 0;
    vpii answer{};
    for (int i = 0; i < n; i++) {
        int v = -1;
        for (int u = 0; u < n; u++) {
            if (!used[u] && (v == -1 || min_edge[u] < min_edge[v])) {
                v = u;
            }
        }
        if (min_edge[v] == INF) {
            return nullopt; // граф не связный
        }
        used[v] = true;
        if (v != 0) {
            answer.emplace_back(v, best_edge[v]);
        }
        for (auto e : g[v]) {
            int u = e.first, w = e.second;
            if (!used[u] && w < min_edge[u]) {
                min_edge[u] = w;
                best_edge[u] = v;
            }
        }
    }
    return answer;
}