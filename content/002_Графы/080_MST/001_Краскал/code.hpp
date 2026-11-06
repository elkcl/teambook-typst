#include "../../../003_Структуры данных/050_Система непересекающихся множеств/code.hpp"

optional<vpii> kruskal(vector<vpii> &g) {
    int n = g.size();
    vpii answer;
    DSU dsu(n);
    vector<pair<int, pii>> edges;
    for (int i = 0; i < n; i++) {
        for (auto e : g[i]) {
            edges.push_back({e.second, {i, e.first}});
        }
    }
    sort(all(edges));
    for (auto e : edges) {
        int u = e.second.first, v = e.second.second, w = e.first;
        if (dsu.unite(u, v)) {
            answer.push_back({u, v});
        }
    }
    if (answer.size() != n - 1) {
        return nullopt; // граф не связный
    }
    return answer;
}