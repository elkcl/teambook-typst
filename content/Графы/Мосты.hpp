void dfs(int v, int p = -1) {
  used[v] = true;
  d[v] = h[v] = (p == -1 ? 0 : h[p] + 1);
  for (int u : g[v]) {
    if (u != p) {
      if (used[u])  // если ребро обратное
        d[v] = min(d[v], h[u]);
      else {  // если ребро прямое
        dfs(u, v);
        d[v] = min(d[v], d[u]);
        if (h[v] < d[u]) {
          // если нельзя другим путем добраться в
          // v или выше, то ребро (v, u) -- мост
        }
      }
    }
  }
}
