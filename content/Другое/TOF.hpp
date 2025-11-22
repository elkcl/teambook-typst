template <typename F>
ll tof(ll A, ll B, const F &f, ll S = 20) {
  auto block = [&](ll blk) {
    ll ans = f(S * blk);
    for (ll i = S * blk;
         i <= S * (blk + 1) && i <= B; i++) {
      ans = max(ans, f(i));
    }
    return ans;
  };
  ll l = A / S, r = B / S + 1;
  while (r - l > 4) {
    ll m1 = (l * 2 + r) / 3;
    ll m2 = (l + r * 2) / 3;
    if (block(m1) > block(m2))
      l = m1;
    else
      r = m2;
  }
  ll ans = l * S;
  for (ll i = l * S; i <= r * S && i <= B; i++)
    if (f(ans) > f(i))
      ans = i;
  return ans;
}
