#pragma once
#include <cassert> 
#include <array>
#include <vector>
#include <span>
#include "../../Utils.cpp"  // brings constexpr int dynamic = -1
template<typename M> concept MatrixType = requires(M A) { A.rows(); A.cols(); A[0, 0]; };   
template<typename M> concept ViewMatrixType = MatrixType<M> && !requires(M A) { A.storage; };
/*
    Matrix. 1D storage, 2D indexing. Dynamic or Fixed size.
    MatrixView. To reinterpret contiguous data as a 2D matrix. 
    SubMatrix. A submatrix view into a matrix.
    Matrify. Convert a nested type (like vector<vector<T>>) with [i][j] access 
            to a 2D matrix interface supporting [i,j] and rows(), cols(). 
            I found it useful on leetcode problems with a nested vector input representing a matrix.
*/

template<typename T>
struct MatrixRules {
    constexpr void copy(const MatrixType auto& other, int n, int m);
    constexpr void fill(T val, int n, int m);
    constexpr void operator=(const MatrixType auto& other) { copy(other, rows(), cols()); }
    constexpr void operator=(T val) { fill(val, rows(), cols()); }
    T& operator[](int i, int j);
    const T& operator[](int i, int j) const;
    constexpr int rows() const;
    constexpr int cols() const;
    constexpr long long size() { return rows() * cols(); }
    constexpr bool operator==(const MatrixType auto& other);
    using value_type = T;
};
template<typename T, int N = dynamic, int M = dynamic>
struct Matrix;

template<typename T, int N, int M> 
struct Matrix : MatrixRules<T> {
    std::array<T, N * M> storage;
    T& operator[](int k) { return storage[k]; }
    const T& operator[](int k) const { return storage[k]; }
    constexpr int rows() { return N; }
    constexpr int cols() { return M; }
    using MatrixRules<T>::operator=;
};

template<typename T, int N, int M> requires (N == dynamic || M == dynamic)
struct Matrix : MatrixRules<T> {
    std::vector<T> storage; int n, m;
    Matrix(int r, int c, T val = {}) : n(max(r, N)), m(max(c, M)), storage(n * m, val);
    Matrix(int dim, T val = {}) requires !(N == dynamic && M == dynamic) : n(N), m(M) {
        if constexpr (N == dynamic) n = dim; 
        else m = dim;
        storage.resize(n * m, val);
    }
    T& operator[](int k) { return storage[k]; }
    const T& operator[](int k) const { return storage[k]; }
    constexpr int rows() { return N == dynamic ? n : N ; }
    constexpr int cols() { return M == dynamic ? m : M; }
    using MatrixRules<T>::operator=;
};

template<MatrixType M>
using HoldThisMatrix = conditional_t<
    ViewMatrixType<M>,
    remove_reference_t<M>,
    M&
>;

// Makes a view of the submatrix defined by corner (i,j) and the dimensions (rows, cols).
// constexpr int dynamic = -1;
// members: M& or M matrix; int start_i, start_j, r, c;
template<MatrixType M, int R = dynamic, int C = dynamic>
struct SubMatrix : MatrixRules<typename M::value_type> {
    HoldThisMatrix<M> matrix;  int start_i, start_j, r, c; 
    auto& operator[](int i, int j) const { return matrix[start_i + i, start_j + j]; }
    constexpr int rows() const { return R == dynamic ? r : R; }
    constexpr int cols() const { return C == dynamic ? c : C; } 
    using MatrixRules<typename M::value_type>::operator=;
};  
auto submatrix(MatrixType auto&& matrix, int i, int j, int rows, int cols) { assert(i + rows <= matrix.rows() && j + cols <= matrix.cols());
    using M = remove_reference_t<decltype(matrix)>;
    return SubMatrix<M>{matrix, i, j, rows, cols}; 
}
template<int R, int C> 
constexpr auto submatrix(MatrixType auto&& matrix, int i, int j, int rows = R, int cols = C) { 
    using M = remove_reference_t<decltype(matrix)>;
    return SubMatrix<M, R, C>{
        matrix, i, j, 
        R == dynamic ? rows : R, 
        C == dynamic ? cols : C
    };
}

constexpr bool is_equal(const MatrixType auto& A, const MatrixType auto& B) {
    if (A.rows() != B.rows() || A.cols() != B.cols()) return false;
    for (int i = 0; i < A.rows(); i++) 
        for (int j = 0; j < A.cols(); j++) 
            if (A[i, j] != B[i, j]) return false;
    return true;
}
constexpr bool is_square(const MatrixType auto& A) { return A.rows() == A.cols(); }

template<typename T> concept NestedType2D = requires(T t) { t[0][0]; size(t); size(t[0]); };
// Convert a 2d array or vector, or similar nested type
// to a 2D interface supporting [i,j] and rows(), cols()
template<NestedType2D G>
struct Matrify {
    G& grid;
    auto& operator[](int i) const { return grid[i]; }
    auto& operator[](int i, int j) const { return grid[i][j]; }
    Matrify(G& g) : grid(g) {}
    constexpr int rows() const { return size(grid); }
    constexpr int cols() const { return size(grid[0]); }
    using value_type = remove_reference_t<decltype(grid[0][0])>;
};


 