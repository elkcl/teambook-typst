#include "preamble.hpp"
#include <cstddef>

struct CentroidDecomposition {
    vvi &g;
    vi centroid_parent;
    vi sizes;
    vc dead;

    CentroidDecomposition(vvi &_g) : g(_g) {
        if (g.empty()) {
            return;
        }
        centroid_parent.resize(g.size(), -1);
        sizes.resize(g.size(), -1);
        dead.resize(g.size(), false);
        for (size_t i = 0; i < g.size(); ++i) {
            if (!dead[i]) {
                build(i);
            }
        }
    }

    int count_size(int cur_v, int from) {
        vpii order{{cur_v, from}};

        for (int i = 0; i < static_cast<int>(order.size()); ++i) {
            auto [v, parent] = order[i];
            sizes[v] = 1;
    
            for (int to : g[v]) {
                if (to != parent && !dead[to]) {
                    order.emplace_back(to, v);
                }
            }
        }
    
        for (int i = static_cast<int>(order.size()) - 1; i > 0; --i) {
            auto [v, parent] = order[i];
            sizes[parent] += sizes[v];
        }
    
        return sizes[cur_v];
    }
 
    int find_centroid(int cur_v, int from, int total) {
        for (auto & to : g[cur_v]) {
            if (to == from || dead[to]) {
                continue;
            }
            if (sizes[to] > total / 2) {
                return find_centroid(to, cur_v, total);
            }
        }
        return cur_v;
    }
 
    int build(int cur_v) {
        int total = count_size(cur_v, -1);
 
        int centroid = find_centroid(cur_v, -1, total);
 
        dead[centroid] = true;
 
        for (auto & to : g[centroid]) {
            if (dead[to]) {
                continue;
            }
 
            int child_centroid = build(to);
            centroid_parent[child_centroid] = centroid;
        }
 
        return centroid;
    }
};