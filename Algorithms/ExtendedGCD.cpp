#pragma once
#include <tuple>
#include <bit>
using ll = long long; using u64 = unsigned long long; using i128 = __int128;
using std::tuple, std::pair, std::tie, std::countr_zero, std::swap;

// naive, std::gcd already exists
ll GCD(ll a, ll b) { return b == 0 ? a : GCD(b, a % b); }

// extended gcd
// recursive, returns r, s, t  where r = sa + tb = gcd(a,b)
inline tuple<ll, ll, ll> ext_gcd_rec(ll a, ll b) {
    if (b == 0) return {a, 1, 0};
    auto [r, s0, t0] = ext_gcd_rec(b, a % b);
    ll s = t0;
    ll t = s0 - (a/b) * t0;
    return {r, s, t};
}

// iterative, returns r, s, t  where r = sa + tb = gcd(a,b)
inline tuple<ll,ll,ll> ext_gcd(ll a, ll b) {
    ll s0 = 1, s1 = 0;
    ll a_ = a, b_ = b;
    while (b_ != 0) {
        ll q = a_ /b_;
        tie(s0, s1) = pair{s1, s0 - s1 * q};
        tie(a_, b_) = pair{b_, a_ - q * b_};
    }
    ll gcd = a_;
    ll t = b == 0 ? 0 : (gcd - (i128)s0 * a) / b;
    return {gcd, s0, t};
}

u64 binary_gcd(u64 a, u64 b) {
    if (!a | !b) return a | b;
    int shift = countr_zero(a | b);
    a >>= countr_zero(a); 
    do {
        b >>= countr_zero(b);
        if (a > b) swap(a, b);
        b -= a;
    } while (b != 0);
    return a << shift;
}

// assume maintaining a' = s*a + t*b, b' = u*a + v*b at each step
// b' - a' = (s2-s1)*a + (t2-t1)*b
// eventually, it will result in gcd = sx + by
tuple<u64,ll,ll> binary_ext_gcd(u64 a, u64 b) {
    if (!a | !b) return {a | b, 1, 0};
    int shift = countr_zero(a | b);
    a >>= countr_zero(a);
    return {0, 0, 0}; // unimplemented
}
