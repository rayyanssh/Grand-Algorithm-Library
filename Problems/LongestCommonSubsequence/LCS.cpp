#include <span>
#include <vector>
#include <algorithm>
#include <ranges>
#include "../../Grand/Matrix.hpp"
 
using namespace Grand;
using ll = long long;

// O(nm) time and O(n) space
template<typename T = int>
int LCS_length(span<const T> A, span<const T> B) {
    if (A.size() > B.size()) swap(A, B);
    int n = A.size(); int m = B.size();
    vector<int> l(n); 
    for (int i = 0; i < m; i++) {
        int diag = 0;
        for (int j = 0; j < n; j++) {
            int up = l[j];
            if (A[j] == B[i]) 
                l[j] = diag + 1;
            else {
                int left = (j > 0) ? l[j-1] : 0;
                l[j] = max(left, up);
            }
            diag = up;
        }
    }
    return l[n-1];
}

// O(nm) time and space, returns vector<T>
auto LCS(const auto& A, const auto& B) {
    int n = A.size(); int m = B.size(); 
    Matrix<int> L(n + 1, m + 1, 0);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            L[i, j] = A[i-1] == B[j-1] ? 
                      L[i-1, j-1] + 1 : 
                      max(L[i-1, j], L[i, j-1]); 
        }
    }
    using T = decltype(A)::value_type;
    vector<T> result;
    for (int i = n, j = m; ; ) {
        if (A[i-1] == B[j-1]) { 
            result.push_back(A[i-1]); 
            i--; j--; 
        }
        else if (L[i-1,j] > L[i,j-1]) i--;
        else j--;
    }
    rgs::reverse(result);
    return result;
}
 