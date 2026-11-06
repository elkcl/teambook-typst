#include "preamble.hpp"

tuple<int, vi, vi, vi> lis(const vi &a) {
    if (a.empty()) {
        return {};
    }

    vi dp(sz(a) + 1, INF);
    vi pos(sz(a) + 1, -1), prev(sz(a) + 1, -1);
    dp[0] = -INF;

    int length = 0;
    for (int i = 0; i < sz(a); i++) {
        // index of the first tail greater than a[i]
        int j = upper_bound(dp.begin() + 1, dp.begin() + length + 1, a[i]) -
                dp.begin();
        if (j == length + 1) {
            ++length;
        }
        if (j == 1 || dp[j - 1] < a[i]) {
            dp[j] = a[i];
            pos[j] = i;
            prev[j] = pos[j - 1];
        }
    }

    return {length, std::move(dp), std::move(pos), std::move(prev)};
}
