ll combine(ll lans, ll rans) { return lans + rans; }

typedef struct Node {
    ll sum;

    Node(int l, int r) : l(l), r(r) {}

    int need_push = 0;
    int set = 0;

    int l, r;
    Node *ln = nullptr;
    Node *rn = nullptr;

    Node *left() { return ln ? ln : ln = new Node(l, (r + l) / 2); }

    Node *right() { return rn ? rn : rn = new Node((r + l) / 2, r); }

    void pull() { sum = combine(left()->sum, right()->sum); }

    void apply(int x) {
        sum = 1LL * x * (r - l);
        need_push = 1;
        set = x;
    }

    void push() {
        if (r - l == 1 || !need_push) {
            return;
        }

        left()->apply(set);
        right()->apply(set);
        need_push = 0;
    }

    ll get(int ql, int qr) {
        if (r <= ql || qr <= l) {
            return 0;
        }
        if (ql <= l && r <= qr) {
            return sum;
        }

        push();
        int m = (l + r) / 2;
        return combine(left()->get(ql, qr), right()->get(ql, qr));
    }

    void upd(int ql, int qr, int x) {
        if (r <= ql || qr <= l) {
            return;
        }
        if (ql <= l && r <= qr) {
            apply(x);
            return;
        }

        push();
        int m = (l + r) / 2;
        left()->upd(ql, qr, x);
        right()->upd(ql, qr, x);
        pull();
    }
} Node;
