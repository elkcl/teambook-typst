#ifndef ONPC
#pragma GCC optimize("O3")
#pragma GCC target("avx2", "avx", "sse", "sse2", "sse3", "ssse3", "sse4",      \
                   "sse4.1", "sse4.2", "lzcnt", "popcnt", "abm", "bmi",        \
                   "bmi2")
#pragma GCC optimize("unroll-loops")
#endif
#include "preamble.hpp"

template <typename T> istream &operator>>(istream &s, vector<T> &v) {
    for (auto &&el : v) {
        s >> el;
    }
    return s;
}

template <typename T> ostream &operator<<(ostream &s, const vector<T> &v) {
    for (auto &&el : v) {
        s << el << ' ';
    }
    s << '\n';
    return s;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
}
