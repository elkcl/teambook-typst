#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/005_Матеша/Метод Гаусса/code.hpp"

using Row = std::vector<double>;
using Matrix = std::vector<Row>;

bool fits(const Matrix &a, const std::vector<double> &ans) {
    int m = (int)ans.size();
    for (const auto &row : a) {
        double sum = 0;
        for (int j = 0; j < m; ++j) {
            sum += ans[j] * row[j];
        }
        if (std::abs(sum - row[m]) > 1e-6) {
            return false;
        }
    }
    return true;
}

}  // namespace

TEST(a_005_Matesha_Metod_Gaussa, UniqueSolutions) {
    std::vector<double> ans;
    Matrix one = {{3, 6}};
    EXPECT_EQ(gauss(one, ans), 1);
    EXPECT_NEAR(ans[0], 2, 1e-8);

    Matrix system = {{2, 1, 5}, {1, -1, 1}};
    EXPECT_EQ(gauss(system, ans), 1);
    EXPECT_NEAR(ans[0], 2, 1e-8);
    EXPECT_NEAR(ans[1], 1, 1e-8);

    Matrix negative = {{-2, 4}};
    EXPECT_EQ(gauss(negative, ans), 1);
    EXPECT_NEAR(ans[0], -2, 1e-8);

    Matrix identity = {{0, 1, 0, 4}, {1, 0, 0, 2}, {0, 0, 1, -3}};
    EXPECT_EQ(gauss(identity, ans), 1);
    EXPECT_NEAR(ans[0], 2, 1e-8);
    EXPECT_NEAR(ans[1], 4, 1e-8);
    EXPECT_NEAR(ans[2], -3, 1e-8);
}

TEST(a_005_Matesha_Metod_Gaussa, NoneAndInfinite) {
    std::vector<double> ans;
    EXPECT_EQ(gauss({{1, 1, 2}, {1, 1, 3}}, ans), 0);
    EXPECT_EQ(gauss({{1, 1, 2}, {2, 2, 4}}, ans), INF);
    EXPECT_TRUE(fits({{1, 1, 2}, {2, 2, 4}}, ans));
    EXPECT_EQ(gauss({{1, 0, 3}, {0, 0, 0}}, ans), INF);
    EXPECT_NEAR(ans[0], 3, 1e-8);
    EXPECT_NEAR(ans[1], 0, 1e-8);
    EXPECT_EQ(gauss({{0, 0, 0}}, ans), INF);
}

TEST(a_005_Matesha_Metod_Gaussa, RandomConsistentSystems) {
    std::mt19937 rng(5051);
    std::uniform_int_distribution<int> coef(-4, 4);
    for (int test = 0; test < 40; ++test) {
        int m = 1 + (int)(rng() % 4);
        int n = 1 + (int)(rng() % 5);
        std::vector<double> hidden(m);
        for (int j = 0; j < m; ++j) {
            hidden[j] = coef(rng);
        }
        Matrix a(n, Row(m + 1));
        for (int i = 0; i < n; ++i) {
            double rhs = 0;
            for (int j = 0; j < m; ++j) {
                a[i][j] = coef(rng);
                rhs += a[i][j] * hidden[j];
            }
            a[i][m] = rhs;
        }
        std::vector<double> ans;
        int status = gauss(a, ans);
        EXPECT_TRUE(status == 1 || status == INF);
        EXPECT_EQ(ans.size(), (size_t)m);
        EXPECT_TRUE(fits(a, ans));
    }
}
