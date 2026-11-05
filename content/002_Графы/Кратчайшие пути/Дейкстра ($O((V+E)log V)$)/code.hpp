#include "preamble.hpp"

vector<int> dijkstra(vector<vpii> &g, int n, int s) {
    vector<int> d(n, inf);
    d[s] = 0;
    priority_queue<pii, vpii, greater<pii>> q{};
    q.push({0, s});
    while (!q.empty()) {
        auto [cur_d, v] = q.top();
        q.pop();
        if (cur_d > d[v]) {
            continue;
        }
        for (auto [u, w] : g[v]) {
            if (d[u] > d[v] + w) {
                d[u] = d[v] + w;
                q.push({d[u], u});
            }
        }
    }
    return d;
}

vector<int> set_dijkstra(vector<vpii> &g, int n, int s) {
    vector<int> d(n, inf);
    d[s] = 0;
    set<pii> q{{0, s}};
    while (!q.empty()) {
        auto [cur_d, v] = *q.begin();
        q.erase(q.begin());
        for (auto [u, w] : g[v]) {
            if (d[u] > d[v] + w) {
                q.erase({d[u], u});
                d[u] = d[v] + w;
                q.insert({d[u], u});
            }
        }
    }
    return d;
}
