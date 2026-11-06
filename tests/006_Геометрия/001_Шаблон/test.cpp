#include <gtest/gtest.h>
#include <bits/stdc++.h>

namespace {
#include "content/006_Геометрия/001_Шаблон/code.hpp"

void expect_near(ld got, ld expected, ld eps = 1e-8) {
    EXPECT_NEAR((double)got, (double)expected, (double)eps);
}

void expect_point(Point got, Point expected, ld eps = 1e-6) {
    EXPECT_NEAR((double)got.x, (double)expected.x, (double)eps);
    EXPECT_NEAR((double)got.y, (double)expected.y, (double)eps);
}

bool on_circle(Point p, Circle c, ld eps = 1e-6) {
    return std::abs(distance(p, c.o) - c.r) <= eps;
}

}  // namespace

TEST(a_006_Geometriya_001_Shablon, PointsLinesAndHalfPlanes) {
    Point a{1, 2};
    Point b{4, -2};
    expect_point(a + b, {5, 0});
    expect_point(b - a, {3, -4});
    expect_point(a * 3, {3, 6});
    expect_point(2 * a, {2, 4});
    expect_point(b / 2, {2, -1});
    expect_near(a * b, 1 * 4 + 2 * -2);
    expect_near(a % b, 1 * -2 - 2 * 4);
    EXPECT_EQ(sgn((ld)0), 0);
    EXPECT_EQ(sgn((ld)2), 1);
    EXPECT_EQ(sgn((ld)-3), -1);
    expect_near(Point{3, 4}.absv(), 5);
    expect_point(Point{3, 4}.norm(), {0.6, 0.8});
    expect_point(Point{1, 0}.rotate(acos((ld)-1)), {-1, 0}, 1e-8);
    expect_near(distance(Point{0, 0}, Point{3, 4}), 5);
    expect_near(angle(Point{1, 0}, Point{0, 1}), acos((ld)0));

    Line ox(Point{0, 0}, Point{2, 0});
    Line oy(Point{0, 0}, Point{0, 3});
    Line shifted(Point{0, 1}, Point{2, 1});
    EXPECT_TRUE(ox.contains({1, 0}));
    EXPECT_FALSE(ox.contains({1, 1}));
    EXPECT_TRUE(ox || shifted);
    EXPECT_FALSE(ox || oy);
    EXPECT_TRUE(ox == Line(Point{5, 0}, Point{6, 0}));
    EXPECT_FALSE(ox == shifted);
    auto crossing = ox ^ oy;
    EXPECT_EQ(crossing.second, 1);
    expect_point(crossing.first, {0, 0});
    EXPECT_EQ((ox ^ shifted).second, 0);
    EXPECT_EQ((ox ^ Line(Point{-1, 0}, Point{4, 0})).second, 2);
    EXPECT_EQ(ox.half_plane({0, 1}), 1);
    EXPECT_EQ(ox.half_plane({0, -2}), -1);
    EXPECT_EQ(ox.half_plane({3, 0}), 0);
    expect_near(distance(Point{0, 5}, ox), 5);
    expect_near(distance(ox, shifted), 1);
    expect_near(distance(oy, ox), 0);
}

TEST(a_006_Geometriya_001_Shablon, SegmentsRaysAndDistances) {
    Segment horizontal(Point{0, 0}, Point{4, 0});
    EXPECT_TRUE(horizontal.contains({2, 0}));
    EXPECT_TRUE(horizontal.contains({0, 0}));
    EXPECT_TRUE(horizontal.contains({4, 0}));
    EXPECT_FALSE(horizontal.contains({5, 0}));
    EXPECT_FALSE(horizontal.contains({2, 1}));
    expect_near(distance(Point{2, 3}, horizontal), 3);
    expect_near(distance(Point{5, 0}, horizontal), 1);
    expect_near(distance(Point{-1, 0}, horizontal), 1);
    expect_near(distance(Point{2, 0}, horizontal), 0);

    Ray right(Point{0, 0}, Point{1, 0});
    EXPECT_TRUE(right.contains({10, 0}));
    EXPECT_TRUE(right.contains({0, 0}));
    EXPECT_FALSE(right.contains({-1, 0}));
    expect_near(distance(Point{-2, 0}, right), 2);
    expect_near(distance(Point{3, 4}, right), 4);

    Segment vertical(Point{2, -1}, Point{2, 3});
    auto hit = horizontal.l ^ vertical.l;
    EXPECT_EQ(hit.second, 1);
    expect_point(hit.first, {2, 0});
    expect_near(distance(horizontal, vertical), 0);
    expect_near(distance(horizontal, Segment(Point{0, 2}, Point{1, 2})), 2);
    expect_near(distance(right, Line(Point{0, 3}, Point{1, 3})), 3);
    expect_near(distance(horizontal, Line(Point{0, 0}, Point{1, 1})), 0);
}

