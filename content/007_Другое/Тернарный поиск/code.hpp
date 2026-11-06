#include "preamble.hpp"

template <typename F>
ll tern(ll l, ll r, const F &f) {
    while (r - l > 4) {
        ll m1 = (l * 2 + r) / 3;
        ll m2 = (l + r * 2 ) / 3;
        if (f(m1) > f(m2)) {
            l = m1;
        } else {
            r = m2;
        }
    }
    ll ans = l;
    for (ll i = l; i <= r; ++i) {
        if (f(ans) > f(i)) {
            ans = i;
        }
    }
    return ans;
}