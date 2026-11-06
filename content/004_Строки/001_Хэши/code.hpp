#include "preamble.hpp"

struct Hash {
    static constexpr ll BASE = 911382323;
    int n;
    vll mods;
    vvll pref, rev, pw;

    template <class Iterable>
    Hash(const Iterable &s, const vll &_mods) : mods(_mods) {
        vector<__int128> a;
        for (auto x : s) {
            using T = decay_t<decltype(x)>;

            if constexpr (is_same_v<T, char> || is_same_v<T, unsigned char>) {
                a.push_back(static_cast<unsigned char>(x));
            } else {
                a.push_back(static_cast<__int128>(x));
            }
        }

        n = a.size();
        int k = mods.size();

        pref.assign(k, vll(n + 1, 0));
        rev.assign(k, vll(n + 1, 0));
        pw.assign(k, vll(n + 1, 1));

        for (int j = 0; j < k; j++) {
            ll m = mods[j];
            ll base = BASE % m;

            for (int i = 0; i < n; i++) {
                pw[j][i + 1] = static_cast<__int128>(pw[j][i] * base) % m;

                ll x = (a[i] + 1) % m;
                ll y = (a[n - 1 - i] + 1) % m;
                if (x < 0) x += m;
                if (y < 0) y += m;

                pref[j][i + 1] = (static_cast<__int128>(pref[j][i] * base) + x) % m;
                rev[j][i + 1] = (static_cast<__int128>(rev[j][i] * base) + y) % m;
            }
        }
    }

    ll get_one(int j, int l, int r, bool reversed = false) const {
        const vll &h = reversed ? rev[j] : pref[j];
        ll m = mods[j];
        ll res = h[r] - static_cast<__int128>(h[l] * pw[j][r - l]) % m;
        if (res < 0) res += m;
        return res;
    }

    vll get(int l, int r, bool reversed = false) const {
        vll res(mods.size());
        for (int j = 0; j < (int)mods.size(); j++) {
            res[j] = get_one(j, l, r, reversed);
        }
        return res;
    }

    bool equal(int l1, int r1, int l2, int r2,
               bool rev1 = false, bool rev2 = false) const {
        if (r1 - l1 != r2 - l2) return false;
        for (int j = 0; j < (int)mods.size(); j++) {
            if (get_one(j, l1, r1, rev1) != get_one(j, l2, r2, rev2)) {
                return false;
            }
        }
        return true;
    }
};