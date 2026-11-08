#include <bits/stdc++.h>
#include "../../../Grand/Structures/SegmentTree.hpp"
using namespace std;
using ull = unsigned long long; 
constexpr int MOD = 998244353; 

struct Node {
    array<ull, 3> dist;
    array<char, 3> target;
    int len;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q; cin >> n >> q; 
    vector<int> p(n+1); 
    vector<int> pow2(n+1); pow2[0] = 1;
    for (int i = 1; i <= n; i++) { cin >> p[i]; } 
    for (int i = 1; i <= n; i++) { pow2[i] = pow2[i-1]*2 % MOD; }

    auto combine = [&](Node a, Node b) { 
        Node c;
        for (int i = 0; i < 3; i++) {
            c.dist[i] = (a.dist[b.target[i]] + pow2[a.len] * b.dist[i]) % MOD;
            c.target[i] = a.target[b.target[i]];
        }
        c.len = a.len + b.len;
        return c;
    };
    Node identity = { {0,0,0}, {0,1,2}, 0 };
    vector<Node> nodes(n+1); nodes[0] = identity;
    auto create_leaf = [&](int p){ 
        Node leaf;
        for (int j = 1; j <= 3; j++) {
            leaf.dist[j]   = !(p == j);
            leaf.target[j] =   p == j ? p : 6 - p - j - 1; // -1 to convert to 0 idxed
        }
        leaf.len = 1;
        return leaf;
    };
    for (int i = 1; i <= n; i++) nodes[i] = create_leaf(p[i]);
    Grand::SegmentTree S(nodes, combine, identity); 
    while (q--) {
        char op; int x, y;
        cin >> op >> x >> y;
        if (op == 'c') {
            S.update(x, create_leaf(y));
        }
        else {
            int l = x; int r = y;
            Node res = S.query(l, r);
            cout << res.dist[0] << '\n';
        }
    }
}
