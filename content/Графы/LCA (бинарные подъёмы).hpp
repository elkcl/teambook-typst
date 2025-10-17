void dfs1(vvi &g, vc &used, vvi &up, vi &tin,
          vi &tout, int logn, int &t, int v) {
  for (int i = 1; i < logn; ++i)
    up[v][i] = up[up[v][i - 1]][i - 1];
  tin[v] = t++;
  used[v] = true;
  for (int to : g[v]) {
    if (!used[to]) {
      up[to][0] = v;
      dfs1(g, used, up, tin, tout, logn, t, to);
    }
  }
  tout[v] = t;
}

int logn = static_cast<int>(ceil(log2(n))) + 1;
vvi up(n, vi(logn, 0));
int t = 0;
dfs1(g, used, up, tin, tout, logn, t, 0);
auto anc = [&tin, &tout](int u, int v) {
  return tin[u] <= tin[v] && tin[v] < tout[u];
};

int lca(int v, int u) {
  if (anc(v, u)) return v;
  if (anc(u, v)) return u;
  for (int l = logn - 1; l >= 0; l--)
    if (!anc(up[v][l], u)) v = up[v][l];
  return up[v][0];
}
