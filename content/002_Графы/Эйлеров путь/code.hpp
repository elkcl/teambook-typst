#include "preamble.hpp"

optional<vi> find_euler_path(vvi &g) {
    vi deg(g.size(), 0);
    int num_edges = 0;
    vector<unordered_set<int>> unused(g.size());
    for (int i = 0; i < g.size(); i++) {
        unused[i] = unordered_set<int>(g[i].begin(), g[i].end());
        num_edges += g[i].size();
        for (auto to : g[i]) {
            deg[to]++;
        }
    }
    num_edges /= 2;
    int start = -1;
    int odd_count = 0;
    for (int i = 0; i < g.size(); i++) {
        if (deg[i] % 2 == 1) {
            odd_count++;
            start = i;
        } else if (deg[i] > 0 && start == -1) {
            start = i;
        }
    }
    if (odd_count != 0 && odd_count != 2 || start == -1) {
        return nullopt;
    }
    vi path{};
    vi stack{};
    stack.push_back(start);
    while (!stack.empty()) {
        int v = stack.back();
    
        if (unused[v].empty()) {
            path.push_back(v);
            stack.pop_back();
        } else {
            int to = *unused[v].begin();
            unused[v].erase(to);
            unused[to].erase(v);
            stack.push_back(to);
        }
    }
    if (path.size() != num_edges + 1) {
        return nullopt;
    }
    reverse(path.begin(), path.end());
    return path;
}