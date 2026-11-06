#include "preamble.hpp"

vi bfs1k(vector<vpii> &g, int s, int k) {
    int n = g.size();
    vi d(n, INF);
    d[s] = 0;
    vector<queue<int>> q(k + 1);
    q[0].push(s);
    for (int dist = 0; dist <= k * (n - 1); dist++) {
        while (!q[dist % (k + 1)].empty()) {
            int v = q[dist % (k + 1)].front();
            q[dist % (k + 1)].pop();
            if (d[v] != dist) {
                continue;
            }
            for (auto [u, w] : g[v]) {
                if (d[u] > d[v] + w) {
                    d[u] = d[v] + w;
                    q[d[u] % (k + 1)].push(u);
                }
            }
        }
    }
    return d;
}
