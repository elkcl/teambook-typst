#include "preamble.hpp"

vvi floyd(vector<vpii> &g) {
    int n = g.size();
    vvi dist(n, vi(n, inf));
    for (int i = 0; i < n; ++i) {
        dist[i][i] = 0;
        for (auto [j, w] : g[i]) {
            dist[i][j] = min(dist[i][j], w);
        }
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] != inf && dist[k][j] != inf) {
                    dist[i][j] = min(dist[i][j], max(dist[i][k] + dist[k][j], -inf));
                }
            }
        }
    }
    return dist;
}
