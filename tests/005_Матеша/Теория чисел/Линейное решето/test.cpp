#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/005_Матеша/Теория чисел/Линейное решето/code.hpp"

void ensure() {
    if (pr.empty()) {
        build_linear_sieve();
    }
}

}  // namespace

TEST(a_005_Matesha_Teoriya_chisel_Lineynoe_resheto, SmallestFactorAndPrimeList) {
    ensure();
    EXPECT_EQ(lp[0], 0);
    EXPECT_EQ(lp[1], 0);
    EXPECT_EQ(lp[2], 2);
    EXPECT_EQ(lp[3], 3);
    EXPECT_EQ(lp[4], 2);
    EXPECT_EQ(lp[9], 3);
    EXPECT_EQ(lp[25], 5);
    EXPECT_EQ(lp[49], 7);
    EXPECT_EQ(lp[97], 97);
    EXPECT_EQ(pr.front(), 2);
    EXPECT_TRUE(std::is_sorted(pr.begin(), pr.end()));

    int prime_count = 0;
    for (int i = 2; i <= N; ++i) {
        if (lp[i] <= 1 || i % lp[i] != 0 || lp[lp[i]] != lp[i] ||
            (i / lp[i] != 1 && lp[i] > lp[i / lp[i]])) {
            ADD_FAILURE() << i << " lp=" << lp[i];
            return;
        }
        if (lp[i] == i) {
            ++prime_count;
        }
    }
    EXPECT_EQ((int)pr.size(), prime_count);
    int seen = 0;
    for (int i = 2; i <= N; ++i) {
        if (lp[i] == i) {
            if (pr[seen] != i) {
                ADD_FAILURE() << "prime " << seen << " is " << pr[seen] << ", expected " << i;
                return;
            }
            ++seen;
        }
    }
}
