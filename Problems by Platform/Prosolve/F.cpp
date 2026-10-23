#include <bits/stdc++.h>
using namespace std;
int find(int x, vector<int>& dsu);
void unite(int a, int b, vector<int>& dsu, int& components);
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); 
    int n, q;
        cin >> n >> q;

        vector<char> type(q);
        vector<int> U(q, 0), V(q, 0);
        for (int i = 0; i < q; i++) {
            cin >> type[i];
            if (type[i] != '?') cin >> U[i] >> V[i];
        }
        vector<int> dsu(n, -1);
        int components = n;
        vector<int> answers;
        for (int i = q - 1; i >= 0; i--) {
            if (type[i] == '-') unite(U[i], V[i], dsu, components);
            else if (type[i] == '?') answers.push_back(components);
        }

    reverse(answers.begin(), answers.end());
    for (int x : answers) cout << x << "\n";

}
int find(int x, vector<int>& dsu) {
    if (dsu[x] < 0) return x;
    return dsu[x] = find(dsu[x], dsu);
}
void unite(int a, int b, vector<int>& dsu, int& components) {
    a = find(a, dsu);
    b = find(b, dsu);
    if (a == b) return;
    if (dsu[a] > dsu[b]) swap(a, b);
    dsu[a] += dsu[b];
    dsu[b] = a;
    components--;
}
