#include <bits/stdc++.h>
using namespace std;
using ll = long long;
void solve() {
    int n;
    cin >> n;
    vector<int> a(n), b(n);
    vector<int> c(n);
    int xora = 0, xorb = 0;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        xora ^= a[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> b[i];
        xorb ^= b[i];
        c[i] = a[i] ^ b[i];   
    }

    if (xora == xorb) cout << "Tie\n";
    int aheada = 0, aheadb = 0;
    int i=n-1;
    
    if (aheada == aheadb) {
        
    }
    if (aheada == 2) {
        cout << "Alice\n";
    } else if (aheadb == 2) {
        cout << "Bob\n";
    } 
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    while (T--) solve();
    return 0;
}