#include "preamble.hpp"
const ll MOD = 998'244'353;
const ll BLOCK = 500; // типа sqrt(MOD), но подбирать ручками

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

ll dlog(ll a, ll b, unordered_map<ll, ll> &fs) {
    ll an = binpow(a, BLOCK, MOD);
    ll curr = an;
    for (int i = 1; i <= MOD / BLOCK + 1; ++i) {
        if (fs.find(curr) == fs.end()) {
            fs[curr] = i;
        }
        curr = (curr * an) % MOD;
    }
    curr = b;
    for (int i = 0; i <= BLOCK; ++i) {
        if (fs.find(curr) != fs.end()) {
            ll ans = fs[curr] * BLOCK - i;
            if (ans < MOD) {
                return ans;
            }
        }
        curr = (curr * a) % MOD;
    }
    return -1;
}
