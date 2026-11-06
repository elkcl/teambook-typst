#include "preamble.hpp"

vi ford_bellman(vector<vpii> &g, int s) {
    int n = g.size();
    vi dist(n, inf);
    dist[s] = 0;
    for (int i = 0; i < n; ++i) {
        bool relaxed = false;
        for (int j = 0; j < n; ++j) {
            for (auto &&[to, w] : g[j]) {
                if (dist[j] != inf && dist[to] > dist[j] + w) {
                    dist[to] = dist[j] + w;
                    relaxed = true;
                }
            }
        }
        if (!relaxed) {
            return dist;
        }
    }
    return vi(n, -1);
}