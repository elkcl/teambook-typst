#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/004_Строки/035_Суффиксный автомат/code.hpp"

SuffixAutomaton build(const std::string &s) {
    SuffixAutomaton automaton;
    for (char ch : s) {
        automaton.add_char(ch - 'a');
    }
    return automaton;
}

bool accepts(const SuffixAutomaton &automaton, const std::string &text) {
    int v = automaton.root;
    for (char ch : text) {
        int next = automaton.nodes[v].nx[ch - 'a'];
        if (next == -1) {
            return false;
        }
        v = next;
    }
    return true;
}

void expect_substrings(const std::string &s) {
    auto automaton = build(s);
    EXPECT_LE((int)automaton.nodes.size(), std::max(1, 2 * (int)s.size()));
    EXPECT_TRUE(accepts(automaton, ""));
    std::set<std::string> parts;
    for (int l = 0; l < (int)s.size(); ++l) {
        for (int r = l; r <= (int)s.size(); ++r) {
            parts.insert(s.substr(l, r - l));
        }
    }
    for (const auto &part : parts) {
        EXPECT_TRUE(accepts(automaton, part)) << s << " / " << part;
    }
    std::mt19937 rng(3535);
    for (int i = 0; i < 30; ++i) {
        std::string text(s.size() + 1, 'a');
        for (char &ch : text) {
            ch = char('a' + rng() % 3);
        }
        bool present = s.find(text) != std::string::npos || text.empty();
        if (!present) {
            EXPECT_FALSE(accepts(automaton, text)) << s << " / " << text;
        }
    }
}

}  // namespace

TEST(a_004_Stroki_035_Suffiksnyy_avtomat, EmptyAndShort) {
    auto empty = build("");
    EXPECT_EQ(empty.nodes.size(), 1u);
    EXPECT_TRUE(accepts(empty, ""));
    EXPECT_FALSE(accepts(empty, "a"));

    expect_substrings("a");
    expect_substrings("ab");
    expect_substrings("aa");
    expect_substrings("aba");
}

TEST(a_004_Stroki_035_Suffiksnyy_avtomat, ClassicAndRandom) {
    for (const char *s : {"banana", "aaaaa", "abcabc", "abababa", "xyzxyz"}) {
        expect_substrings(s);
    }
    std::mt19937 rng(3536);
    for (int n = 0; n <= 18; ++n) {
        std::string s(n, 'a');
        for (char &ch : s) {
            ch = char('a' + rng() % 3);
        }
        expect_substrings(s);
    }
}
