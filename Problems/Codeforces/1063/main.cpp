#include <bits/stdc++.h>
using namespace std;
using ll = long long;
bool check(vector<int>& a, int n) {
    if (n == 2) return true;
    for (int i = 1; i <n-1; i += 2) {
        if (a[i] != a[i + 1]) {
            return false;
        }
    }
    return true;
}
void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    sort(a.begin(), a.end());
    if (check(a, n)) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    while (T--) solve();
    return 0;
}
