// Найти первый индекс из [l; r),
// на котором f(x) == false
// Вернёт r, если f(x) = true на всём
// полуинтервале
template <typename F>
int bin_search(int l, int r, const F& f) {
  --l;
  while (r - l > 1) {
    int m = l + (r - l) / 2;
    if (f(m)) {
      l = m;
    } else {
      r = m;
    }
  }
  return r;
}
