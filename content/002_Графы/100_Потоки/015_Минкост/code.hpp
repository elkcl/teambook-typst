#include "../001_Общее/code.hpp"


// обратным ребрам при добавлении надо ставить вес -w
pll min_cost(const vector<Edge *> &edges, int n, int s, int f) {
    if (s == f) {
        return {0, 0};
    }

    vector<ll> d(n);
    vector<Edge *> p(n);

    auto ford_bellman = [&edges, &d, &p, s, f]() {
        d.assign(sz(d), INF);
        p.assign(sz(p), nullptr);
        d[s] = 0;

        for (int i = 1; i < sz(p); i++) {
            bool changed = false;
            for (auto e : edges) {
                if (e->c > e->f && d[e->v] != INF &&
                    d[e->u] > d[e->v] + e->w) {
                    d[e->u] = d[e->v] + e->w;
                    p[e->u] = e;
                    changed = true;
                }
            }
            if (!changed) {
                break;
            }
        }

        if (d[f] == INF) {
            return ll(0);
        }

        ll flow = INF;
        for (Edge *e = p[f]; e != nullptr; e = p[e->v]) {
            flow = min(flow, e->c - e->f);
        }
        for (Edge *e = p[f]; e != nullptr; e = p[e->v]) {
            e->f += flow;
            e->rev->f -= flow;
        }
        return flow;
    };

    ll ans = 0, flow = 0;
    while (true) {
        ll new_flow = ford_bellman();
        if (new_flow == 0) {
            break;
        }
        flow += new_flow;
        ans += d[f] * new_flow;
    }
    return {flow, ans};
}
