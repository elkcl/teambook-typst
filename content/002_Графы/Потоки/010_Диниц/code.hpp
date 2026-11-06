#include "../001_Общее/code.hpp"

ll dinic(const vector<vector<Edge *>> &g, int s, int f) {
    if (s == f) {
        return 0;
    }

    vector<ll> d(sz(g));
    vector<size_t> p(sz(g));

    auto check_bfs = [&g, &d, s, f]() {
        d.assign(sz(g), INF);
        d[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto e : g[v]) {
                if (e->f < e->c && d[e->u] == INF) {
                    d[e->u] = d[v] + 1;
                    q.push(e->u);
                }
            }
        }
        return d[f] != INF;
    };

    auto dfs = [&g, &d, &p, f](auto dfs, int v, ll min_flow = INF) {
        if (v == f) {
            return min_flow;
        }
        for (; p[v] < sz(g[v]); p[v]++) {
            auto e = g[v][p[v]];
            if (e->f < e->c && d[e->u] == d[v] + 1) {
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
    while (check_bfs()) {
        p.assign(sz(g), 0);
        while (true) {
            ll new_flow = dfs(dfs, s);
            if (new_flow == 0) {
                break;
            }
            flow += new_flow;
        }
    }

    return flow;
}
