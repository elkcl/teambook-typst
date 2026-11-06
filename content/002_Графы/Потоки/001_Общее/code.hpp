#include "preamble.hpp"

struct Edge {
    int v, u; // v нужно при списке ребер
    ll f = 0, c;
    ll w = 0; // для минкоста
    Edge *rev = nullptr;

    Edge(int v, int u, ll c) : v(v), u(u), c(c) {}
};

void addEdge(vector<vector<Edge *>> &g, int v, int u, ll c = 0) {
    Edge *normal = new Edge(v, u, c);
    Edge *rev = new Edge(u, v, 0);
    normal->rev = rev;
    rev->rev = normal;
    g[v].push_back(normal);
    g[u].push_back(rev);
}
