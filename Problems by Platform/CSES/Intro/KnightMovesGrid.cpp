#include "../../../Grand/Structures/Deque.hpp"
#include <cassert>
#include <iostream>
using namespace Grand;
using namespace std;
template<typename T>
struct Matrix {
    int n,m; vector<T> v;
    Matrix(int r, int c, T val = {}) : n(r), m(c), v(r*c,val) {}
    T& operator[](int i, int j) { return v[i*m + j]; }
};
void print(auto&& M) { for (int i = 0; i < M.n; i++) { for (int j = 0; j < M.m; j++) { cout << M[i,j] << ' '; } cout << '\n'; } }

array<pair<int,int>, 8> neighbors(int i, int j) {
    return {
        pair{i-1,j-2}, {i-2,j-1}, {i-2,j+1}, {i-1,j+2}, 
            {i+1,j+2}, {i+2,j+1}, {i+2,j-1}, {i+1,j-2}
    };
}

Matrix<int> knight_moves_grid(int n) { assert(n>=4);
    Matrix<int> M(n,n);
    M[0,0] = M[2,1] = M[1,2] = 1;
    Deque<pair<int, int>> q; q.reserve(4*n);
    q.push_back({2,1});
    auto valid = [=](int i, int j) { return 0<=i&&i<n&&0<=j&&j<n; };
    while (!q.empty()) {
        auto [i,j] = q.pop_front();
        for (auto [ni, nj] : neighbors(i, j)) {
            if (valid(ni, nj) && !M[ni, nj]) {
                M[ni, nj] = M[nj, ni] = M[i, j] + 1;
                q.push_back({ni, nj});
            }  
        }
    }
    M[0,0] = 0;
    return M;
} 

int main() {
    int n;
    cin >> n;
    print(knight_moves_grid(n));
}