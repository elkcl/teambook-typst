#include "preamble.hpp"
struct dsu {
    vi p;
    vi sz;

    dsu(int n) {
        p.resize(n);
        sz.resize(n, 1);
        iota(all(p), 0);
    }

    int root(int x) {
        if (p[x] == x) {
            return x;
        }
        return p[x] = root(p[x]);
    }

    void unite(int a, int b) {
        a = root(a);
        b = root(b);
        if (a == b) {
            return;
        }
        if (sz[a] < sz[b]) {
            swap(a, b);
        }
        p[b] = a;
        sz[a] += sz[b];
    }
};
