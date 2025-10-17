vector<V> graham(vector<V> points) {
  V p0 = points[0];
  for (V p : points)
    if (p.x < p0.x || (p.x == p0.x && p.y < p0.y))
      p0 = p;
  sort_by_angle(points, p0);
  vector<V> hull;
  for (V p : points) {
    while (hull.size() >= 2 &&
           ((p - hull.back()) ^
            (hull[hull.size() - 2] -
             hull.back())) <= 0)
      hull.pop_back();
    hull.push_back(p);
  }
  return hull;
}
