#include <bits/stdc++.h>
#include "../../../Grand/Structures/SegmentTree.hpp"
using namespace std;
using ull = unsigned long long; 
constexpr int MOD = 998244353;
template<int MOD>
ull TOH_distance(std::span<char> state) {
    int n = state.size(); int target = 0;
    ull dist = 0;
    for (int k = n - 1; k >= 0; k--) {
        int b = state[k] != target;
        dist = (dist * 2 + b) % MOD;
        if (b) target = 3 - state[k] - target;
    }
    return dist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q; cin >> n >> q; 
    vector<char> p(n+1);
    for (int i = 1; i <= n; i++) { int v; cin >> v; p[i] = v-1; }
    auto f = [](int a, int b) { return a + b ;};
    Grand::SegmentTree S(p, f); 
    while (q--) {
        char op; int x, y;
        cin >> op >> x >> y;
        if (op == 'c') p[x] = char(y) - 1;
        else {
            int l = x; int r = y;
            span<char> range(&p[l], r - l + 1);
            int moves = TOH_distance<MOD>(range);
            cout << moves << '\n';
        }
    }
}
