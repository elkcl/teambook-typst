#include "preamble.hpp"
bool used[MAXN];
vector<pair<int, int>> g[MAXN];
int min_edge[MAXN] = {INF}, best_edge[MAXN];
min_edge[0] = 0;
// ...
for (int i = 0; i < n; i++) {
    int v = -1;
    for (int u = 0; u < n; u++) {
        if (!used[u] && (v == -1 || min_edge[u] < min_edge[v])) {
            v = u;
        }
    }
    used[v] = 1;
    if (v != 0) {
        cout << v << " " << best_edge[v] << endl;
    }
    for (auto e : g[v]) {
        int u = e.first, w = e.second;
        if (w < min_edge[u]) {
            min_edge[u] = w;
            best_edge[u] = v;
        }
    }
}
