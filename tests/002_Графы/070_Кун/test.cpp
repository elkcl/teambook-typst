#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/002_Графы/070_Кун/code.hpp"

bool is_maximum_matching(const vvi &g, int k, int n, const vpii &matching) {
    if ((int)g.size() < k) {
        ADD_FAILURE() << "в графе меньше левых вершин, чем k";
        return false;
    }
    vi ml(k, -1), mr(n, -1);
    for (auto [l, r] : matching) {
        if (l < 0 || l >= k || r < 0 || r >= n) {
            ADD_FAILURE() << "пара вне долей: " << l << " " << r;
            return false;
        }
        if (ml[l] != -1 || mr[r] != -1) {
            ADD_FAILURE() << "вершина покрыта дважды: " << l << " " << r;
            return false;
        }
        bool exists = false;
        for (int to : g[l]) {
            if (to == r) {
                exists = true;
            }
        }
        if (!exists) {
            ADD_FAILURE() << "нет ребра " << l << " -> " << r;
            return false;
        }
        ml[l] = r;
        mr[r] = l;
    }

    vi seen(k, 0);
    int stamp = 0;
    auto dfs = [&](auto &&self, int v) -> bool {
        if (seen[v] == stamp) {
            return false;
        }
        seen[v] = stamp;
        for (int to : g[v]) {
            if (to < 0 || to >= n) {
                continue;
            }
            int other = mr[to];
            if (other == -1 || self(self, other)) {
                return true;
            }
        }
        return false;
    };
    for (int v = 0; v < k; ++v) {
        if (ml[v] != -1) {
            continue;
        }
        ++stamp;
        if (dfs(dfs, v)) {
            ADD_FAILURE() << "есть увеличивающая цепь из " << v << ", размер паросочетания " << matching.size();
            return false;
        }
    }
    return true;
}

int brute_size(const vvi &g, int k, int n) {
    int best = 0;
    vi used(n, 0);
    auto rec = [&](auto &&self, int i, int cnt) -> void {
        if (cnt + (k - i) <= best) {
            return;
        }
        if (i == k) {
            best = std::max(best, cnt);
            return;
        }
        for (int to : g[i]) {
            if (to < 0 || to >= n || used[to]) {
                continue;
            }
            used[to] = 1;
            self(self, i + 1, cnt + 1);
            used[to] = 0;
        }
        self(self, i + 1, cnt);
    };
    rec(rec, 0, 0);
    return best;
}

void check(vvi g, int k, int n, bool with_brute) {
    ASSERT_EQ((int)g.size(), k);
    auto matching = get_matching(g, k, n);
    EXPECT_TRUE(is_maximum_matching(g, k, n, matching));
    if (with_brute) {
        EXPECT_EQ((int)matching.size(), brute_size(g, k, n));
    }
}

vvi empty_left(int k) { return vvi(k); }

}  // namespace

TEST(a_002_Grafy_070_Kun, EmptySides) {
    check({}, 0, 0, true);
    check({}, 0, 4, true);
    check(empty_left(3), 3, 0, true);
    check(empty_left(4), 4, 5, true);
}

TEST(a_002_Grafy_070_Kun, SingleVertex) {
    check(empty_left(1), 1, 1, true);
    vvi g(1);
    g[0] = {0};
    check(g, 1, 1, true);
    g[0] = {0, 0, 0};
    check(g, 1, 1, true);
}

TEST(a_002_Grafy_070_Kun, OneRightVertex) {
    vvi g(5);
    for (int i = 0; i < 5; ++i) {
        g[i] = {0};
    }
    check(g, 5, 1, true);
    g[0].clear();
    g[4].clear();
    check(g, 5, 1, true);
}

TEST(a_002_Grafy_070_Kun, DisjointEdges) {
    vvi g(4);
    g[0] = {0};
    g[1] = {1};
    g[2] = {2};
    g[3] = {3};
    check(g, 4, 4, true);
    check(g, 4, 6, true);
}

TEST(a_002_Grafy_070_Kun, PerfectMatchingNeedsRematch) {
    vvi g(4);
    g[0] = {0};
    g[1] = {0, 1};
    g[2] = {1, 2};
    g[3] = {2, 3};
    check(g, 4, 4, true);

    vvi reversed(4);
    reversed[0] = {0};
    reversed[1] = {1, 0};
    reversed[2] = {2, 1};
    reversed[3] = {3, 2};
    check(reversed, 4, 4, true);
}

