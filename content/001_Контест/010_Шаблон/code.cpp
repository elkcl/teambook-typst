#ifndef ONPC
#pragma GCC optimize("O3")
#pragma GCC target("avx2", "avx", "sse", "sse2", "sse3", "ssse3", "sse4",      \
                   "sse4.1", "sse4.2", "lzcnt", "popcnt", "abm", "bmi",        \
                   "bmi2")
#pragma GCC optimize("unroll-loops")
#endif
#include "preamble.hpp"
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template <typename T>
using OrderedSet =
    tree<T, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update>;

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

void solve() { /* ... */ }

int32_t main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);

    int t = 1;
    // cin >> t;

    while (t--) {
        solve();
    }
}
