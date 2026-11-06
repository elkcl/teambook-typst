#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/005_Матеша/Теория чисел/Решето Эратосфена/code.hpp"

std::vector<char> brute(int n) {
    std::vector<char> prime(n + 1, true);
    if (n >= 0) {
        prime[0] = false;
    }
    if (n >= 1) {
        prime[1] = false;
    }
    for (int i = 2; i <= n; ++i) {
        if (!prime[i]) {
            continue;
        }
        for (int j = i + i; j <= n; j += i) {
            prime[j] = false;
        }
    }
    return prime;
}

}  // namespace

TEST(a_005_Matesha_Teoriya_chisel_Resheto_Eratosfena, TinyAndKnownPrimes) {
    EXPECT_EQ(eratosthenes(0), brute(0));
    EXPECT_EQ(eratosthenes(1), (std::vector<char>{false, false}));
    EXPECT_EQ(eratosthenes(2), (std::vector<char>{false, false, true}));
    auto ten = eratosthenes(10);
    EXPECT_TRUE(ten[2] && ten[3] && ten[5] && ten[7]);
    EXPECT_FALSE(ten[0] || ten[1] || ten[4] || ten[6] || ten[8] || ten[9] || ten[10]);
}

TEST(a_005_Matesha_Teoriya_chisel_Resheto_Eratosfena, MatchesTrialSieve) {
    for (int n : {1, 2, 3, 17, 100, 1000, 5000}) {
        EXPECT_EQ(eratosthenes(n), brute(n)) << n;
    }
    auto big = eratosthenes(200000);
    auto reference = brute(200000);
    EXPECT_EQ(big, reference);
    EXPECT_TRUE(big[199999]);
}
