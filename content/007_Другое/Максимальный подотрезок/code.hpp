#include "preamble.hpp"

pii max_seg(vi &a) {
    int ans_l = 0, ans_r = 0, minus_pos = -1;
    ll ans = a[0], sum = 0;
    for (int r = 0; r < a.size(); ++r) {
        sum += a[r];
        if (sum > ans) {
            ans = sum;
            ans_l = minus_pos + 1;
            ans_r = r;
        }
        if (sum < 0) {
            sum = 0;
            minus_pos = r;
        }
    }
    return {ans_l, ans_r};
}
