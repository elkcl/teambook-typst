#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/003_Структуры данных/005_Дерево отрезков/030_На указателях/code.hpp"

void destroy(Node *node) {
    if (!node) {
        return;
    }
    std::vector<Node *> stack{node};
    while (!stack.empty()) {
        Node *current = stack.back();
        stack.pop_back();
        if (current->ln) {
            stack.push_back(current->ln);
        }
        if (current->rn) {
            stack.push_back(current->rn);
        }
        delete current;
    }
}

void poison_heap() {
    std::vector<Node *> junk;
    junk.reserve(100000);
    for (int i = 0; i < 100000; ++i) {
        Node *node = new Node(0, 1);
        node->sum = 0x1111111111111111LL;
        junk.push_back(node);
    }
    for (Node *node : junk) {
        delete node;
    }
}

struct Root {
    Node *node;
    explicit Root(int left, int right) : node(new Node(left, right)) {}
    ~Root() { destroy(node); }
};

}  // namespace

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_030_Na_ukazatelyah, FullAssignThenQuery) {
    poison_heap();
    Root tree(0, 8);
    tree.node->upd(0, 8, 3);
    EXPECT_EQ(tree.node->get(0, 8), 24);
    EXPECT_EQ(tree.node->get(0, 1), 3);
    EXPECT_EQ(tree.node->get(7, 8), 3);
    EXPECT_EQ(tree.node->get(2, 5), 9);
    EXPECT_EQ(tree.node->get(3, 3), 0);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_030_Na_ukazatelyah, SingleCell) {
    poison_heap();
    Root tree(0, 1);
    EXPECT_EQ(tree.node->get(0, 0), 0);
    tree.node->upd(0, 1, -4);
    EXPECT_EQ(tree.node->get(0, 1), -4);
    tree.node->upd(0, 1, 0);
    EXPECT_EQ(tree.node->get(0, 1), 0);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_030_Na_ukazatelyah, PointQueryAfterPointAssign) {
    poison_heap();
    Root tree(0, 4);
    tree.node->upd(0, 1, 5);
    EXPECT_EQ(tree.node->get(0, 1), 5);
    EXPECT_EQ(tree.node->get(1, 2), 0);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_030_Na_ukazatelyah, PartialAssignKeepsImplicitZeros) {
    poison_heap();
    Root tree(0, 4);
    tree.node->upd(0, 1, 5);
    EXPECT_EQ(tree.node->get(0, 2), 5);
    EXPECT_EQ(tree.node->get(0, 4), 5);
    EXPECT_EQ(tree.node->sum, 5);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_030_Na_ukazatelyah, UntouchedTreeIsZero) {
    poison_heap();
    Root tree(0, 8);
    EXPECT_EQ(tree.node->get(0, 8), 0);
    EXPECT_EQ(tree.node->get(3, 6), 0);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_030_Na_ukazatelyah, DisjointAssignsAndGap) {
    poison_heap();
    Root tree(0, 8);
    tree.node->upd(0, 2, 3);
    tree.node->upd(6, 8, 4);
    EXPECT_EQ(tree.node->get(0, 2), 6);
    EXPECT_EQ(tree.node->get(6, 8), 8);
    EXPECT_EQ(tree.node->get(2, 6), 0);
    EXPECT_EQ(tree.node->get(0, 8), 14);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_030_Na_ukazatelyah, OverwriteAndNegative) {
    poison_heap();
    Root tree(0, 10);
    tree.node->upd(0, 10, 2);
    tree.node->upd(3, 8, -1);
    tree.node->upd(8, 10, 0);
    EXPECT_EQ(tree.node->get(0, 3), 6);
    EXPECT_EQ(tree.node->get(3, 8), -5);
    EXPECT_EQ(tree.node->get(8, 10), 0);
    EXPECT_EQ(tree.node->get(0, 10), 1);
    EXPECT_EQ(tree.node->get(4, 4), 0);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_030_Na_ukazatelyah, SparseCoordinatePoint) {
    poison_heap();
    const int right = 1000000000;
    Root tree(0, right);
    tree.node->upd(123456789, 123456790, 7);
    EXPECT_EQ(tree.node->get(123456789, 123456790), 7);
    EXPECT_EQ(tree.node->get(0, 1), 0);
    EXPECT_EQ(tree.node->get(123456788, 123456789), 0);
    EXPECT_EQ(tree.node->get(0, right), 7);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_030_Na_ukazatelyah, StressAgainstArray) {
    poison_heap();
    std::mt19937 rng(2030);
    for (int n = 1; n <= 16; ++n) {
        for (int round = 0; round < 10; ++round) {
            Root tree(0, n);
            std::vector<long long> values(n, 0);
            for (int step = 0; step < 20; ++step) {
                int left = (int)(rng() % (n + 1));
                int right = (int)(rng() % (n + 1));
                if (left > right) {
                    std::swap(left, right);
                }
                int x = (int)(rng() % 11) - 5;
                tree.node->upd(left, right, x);
                for (int i = left; i < right; ++i) {
                    values[i] = x;
                }
                for (int ql = 0; ql <= n; ++ql) {
                    for (int qr = ql; qr <= n; ++qr) {
                        long long expect = 0;
                        for (int i = ql; i < qr; ++i) {
                            expect += values[i];
                        }
                        ASSERT_EQ(tree.node->get(ql, qr), expect)
                            << "n=" << n << " [" << ql << ", " << qr << ")";
                    }
                }
            }
        }
    }
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_030_Na_ukazatelyah, HeavyProductFits) {
    poison_heap();
    Root tree(0, 20);
    tree.node->upd(0, 20, 1000000000);
    EXPECT_EQ(tree.node->get(0, 20), 20000000000LL);
    tree.node->upd(0, 20, -1000000000);
    EXPECT_EQ(tree.node->get(0, 20), -20000000000LL);
}

TEST(a_003_Struktury_dannyh_005_Derevo_otrezkov_030_Na_ukazatelyah, LoadSparseUpdates) {
    poison_heap();
    const int right = 1000000000;
    const int ops = 20000;
    Root tree(0, right);
    std::mt19937 rng(2031);
    std::map<int, int> last;
    for (int step = 0; step < ops; ++step) {
        int pos = (int)(rng() % right);
        int x = (int)(rng() % 5);
        tree.node->upd(pos, pos + 1, x);
        last[pos] = x;
    }
    long long expect = 0;
    int checked = 0;
    for (auto [pos, x] : last) {
        expect += x;
        if (checked < 50) {
            EXPECT_EQ(tree.node->get(pos, pos + 1), x) << pos;
            ++checked;
        }
    }
    EXPECT_EQ(tree.node->get(0, right), expect);
}
