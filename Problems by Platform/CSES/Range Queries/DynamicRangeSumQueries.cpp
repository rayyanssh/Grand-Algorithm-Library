#include "../../../fallback/cpp23gcc13/Structures/FenwickTree"
#include <iostream>
using namespace Grand; 
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q; cin >> n >> q;
    vector<ll> nums(n+1);
    for (int i = 1; i <= n; i++) cin >> nums[i];
    FenwickTree T(nums);
    while (q--) {
        int C, k, u; cin >> C >> k >> u;
        if (C == 1) T.update(k, u);
        else cout << T.query(k, u) << '\n';
    }
    
}