// ДО с массовыми операциями для операция вида
// ax+b и суммы на отрезке по модулю

struct affine {
  ll k, b;
};

struct segtree {
  int n;
  vll t;
  vector<affine> aff;

  segtree(const vi &a) {
    n = a.size();
    t.resize(n * 4, 0);
    aff.resize(n * 4, {1, 0});
    build(a, 1, 0, n);
  }

  ll build(const vi &a, int v, int l, int r) {
    if (l + 1 == r) {
      return t[v] = a[l];
    }
    int m = (l + r) / 2;
    return t[v] = (build(a, 2 * v, l, m) +
                   build(a, 2 * v + 1, m, r)) %
                  M;
  }

  void push(int v, int l, int r) {
    t[v] *= aff[v].k;
    t[v] += aff[v].b * (r - l);
    t[v] %= M;

    aff[2 * v].k *= aff[v].k;
    aff[2 * v].b *= aff[v].k;
    aff[2 * v].b += aff[v].b;
    aff[2 * v].k %= M;
    aff[2 * v].b %= M;

    aff[2 * v + 1].k *= aff[v].k;
    aff[2 * v + 1].b *= aff[v].k;
    aff[2 * v + 1].b += aff[v].b;
    aff[2 * v + 1].k %= M;
    aff[2 * v + 1].b %= M;

    aff[v].k = 1;
    aff[v].b = 0;
  }

  ll fresh(int v, int l, int r) const {
    return (t[v] * aff[v].k +
            aff[v].b * (r - l)) %
           M;
  }

  ll get(int ql, int qr, int v = 1, int l = 0,
         int r = -1) {
    if (r == -1) r = n;
    if (qr <= l || r <= ql) return 0;
    if (ql <= l && r <= qr) return fresh(v, l, r);

    push(v, l, r);
    int m = (l + r) / 2;
    return (get(ql, qr, 2 * v, l, m) +
            get(ql, qr, 2 * v + 1, m, r)) %
           M;
  }

  void upd(int ql, int qr, int k, int b,
           int v = 1, int l = 0, int r = -1) {
    if (r == -1) r = n;
    if (qr <= l || r <= ql) return;
    if (ql <= l && r <= qr) {
      aff[v].k *= k;
      aff[v].b *= k;
      aff[v].b += b;
      aff[v].k %= M;
      aff[v].b %= M;
      return;
    }

    push(v, l, r);
    int m = (l + r) / 2;
    upd(ql, qr, k, b, 2 * v, l, m);
    upd(ql, qr, k, b, 2 * v + 1, m, r);
    t[v] = (fresh(2 * v, l, m) +
            fresh(2 * v + 1, m, r)) %
           M;
  }
};
