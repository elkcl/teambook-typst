#include "preamble.hpp"
// ДО с массовыми преобразованиями a[i] = k * a[i] + b
// и суммой на отрезке по модулю M.

struct affine {
    ll k = 1, b = 0;
};

ll combine(ll a, ll b) { return (a + b) % M; }

void apply_push(int v, int l, int r, const affine &f) {
    t[v] = (t[v] * f.k + f.b * (r - l)) % M;

    need_push[v].k = need_push[v].k * f.k % M;
    need_push[v].b = (need_push[v].b * f.k + f.b) % M;
}

void push(int v, int l, int r) {
    if (r - l == 1) {
        return;
    }
    if (need_push[v].k == 1 && need_push[v].b == 0) {
        return;
    }

    int m = (l + r) / 2;

    apply_push(2 * v, l, m, need_push[v]);
    apply_push(2 * v + 1, m, r, need_push[v]);

    need_push[v] = {};
}
