const ll MOD = 998'244'353;
const ll BLOCK =
    500;  // типа sqrt(MOD), но подбирать ручками

ll dlog(ll a, ll b, unordered_map<ll, ll> &fs) {
  ll an = binpow(a, BLOCK, MOD);
  unordered_map<ll, ll> fs;
  ll curr = an;
  for (int i = 1; i <= MOD / BLOCK + 1; ++i) {
    if (fs.find(curr) == fs.end()) fs[curr] = i;
    curr = (curr * an) % MOD;
  }
  ll curr = b;
  for (int i = 0; i <= BLOCK; ++i) {
    if (fs.find(curr) != fs.end()) {
      ll ans = fs[curr] * BLOCK - i;
      if (ans < MOD) return ans;
    }
    curr = (curr * a) % MOD;
  }
  return -1;
}
