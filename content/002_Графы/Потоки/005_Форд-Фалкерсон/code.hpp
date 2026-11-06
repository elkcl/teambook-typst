#include "../001_Общее/code.hpp"

ll ford_fulckerson(const vector<vector<Edge *>> &g, int s, int f) {
    if (s == f) {
        return 0;
    }

    vector<int> used(sz(g), 0);
    int use_ind = 0;

    auto dfs = [&g, &used, f, &use_ind](auto dfs, int v, ll min_flow = INF) {
        if (v == f) {
            return min_flow;
        }
        used[v] = use_ind;
        for (auto e : g[v]) {
            if (e->f < e->c && used[e->u] != use_ind) {
                ll flow = dfs(dfs, e->u, min(min_flow, e->c - e->f));
                if (flow > 0) {
                    e->f += flow;
                    e->rev->f -= flow;
                    return flow;
                }
            }
        }
        return ll(0);
    };

    ll flow = 0;
    while (true) {
        use_ind++;

        ll new_flow = dfs(dfs, s);
        if (new_flow == 0) {
            break;
        }
        flow += new_flow;
    }
    return flow;
}
