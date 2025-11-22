int encode_perm(vi perm) {
  int n = perm.size();
  int val = 0;
  for (int i = 0; i < n; i++) {
    val += fac[n - 1 - i] * (perm[i]);
    for (int j = i + 1; j < n; j++)
      if (perm[j] > perm[i])
        perm[j]--;
  }
  return val;
}
vi decode_perm(int val, int base) {
  vi perm(base);
  vi amogus;
  for (int i = 0; i < base; i++)
    amogus.push_back(i);
  for (int i = 0; i < base; i++) {
    int j = val / fac[base - i - 1];
    perm[i] = amogus[j];
    amogus.erase(amogus.begin() + j);
    val %= fac[base - i - 1];
  }
  return perm;
}
