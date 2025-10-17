#ifndef ONPC
#pragma GCC optimize("O3")
#pragma GCC target( \
    "avx2,avx,sse,sse2,\
sse3,ssse3,sse4,sse4.1,sse4.2,\
lzcnt,popcnt,abm,bmi,bmi2")
#pragma GCC optimize("unroll-loops")
#endif

#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using ll = int64_t;
using ii = pair<int, int>;
using vi = vector<int>;
using vll = vector<ll>;
using vii = vector<ii>;
using vvi = vector<vi>;
using vc = vector<char>;
using vvc = vector<vc>;
using ui = uint32_t;
using ull = uint64_t;
using ld = long double;

using namespace __gnu_pbds;
template <typename T>
using OrderedSet<T>
    tree<T, null_type, less<>, rb_tree_tag,
         tree_order_statistics_node_update>;

#define all(x) (x).begin(), (x).end()
#define nl '\n'

template <typename T>
istream& operator>>(istream& s, vector<T>& v) {
  for (auto&& el : v) s >> el;
  return s;
}

template <typename T>
ostream& operator<<(ostream& s,
                    const vector<T>& v) {
  for (auto&& el : v) s << el << ' ';
  s << '\n';
  return s;
}

signed main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
}
