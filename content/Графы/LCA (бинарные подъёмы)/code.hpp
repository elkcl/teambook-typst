#include "preamble.hpp"


struct LCA {
    vvi up;
    vi tin, tout;
    int logn;
    int t;
    LCA(vvi &g, int n) {
        logn = static_cast<int>(ceil(log2(n))) + 1;
        up = vvi(n, vi(logn, 0));
        tin = vi(n);
        tout = vi(n);
        t = 0;
        vc used(n, false);
        dfs1(0, used, g);
    }

    void dfs1(int v, vc &used, vvi &g) {
        for (int i = 1; i < logn; ++i) {
            up[v][i] = up[up[v][i - 1]][i - 1];
        }
        tin[v] = t;
        t++;
        used[v] = true;
        for (int to : g[v]) {
            if (!used[to]) {
                up[to][0] = v;
                dfs1(to, used, g);
            }
        }
        tout[v] = t;
    }

    int lca(int v, int u) {
        if (anc(v, u)) {
            return v;
        }
        if (anc(u, v)) {
            return u;
        }
        for (int l = logn - 1; l >= 0; l--) {
            if (!anc(up[v][l], u)) {
                v = up[v][l];
            }
        }
        return up[v][0];
    }
    bool anc(int u, int v) {
        return tin[u] <= tin[v] && tin[v] < tout[u];
    }
};