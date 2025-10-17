using ll = long long;

struct Edge {
  int v, u;
  ll f, c;
  ll w;
  Edge* rev;

  Edge(int v_, int u_, ll f_, ll c_, ll w_) {
    v = v_;
    u = u_;
    f = f_;
    c = c_;
    w = w_;

    rev = nullptr;
  }
};

const int MAXN = 150;
const ll INF = 1e18;

ll ans = 0;
int n, m, s, f;
vector<Edge*> edges, p(MAXN);
vector<ll> d(MAXN);

void addEdge(int v, int u, ll c = 0, ll w = 0) {
  Edge* normal = new Edge(v, u, 0, c, w);
  Edge* rev = new Edge(u, v, 0, 0, -w);

  normal->rev = rev;
  rev->rev = normal;

  edges.push_back(normal);
  edges.push_back(rev);
}

ll ford_bellman() {
  d.assign(MAXN, INF);
  p.assign(MAXN, nullptr);
  d[s] = 0;

  ll flow = INF;
  for (int i = 1; i < n; i++) {
    for (auto e : edges) {
      if (e->c > e->f &&
          d[e->u] > d[e->v] + e->w) {
        d[e->u] = d[e->v] + e->w;
        p[e->u] = e;
        flow = min(flow, e->c - e->f);
      }
    }
  }

  if (d[f] == INF) {
    return 0;
  }

  Edge* e = p[f];
  while (e != nullptr) {
    e->f += flow;
    e->rev->f -= flow;

    e = p[e->v];
  }

  ans += flow * d[f];
  return flow;
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
    ll c, w;
    cin >> v >> u >> c >> w;

    addEdge(v - 1, u - 1, c, w);
  }

  ll flow = 0;
  while (true) {
    ll new_flow = ford_bellman();
    if (new_flow == 0) {
      break;
    }

    flow += new_flow;
  }

  cerr << flow << "\n";
  cout << ans;
  return 0;
}
