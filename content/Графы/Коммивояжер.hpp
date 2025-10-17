for (int i = 0; i < n; i++) {
        dp[(1 << i)][i] = 0;
    }

    for (int mask = 1; mask < (1 << n); mask++) {
        for (int v = 0; v < n; v++) {
            if ((mask & (1 << v)) == 0) {
                continue;
            }

            int submask = mask ^ (1 << v);
            for (int u = 0; u < n; u++) {
                if (u == v || (submask & (1 << u)) == 0 || g[u][v] == -1) {
                    continue;
                }

                ll answer = dp[submask][u] + g[u][v];
                if (answer < dp[mask][v]) {
                    dp[mask][v] = answer;
                }
            }
        }
    }

    int mask = (1 << n) - 1;
    ll answer = INF;
    for (int v = 0; v < n; v++) {
        if (dp[mask][v] < answer) {
            answer = dp[mask][v];
        }
    }
