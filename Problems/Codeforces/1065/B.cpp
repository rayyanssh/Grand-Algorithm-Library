#include <bits/stdc++.h>
using namespace std;
using ll = long long;
void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    if (a[0] == -1 && a[n-1] != -1) a[0] = a[n-1];
    else if (a[n-1] == -1 && a[0] != -1) a[n-1] = a[0];
    else if (a[0] == -1 && a[n-1] == -1) a[0] = a[n-1] = 0;
    cout << abs(a[n-1] - a[0]) << '\n';
    for (int i = 0; i < n; ++i) cout << max(a[i], 0) << ' ';
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    while (T--) solve();
    return 0;
}