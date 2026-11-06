#include "preamble.hpp"
struct Edge {
    int to, id;
};

int dfs(vector<vector<Edge>> &g, vector<char> &color, vector<Edge> &p, int v) {
    color[v] = 1;
    for (auto [to, e] : g[v]) {
        if (color[to] == 0) {
            p[to] = {v, e};
            int res = dfs(g, color, p, to);
            if (res != -1) {
                return res;
            }
        } else if (color[to] == 1) {
            if (p[v].to == to && p[v].id == e) {
                continue;
            }
            p[to] = {v, e};
            return to;
        }
    }
    color[v] = 2;
    return -1;
}

optional<pair<vi, vi>> find_cycles(vector<vector<Edge>> &g) {
    size_t n = g.size();
    vector<char> color(n, 0);
    vector<Edge> p(n, {-1, -1});
    for (size_t v = 0; v < n; ++v) {
        if (color[v] != 0) {
            continue;
        }
        int end = dfs(g, color, p, v);
        if (end == -1) {
            continue;
        }
        auto cur = p[end];
        vi vs{cur.to}, es{cur.id};
        for (cur = p[cur.to]; cur.to != vs[0]; cur = p[cur.to]) {
            vs.push_back(cur.to);
            es.push_back(cur.id);
        }
        reverse(all(vs));
        reverse(all(es));
        return pair{vs, es};
    }
    return nullopt;
}
