vvi dist(n, vi(n, INF));
for (int i = 0; i < n; ++i) {
  dist[i][i] = 0;
}
for (int i = 0; i < m; ++i) {
  ll a, b, w;
  cin >> a >> b >> w;
  dist[a - 1][b - 1] = min(w, dist[a - 1][b - 1]);
}

for (int k = 0; k < n; ++k) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (dist[i][k] != INF &&
          dist[k][j] != INF) {
        dist[i][j] = min(
            dist[i][j],
            max(dist[i][k] + dist[k][j], -INF));
      }
    }
  }
}
