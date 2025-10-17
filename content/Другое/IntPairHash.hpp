struct IntPairHash {
  size_t
  operator()(const pair<int, int> &p) const {
    return hash<uint64_t>{}(
        (((uint64_t) p.first) << 32) |
        ((uint64_t) p.second));
  }
};
