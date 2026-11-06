#include "preamble.hpp"
struct DSU {
    vi p;
    vi sz;

    DSU(int n) {
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

    bool unite(int a, int b) {
        a = root(a);
        b = root(b);
        if (a == b) {
            return false;
        }
        if (sz[a] < sz[b]) {
            swap(a, b);
        }
        p[b] = a;
        sz[a] += sz[b];
        return true;
    }
};
