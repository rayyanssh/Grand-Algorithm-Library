#include <vector> 
#include <iostream>
#include "../../../fallback/cpp23gcc13/Structures/FenwickTree"
using namespace std; 
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<ll> nums(n+1);
    vector<ll> diff(n+2);
    for (int i = 1; i <= n; i++) cin >> nums[i]; 
    for (int i = 1; i <= n; i++) diff[i] = nums[i] - nums[i-1];
    diff[n+1] = -nums[n];
    Grand::FenwickTree<ll> F(diff);
    while (q--) {
        int C; cin >> C;
        if (C == 1) {
            int a,b,u; cin >> a >> b >> u;
            F.apply(a, u); 
            F.apply(b+1, -u);
        }
        else {
            int 
        }
    }   
}