#pragma once
#include <vector>
#include <span>
#include <bit>
#include "Matrix.cpp"
using namespace std;

// Builds a Disjoint sparse table from input nums, into table. table dimensions must be (ceil(log2(n)), n).
void dst_build(auto& table, const auto& nums, auto f) { assert(table.rows() == bit_width(size(nums)));
    unsigned int n = size(nums);
    int ceil_log2_n = bit_width(n);
    unsigned int w = (1 << ceil_log2_n);
    for (int i = 0; i < ceil_log2_n; i++) { 
        for (int start = 0; start < n; start += w) {
            int mid = min(start + w/2 - 1, n-1);
            int end = min(start + w - 1, n-1);
            for (int j = mid; j >= start; j--) {
                table[i, j] = (j < mid) ? f(nums[j], table[i, j + 1]) : nums[j];
            }
            for (int j = mid + 1; j <= end; j++) {
                table[i, j] = (j > mid + 1) ? f(table[i, j - 1], nums[j]) : nums[j];
            }
        }
        w /= 2;
    }
}

// Queries the Disjoint Sparse Table made from n elements, from a to b. UNDEFINED for a >= b
auto dst_query(auto&& table, unsigned int n, unsigned int a, unsigned int b, auto f) {
    assert(a < b);
    int i = bit_width(n) - bit_width(a^b); // first differing bit index
    return f(table[i, a], table[i, b]);
}

template<typename T, typename F, typename Table = Matrix<T>>
struct DisjointSparseTable {
    unsigned int n;
    int k;
    F f;
    Table table;
    span<const T> nums;
    DisjointSparseTable(F func = F{}) : f(func) {} 
    void build(const auto& input) {
        n = size(input);
        nums = input;
        k = bit_width(n); 
        table.resize(k, n);
        dst_build(table, input, f);
    }
    
    T query(unsigned int a, unsigned int b) {
        if (a == b) return nums[a];
        return dst_query(table, n, a, b, f);
    }
};


/*
#include <random>
#include <iostream>
int brute_sum(const vector<int>& nums, int a, int b) {
    int result = nums[a];
    for (int i = a+1; i <= b; i++) result += nums[i];
    return result;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    mt19937 rng(42);
    int n;
    cin >> n;
    vector<int> nums(n);
    for (auto& x : nums) x = rng() % 100 + 1;
    DisjointSparseTable<int, plus<int>> dst; dst.build(nums);
    cout << "n=" << n << " nums:";
    for (int x : nums) cout << " " << x;
    cout << endl;
    
    for (int a = 0; a < n; a++) {
        for (int b = a; b < n; b++) {
            cout << "query a=" << a << " b=" << b
                    << " brute=" << brute_sum(nums, a, b) << '\n'
                    << "dst=" << dst.query(a, b) << '\n';
            if (brute_sum(nums, a, b) != dst.query(a, b)) {
                cout << "Error: mismatch for query(" << a << ", " << b << ")\n";
                return 1;
            }
        }
    }
    cout << "All queries passed successfully!" << endl;
    
}
*/