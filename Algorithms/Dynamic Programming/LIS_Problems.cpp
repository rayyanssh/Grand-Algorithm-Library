#include <span>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ranges>
#include "LIS_longest_increasing_subsequence.cpp"
using namespace std;

// using LIS to solve LCS
int lcs_via_lis_length(span<int> A, span<int> B) {
    int n = A.size(); int m = B.size();
    for (int i = 0; i < m; i++) 
        int j = ranges::find(A, B[i]);
} 