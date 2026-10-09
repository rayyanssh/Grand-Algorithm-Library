#include <bits/stdc++.h>
using namespace std;

void towerofhanoi_iterative(int n) {
    int moves = (1<<n) - 1;
    int A = 1; int B = 2; int C = 3;
    vector<tuple<int,int,int,int>> v;
    while (moves) {
        while (n) {
            v.push_back({A, B, C, n});
            n--;
            swap(B,C);
            //cout << A << ' ' << B << " " << C << " " << n << '\n';
        }
        tie(A, B, C, n) = v.back();
        v.pop_back();
        cout << A << " " << C << " " << '\n';
        moves--;
        swap(A,B); n--;
        //cout << A << " " << B << " " << C << " " << n << '\n';
    }
}
void towerofhanoi(int n, int A, int B, int C) {
    if (n == 0) return;
    towerofhanoi(n-1, A, C, B);
    towerofhanoi(n-1, B, A, C);
}
void solve() {
    int n;
    cin >> n;

    cout << (1<<n) - 1 << '\n';
    towerofhanoi_iterative(n);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    // int T; cin >> T;
    // while(T--) solve();
    return 0;
}
