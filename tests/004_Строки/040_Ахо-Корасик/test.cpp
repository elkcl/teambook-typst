#include <gtest/gtest.h>
#include <bits/stdc++.h>

#define fast_io
namespace {
#include "content/004_Строки/040_Ахо-Корасик/code.hpp"

std::vector<int> lengths_at(const Trie &trie, int v) {
    std::vector<int> lengths;
    if (v < 0) {
        return lengths;
    }
    if (trie.t[v].term) {
        lengths.push_back(trie.t[v].len);
    }
    for (int u = trie.t[v].zip; u != -1; u = trie.t[u].zip) {
        if (trie.t[u].term) {
            lengths.push_back(trie.t[u].len);
        }
    }
    std::sort(lengths.begin(), lengths.end());
    return lengths;
}

std::vector<int> brute_lengths(const std::vector<std::string> &patterns, const std::string &text, int end) {
    std::vector<int> lengths;
    for (const auto &pattern : patterns) {
        if (pattern.size() <= (size_t)end + 1 &&
            text.compare(end + 1 - (int)pattern.size(), pattern.size(), pattern) == 0) {
            lengths.push_back((int)pattern.size());
        }
    }
    std::sort(lengths.begin(), lengths.end());
    return lengths;
}

void expect_text(const std::vector<std::string> &patterns, const std::string &text) {
    Trie trie(patterns);
    int v = 0;
    for (int i = 0; i < (int)text.size(); ++i) {
        v = trie.t[v].next[text[i] - 'a'];
        EXPECT_EQ(lengths_at(trie, v), brute_lengths(patterns, text, i)) << text << " @" << i;
    }
}

}  // namespace
#undef fast_io

TEST(a_004_Stroki_040_Aho_Korasik, EmptyAndSingleLetters) {
    expect_text({}, "");
    expect_text({}, "abc");
    expect_text({""}, "ab");
    expect_text({"a"}, "a");
    expect_text({"a"}, "b");
    expect_text({"a", "b", "c"}, "abc");
    expect_text({"aa", "a"}, "aaa");
}

TEST(a_004_Stroki_040_Aho_Korasik, OverlapsAndClassicDictionary) {
    expect_text({"he", "she", "his", "hers"}, "ushers");
    expect_text({"a", "ab", "bab", "bc", "bca", "c", "caa"}, "abccab");
    expect_text({"aba", "ba", "a"}, "abababa");
    expect_text({"xyz"}, "xyxyxyz");
}

TEST(a_004_Stroki_040_Aho_Korasik, FailureLinkAndTreeOrder) {
    Trie trie({"abc", "bc", "c"});
    int a = trie.t[0].next['a' - 'a'];
    int b = trie.t[a].next['b' - 'a'];
    int c = trie.t[b].next['c' - 'a'];
    EXPECT_EQ(trie.t[0].link, 0);
    EXPECT_EQ(trie.t[c].len, 3);
    EXPECT_TRUE(trie.t[c].term);
    auto at_c = lengths_at(trie, c);
    EXPECT_EQ(at_c, (std::vector<int>{1, 2, 3}));

    std::vector<Vertex> g(4);
    g[0].e = {1, 2};
    g[1].e = {3};
    vi tin(4, -1), tout(4, -1);
    int timer = 0;
    dfs(g, tin, tout, timer, 0, -1, 0);
    EXPECT_EQ(g[0].d, 0);
    EXPECT_EQ(g[1].d, 1);
    EXPECT_EQ(g[3].d, 2);
    auto ancestor = [&](int u, int v) {
        return tin[u] <= tin[v] && tin[v] < tout[u];
    };
    EXPECT_TRUE(ancestor(0, 0));
    EXPECT_TRUE(ancestor(0, 1));
    EXPECT_TRUE(ancestor(0, 2));
    EXPECT_TRUE(ancestor(0, 3));
    EXPECT_TRUE(ancestor(1, 3));
    EXPECT_FALSE(ancestor(1, 2));
    EXPECT_FALSE(ancestor(2, 3));
    EXPECT_FALSE(ancestor(3, 1));
}

TEST(a_004_Stroki_040_Aho_Korasik, RandomDictionaries) {
    std::mt19937 rng(4040);
    for (int test = 0; test < 30; ++test) {
        int count = (int)(rng() % 6);
        std::vector<std::string> patterns;
        for (int i = 0; i < count; ++i) {
            int len = (int)(rng() % 4);
            std::string pattern(len, 'a');
            for (char &ch : pattern) {
                ch = char('a' + rng() % 3);
            }
            patterns.push_back(pattern);
        }
        std::sort(patterns.begin(), patterns.end());
        patterns.erase(std::unique(patterns.begin(), patterns.end()), patterns.end());
        int n = (int)(rng() % 12);
        std::string text(n, 'a');
        for (char &ch : text) {
            ch = char('a' + rng() % 3);
        }
        expect_text(patterns, text);
    }
}
