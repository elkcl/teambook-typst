#include "preamble.hpp"
vector<char> eratosthenes(int n) {
    vector<char> prime(n + 1, true);
    if (n >= 0) {
        prime[0] = false;
    }
    if (n >= 1) {
        prime[1] = false;
    }
    for (int i = 2; i * i <= n; ++i) {
        if (prime[i]) {
            if (i * 1ll * i <= n) {
                for (int j = i * i; j <= n; j += i) {
                    prime[j] = false;
                }
            }
        }
    }
    return prime;
}
