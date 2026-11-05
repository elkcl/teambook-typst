vi z_func(const string &s) {
    vi z(s.size(), 0);
    int l = 0, r = 0;
    for (int i = 1; i < s.size(); i++) {
        if (i < r) {
            z[i] = min(r - i, z[i - l]);
        }
        if (z[i] < 0) {
            z[i] = 0;
        }
        while (i + z[i] < s.size() && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }
    return z;
}
