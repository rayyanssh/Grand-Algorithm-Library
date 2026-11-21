#include "../../../fallback/cpp23gcc13/Structures/SegmentTree"
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
    SegmentTree S(nums, std::plus{}, 0);
    while (q--) {
        int C, k, u; cin >> C >> k >> u;
        if (C == 1) S.update(k, u);
        else cout << S.query(k, u) << '\n';
    }
    
}