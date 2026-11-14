#include "../../../Grand/Structures/SparseTable.hpp"
using namespace Grand; 
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<int> nums(n+1);
    for (int i = 1; i <= n; i++) cin >> nums[i]; 
    SparseTable S(nums, [&](int a, int b) { return min(a,b); });
    while (q--) {
        int l, r; cin >> l >> r;
        cout << S.query(l, r) << '\n';
    }
}