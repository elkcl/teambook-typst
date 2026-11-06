#include <gtest/gtest.h>

#include "content/preamble.hpp"

namespace {

#include "content/007_Другое/Номера перестановок/code.hpp"

ll factorial(int n) {
    ll value = 1;
    for (int i = 2; i <= n; ++i) {
        value *= i;
    }
    return value;
}

vll identity(int n) {
    vll perm(n);
    iota(perm.begin(), perm.end(), 0);
    return perm;
}

vll reversed(int n) {
    vll perm = identity(n);
    reverse(perm.begin(), perm.end());
    return perm;
}

void expect_lexicographic(int n) {
    Perm perm(n);
    vll order = identity(n);
    ll index = 0;
    do {
        if (perm.encode_perm(order) != index || perm.decode_perm(index, n) != order) {
            ADD_FAILURE() << "n=" << n << ", номер " << index;
            return;
        }
        ++index;
    } while (next_permutation(order.begin(), order.end()));
    EXPECT_EQ(index, factorial(n));
}

}  // namespace

TEST(a_007_Drugoe_Nomera_perestanovok, FactorialsThrough20) {
    Perm perm(20);
    ASSERT_EQ(perm.fac.size(), 21u);
    ll value = 1;
    EXPECT_EQ(perm.fac[0], 1);
    for (int i = 1; i <= 20; ++i) {
        value *= i;
        EXPECT_EQ(perm.fac[i], value) << "n=" << i;
    }
    EXPECT_EQ(perm.fac[20], 2432902008176640000LL);
}

TEST(a_007_Drugoe_Nomera_perestanovok, EmptyAndSingle) {
    Perm empty(0);
    EXPECT_EQ(empty.encode_perm({}), 0);
    EXPECT_EQ(empty.decode_perm(0, 0), vll{});

    Perm one(1);
    EXPECT_EQ(one.encode_perm({0}), 0);
    EXPECT_EQ(one.decode_perm(0, 1), (vll{0}));
}

TEST(a_007_Drugoe_Nomera_perestanovok, LengthThreeInLexOrder) {
    Perm perm(3);
    vector<pair<vll, ll>> order = {
        {{0, 1, 2}, 0}, {{0, 2, 1}, 1}, {{1, 0, 2}, 2},
        {{1, 2, 0}, 3}, {{2, 0, 1}, 4}, {{2, 1, 0}, 5},
    };
    for (auto [p, rank] : order) {
        EXPECT_EQ(perm.encode_perm(p), rank);
        EXPECT_EQ(perm.decode_perm(rank, 3), p);
    }
}

TEST(a_007_Drugoe_Nomera_perestanovok, AllPermutationsUpToLength8) {
    for (int n = 0; n <= 8; ++n) {
        expect_lexicographic(n);
    }
}

TEST(a_007_Drugoe_Nomera_perestanovok, FirstAndLastUpTo20) {
    for (int n : {1, 12, 20}) {
        Perm perm(n);
        vll first = identity(n);
        vll last = reversed(n);
        ll last_rank = factorial(n) - 1;
        EXPECT_EQ(perm.encode_perm(first), 0) << "n=" << n;
        EXPECT_EQ(perm.encode_perm(last), last_rank) << "n=" << n;
        EXPECT_EQ(perm.decode_perm(0, n), first) << "n=" << n;
        EXPECT_EQ(perm.decode_perm(last_rank, n), last) << "n=" << n;
    }
}

TEST(a_007_Drugoe_Nomera_perestanovok, EncodeDoesNotChangeArgument) {
    vll perm_value = {2, 0, 1};
    vll copy = perm_value;
    Perm perm(3);
    EXPECT_EQ(perm.encode_perm(perm_value), 4);
    EXPECT_EQ(perm_value, copy);
}

TEST(a_007_Drugoe_Nomera_perestanovok, ShorterLengthUsesPreparedFactorials) {
    Perm perm(8);
    vll swap_first = {1, 0};
    EXPECT_EQ(perm.encode_perm(swap_first), 1);
    EXPECT_EQ(perm.decode_perm(1, 2), swap_first);

    vll sample = {2, 0, 4, 1, 3};
    ll rank = perm.encode_perm(sample);
    EXPECT_EQ(perm.decode_perm(rank, 5), sample);
    EXPECT_EQ(perm.encode_perm(perm.decode_perm(rank, 5)), rank);
}

TEST(a_007_Drugoe_Nomera_perestanovok, RepeatedCallsStayConsistent) {
    Perm perm(6);
    vll sample = {3, 1, 4, 0, 5, 2};
    ll rank = perm.encode_perm(sample);
    EXPECT_EQ(perm.encode_perm(sample), rank);
    EXPECT_EQ(perm.decode_perm(rank, 6), sample);
    EXPECT_EQ(perm.decode_perm(rank, 6), sample);
    EXPECT_EQ(perm.encode_perm(identity(6)), 0);
    EXPECT_EQ(perm.decode_perm(rank, 6), sample);
}
