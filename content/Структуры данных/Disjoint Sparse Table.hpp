/*! T is the type of the elements
 * Monoid is the operation functor type
 * identity is the identity element of the Monoid
 * (e.g. 0 for addition and inf for minimum)
 */
template <typename T, typename Monoid,
          auto identity>
struct DisjointSparseTable {
  vector<vector<T>> sum;
  explicit DisjointSparseTable(vector<T> arr) {
    // Find the highest cnt such that pow2 = 2^cnt
    // >= x
    int pow2 = 1, cnt = 0;
    for (; pow2 < arr.size(); pow2 *= 2, ++cnt)
      ;
    arr.resize(pow2, identity);
    sum.resize(cnt, vector<T>(pow2));
    for (int level = 0; level < sum.size();
         ++level) {
      for (int block = 0; block < 1 << level;
           ++block) {
        // The first half of the block contains
        // suffix sums, the second half contains
        // prefix sums
        const auto start =
            block << (sum.size() - level);
        const auto end = (block + 1)
                         << (sum.size() - level);
        const auto middle = (end + start) / 2;
        auto val = arr[middle];
        sum[level][middle] = val;
        for (int x = middle + 1; x < end; ++x) {
          val = Monoid{}(val, arr[x]);
          sum[level][x] = val;
        }
        val = arr[middle - 1];
        sum[level][middle - 1] = val;
        for (int x = middle - 2; x >= start;
             --x) {
          val = Monoid{}(val, arr[x]);
          sum[level][x] = val;
        }
      }
    }
  }
  /*! Returns Monoid sum over range [l, r)*/
  T query(int l, int r) const {
    assert(l < r);
    // Convert half open interval to closed
    // interval
    --r;
    if (r == l - 1)
      return identity;
    if (l == r)
      return sum.back()[l];
    // Position of the leftmost different bit from
    // the right
    const auto pos_diff =
        (sizeof(ll) * CHAR_BIT) - 1 -
        __builtin_clzll(l ^ r);
    const auto level = sum.size() - 1 - pos_diff;
    return Monoid{}(sum[level][l], sum[level][r]);
  }
};
