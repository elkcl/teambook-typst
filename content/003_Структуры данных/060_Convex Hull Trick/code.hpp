#include "preamble.hpp"
struct CHT {
    struct Line {
        ll k, b;

        Line() {}

        Line(ll a, ll _b) { k = a, b = _b; }

        ll get(ll x) { return k * x + b; }
    };

    vector<Line> lines; // прямые нижней огибающей

    vll dots; // ceil(x-коорд) с которой прямая оптимальна

    ll cross(Line a, Line b) { // a.k > b.k
        return (b.b - a.b) / (a.k - b.k) + ((b.b - a.b) % (a.k - b.k) > 0);
    }

    void add(Line cur) {
        while (!lines.empty() && cross(lines.back(), cur) <= dots.back()) {
            lines.pop_back();
            dots.pop_back();
        }
        if (lines.empty()) {
            dots.push_back(-INF);
        } else {
            dots.push_back(cross(lines.back(), cur));
        }
        lines.push_back(cur);
    }

    ll get(int x) {
        int pos = upper_bound(dots.begin(), dots.end(), x) - dots.begin() - 1;
        return lines[pos].get(x);
    }

    int ptr = 0;

    ll get_ordered(int x) {
        while (ptr < sz(lines) && dots[ptr] <= x) {
            ptr++;
        }
        ptr--;
        return lines[ptr].get(x);
    }
};
