#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/003_Структуры данных/020_Декартово дерево по неявному ключу/code.hpp"

std::vector<ll> to_vector(Node *root) {
    std::vector<ll> values;
    std::vector<Node *> stack;
    Node *current = root;
    while (current || !stack.empty()) {
        while (current) {
            stack.push_back(current);
            current = current->l;
        }
        current = stack.back();
        stack.pop_back();
        values.push_back(current->elem);
        current = current->r;
    }
    return values;
}

bool sizes_ok(Node *node) {
    if (!node) {
        return true;
    }
    if (node->sz != 1 + get_sz(node->l) + get_sz(node->r)) {
        return false;
    }
    return sizes_ok(node->l) && sizes_ok(node->r);
}

Node *build(const std::vector<ll> &values) {
    Node *root = nullptr;
    for (ll value : values) {
        root = merge(root, new Node(value));
    }
    return root;
}

void expect_sequence(Node *root, const std::vector<ll> &values) {
    EXPECT_EQ(get_sz(root), (ll)values.size());
    EXPECT_TRUE(sizes_ok(root));
    EXPECT_EQ(to_vector(root), values);
}

struct Guard {
    Node *&root;
    ~Guard() { tfree(root); }
};

}  // namespace

TEST(a_003_Struktury_dannyh_020_Dekartovo_derevo_po_neyavnomu_klyuchu, EmptyMergeAndSplit) {
    EXPECT_EQ(merge(nullptr, nullptr), nullptr);
    EXPECT_EQ(get_sz(nullptr), 0);
    auto [left, right] = split(nullptr, 0);
    EXPECT_EQ(left, nullptr);
    EXPECT_EQ(right, nullptr);
    std::tie(left, right) = split(nullptr, 5);
    EXPECT_EQ(left, nullptr);
    EXPECT_EQ(right, nullptr);
    tfree(nullptr);
}

TEST(a_003_Struktury_dannyh_020_Dekartovo_derevo_po_neyavnomu_klyuchu, SingleNodeSplits) {
    Node *root = new Node(-5);
    expect_sequence(root, {-5});

    auto [left, right] = split(root, 0);
    EXPECT_EQ(left, nullptr);
    expect_sequence(right, {-5});
    root = merge(left, right);

    std::tie(left, right) = split(root, 1);
    expect_sequence(left, {-5});
    EXPECT_EQ(right, nullptr);
    root = merge(left, right);

    std::tie(left, right) = split(root, 4);
    expect_sequence(left, {-5});
    EXPECT_EQ(right, nullptr);
    tfree(left);
}

TEST(a_003_Struktury_dannyh_020_Dekartovo_derevo_po_neyavnomu_klyuchu, BuildPreservesOrderAndDuplicates) {
    std::vector<ll> values{3, 1, 1, 0, -8, LLONG_MAX, LLONG_MIN, 3};
    Node *root = build(values);
    expect_sequence(root, values);
    tfree(root);
}

TEST(a_003_Struktury_dannyh_020_Dekartovo_derevo_po_neyavnomu_klyuchu, SplitAtEveryPosition) {
    std::vector<ll> values;
    for (int i = 0; i < 30; ++i) {
        values.push_back(i * 10 - 7);
    }
    Node *root = build(values);
    for (int cut = 0; cut <= (int)values.size(); ++cut) {
        auto [left, right] = split(root, cut);
        expect_sequence(left, {values.begin(), values.begin() + cut});
        expect_sequence(right, {values.begin() + cut, values.end()});
        EXPECT_EQ(get_sz(left) + get_sz(right), (ll)values.size());
        root = merge(left, right);
        expect_sequence(root, values);
    }
    tfree(root);
}

TEST(a_003_Struktury_dannyh_020_Dekartovo_derevo_po_neyavnomu_klyuchu, ConcatenateAndRearrangeBlocks) {
    Node *left = build({1, 2, 3});
    Node *right = build({4, 5});
    Node *root = merge(left, right);
    expect_sequence(root, {1, 2, 3, 4, 5});

    auto [head, tail] = split(root, 2);
    auto [middle, rest] = split(tail, 2);
    root = merge(merge(middle, head), rest);
    expect_sequence(root, {3, 4, 1, 2, 5});
    tfree(root);
}

TEST(a_003_Struktury_dannyh_020_Dekartovo_derevo_po_neyavnomu_klyuchu, InsertAndEraseAgainstVector) {
    std::vector<ll> values;
    Node *root = nullptr;
    std::mt19937 rng(2001);
    for (int step = 0; step < 400; ++step) {
        bool erase = !values.empty() && rng() % 3 == 0;
        if (erase) {
            int pos = (int)(rng() % values.size());
            auto [left, midright] = split(root, pos);
            auto [mid, right] = split(midright, 1);
            tfree(mid);
            root = merge(left, right);
            values.erase(values.begin() + pos);
        } else {
            int pos = values.empty() ? 0 : (int)(rng() % (values.size() + 1));
            ll value = (ll)(rng() % 21) - 10;
            auto [left, right] = split(root, pos);
            root = merge(merge(left, new Node(value)), right);
            values.insert(values.begin() + pos, value);
        }
        expect_sequence(root, values);
    }
    while (!values.empty()) {
        auto [left, midright] = split(root, 0);
        auto [mid, right] = split(midright, 1);
        tfree(mid);
        root = merge(left, right);
        values.erase(values.begin());
        expect_sequence(root, values);
    }
    tfree(root);
}

