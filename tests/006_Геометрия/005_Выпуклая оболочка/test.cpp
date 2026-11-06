#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/006_Геометрия/005_Выпуклая оболочка/code.hpp"

bool less_point(Point a, Point b) {
    if (a.x != b.x) {
        return a.x < b.x;
    }
    return a.y < b.y;
}

std::vector<Point> monotone(std::vector<Point> points) {
    std::sort(points.begin(), points.end(), less_point);
    points.erase(std::unique(points.begin(), points.end()), points.end());
    if (points.size() <= 2) {
        return points;
    }
    auto cross = [](Point o, Point a, Point b) { return (a - o) % (b - o); };
    std::vector<Point> lower;
    std::vector<Point> upper;
    for (Point p : points) {
        while (lower.size() >= 2 && cross(lower[lower.size() - 2], lower.back(), p) <= 0) {
            lower.pop_back();
        }
        lower.push_back(p);
    }
    for (int i = (int)points.size() - 1; i >= 0; --i) {
        Point p = points[i];
        while (upper.size() >= 2 && cross(upper[upper.size() - 2], upper.back(), p) <= 0) {
            upper.pop_back();
        }
        upper.push_back(p);
    }
    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

std::set<std::pair<long long, long long>> key_set(const std::vector<Point> &points) {
    std::set<std::pair<long long, long long>> keys;
    for (Point p : points) {
        keys.insert({llround((double)p.x * 1000000), llround((double)p.y * 1000000)});
    }
    return keys;
}

void expect_hull(const std::vector<Point> &points) {
    auto got = graham(points);
    auto expected = monotone(points);
    EXPECT_EQ(key_set(got), key_set(expected));
    if (got.size() >= 3) {
        ld area = 0;
        for (int i = 0; i < (int)got.size(); ++i) {
            Point a = got[i];
            Point b = got[(i + 1) % got.size()];
            area += a % b;
        }
        EXPECT_GT(area, 0);
    }
}

}  // namespace

TEST(a_006_Geometriya_005_Vypuklaya_obolochka, EmptyShortAndCollinear) {
    expect_hull({});
    expect_hull({{2, 3}});
    expect_hull({{0, 0}, {1, 1}});
    expect_hull({{0, 0}, {1, 1}, {2, 2}});
    expect_hull({{0, 1}, {0, 0}, {0, 2}, {0, 0}});
    auto square = graham({{0, 0}, {1, 0}, {1, 1}, {0, 1}, {0.5, 0.2}});
    EXPECT_EQ(square.size(), 4u);
    EXPECT_EQ(key_set(square), key_set({{0, 0}, {1, 0}, {1, 1}, {0, 1}}));
}

TEST(a_006_Geometriya_005_Vypuklaya_obolochka, AngleSortKeepsOrigin) {
    std::vector<Point> points = {{0, 0}, {0, 2}, {2, 0}, {1, 1}};
    sort_by_angle(points, points[0]);
    EXPECT_TRUE(points[0] == (Point{0, 0}));
    for (int i = 2; i < (int)points.size(); ++i) {
        Point u = points[i - 1] - points[0];
        Point v = points[i] - points[0];
        EXPECT_GE(u % v, 0);
    }
}

TEST(a_006_Geometriya_005_Vypuklaya_obolochka, RandomMatchesMonotoneChain) {
    std::mt19937 rng(605);
    std::uniform_int_distribution<int> coord(-20, 20);
    for (int n : {0, 1, 2, 3, 5, 10, 30}) {
        for (int test = 0; test < 15; ++test) {
            std::vector<Point> points(n);
            for (int i = 0; i < n; ++i) {
                points[i] = {(ld)coord(rng), (ld)coord(rng)};
            }
            expect_hull(points);
        }
    }
}
