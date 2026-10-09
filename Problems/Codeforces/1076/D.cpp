#include <bits/stdc++.h>
using namespace std;
#define MAX 1000000000
using ll = long long;
void solve() {
    int n; cin >> n;
    vector<int> a(n+1,0), b(n+1,0);
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) cin >> b[i];
    int m = 0; int N = n;
    while (n - b[m+1] >= 0 and m + 1< N) {
        n -= b[m+1];
        m++;
    }
    ll max_score = 0; int min_a = 1e9; ll curr_score = 0; 
    for (int i = 1; i <= m; i++) {
        if (a[i] < min_a) {
            min_a = a[i];
            if (max_score > min_a * m) break;
        }
        curr_score = min_a * i;
        max_score = max(max_score, curr_score);
        cout << "i: " << i << " min_a: " << min_a << " curr_score: " << curr_score << " max_score: " << max_score << "\n";
    }
    cout << max_score << "\n";
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