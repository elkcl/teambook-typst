#include <bits/stdc++.h>
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

const int MAXN = 1e5;
const int INF = 1e9;
const ll INF_LL = 1e18;
const ld EPS = 1e-10;
