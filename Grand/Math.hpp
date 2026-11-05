#pragma once
#include "ExtendedGCD.cpp"
namespace Grand {
using ll = long long;
using i128 = __int128;

inline ll modinv(ll a, ll m) {
    auto [g, x, y] = ext_gcd(a, m); 
    return (x % m + m) % m;
}

inline constexpr ll binpow(ll a, ll b) {
    ll res = 1;
    while (b > 0) {
        if (b & 1) res *= a;
        a *= a;
        b >>= 1;
    }
    return res;
}


inline constexpr ll modpow(ll a, ll b, auto m) {
    a = (a % m + m) % m;
    ll res = 1;
    while (b > 0) {
        if (b & 1) res = (i128) res * a % m;
        a = (i128) a * a % m;
        b >>= 1;
    }
    return res;
}
inline constexpr ll floor_mod(ll a, auto m) {
    return (a % m + m) % m;
}
inline constexpr ll modinv_prime(ll a, auto m) { return modpow(a, m-2, m); }

}