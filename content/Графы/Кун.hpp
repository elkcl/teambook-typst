vi lr(k, -1), rl(n, -1);
vi used(k, 0);
int curr = 1;
auto dfs = [&g, &used, &lr, &rl, &curr](
               const auto &rec, int v) -> bool {
  if (used[v] == curr) return false;
  used[v] = curr;
  for (int to : g[v]) {
    if (rl[to] == -1 || rec(rec, rl[to])) {
      rl[to] = v;
      lr[v] = to;
      return true;
    }
  }
  return false;
};
ll ans = 0;
for (bool run = true; run;) {
  run = false;
  ++curr;
  for (int i = 0; i < k; ++i) {
    if (lr[i] == -1 && dfs(dfs, i)) {
      run = true;
      ++ans;
    }
  }
}
