#include <span>
#include <vector>
#include <algorithm>
#include <ranges>
#include "../Structures/Matrix.cpp"

using namespace std;
using ll = long long;

// O(n*m) time and O(m) space
int lcs_length(span<int> A, span<int> B) {
    int n = A.size(); int m = B.size();
    if (n < m) swap(A, B), swap(n, m);
    vector<int> L(m + 1, 0);
    for (int i = 1; i <= n; i++) {
        int diag = 0;
        for (int j = 1; j <= m; j++) {
            int up = L[j];
            if (A[i-1] == B[j-1]) {
                L[j] = diag + 1;
            }
            else L[j] = max(L[j], L[j-1]); 
            diag = up;  
        }
    }
    return L[m];
}

// O(n*m) time and space
vector<int> lcs(span<int> A, span<int> B) {
    int n = A.size(); int m = B.size(); 
    Matrix<int> L(n + 1, m + 1, 0);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            L[i, j] = A[i-1] == B[j-1] ? L[i-1, j-1] + 1 : max(L[i-1, j], L[i, j-1]); 
        }
    }
    vector<int> result;
    for (int i = n, j = m; i > 0 && j > 0; ) {
        if (A[i-1] == B[j-1]) result.push_back(A[i-1]), i--, j--;
        else if (L[i-1, j] > L[i, j-1]) i--;
        else j--;
    }
    ranges::reverse(result);
    return result;
}

// O(nlog n) time best case, O(m*n) worst case, O(m) space
int lcs_via_lis() {}

// O(nlog n) time best case, O(m*n) worst case, O(m) space
vector<int> lcs_via_lis(int a) {}

// depends on m, whether to use lis, or stick to lcs. 
vector<int> lcs_hybrid() {}