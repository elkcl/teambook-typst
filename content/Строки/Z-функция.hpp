void z_func(int n) {
  z[0] = n;
  int l = 0, r = 0;
  for (int i = 1; i < n; ++i) {
    z[i] = MIN(r - i, z[i - l]);
    if (z[i] < 0) z[i] = 0;
    while (i + z[i] < n && s[z[i]] == s[i + z[i]])
      ++z[i];
    if (i + z[i] > r) {
      l = i;
      r = i + z[i];
    }
  }
}
