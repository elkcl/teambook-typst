struct line {
  int k, b;
  line() {}
  line(int a, int _b) { k = a, b = _b; }
  int get(int x) { return k * x + b; }
};

vector<line>
    lines;  // храним прямые нижней огибающей
vector<int> dots;  // храним x-координаты точек
                   // нижней огибающей
// первое правило вещественных чисел
// считаем, что в dots лежит округленная вниз
// x-координата

int cross(line a,
          line b) {  // считаем точку пересечения
                     // считаем a.k > b.k
  int x = (b.b - a.b) / (a.k - b.k);
  if (b.b < a.b)
    x--;  // боремся с округлением у отрицательных
          // чисел
  return x;
}

void add(line cur) {
  while (lines.size() &&
         lines.back().get(dots.back()) >
             cur.get(dots.back())) {
    lines.pop_back();
    dots.pop_back();
  }
  if (lines.empty())
    dots.push_back(-inf);
  else
    dots.push_back(cross(lines.back(), cur));
  lines.push_back(cur);
}

int get(int x) {
  int pos =
      lower_bound(dots.begin(), dots.end(), x) -
      dots.begin() - 1;
  return lines[pos].get(x);
}
