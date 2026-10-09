#include <bits/stdc++.h>
using namespace std;
int MAX = 1e9;
void fastio() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
}

bool ok(auto& a) {
    int n = a.size() - 2;
    bool flag = true;
    for (int j = 1; j <= n; j++) 
        for (int k = a[j] - 1; k <= a[j] + 1; k+=2) {
            vector<int> b(n + 2, 0);
            flag = true;
            for (int i = 1; i <= n; i++) {
                if (a[i] > k) {
                    b[i + 1] = a[i];
                }
                else if (a[i] < k) {
                    b[i - 1] = a[i];
                }
                else {flag = false; break;}
            }
            for (int i = 1; i <= n; i++) {
                if (b[i] == 0) {flag = false; break;}
            }
            if (flag) return true;
        }
    return false;
}

void solve() {
    int n; cin >> n;
    vector<int> a(n+2);
    for (int i = 1; i <= n; i++) 
        cin >> a[i]; 
    if (!ok(a)) cout << "NO\n";
    else cout << "YES\n";
}

int main() {
    fastio();
    int t;
    cin >> t; 
    while (t--) {
        solve();
    }
}