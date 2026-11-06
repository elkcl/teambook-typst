#include "preamble.hpp"

struct Permutations {
    vll fac;

    Permutations(int n) {
        fac.resize(n + 1);
        fac[0] = 1;
        for (int i = 1; i <= n; i++) {
            fac[i] = fac[i - 1] * i;
        }
    }

    ll encode_perm(vll perm) {
        int n = perm.size();
        ll val = 0;
        for (int i = 0; i < n; i++) {
            val += fac[n - 1 - i] * perm[i];
            for (int j = i + 1; j < n; j++) {
                if (perm[j] > perm[i]) {
                    perm[j]--;
                }
            }
        }
        return val;
    }

    vll decode_perm(ll val, ll base) {
        vll perm(base);
        vll unused;
        for (ll i = 0; i < base; i++) {
            unused.push_back(i);
        }
        for (ll i = 0; i < base; i++) {
            ll j = val / fac[base - i - 1];
            perm[i] = unused[j];
            unused.erase(unused.begin() + j);
            val %= fac[base - i - 1];
        }
        return perm;
    }
};
