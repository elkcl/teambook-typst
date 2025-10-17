vvi dist(k + 1, vi(n, INF));
dist[0][s] = 0;
for (int i = 1; i < k + 1; ++i) {
  for (auto&& [from, to, w] : g) {
    if (dist[i - 1][from] != INF)
      dist[i][to] =
          min(dist[i][to], dist[i - 1][from] + w);
  }
}
