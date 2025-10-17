vi logs(n + 1, 0);
int last_pow = 1;
for (int i = 2; i <= n; ++i) {
  logs[i] = logs[i - 1];
  if (i >= last_pow * 2) {
    last_pow *= 2;
    ++logs[i];
  }
}

vvi sp(n, vi(logs[n] + 1, INF));
for (int i = 0; i < n; ++i) sp[i][0] = a[i];
for (int l = 1; l < logs[n] + 1; ++l) {
  for (int i = 0; i < n; ++i) {
    if (i + (1 << (l - 1)) >= n) break;
    sp[i][l] = min(sp[i][l - 1],
                   sp[i + (1 << (l - 1))][l - 1]);
  }

  while (q--) {
    int l, r;
    cin >> l >> r;
    int len = logs[r - l];
    cout << min(sp[l][len],
                sp[r - (1 << len)][len])
         << '\n';
  }
