#include <bits/stdc++.h>
#include "../../Algorithms/Structures/sparse_table.cpp"
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<int> nums(n);
    for (int i = 0; i < n; i++) cin >> nums[i];

    struct Min { int operator()(int a, int b) { return min(a, b); } };
    SparseTable<int, Min> st; st.build(nums);
    
    while (q--) {
        int a, b; cin >> a >> b;
        cout << st.query(a-1, b-1) << '\n';
    }
}