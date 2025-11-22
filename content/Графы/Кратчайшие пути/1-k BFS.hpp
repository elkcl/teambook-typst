int d[maxn];
d[s] = 0;
queue<int> q[maxd];
q[0].push_back(s);
for (int dist = 0; dist < maxd; dist++) {
  while (!q[dist].empty()) {
    int v = q[dist].front();
    q[dist].pop();
    if (d[v] > dist)
      continue;
    for (auto [u, w] : g[v]) {
      if (d[u] < d[v] + w) {
        d[u] = d[v] + w;
        q[d[u]].push(u);
      }
    }
  }
}
