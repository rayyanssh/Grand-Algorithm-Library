#include <bits/stdc++.h>
using namespace std;
namespace rgs=ranges;

template<typename T>
struct Matrix {
    int n,m; vector<T> v;
    Matrix(int r, int c, T val = {}) : n(r), m(c), v(r*c,val) {}
    T& operator[](int i, int j) { return v[i*m + j]; }
};
// O(nm) time and space
vector<int> LCS(span<int> A, span<int> B) {
    int n = A.size(); int m = B.size(); 
    Matrix<int> L(n + 1, m + 1, 0);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            L[i, j] = A[i-1] == B[j-1] ? 
                      L[i-1, j-1] + 1 : 
                      max(L[i-1, j], L[i, j-1]); 
        }
    }
    vector<int> result;
    for (int i = n, j = m; i > 0 && j > 0; ) {
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

int main() {
    int n, m; 
    cin >> n >> m;
    vector<int> A(n), B(m);
    for (int i = 0 ; i < n; i++) cin >> A[i];
    for (int i = 0 ; i < m; i++) cin >> B[i];
    vector<int> lcs = LCS(A,B);
    cout << lcs.size() << '\n';
    for (int i = 0; i < lcs.size(); i++) cout << lcs[i] << ' ';
}