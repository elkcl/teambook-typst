void pref_func(size_t n, const char str[n],
               int p[n]) {
  p[0] = 0;
  for (int i = 1; i < n; ++i) {
    int k = p[i - 1];
    while (k > 0 && str[i] != str[k])
      k = p[k - 1];
    if (str[i] == str[k]) ++k;
    p[i] = k;
  }
}
