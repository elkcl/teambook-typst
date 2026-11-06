#include "preamble.hpp"

optional<vi> topological_sort(vvi &g) {
    vi in_degree(g.size(), 0);
    for (int i = 0; i < g.size(); i++) {
        for (auto to : g[i]) {
            in_degree[to]++;
        }
    }
    queue<int> q;
    for (int i = 0; i < g.size(); i++) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }
    vi order;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        order.push_back(v);
        for (auto to : g[v]) {
            in_degree[to]--;
            if (in_degree[to] == 0) {
                q.push(to);
            }
        }
    }
    if (order.size() == g.size()) {
        return order;
    }
    return nullopt;
}