TEST(a_003_Struktury_dannyh_020_Dekartovo_derevo_po_neyavnomu_klyuchu, MoveSegment) {
    std::vector<ll> values{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Node *root = build(values);
    auto move_range = [](Node *root, int left, int right, int to) {
        auto [head, midright] = split(root, left);
        auto [mid, tail] = split(midright, right - left);
        root = merge(head, tail);
        auto [before, after] = split(root, to);
        return merge(merge(before, mid), after);
    };
    root = move_range(root, 2, 5, 0);
    values = {2, 3, 4, 0, 1, 5, 6, 7, 8, 9};
    expect_sequence(root, values);

    root = move_range(root, 0, 3, 7);
    values = {0, 1, 5, 6, 7, 8, 9, 2, 3, 4};
    expect_sequence(root, values);
    tfree(root);
}

TEST(a_003_Struktury_dannyh_020_Dekartovo_derevo_po_neyavnomu_klyuchu, SplitMergeIsIdentity) {
    std::mt19937 rng(2002);
    for (int n = 1; n <= 80; ++n) {
        std::vector<ll> values(n);
        for (int i = 0; i < n; ++i) {
            values[i] = (ll)rng();
        }
        Node *root = build(values);
        for (int cut = 0; cut <= n; ++cut) {
            auto [left, right] = split(root, cut);
            root = merge(right, left);
            std::rotate(values.begin(), values.begin() + cut, values.end());
            expect_sequence(root, values);
            auto [back_left, back_right] = split(root, n - cut);
            root = merge(back_right, back_left);
            std::rotate(values.begin(), values.begin() + (n - cut), values.end());
            expect_sequence(root, values);
        }
        tfree(root);
    }
}

TEST(a_003_Struktury_dannyh_020_Dekartovo_derevo_po_neyavnomu_klyuchu, StressRandomEdits) {
    std::mt19937 rng(2003);
    for (int round = 0; round < 30; ++round) {
        std::vector<ll> values;
        Node *root = nullptr;
        int ops = 80 + (int)(rng() % 80);
        for (int step = 0; step < ops; ++step) {
            int kind = (int)(rng() % 5);
            if (values.empty() || kind == 0) {
                int pos = values.empty() ? 0 : (int)(rng() % (values.size() + 1));
                ll value = (ll)((int)rng() % 1000 - 500);
                auto [left, right] = split(root, pos);
                root = merge(merge(left, new Node(value)), right);
                values.insert(values.begin() + pos, value);
            } else if (kind == 1) {
                int pos = (int)(rng() % values.size());
                auto [left, midright] = split(root, pos);
                auto [mid, right] = split(midright, 1);
                tfree(mid);
                root = merge(left, right);
                values.erase(values.begin() + pos);
            } else if (kind == 2) {
                int cut = (int)(rng() % (values.size() + 1));
                auto [left, right] = split(root, cut);
                root = merge(left, right);
            } else if (kind == 3 && (int)values.size() >= 2) {
                int left = (int)(rng() % values.size());
                int right = left + 1 + (int)(rng() % (values.size() - left));
                int to = (int)(rng() % (values.size() - (right - left) + 1));
                auto [head, midright] = split(root, left);
                auto [mid, tail] = split(midright, right - left);
                root = merge(head, tail);
                auto [before, after] = split(root, to);
                root = merge(merge(before, mid), after);
                std::vector<ll> block(values.begin() + left, values.begin() + right);
                values.erase(values.begin() + left, values.begin() + right);
                values.insert(values.begin() + to, block.begin(), block.end());
            } else {
                int cut = (int)(rng() % (values.size() + 1));
                auto [left, right] = split(root, cut);
                root = merge(right, left);
                std::rotate(values.begin(), values.begin() + cut, values.end());
            }
            expect_sequence(root, values);
        }
        tfree(root);
    }
}

TEST(a_003_Struktury_dannyh_020_Dekartovo_derevo_po_neyavnomu_klyuchu, LoadAppendSplitAndJoin) {
    const int n = 100000;
    std::vector<ll> values(n);
    Node *root = nullptr;
    for (int i = 0; i < n; ++i) {
        values[i] = i * 3LL - 20;
        root = merge(root, new Node(values[i]));
    }
    expect_sequence(root, values);

    std::mt19937 rng(2004);
    for (int step = 0; step < 200; ++step) {
        int cut = (int)(rng() % (n + 1));
        auto [left, right] = split(root, cut);
        EXPECT_EQ(get_sz(left), cut);
        EXPECT_EQ(get_sz(right), n - cut);
        root = merge(left, right);
    }
    EXPECT_EQ(get_sz(root), n);
    EXPECT_EQ(to_vector(root).front(), values.front());
    EXPECT_EQ(to_vector(root).back(), values.back());
    EXPECT_EQ(to_vector(root)[n / 2], values[n / 2]);
    tfree(root);
}
