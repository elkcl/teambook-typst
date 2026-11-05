typedef struct Node {
    ll elem;

    Node(ll elem) : elem(elem) { prior = rndll(); }

    ll sz = 1, prior;
    Node *l = nullptr;
    Node *r = nullptr;
} Node;

void apply_op(Node *n) { /* ... */ }

void push(Node *n) {
    if (!n) {
        return;
    }
    // delayed apply_op
}

ll get_sz(Node *n) { return !n ? 0 : n->sz; }

void update(Node *n) {
    if (!n) {
        return;
    }
    n->sz = 1 + get_sz(n->l) + get_sz(n->r);
}

pair<Node *, Node *> split(Node *n, int key) {
    if (!n) {
        return {nullptr, nullptr};
    }
    push(n);
    if (get_sz(n->l) < key) {
        auto [l, r] = split(n->r, key - get_sz(n->l) - 1);
        n->r = l;
        update(n);
        return {n, r};
    } else {
        auto [l, r] = split(n->l, key);
        n->l = r;
        update(n);
        return {l, n};
    }
}

Node *merge(Node *l, Node *r) {
    if (!l) {
        return r;
    }
    if (!r) {
        return l;
    }
    if (l->prior < r->prior) {
        push(r);
        r->l = merge(l, r->l);
        update(r);
        return r;
    } else {
        push(l);
        l->r = merge(l->r, r);
        update(l);
        return l;
    }
}

void tfree(Node *n) {
    if (!n) {
        return;
    }
    tfree(n->l);
    tfree(n->r);
    delete n;
}
