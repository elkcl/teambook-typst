#include "preamble.hpp"

bool kuhn_dfs(vvi &g, vi &iteration, vi &lr, vi &rl, int curr, int v) {
    if (iteration[v] == curr) {
        return false;
    }
    iteration[v] = curr;
    for (int to : g[v]) {
        if (rl[to] == -1 || kuhn_dfs(g, iteration, lr, rl, curr, rl[to])) {
            rl[to] = v;
            lr[v] = to;
            return true;
        }
    }
    return false;
}

vpii get_matching(vvi &g, int k, int n) {
    vpii matching;
    vi lr(k, -1), rl(n, -1);
    vi iteration(k, 0);
    for (int i = 0; i < k; ++i) {
        kuhn_dfs(g, iteration, lr, rl, i + 1, i);
    }
    for (int i = 0; i < k; ++i) {
        if (lr[i] != -1) {
            matching.emplace_back(i, lr[i]);
        }
    }
    return matching;
}
