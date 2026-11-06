#include "preamble.hpp"

ll binpow(ll a, ll n, ll mod) {
    ll res = 1 % mod;
    a %= mod;
    if (a < 0) {
        a += mod;
    }
    while (n) {
        if (n & 1) {
            res = res * a % mod;
        }
        a = a * a % mod;
        n >>= 1;
    }
    return res;
}
