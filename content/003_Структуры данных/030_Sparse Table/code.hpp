struct sparse_table {
    int n;
    vi logs;
    vvi sp;

    sparse_table(const vi &a) : n(sz(a)) {
        logs.resize(n + 1, 0);

        for (int i = 2; i <= n; ++i) {
            logs[i] = logs[i / 2] + 1;
        }

        sp.resize(n, vi(logs[n] + 1, inf));

        for (int i = 0; i < n; ++i) {
            sp[i][0] = a[i];
        }

        for (int k = 1; k <= logs[n]; k++) {
            for (int i = 0; i + (1 << (k - 1)) < n; i++) {
                sp[i][k] = min(sp[i][k - 1], sp[i + (1 << (k - 1))][k - 1]);
            }
        }
    }

    // min on [l, r)
    int get(int l, int r) {
        int k = logs[r - l];
        return min(sp[l][k], sp[r - (1 << k)][k]);
    }
};
