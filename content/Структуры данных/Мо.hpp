// границы и номер запроса, на который нужно
// ответить
struct query {
  int l, r, idx;
};

int a[maxn],
    ans[maxq];  // исходный массив и массив
                // ответов на запросы
vector<query> b[c];

// где-то в main:

for (query q : queries) b[q.l / c].push_back(q);

for (int i = 0; i < c; i++) {
  sort(
      b[i].begin(), b[i].end(),
      [](query a, query b) { return a.r < b.r; });
}

int cnt[maxn];
int res;

void add(int k) {
  if (cnt[a[k]]++ == 0) res++;
}

void del(int k) {
  if (--cnt[a[k]] == 0) res--;
}

for (int i = 0; i < c; i++) {
  // обнуляем переменные
  int l = i * c, r = i * c - 1;
  memset(cnt, 0, sizeof cnt);
  res = 0;
  for (query q : b[i]) {
    // пока правая граница не дошла до границы
    // запроса
    while (r < q.r) add(++r);
    // дальше делаем так, чтобы левая граница
    // совпала
    while (l < q.l) del(l++);
    while (l > q.l) add(--l);
    ans[q.idx] = res;
  }
}
