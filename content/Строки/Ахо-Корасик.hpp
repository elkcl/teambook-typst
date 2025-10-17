// Здесь код какой-то задачи на Ахо-Корасик
// Вряд ли понадобится, и я уже не помню, что
// здесь происходит

struct query {
  int l, r;
};

bool operator<(const query &a, const query &b) {
  if (a.l == b.l) return a.r < b.r;
  return a.l < b.l;
}

const int ALPHA = 26;
const char OFF = 'a';

struct Node {
  array<int, ALPHA> next;
  int p, link, zip;
  bool term;
  int pchar;
  int len;
};

struct Trie {
  vector<Node> t;
  int n = 0;

  Trie(const vector<string> &vec) {
    create(-1, -1);
    for (auto &s : vec) insert(s);
    build();
  }

  int create(int p, int pchar) {
    t.push_back({});
    fill(all(t[n].next), -1);
    t[n].p = p;
    t[n].pchar = pchar;
    t[n].link = t[n].zip = -1;
    t[n].term = false;
    t[n].len = -1;
    ++n;
    return n - 1;
  }

  void insert(const string &s, int pos = 0,
              int v = 0) {
    if (pos == static_cast<int>(s.size())) {
      t[v].term = true;
      t[v].len = pos;
      return;
    }
    int c = s[pos] - OFF;
    if (t[v].next[c] == -1)
      t[v].next[c] = create(v, c);
    insert(s, pos + 1, t[v].next[c]);
  }

  void build() {
    deque<int> q;
    for (int i = 0; i < ALPHA; ++i) {
      if (t[0].next[i] == -1)
        t[0].next[i] = 0;
      else
        q.push_back(t[0].next[i]);
    }
    t[0].link = 0;

    while (!q.empty()) {
      int v = q.front();
      q.pop_front();
      if (t[v].p == 0)
        t[v].link = 0;
      else
        t[v].link =
            t[t[t[v].p].link].next[t[v].pchar];
      if (t[t[v].link].term)
        t[v].zip = t[v].link;
      else if (t[v].link == 0)
        t[v].zip = -1;
      else
        t[v].zip = t[t[v].link].zip;
      for (int i = 0; i < ALPHA; ++i) {
        if (t[v].next[i] == -1)
          t[v].next[i] = t[t[v].link].next[i];
        else
          q.push_back(t[v].next[i]);
      }
    }
  }
};

struct vertex {
  vi e;
  int d;
};

void dfs(vector<vertex> &g, vi &tin, vi &tout,
         int &t, int v, int p, int d) {
  tin[v] = t++;
  g[v].d = d;
  for (int to : g[v].e) {
    if (to == p) continue;
    dfs(g, tin, tout, t, to, v, d + 1);
  }
  tout[v] = t;
}

int32_t main() {
  fast_io

      int n;
  cin >> n;
  vector<string> vec(n);
  for (auto &s : vec) {
    cin >> s;
    reverse(all(s));
  }
  Trie trie(vec);

  string s;
  cin >> s;
  int len = static_cast<int>(s.size());
  vi v(len);
  int curr = 0;
  for (int i = len - 1; i >= 0; --i) {
    curr = trie.t[curr].next[s[i] - OFF];
    v[i] = curr;
  }
  vector<vertex> g(len + 1);
  for (int i = len - 1; i >= 0; --i) {
    int ln = -1;
    int currv = v[i];
    if (trie.t[currv].term)
      ln = trie.t[currv].len;
    if (ln == -1 && trie.t[currv].zip != -1) {
      currv = trie.t[currv].zip;
      ln = trie.t[currv].len;
    }
    if (ln != -1) {
      g[i + ln].e.push_back(i);
    }
  }
  vi tin(len + 1, -1), tout(len + 1, -1);
  int t = 0;
  for (int root = len; root >= 0; --root) {
    if (tin[root] == -1)
      dfs(g, tin, tout, t, root, -1, 0);
  }

  auto anc = [&tin, &tout](int u, int v) -> bool {
    if (tin[u] == -1 || tin[v] == -1)
      return false;
    return tin[u] <= tin[v] && tin[v] < tout[u];
  };

  int m;
  cin >> m;
  while (m--) {
    int l, r;
    cin >> l >> r;
    --l;
    if (!anc(l, r) && !anc(r, l))
      cout << "-1\n";
    else
      cout << abs(g[l].d - g[r].d) << '\n';
  }

  int k;
  cin >> k;
  while (k--) {
    ll sum = 0;
    int T, a, b, c, d, e, li, ri;
    cin >> T >> a >> b >> c >> d >> e >> li >> ri;
    int l, r;
    for (int i = 0; i < T; ++i) {
      l = min(li % len, ri % len);
      r = max(li % len + 1, ri % len + 1);
      int ans = 0;
      if (!anc(l, r) && !anc(r, l)) {
        ans = -1;
      } else {
        ans = abs(g[l].d - g[r].d);
        sum += ans;
        sum %= e;
      }
      li = (a * li + b) % e;
      ri = (c * ri + d + ans) % e;
    }
    cout << sum << '\n';
  }
}
