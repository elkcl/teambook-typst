#include "preamble.hpp"
// границы и номер запроса, на который нужно
// ответить
struct Query {
    int l, r, idx;
};

const int MAXQ = MAXN;
int a[MAXN],
    ans[MAXQ]; // исходный массив и массив
               // ответов на запросы
vector<vector<Query>> b;
int cnt[MAXN];
int res;

void add(int k) {
    if (cnt[a[k]]++ == 0) {
        res++;
    }
}

void del(int k) {
    if (--cnt[a[k]] == 0) {
        res--;
    }
}

void mo(int c, const vector<Query> &queries) {
    b.assign(c, {});
    for (Query q : queries) {
        b[q.l / c].push_back(q);
    }
    for (int i = 0; i < c; i++) {
        sort(b[i].begin(), b[i].end(),
             [](Query a, Query b) { return a.r < b.r; });
    }
    for (int i = 0; i < c; i++) {
        // обнуляем переменные
        int l = i * c, r = i * c - 1;
        memset(cnt, 0, sizeof cnt);
        res = 0;
        for (Query q : b[i]) {
            // пока правая граница не дошла до границы
            // запроса
            while (r < q.r) {
                add(++r);
            }
            // дальше делаем так, чтобы левая граница
            // совпала
            while (l < q.l) {
                del(l++);
            }
            while (l > q.l) {
                add(--l);
            }
            ans[q.idx] = res;
        }
    }
}
