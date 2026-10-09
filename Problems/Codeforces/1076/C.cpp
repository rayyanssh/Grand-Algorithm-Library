#include <bits/stdc++.h>
using namespace std;
void solve() {
    int n, q; cin >> n >> q;
    vector<int> a(n), b(n), pref(n + 1, 0);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < n; i++) cin >> b[i];
    for (int i = 0; i < n; i++) a[i] = max(a[i], b[i]);
    for (int i = n - 2; i >= 0; i--) a[i] = max(a[i], a[i + 1]);
    for (int i = 1; i <= n; i++) pref[i] = pref[i - 1] + a[i - 1];
    for (int i = 0; i < q; i++) {
        int l, r;
        cin >> l >> r;
        cout << pref[r] - pref[l - 1] << '\n';
    }

}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int T; cin >> T;
    while (T--) {
        solve();
    }
    // Your code here
    
    return 0;
}                          