#include <bits/stdc++.h>
using namespace std;
using ll = long long;
void solve() {
    int n; cin >> n; int N = 2*n; int MAX = 400000;
    vector<int> a(N); int seen[MAX+1] = {0};
    for (int i=0; i<N; i++) {
        cin >> a[i];
        seen[a[i]]++;
    }
    vector<int> counts;
    for (int i=1; i<=N; i++) {
        if (seen[i] > 0) {
            counts.push_back(seen[i]);
        }
    }
    sort(counts.begin(), counts.end()); int C = counts.size(); int f=0;
    for (int i=0; i<C; i++) {
        if (counts[i] % 2 == 1) f++;
        else f+=2;
    }
    cout << f << '\n';
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    while (T--) solve();
    return 0;
}