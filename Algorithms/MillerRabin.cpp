#include <span>
#include "math.cpp"

using ll = long long;
using std::span;
using u64 = unsigned long long;

bool miller_rabin_test(u64 n, span<int> bases) {
    u64 d = n - 1; int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }
    for (int base : bases) {
        u64 x = modpow(base, d, n);
        if (x == 1 || x == n - 1) continue;
        while (--s) {
            x = (x * x) % n;
            if (x == 1) return false;
            if (x == n - 1) break;
        }
        if (s == 0) return false;
    }
    return true;
}
bool is_prime_miller_rabin(u64 n) {
    int bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    return miller_rabin_test(n, bases);
}