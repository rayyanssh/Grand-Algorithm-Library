#include <bits/stdc++.h>
using namespace std;
tuple<int, int, int> history[(1 << 20) + 67];
vector<int> layer(21);
void move_cakes(int l, int A, int B, int C, int& moves) { 
    if (l == 0) return; // no more layers to move
    int a = layer[l]; 
    if (a == 0) {
        move_cakes(l - 1, A, C, B, moves);
        moves++;
        history[moves] = {l, A, C};
        move_cakes(l - 1, B, A, C, moves);
    }
    else {
        move_cakes(l - a - 1, A, C, B, moves);
        moves++;
        history[moves] = {l, A, C};
        move_cakes(l - a - 1, B, C, A, moves);      
        move_cakes(l - 1, A, B, C, moves);  
    }
}
bool possible(int n) {  
    for (int i = 1; i <= n; i++) if (layer[i] >= i) return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t; cin >> t;
    while (t--) {
        int n; cin >> n; 
        for (int i = 1; i <= n; i++) {
            int a; cin >> a;
            layer[i] = a;
        } 
        int moves = 0;  
        if (possible(n)) {
            move_cakes(n, 1, 2, 3, moves);
            cout << "YES\n" << moves << '\n';
            for (int i = 1; i <= moves; i++) {
                auto [l, A, C] = history[i];
                cout << l << ' ' << A << ' ' << C << '\n';
            }
        } else {
            cout << "NO\n";
        }
    }
}