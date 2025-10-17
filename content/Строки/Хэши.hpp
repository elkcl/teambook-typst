ll m = 1e9 + 7;
ll k = 53;
vll h1(n);
vll kp(n + 1);
kp[0] = 1;
h1[0] = mod((s1[0] - 'A' + 1), m);
for (int i = 1; i < n; ++i) {
  kp[i] = mod((kp[i - 1] * k), m);
  ll c1 = s1[i] - 'A' + 1;
  h1[i] = mod((h1[i - 1] * k + c1), m);
}
kp[n] = mod((kp[n - 1] * k), m);
auto ss1 = [&h1, &kp, m](int l, int r) -> ll {
  if (l == 0) return h1[r];
  return mod((h1[r] - h1[l - 1] * kp[r - l + 1]),
             m);
};
