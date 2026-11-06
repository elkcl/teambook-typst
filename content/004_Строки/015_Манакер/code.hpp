#include "preamble.hpp"

vi manacher_odd(const string &s) {
    vector<int> d(sz(s), 1);
    int l = 0, r = 0;
    for (int i = 1; i < sz(s); i++) {
        d[i] = (i <= r ? min(r - i + 1, d[l + r - i]) : 1);
        while (i - d[i] >= 0 && i + d[i] < sz(s) &&
               s[i - d[i]] == s[i + d[i]]) {
            d[i]++;
        }
        if (i + d[i] - 1 > r) {
            l = i - d[i] + 1, r = i + d[i] - 1;
        }
    }
    return d;
}

vi manacher_even(const string &s) {
    vector<int> d(sz(s), 0);
    int l = -1, r = -1;
    for (int i = 0; i < sz(s) - 1; i++) {
        d[i] = (i < r ? min(r - i, d[l + r - i - 1]) : 0);
        while (i - d[i] >= 0 && i + d[i] + 1 < sz(s) &&
               s[i - d[i]] == s[i + d[i] + 1]) {
            d[i]++;
        }
        if (i + d[i] > r) {
            l = i - d[i] + 1, r = i + d[i];
        }
    }
    return d;
}
