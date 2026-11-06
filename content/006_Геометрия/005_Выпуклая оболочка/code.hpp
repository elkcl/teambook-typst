#include "../001_Шаблон/code.hpp"

void sort_by_angle(vector<Point> &points, Point p0) {
    sort(points.begin() + 1, points.end(), [p0](Point a, Point b) {
        Point u = a - p0;
        Point v = b - p0;
        ld cr = u % v;
        if (cr != 0) {
            return cr > 0;
        }
        return (u * u) < (v * v);
    });
}

vector<Point> graham(vector<Point> points) {
    sort(all(points), [](Point a, Point b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
    });
    points.erase(unique(all(points)), points.end());
    
    if (sz(points) <= 2) {
        return points;
    }
    Point p0 = points[0];

    sort_by_angle(points, p0);
    vector<Point> hull;
    for (Point p : points) {
        while (hull.size() >= 2 && ((p - hull.back()) % (hull[hull.size() - 2] -
                                                         hull.back())) <= 0) {
            hull.pop_back();
        }
        hull.push_back(p);
    }
    return hull;
}
