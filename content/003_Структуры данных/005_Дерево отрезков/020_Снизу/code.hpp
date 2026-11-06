#include "preamble.hpp"
// ДО снизу от peltorator
struct Segtree {
    int sz;
    vll t;

    Segtree(const vi &a) {
        sz = a.size();
        t.resize(sz * 2, 0);
        build(a);
    }

    void build(const vi &a) {
        for (int i = 0; i < sz; i++) {
            t[sz + i] = a[i];
        }
        for (int i = sz - 1; i > 0; i--) {
            t[i] = t[i << 1] + t[(i << 1) | 1];
        }
    }

    // arr[pos] := newval
    void upd(int pos, int newval) {
        pos += sz;
        t[pos] = newval;
        pos >>= 1;
        while (pos > 0) {
            t[pos] = t[pos << 1] + t[(pos << 1) | 1];
            pos >>= 1;
        }
    }

    ll sum(int l, int r) { // [l, r)
        l += sz;
        r += sz;
        ll ans = 0;
        while (l < r) {
            if (l & 1) {
                ans += t[l++];
            }
            if (r & 1) {
                ans += t[--r];
            }
            l >>= 1;
            r >>= 1;
        }
        return ans;
    }
};