TEST(a_006_Geometriya_001_Shablon, TrianglesPolygonsAndCircles) {
    Triangle triangle{Point{0, 0}, Point{4, 0}, Point{0, 4}};
    EXPECT_TRUE(triangle.contains({1, 1}));
    EXPECT_TRUE(triangle.contains({0, 0}));
    EXPECT_TRUE(triangle.contains({2, 0}));
    EXPECT_FALSE(triangle.contains({3, 3}));
    EXPECT_FALSE(triangle.contains({-1, 0}));

    std::vector<Point> square = {{0, 0}, {2, 0}, {2, 2}, {0, 2}};
    ConvexPolygon polygon(square);
    EXPECT_TRUE(polygon.contains({1, 1}));
    EXPECT_TRUE(polygon.contains({0, 0}));
    EXPECT_TRUE(polygon.contains({2, 1}));
    EXPECT_FALSE(polygon.contains({3, 1}));
    EXPECT_FALSE(polygon.contains({-0.1, 0.5}));

    Circle unit{Point{0, 0}, 1};
    EXPECT_TRUE(unit.contains({0, 0}));
    EXPECT_TRUE(unit.contains({1, 0}));
    EXPECT_FALSE(unit.contains({1.1, 0}));
    EXPECT_TRUE((unit == Circle{Point{0, 0}, 1}));

    auto through = Line(Point{-2, 0}, Point{2, 0}) ^ unit;
    EXPECT_EQ(through.second, 2);
    EXPECT_TRUE(on_circle(through.first.first, unit));
    EXPECT_TRUE(on_circle(through.first.second, unit));

    auto tangent_line = Line(Point{-2, 1}, Point{2, 1}) ^ unit;
    EXPECT_EQ(tangent_line.second, 1);
    expect_point(tangent_line.first.first, {0, 1});

    auto miss = Line(Point{0, 2}, Point{1, 2}) ^ unit;
    EXPECT_EQ(miss.second, 0);

    Circle other{Point{1, 0}, 1};
    auto two = unit ^ other;
    EXPECT_EQ(two.second, 2);
    EXPECT_TRUE(on_circle(two.first.first, unit) && on_circle(two.first.first, other));
    EXPECT_TRUE(on_circle(two.first.second, unit) && on_circle(two.first.second, other));

    auto touch = unit ^ Circle{Point{2, 0}, 1};
    EXPECT_EQ(touch.second, 1);
    expect_point(touch.first.first, {1, 0});

    EXPECT_EQ((unit ^ Circle{Point{3, 0}, 1}).second, 0);
    EXPECT_EQ((unit ^ unit).second, 3);
    EXPECT_EQ((unit ^ Circle{Point{0.2, 0}, 0.2}).second, 0);

    auto tangents_from = tangents(Point{2, 0}, unit);
    EXPECT_EQ(tangents_from.second, 2);
    EXPECT_TRUE(on_circle(tangents_from.first.first, unit));
    EXPECT_TRUE(on_circle(tangents_from.first.second, unit));
    expect_near(distance(tangents_from.first.first, Point{2, 0}), std::sqrt(3.0));
    expect_near(distance(tangents_from.first.second, Point{2, 0}), std::sqrt(3.0));
    EXPECT_EQ(tangents(Point{1, 0}, unit).second, 1);
    EXPECT_EQ(tangents(Point{0, 0}, unit).second, 0);
}