TEST(a_002_Grafy_070_Kun, OnlyLastLeftVertexIsUseful) {
    vvi g(4);
    g[3] = {0, 1, 2};
    check(g, 4, 3, true);
}

TEST(a_002_Grafy_070_Kun, OnlyFirstLeftVertexIsUseful) {
    vvi g(4);
    g[0] = {2, 0, 1};
    check(g, 4, 3, true);
}

TEST(a_002_Grafy_070_Kun, HallViolator) {
    vvi g(3);
    g[0] = {0, 1};
    g[1] = {1, 0};
    g[2] = {0};
    check(g, 3, 2, true);
}

TEST(a_002_Grafy_070_Kun, CompleteBipartite) {
    for (int k = 0; k <= 6; ++k) {
        for (int n = 0; n <= 6; ++n) {
            vvi g(k);
            for (int i = 0; i < k; ++i) {
                g[i].resize(n);
                std::iota(g[i].begin(), g[i].end(), 0);
            }
            check(g, k, n, true);
        }
    }
}

TEST(a_002_Grafy_070_Kun, IsolatedInTheMiddle) {
    vvi g(5);
    g[0] = {0};
    g[2] = {1, 2};
    g[4] = {2};
    check(g, 5, 3, true);
}

TEST(a_002_Grafy_070_Kun, DuplicateEdgesAndSkippedRight) {
    vvi g(3);
    g[0] = {2, 2, 2};
    g[1] = {2, 0, 0};
    g[2] = {0};
    check(g, 3, 4, true);
}

TEST(a_002_Grafy_070_Kun, CycleShiftForcesAugmentingChains) {
    for (int n : {1, 2, 3, 5, 8, 20}) {
        vvi g(n);
        for (int i = 0; i < n; ++i) {
            g[i] = {i, (i + 1) % n};
        }
        check(g, n, n, n <= 8);
    }
}

TEST(a_002_Grafy_070_Kun, LeftLargerAndRightLarger) {
    vvi left_heavy(5);
    for (int i = 0; i < 5; ++i) {
        left_heavy[i] = {i % 2};
    }
    check(left_heavy, 5, 2, true);

    vvi right_heavy(2);
    right_heavy[0] = {0, 2, 4};
    right_heavy[1] = {1, 3};
    check(right_heavy, 2, 6, true);
}

TEST(a_002_Grafy_070_Kun, RandomVersusBrute) {
    std::mt19937 rng(701);
    for (int it = 0; it < 80; ++it) {
        int k = (int)(rng() % 8);
        int n = (int)(rng() % 8);
        vvi g(k);
        int m = (int)(rng() % 16);
        for (int i = 0; i < m && k > 0 && n > 0; ++i) {
            int l = (int)(rng() % k);
            int r = (int)(rng() % n);
            g[l].push_back(r);
        }
        check(g, k, n, true);
    }
}

TEST(a_002_Grafy_070_Kun, StressMedium) {
    std::mt19937 rng(702);
    for (int it = 0; it < 20; ++it) {
        int k = 20 + (int)(rng() % 30);
        int n = 20 + (int)(rng() % 30);
        vvi g(k);
        for (int l = 0; l < k; ++l) {
            int deg = (int)(rng() % 4);
            for (int j = 0; j < deg; ++j) {
                g[l].push_back((int)(rng() % n));
            }
        }
        check(g, k, n, false);
    }
}

TEST(a_002_Grafy_070_Kun, Load) {
    std::mt19937 rng(703);
    const int n = 400;
    vvi shifted(n);
    for (int i = 0; i < n; ++i) {
        shifted[i] = {i, (i + 1) % n, (i + 7) % n};
    }
    check(shifted, n, n, false);

    vvi random_g(500);
    for (int i = 0; i < 500; ++i) {
        for (int j = 0; j < 8; ++j) {
            random_g[i].push_back((int)(rng() % 500));
        }
    }
    check(random_g, 500, 500, false);

    vvi complete(80);
    for (int i = 0; i < 80; ++i) {
        complete[i].resize(90);
        std::iota(complete[i].begin(), complete[i].end(), 0);
    }
    check(complete, 80, 90, false);
}
