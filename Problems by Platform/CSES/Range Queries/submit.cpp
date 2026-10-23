#line 1 "static_range_minimum_queries.cpp"
#include <bits/stdc++.h>
#line 2 "/home/rayyan/Desktop/library/Algorithms/Structures/sparse_table.cpp" 
#include <bit>

using namespace std;

// Builds a sparse table from input nums of size n, into a 2D mdspan table, using f. The table must be preallocated with size k x n, where k = bit_width(n). 
void build_sparse_table(const auto& nums, auto& table, auto f) {
    unsigned int n = size(nums); 
    int k = bit_width(n) - 1; 
    for (int j = 0; j < n; j++) table[0][j] = nums[j];
    for (int i = 1; i <= k; i++) {
        for (int j = 0; j <= n - (1 << i); j++) {
            table[i][j] = f(table[i-1][j], table[i-1][j + (1 << (i-1))]);
        }
    }
}

auto query_sparse_table(int a, int b, auto& table, auto f) {
    unsigned int len = b - a + 1;
    int k = bit_width(len) - 1;
    int w = 1 << k;
    return f(table[k][a], table[k][b - w + 1]);
}

template<typename T, typename F>
struct SparseTable {
    int n, k;
    F f;
    vector<T> container;
    vector<vector<T>> table;

    void build(const auto& nums) {
        n = size(nums);
        k = bit_width((unsigned int)n) - 1;
        container.resize((k + 1) * n);
        table.resize(k + 1, vector<T>(n));
        build_sparse_table(nums, table, f);
    }
    
    T query(int a, int b) {
        return query_sparse_table(a, b, table, f);
    }
};


// Example usage 
/* 
vector<int> nums = {1,3,5,7,9,4,8,2}; 
vector<int> store(4 * 8); 
mdspan S(store.data(), 4, 8);
build_sparse_table(nums, S, ranges::min); // works
struct min_functor { int operator()(int a, int b) { return min(a, b); } };
SparseTable<int, min_functor> st; st.build(nums); // works 
*/

#line 3 "static_range_minimum_queries.cpp"
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
