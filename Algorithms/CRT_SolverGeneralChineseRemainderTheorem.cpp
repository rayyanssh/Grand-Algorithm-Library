#pragma once

#include "math.cpp"

using ll = long long;
using i128 = __int128;

// x === a (mod m)
struct Congruence { ll a, m; };

// x === a (mod m) where gcd(m_i, m_j) == 1 for all i, j.
ll crt(const auto& congruences) {
    i128 x = 0;
    ll prod = 1;
    for (auto [a, m] : congruences) prod *= m;
    for (auto [a, m] : congruences) {
        ll M = prod/m;
        ll N = modinv(M, m);
        x = x + (i128)a * M * N;
    }
    x = (x % prod + prod) % prod;
    return x;
}

// x === a (mod m) where gcd(m_i, m_j) may not be 1. 
ll general_crt(auto& congruences) {
    int s = ssize(congruences) - 1;
    while (s >= 0) {
        auto [a, m] = congruences[s--]; 
        auto [b, n] = congruences[s];
        ll g = gcd(m, n);
        if (a % g != b % g) return -1; // no solution
        ll M = modinv(m/g, n/g);
        ll lcm = m/g * n;
        ll x = a + (i128)M * m/g * (b - a);
        x = (x % lcm + lcm) % lcm;
        congruences[s] = {x, lcm};
    }
    auto [x, m] = congruences[0];
    return x;
}
