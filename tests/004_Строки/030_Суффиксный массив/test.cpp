#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/004_Строки/030_Суффиксный массив/code.hpp"

std::string suffix(const std::string &s, int index) {
    if (index >= (int)s.size()) {
        return "";
    }
    return s.substr(index);
}

std::vector<int> brute_sa(const std::string &s) {
    int n = (int)s.size() + 1;
    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b) { return suffix(s, a) < suffix(s, b); });
    return order;
}

std::vector<int> brute_lcp(const std::string &s, const std::vector<int> &order) {
    int n = (int)order.size();
    std::vector<int> lcp(n, 0);
    for (int i = 1; i < n; ++i) {
        const std::string &a = suffix(s, order[i - 1]);
        const std::string &b = suffix(s, order[i]);
        int k = 0;
        while (k < (int)a.size() && k < (int)b.size() && a[k] == b[k]) {
            ++k;
        }
        lcp[i] = k;
    }
    return lcp;
}

void expect_string(const std::string &s) {
    auto order = suffix_array(s);
    auto expected = brute_sa(s);
    EXPECT_EQ(order, expected) << s;
    EXPECT_EQ(largest_common_prefix(s, order), brute_lcp(s, expected)) << s;
}

}  // namespace

TEST(a_004_Stroki_030_Suffiksnyy_massiv, EmptyAndHandChecked) {
    expect_string("");
    expect_string("a");
    expect_string("ba");
    expect_string("banana");
    expect_string("aaaa");
    expect_string("abcabc");
    EXPECT_EQ(suffix_array("banana"), (std::vector<int>{6, 5, 3, 1, 0, 4, 2}));
}

TEST(a_004_Stroki_030_Suffiksnyy_massiv, SortedOrderAndRandom) {
    expect_string(std::string(30, 'a'));
    expect_string("abababababab");
    std::mt19937 rng(3030);
    for (int n = 0; n <= 40; ++n) {
        std::string s(n, 'a');
        for (char &ch : s) {
            ch = char('a' + rng() % 3);
        }
        expect_string(s);
    }
    std::string longer(400, 'a');
    for (int i = 0; i < 400; ++i) {
        longer[i] = char('a' + (i * 3) % 5);
    }
    auto order = suffix_array(longer);
    ASSERT_EQ(order.size(), longer.size() + 1);
    std::vector<char> seen(order.size(), 0);
    for (int index : order) {
        ASSERT_GE(index, 0);
        ASSERT_LT(index, (int)order.size());
        EXPECT_EQ(seen[index], 0);
        seen[index] = 1;
    }
    for (int i = 1; i < (int)order.size(); ++i) {
        EXPECT_TRUE(suffix(longer, order[i - 1]) < suffix(longer, order[i]));
    }
}
