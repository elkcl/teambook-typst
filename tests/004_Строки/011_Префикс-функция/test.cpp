#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/004_Строки/011_Префикс-функция/code.hpp"

vi naive_pref(const std::string &s) {
    int n = (int)s.size();
    vi prefix(n, 0);
    for (int i = 1; i < n; ++i) {
        for (int len = i; len >= 1; --len) {
            if (s.compare(0, len, s, i - len + 1, len) == 0) {
                prefix[i] = len;
                break;
            }
        }
    }
    return prefix;
}

void expect_pref(const std::string &s) {
    EXPECT_EQ(pref_func(s), naive_pref(s)) << s;
}

std::string repeat(const std::string &block, int times) {
    std::string s;
    s.reserve(block.size() * times);
    for (int i = 0; i < times; ++i) {
        s += block;
    }
    return s;
}

}  // namespace

TEST(a_004_Stroki_011_Prefiks_funktsiya, EmptyAndShort) {
    expect_pref("");
    expect_pref("a");
    expect_pref("aa");
    expect_pref("ab");
    expect_pref("aba");
    EXPECT_EQ(pref_func("a"), vi{0});
    EXPECT_EQ(pref_func("aa"), (vi{0, 1}));
    EXPECT_EQ(pref_func("ab"), (vi{0, 0}));
}

TEST(a_004_Stroki_011_Prefiks_funktsiya, ClassicPatterns) {
    EXPECT_EQ(pref_func("abacaba"), (vi{0, 0, 1, 0, 1, 2, 3}));
    EXPECT_EQ(pref_func("abcabcabc"), (vi{0, 0, 0, 1, 2, 3, 4, 5, 6}));
    EXPECT_EQ(pref_func("aaaa"), (vi{0, 1, 2, 3}));
    EXPECT_EQ(pref_func("ababab"), (vi{0, 0, 1, 2, 3, 4}));
    EXPECT_EQ(pref_func("aaaabaaa"), (vi{0, 1, 2, 3, 0, 1, 2, 3}));
    EXPECT_EQ(pref_func("abcabdabc"), (vi{0, 0, 0, 1, 2, 0, 1, 2, 3}));
}

TEST(a_004_Stroki_011_Prefiks_funktsiya, EmbeddedNulAndNonLetters) {
    expect_pref(std::string("ab\0ab", 5));
    EXPECT_EQ(pref_func(std::string("ab\0ab", 5)), (vi{0, 0, 0, 1, 2}));
    expect_pref("a a a");
    expect_pref("...\n...");
    expect_pref(std::string("\0\0\0", 3));
    EXPECT_EQ(pref_func(std::string("\0\0\0", 3)), (vi{0, 1, 2}));
}

TEST(a_004_Stroki_011_Prefiks_funktsiya, AllBinaryStrings) {
    for (int n = 0; n <= 14; ++n) {
        int total = 1 << n;
        for (int mask = 0; mask < total; ++mask) {
            std::string s(n, 'a');
            for (int i = 0; i < n; ++i) {
                if ((mask >> i) & 1) {
                    s[i] = 'b';
                }
            }
            expect_pref(s);
        }
    }
}

TEST(a_004_Stroki_011_Prefiks_funktsiya, AllTernaryStrings) {
    for (int n = 0; n <= 8; ++n) {
        int total = 1;
        for (int i = 0; i < n; ++i) {
            total *= 3;
        }
        for (int mask = 0; mask < total; ++mask) {
            std::string s(n, 'a');
            int value = mask;
            for (int i = 0; i < n; ++i) {
                s[i] = char('a' + value % 3);
                value /= 3;
            }
            expect_pref(s);
        }
    }
}

TEST(a_004_Stroki_011_Prefiks_funktsiya, RandomMedium) {
    std::mt19937 rng(4011);
    std::uniform_int_distribution<int> length(0, 40);
    std::uniform_int_distribution<int> letter(0, 3);
    for (int round = 0; round < 200; ++round) {
        int n = length(rng);
        std::string s(n, 'a');
        for (int i = 0; i < n; ++i) {
            s[i] = char('a' + letter(rng));
        }
        expect_pref(s);
    }
    std::uniform_int_distribution<int> long_letter(0, 25);
    for (int round = 0; round < 20; ++round) {
        std::string s(1500, 'a');
        for (char &ch : s) {
            ch = char('a' + long_letter(rng));
        }
        expect_pref(s);
    }
}

TEST(a_004_Stroki_011_Prefiks_funktsiya, ClosedForms) {
    const int n = 100000;
    std::string same(n, 'a');
    vi prefix = pref_func(same);
    ASSERT_EQ((int)prefix.size(), n);
    for (int i = 0; i < n; ++i) {
        ASSERT_EQ(prefix[i], i) << i;
    }

    std::string almost = std::string(n - 1, 'a') + 'b';
    prefix = pref_func(almost);
    for (int i = 0; i < n - 1; ++i) {
        ASSERT_EQ(prefix[i], i);
    }
    EXPECT_EQ(prefix[n - 1], 0);

    std::string periodic = repeat("ab", n / 2);
    prefix = pref_func(periodic);
    EXPECT_EQ(prefix[0], 0);
    for (int i = 1; i < (int)periodic.size(); ++i) {
        ASSERT_EQ(prefix[i], i - 1) << i;
    }
}

TEST(a_004_Stroki_011_Prefiks_funktsiya, BorderFailsAndRestarts) {
    const int half = 80000;
    std::string s(half, 'a');
    s.push_back('b');
    s += std::string(half, 'a');
    vi prefix = pref_func(s);
    ASSERT_EQ((int)prefix.size(), 2 * half + 1);
    for (int i = 0; i < half; ++i) {
        ASSERT_EQ(prefix[i], i);
    }
    EXPECT_EQ(prefix[half], 0);
    for (int k = 1; k <= half; ++k) {
        ASSERT_EQ(prefix[half + k], k);
    }
}
