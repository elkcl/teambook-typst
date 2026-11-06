#include "preamble.hpp"
const int MAXC = 500010;
ll n, x[MAXC];

ll mul(ll a, ll b, ll m) { // m <= 8e18
    ll k = ((ld) a * b) / m;
    ll r = a * b - k * m;
    while (r < 0) {
        r += m;
    }
    while (r >= m) {
        r -= m;
    }
    return r;
}

// -1, если нетривиальный делитель не найден
ll pollard(ll value) {
    n = value;
    if (n <= (int) 1e6) {
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                return i;
            }
        }
        return -1;
    }
    ll C = 2 * pow(n, 1.0 / 4);
    for (int cnt = 0; cnt < 4; cnt++) {
        x[0] = abs((int) rng()) % (n - 1) + 1;
        for (int i = 0; i < C; i++) {
            x[i + 1] = (mul(x[i], x[i], n) + 3) % n;
        }
        for (int i = 0; i < C; i++) {
            ll g = gcd(abs(x[i] - x[C]), n);
            if (g != 1 && g != n) {
                return g;
            }
        }
    }
    return -1;
}
