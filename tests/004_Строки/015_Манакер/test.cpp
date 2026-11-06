#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/004_Строки/015_Манакер/code.hpp"

vi brute_odd(const std::string &s) {
    int n = (int)s.size();
    vi d(n, 1);
    for (int i = 0; i < n; ++i) {
        while (i - d[i] >= 0 && i + d[i] < n && s[i - d[i]] == s[i + d[i]]) {
            ++d[i];
        }
    }
    return d;
}

vi brute_even(const std::string &s) {
    int n = (int)s.size();
    vi d(n, 0);
    for (int i = 0; i < n; ++i) {
        while (i - d[i] >= 0 && i + d[i] + 1 < n && s[i - d[i]] == s[i + d[i] + 1]) {
            ++d[i];
        }
    }
    return d;
}

void expect_both(const std::string &s) {
    EXPECT_EQ(manacher_odd(s), brute_odd(s)) << s;
    EXPECT_EQ(manacher_even(s), brute_even(s)) << s;
}

}  // namespace

TEST(a_004_Stroki_015_Manaker, EmptyShortAndClassic) {
    expect_both("");
    expect_both("a");
    expect_both("aa");
    expect_both("ab");
    expect_both("aba");
    expect_both("abba");
    expect_both("aabaa");
    expect_both("aaaa");
    EXPECT_EQ(manacher_odd("aabaa"), (vi{1, 1, 3, 1, 1}));
    EXPECT_EQ(manacher_even("abba"), (vi{0, 2, 0, 0}));
    EXPECT_EQ(manacher_even("aaaa"), (vi{1, 2, 1, 0}));
}

TEST(a_004_Stroki_015_Manaker, RepeatsAndRandom) {
    expect_both(std::string(50, 'a'));
    expect_both(std::string(51, 'b'));
    std::string alternating;
    for (int i = 0; i < 40; ++i) {
        alternating += (i % 2 == 0 ? 'a' : 'b');
    }
    expect_both(alternating);
    std::mt19937 rng(151);
    for (int n = 0; n <= 60; ++n) {
        std::string s(n, 'a');
        int alphabet = 1 + (n % 4);
        for (int i = 0; i < n; ++i) {
            s[i] = char('a' + rng() % alphabet);
        }
        expect_both(s);
    }
}
