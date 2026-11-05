uint32_t bit_ceil(uint32_t n) {
    if (n <= 1) {
        return 1;
    }
    return 1u << (32 - __builtin_clz(n - 1));
}

struct segtree {
    static constexpr ll NEUTRAL = 0;

    int n, sz;
    vll t;
    vc need_push;
    vi lazy;

    segtree(int n) : n(n) {
        sz = bit_ceil(n);
        t.resize(2 * sz, NEUTRAL);
        need_push.resize(2 * sz, 0);
        lazy.resize(2 * sz, NEUTRAL);
    }

    segtree(const vi &a) : segtree(a.size()) { build(a, 1, 0, sz); }

    ll combine(ll a, ll b) { return (a + b); }

    void pull(int v) { t[v] = combine(t[2 * v], t[2 * v + 1]); }

    void apply(int v, int l, int r, int x) {
        t[v] = 1LL * x * (r - l);

        lazy[v] = x;
        need_push[v] = 1;
    }

    void push(int v, int l, int r) {
        if (r - l == 1) {
            return;
        }
        if (!need_push[v]) {
            return;
        }

        int m = (l + r) / 2;

        apply(2 * v, l, m, lazy[v]);
        apply(2 * v + 1, m, r, lazy[v]);

        need_push[v] = 0;
    }

    void build(const vi &a, int v, int l, int r) {
        if (r - l == 1) {
            if (l < a.size()) {
                t[v] = a[l];
            }
            return;
        }

        int m = (l + r) / 2;

        build(a, 2 * v, l, m);
        build(a, 2 * v + 1, m, r);

        pull(v);
    }

    ll get(int ql, int qr, int v = 1, int l = 0, int r = -1) {
        if (r == -1) {
            r = sz;
        }

        if (qr <= l || r <= ql) {
            return NEUTRAL;
        }
        if (ql <= l && r <= qr) {
            return t[v];
        }

        push(v, l, r);

        int m = (l + r) / 2;
        return combine(get(ql, qr, 2 * v, l, m), get(ql, qr, 2 * v + 1, m, r));
    }

    void upd(int ql, int qr, int x, int v = 1, int l = 0, int r = -1) {
        if (r == -1) {
            r = sz;
        }

        if (qr <= l || r <= ql) {
            return;
        }

        if (ql <= l && r <= qr) {
            apply(v, l, r, x);
            return;
        }

        push(v, l, r);

        int m = (l + r) / 2;
        upd(ql, qr, x, 2 * v, l, m);
        upd(ql, qr, x, 2 * v + 1, m, r);

        pull(v);
    }

    template <class F>
    int max_right_impl(int ql, F &pred, ll &acc, int v, int l, int r) {
        if (r <= ql) {
            return -1;
        }

        if (ql <= l) {
            ll nxt = combine(acc, t[v]);

            if (pred(nxt)) {
                acc = nxt;
                return -1;
            }

            if (r - l == 1) {
                return l;
            }
        }

        push(v, l, r);

        int m = (l + r) / 2;

        int res = max_right_impl(ql, pred, acc, 2 * v, l, m);

        if (res != -1) {
            return res;
        }

        return max_right_impl(ql, pred, acc, 2 * v + 1, m, r);
    }

    template <class F> int max_right(int l, F pred) {
        ll acc = NEUTRAL;

        int res = max_right_impl(l, pred, acc, 1, 0, sz);

        return res == -1 ? n : min(res, n);
    }

    template <class F>
    int min_left_impl(int qr, F &pred, ll &acc, int v, int l, int r) {
        if (qr <= l) {
            return -1;
        }

        if (r <= qr) {
            ll nxt = combine(t[v], acc);

            if (pred(nxt)) {
                acc = nxt;
                return -1;
            }

            if (r - l == 1) {
                return r;
            }
        }

        push(v, l, r);

        int m = (l + r) / 2;

        int res = min_left_impl(qr, pred, acc, 2 * v + 1, m, r);

        if (res != -1) {
            return res;
        }

        return min_left_impl(qr, pred, acc, 2 * v, l, m);
    }

    template <class F> int min_left(int r, F pred) {
        ll acc = NEUTRAL;

        int res = min_left_impl(r, pred, acc, 1, 0, sz);

        return res == -1 ? 0 : res;
    }
};
