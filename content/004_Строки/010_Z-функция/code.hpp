#include "preamble.hpp"

vi z_func(const string &s) {
    vi z(sz(s), 0);
    int l = 0, r = 0;
    for (int i = 1; i < sz(s); i++) {
        z[i] = (i < r ? min(r - i, z[i - l]) : 0);
        while (i + z[i] < sz(s) && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}
