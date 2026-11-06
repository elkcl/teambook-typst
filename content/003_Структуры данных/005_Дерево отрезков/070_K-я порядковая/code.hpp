#include "preamble.hpp"

struct KthOrder {
    static constexpr int MAX_DEPTH = 19;
    static constexpr int SIZE = 1 << MAX_DEPTH;

    struct Node {
        int sum = 0;
        Node *left = nullptr;
        Node *right = nullptr;

        Node(ll x = 0, Node *l = nullptr, Node *r = nullptr)
            : sum(x), left(l), right(r) {}

        Node(Node *p) : sum(p->sum), left(p->left), right(p->right) {}
    };

    vector<Node *> roots;

    KthOrder(const vi &a) {
        roots.push_back(new Node());
        build(roots.back());

        for (int i : a) {
            roots.pb(upd(roots.back(), i, 0, SIZE));
        }
    }

    void build(Node *p, int depth = 0) {
        if (depth == MAX_DEPTH) {
            return;
        }
        p->left = new Node();
        p->right = new Node();
        build(p->left, depth + 1);
        build(p->right, depth + 1);
    }

    Node *upd(Node *p, int pos, int l, int r) {
        if (r - l == 1) {
            return new Node(p->sum + 1, p->left, p->right);
        }

        int mid = (l + r) / 2;

        auto new_p = new Node(p);
        if (pos < mid) {
            new_p->left = upd(new_p->left, pos, l, mid);
        } else {
            new_p->right = upd(new_p->right, pos, mid, r);
        }
        new_p->sum = new_p->left->sum + new_p->right->sum;
        return new_p;
    }

    ll get(int l, int r, int qk) {
        return get(roots[l], roots[r], qk, 0, SIZE);
    }

    ll get(Node *ln, Node *rn, int qk, int l, int r) {
        if (r - l == 1) {
            return l;
        }

        ll mid = (l + r) / 2;
        ll left_sum = rn->left->sum - ln->left->sum;

        if (left_sum >= qk) {
            return get(ln->left, rn->left, qk, l, mid);
        }
        return get(ln->right, rn->right, qk - left_sum, mid, r);
    }
};
