#pragma once
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;

template <typename T>
using OrderedSet =
    tree<T, null_type, less<>, rb_tree_tag, tree_order_statistics_node_update>;

using namespace std;
using ll = int64_t;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;
using vpii = vector<pii>;
using vpll = vector<pll>;
using vvi = vector<vi>;
using vc = vector<char>;
using vvc = vector<vc>;
using ui = uint32_t;
using ull = uint64_t;
using ld = long double;

#define all(x) (x).begin(), (x).end()
#define sz(x) int((x).size())
#define pb push_back
#define ins insert
#define nl '\n'

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

ll rndll() { return uniform_int_distribution<ll>(LLONG_MIN, LLONG_MAX)(rng); }

const int maxn = 1e5;
const int inf = 1e9;
const ll INF = 1e18;
const ld EPS = 1e-10;
