#include <bits/stdc++.h>
using namespace std;

struct Edge {
  int u, f, c;
  Edge* rev;

  Edge(int u_, int f_, int c_) {
    u = u_;
    f = f_;
    c = c_;

    rev = nullptr;
  }
};

const int MAXN = 1e5 + 10;

int n, m, s, f;
vector<Edge*> g[MAXN];
vector<bool> used(MAXN);
vector<int> path;

int dfs(int v, int min_flow = 1e9) {
  if (v == f) {
    return min_flow;
  }

  used[v] = true;
  for (auto e : g[v]) {
    if (e->f < e->c && !used[e->u]) {
      int flow =
          dfs(e->u, min(min_flow, e->c - e->f));
      if (flow > 0) {
        e->f += flow;
        e->rev->f -= flow;
        return flow;
      }
    }
  }

  return 0;
}

int32_t main() {
  cin.tie(0);
  cout.tie(0);
  ios::sync_with_stdio(0);

  cin >> n >> m;

  s = 0;
  f = n - 1;
  vector<Edge*> edges;
  for (int i = 0; i < m; i++) {
    int v, u, w;
    cin >> v >> u >> w;

    v--;
    u--;

    if (u == v) {
      continue;
    }

    Edge* normal = new Edge(u, 0, w);
    Edge* rev = new Edge(v, 0, w);

    normal->rev = rev;
    rev->rev = normal;

    g[v].push_back(normal);
    g[u].push_back(rev);
    edges.push_back(normal);
  }

  int flow = 0;
  while (true) {
    used.assign(n, 0);
    int new_flow = dfs(s);
    if (new_flow == 0) {
      break;
    }

    flow += new_flow;
  }

  cout << flow << "\n";
  for (auto e : edges) {
    cout << e->f << "\n";
  }

  return 0;
}
