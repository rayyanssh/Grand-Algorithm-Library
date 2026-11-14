#include <vector> 
#include <iostream>
using namespace std; 
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<ll> prefix(n+1);
    for (int i = 1; i <= n; i++) cin >> prefix[i]; 
    for (int i = 1; i <= n; i++) prefix[i] += prefix[i-1];
    while (q--) {
        int a, b;
        cin >> a >> b;
        cout << prefix[b] - prefix[a-1] << '\n';
    }
    
}