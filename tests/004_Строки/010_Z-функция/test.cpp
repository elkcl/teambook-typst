#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/004_Строки/010_Z-функция/code.hpp"

vi naive_z(const std::string &s) {
    int n = (int)s.size();
    vi z(n, 0);
    for (int i = 1; i < n; ++i) {
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            ++z[i];
        }
    }
    return z;
}

void expect_z(const std::string &s) {
    EXPECT_EQ(z_func(s), naive_z(s)) << s;
}

}  // namespace

TEST(a_004_Stroki_010_Z_funktsiya, EmptyAndShort) {
    expect_z("");
    expect_z("a");
    expect_z("aa");
    expect_z("ab");
    expect_z("aba");
    EXPECT_EQ(z_func("a"), vi{0});
    EXPECT_EQ(z_func("aa"), (vi{0, 1}));
    EXPECT_EQ(z_func("ab"), (vi{0, 0}));
}

TEST(a_004_Stroki_010_Z_funktsiya, ClassicPatterns) {
    EXPECT_EQ(z_func("abacaba"), (vi{0, 0, 1, 0, 3, 0, 1}));
    EXPECT_EQ(z_func("aaaa"), (vi{0, 3, 2, 1}));
    EXPECT_EQ(z_func("ababab"), (vi{0, 0, 4, 0, 2, 0}));
    EXPECT_EQ(z_func("abcabcabc"), (vi{0, 0, 0, 6, 0, 0, 3, 0, 0}));
    EXPECT_EQ(z_func("aaabaab"), (vi{0, 2, 1, 0, 2, 1, 0}));
    EXPECT_EQ(z_func("aabaab"), (vi{0, 1, 0, 3, 1, 0}));
}

TEST(a_004_Stroki_010_Z_funktsiya, EmbeddedNulAndNonLetters) {
    expect_z(std::string("ab\0ab", 5));
    EXPECT_EQ(z_func(std::string("ab\0ab", 5)), (vi{0, 0, 0, 2, 0}));
    expect_z(std::string("\0\0\0", 3));
    EXPECT_EQ(z_func(std::string("\0\0\0", 3)), (vi{0, 2, 1}));
    expect_z("a a a");
}

TEST(a_004_Stroki_010_Z_funktsiya, AllBinaryStrings) {
    for (int n = 0; n <= 14; ++n) {
        int total = 1 << n;
        for (int mask = 0; mask < total; ++mask) {
            std::string s(n, 'a');
            for (int i = 0; i < n; ++i) {
                if ((mask >> i) & 1) {
                    s[i] = 'b';
                }
            }
            expect_z(s);
        }
    }
}

TEST(a_004_Stroki_010_Z_funktsiya, AllTernaryStrings) {
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
            expect_z(s);
        }
    }
}

TEST(a_004_Stroki_010_Z_funktsiya, RandomMedium) {
    std::mt19937 rng(4010);
    std::uniform_int_distribution<int> letter(0, 2);
    for (int round = 0; round < 30; ++round) {
        std::string s(400, 'a');
        for (char &ch : s) {
            ch = char('a' + letter(rng));
        }
        expect_z(s);
    }
    std::uniform_int_distribution<int> wide(0, 25);
    for (int round = 0; round < 10; ++round) {
        std::string s(2500, 'a');
        for (char &ch : s) {
            ch = char('a' + wide(rng));
        }
        expect_z(s);
    }
}

TEST(a_004_Stroki_010_Z_funktsiya, ClosedForms) {
    const int n = 200000;
    std::string same(n, 'q');
    vi z = z_func(same);
    ASSERT_EQ((int)z.size(), n);
    EXPECT_EQ(z[0], 0);
    for (int i = 1; i < n; ++i) {
        ASSERT_EQ(z[i], n - i) << i;
    }

    std::string periodic;
    periodic.reserve(n);
    for (int i = 0; i < n; ++i) {
        periodic.push_back(i % 2 == 0 ? 'a' : 'b');
    }
    z = z_func(periodic);
    EXPECT_EQ(z[0], 0);
    for (int i = 1; i < n; ++i) {
        int expect = (i % 2 == 0) ? n - i : 0;
        ASSERT_EQ(z[i], expect) << i;
    }
}

TEST(a_004_Stroki_010_Z_funktsiya, MatchStopsAtMismatchAndRestarts) {
    const int half = 70000;
    std::string s(half, 'a');
    s.push_back('b');
    s += std::string(half, 'a');
    vi z = z_func(s);
    int n = (int)s.size();
    ASSERT_EQ((int)z.size(), n);
    EXPECT_EQ(z[0], 0);
    for (int i = 1; i < half; ++i) {
        ASSERT_EQ(z[i], half - i) << i;
    }
    EXPECT_EQ(z[half], 0);
    for (int i = half + 1; i < n; ++i) {
        ASSERT_EQ(z[i], n - i) << i;
    }
}
