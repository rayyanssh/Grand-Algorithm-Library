#include <vector>
#include "Matrix.cpp"
#include <bit>

using namespace std;

// Builds a sparse table from input nums of size n, into a 2D table, using f. The table must be preallocated with size k x n, where k = bit_width(n). 
void build_sparse_table(const auto& nums, auto& table, auto f) {
    auto n = size(nums); 
    int k = bit_width(n) - 1; 
    for (int j = 0; j < n; j++) table[0, j] = nums[j];
    for (int i = 1; i <= k; i++) {
        for (int j = 0; j <= n - (1 << i); j++) {
            table[i, j] = f(table[i-1, j], table[i-1, j + (1 << (i-1))]);
        }
    }
}

// Queries the Sparse Table, from a to b.
auto query_sparse_table(integral auto a, integral auto b, auto& table, auto f) {
    unsigned int len = b - a + 1;
    int k = bit_width(len) - 1;
    int w = 1 << k;
    return f(table[k, a], table[k, b - w + 1]);
}

template<typename T, typename F, typename Table = Matrix<T>>
struct SparseTable {
    int n, k;
    F f;
    Table table;
    SparseTable(F func = F{}) : f(func) {}
    void build(const auto& nums) {
        n = size(nums);
        k = bit_width((unsigned int)n); // log2(n) + 1
        table = Table(k, n);
        build_sparse_table(nums, table, f);
    }
    
    T query(int a, int b) {
        return query_sparse_table(a, b, table, f);
    }
};
auto sparse_table(const auto& nums, auto f) {
    SparseTable<typename std::remove_cvref_t<decltype(nums[0])>, decltype(f)> st(f);
    st.build(nums);
    return st;
}



// Example usage 
/* 
*/
#include <iostream>
int main() {
    vector<int> nums = {1,3,5,7,9,4,8,2}; 
    vector<int> store(4 * 8); 
    MatrixView S(store, 4, 8);
    build_sparse_table(nums, S, ranges::min); // works
    struct min_functor { int operator()(int a, int b) { return min(a, b); } };
    SparseTable<int, min_functor> st; st.build(nums); // works 
    cout << st.query(1, 5) << endl; // should output 3

    auto Min = [](int a, int b) { return min(a, b); };
    SparseTable<int, decltype(Min)> st2(Min); st2.build(nums); // works
    cout << st2.query(1, 5) << endl; // 3

    auto st3 = sparse_table(nums, Min); // works
    cout << st3.query(1, 5) << endl; // 3
}

