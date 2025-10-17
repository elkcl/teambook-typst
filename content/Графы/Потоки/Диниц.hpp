using ll = long long;

struct Edge {
  int u;
  ll f, c;
  Edge* rev;

  Edge(int u_, ll f_, ll c_) {
    u = u_;
    f = f_;
    c = c_;

    rev = nullptr;
  }
};

const int MAXN = 1000;
const ll INF = 1e18;

int n, m, s, f;
vector<Edge*> g[MAXN];
vector<ll> d(MAXN), p(MAXN);

bool bfs() {
  d.assign(MAXN, INF);
  d[s] = 0;

  queue<int> q;
  q.push(s);

  while (!q.empty()) {
    int v = q.front();
    q.pop();

    for (auto e : g[v]) {
      if (e->f < e->c && d[e->u] == INF) {
        d[e->u] = d[v] + 1;
        q.push(e->u);
      }
    }
  }

  return d[f] != INF;
}

int dfs(int v, ll min_flow = INF) {
  if (v == f) {
    return min_flow;
  }

  for (; p[v] < g[v].size(); p[v]++) {
    auto e = g[v][p[v]];
    if (e->f < e->c && d[e->u] == d[v] + 1) {
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

vector<Edge*> edges;
void addEdge(int v, int u, ll w = 0) {
  Edge* normal = new Edge(u, 0, w);
  Edge* rev = new Edge(v, 0, 0);

  normal->rev = rev;
  rev->rev = normal;

  g[v].push_back(normal);
  g[u].push_back(rev);
}

int32_t main() {
  cin.tie(0);
  cout.tie(0);
  ios::sync_with_stdio(0);

  cin >> n >> m;

  s = 0;
  f = n - 1;

  for (int i = 0; i < m; i++) {
    int v, u;
    ll w;
    cin >> v >> u >> w;

    addEdge(v - 1, u - 1, w);
  }

  ll flow = 0;
  while (bfs()) {
    p.assign(MAXN, 0);
    while (true) {
      ll new_flow = dfs(s);
      if (new_flow == 0) {
        break;
      }

      flow += new_flow;
    }
  }

  cout << flow;
  return 0;
}
