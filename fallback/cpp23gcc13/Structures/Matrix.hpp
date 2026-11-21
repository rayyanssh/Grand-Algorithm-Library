#pragma once
#include <algorithm>
#include <cassert>
#include <vector>
#include <ostream>
#include "../Utils.hpp"  // brings constexpr int dynamic = -1
namespace Grand {
template<typename M> concept MatrixType = requires(M A) { A.rows(); A.cols(); A[0, 0]; };   
/*
    `Matrix`.    Contiguous storage, 2D indexing. Dynamic or Fixed size. 
    `SubMatrix`. A submatrix view into a matrix.
     Helpers.    `operator A == B` , `is_square(A)`, `print(A)` do what they say.
                 `getN<MatrixType>, getM<MatrixType>`, gets the compile-time dimensions of the matrix,
                 `-1 (dynamic)` if not compile-time.
                 `HoldThisMatrix<M>` holds a handle to the matrix without deep copying it.
    `Matrify`.   Convert a nested type (like `vector<vector<T>>`) with `[i][j]` access 
                 to a 2D matrix interface with `[i,j]` access and `rows(), cols()`. 
                 I found it useful on leetcode problems with a nested vector input representing a matrix.
    For all matrices,`operator=(MatrixType other)` copies the elements of the other matrix to this
                     `operator=(val)` fills the matrix with `val`.
                     `copy(other, n, m); fill(n, m, val);`
                     `operator==(A, B)` returns whether `A == B`
*/

template<typename Derived, typename T>
struct MatrixRules {
    using value_type = T;
    constexpr void copy(const MatrixType auto& other, int n, int m) {
        auto& self = static_cast<Derived&>(*this);
        assert(n <= self.rows() && m <= self.cols()); assert(n <= other.rows() && m <= other.cols());
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                self[i, j] = other[i, j]; 
    } 
    constexpr void fill(int n, int m, T val) {
        auto& self = static_cast<Derived&>(*this);
        assert(n <= self.rows() && m <= self.cols());
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                self[i, j] = val;
    }
    constexpr void fill(T val) { 
        auto& self = static_cast<Derived&>(*this);
        fill(self.rows(), self.cols(), val); 
    }
    // Assignment operator=(MatrixType other) copies the elements of the other matrix to this.
    constexpr Derived& operator=(const MatrixType auto& other) {
        auto& self = static_cast<Derived&>(*this);
        assert(other.rows() == self.rows() && other.cols() == self.cols());
        self.copy(other, other.rows(), other.cols()); 
        return self;
    }
    // Assignment operator=(val) fills the matrix with a value.
    constexpr Derived& operator=(T val)  requires Number<T> {
        auto& self = static_cast<Derived&>(*this);
        self.fill(self.rows(), self.cols(), val); 
        return self;
    }
    constexpr long long size() const {
        const auto& self = static_cast<const Derived&>(*this);
        return 1LL * self.rows() * self.cols();
    }
};

template<typename T, int N = dynamic, int M = dynamic>
struct Matrix;

// Fixed size matrix. For example: Matrix<int,3,4> M; creates a 3x4 matrix of ints.
template<typename T, int N, int M> 
struct Matrix : MatrixRules<Matrix<T, N, M>, T> {
    array<T, N * M> storage;
    constexpr Matrix() : storage{} {}
    constexpr Matrix(T val) { storage.fill(val); }
    constexpr explicit Matrix(int n, int m, T val = {}) { storage.fill(val); assert(n == N && m == M); }
    constexpr Matrix(init_list<T> init) : storage{} { rgs::copy(init, storage.begin()); } 
    constexpr Matrix(const MatrixType auto& other) { *this = other; } // Deep copy

    constexpr T& operator[](int k) { return storage[k]; }
    constexpr const T& operator[](int k) const { return storage[k]; }
    constexpr T& operator[](int i, int j) { return storage[i * M + j]; }
    constexpr const T& operator[](int i, int j) const { return storage[i * M + j]; }
    constexpr auto row(int i) { return span(&(*this)[i, 0], cols()); }
    constexpr auto row(int i) const { return span(&(*this)[i, 0], cols()); }
    // Returns a view of the matrix. `MatrixView`.
    constexpr auto view();
    constexpr auto view() const;

    constexpr int rows() const { return N; }
    constexpr int cols() const { return M; }
    constexpr auto begin() { return storage.begin(); }
    constexpr auto begin() const { return storage.begin(); }
    constexpr auto   end() { return storage.end(); }
    constexpr auto   end() const { return storage.end(); }
    constexpr auto* data() { return storage.data(); }
    constexpr const auto* data() const { return storage.data(); }
    using MatrixRules<Matrix<T, N, M>, T>::operator=; 
};

// Dynamic size matrix. 
// Example: Matrix<int> M(3,4) creates a 3x4 matrix of ints.
// Example: Matrix<int, dynamic, 5> M(3,5) creates a 3x5 matrix of ints. 
// .storage is a vector<T> of size n*m. T = bool won't compile.
// A 1 by m or n by 1 Matrix is a Vector, defined by an alias,
template<typename T, int N, int M> requires (N == dynamic || M == dynamic)
struct Matrix<T, N, M> : MatrixRules<Matrix<T, N, M>, T> {
    int n, m;
    std::vector<T> storage;
    Matrix() : n(max(0, N)), m(max(0, M)) {}
    explicit Matrix(int r, int c, T val= {}) requires (N != 1 && M != 1) { resize(r, c, val); }
    explicit Matrix(int r, int c, init_list<T> init) { 
        resize(r, c, {});
        rgs::copy(init, storage.begin()); 
    }
    Matrix(const MatrixType auto& other) requires (N != 1 && M != 1) { *this = other; } // Deep copy

    Matrix& operator=(const MatrixType auto& other) requires (N != 1 && M != 1) { 
        resize(other.rows(), other.cols());
        this->copy(other, n, m); 
        return *this;
    }

    T& operator[](int k) { return storage[k]; }
    const T& operator[](int k) const { return storage[k]; }
    T& operator[](int i, int j) { return storage[i * cols() + j]; }
    const T& operator[](int i, int j) const { return storage[i * cols() + j]; }
    auto row(int i) { return span(&(*this)[i, 0], cols()); }
    auto row(int i) const { return span(&(*this)[i, 0], cols()); }
    // Returns a view of the matrix. `MatrixView`.
    constexpr auto view();
    constexpr auto view() const;

    void resize(int r, int c, T val = {}) requires (N != 1 && M != 1) { 
        n = r;      assert(N == dynamic || r == N); 
        m = c;      assert(M == dynamic || c == M);
        storage.resize(n*m, val); 
    } 

    void append_row(const auto& row) requires (N == dynamic && M != 1) { 
        assert(row.size() == cols()); 
        storage.insert(storage.end(), row.begin(), row.end()); 
        n++; 
    }  
    void clear() { 
        if constexpr (N == dynamic) n = 0; 
        if constexpr (M == dynamic) m = 0;
        storage.clear(); 
    }
    void reserve(long long size) { storage.reserve(size); } 

    constexpr int rows() const { return N == dynamic ? n : N; }
    constexpr int cols() const { return M == dynamic ? m : M; } 
    auto begin() { return storage.begin(); }
    auto begin() const { return storage.begin(); }
    auto   end() { return storage.end(); }
    auto   end() const { return storage.end(); }
    auto* data() { return storage.data(); }
    const auto* data() const { return storage.data(); }
    using MatrixRules<Matrix<T, N, M>, T>::operator=; 

    // Specialization for Vectors: 1 by m or n by 1 matrices.

    explicit Matrix(int d, T val = {}) requires (N == 1 || M == 1) { resize(d, val); }
    explicit Matrix(int d, init_list<T> init) {
        resize(d);
        rgs::copy(init, storage.begin());
    }
    void resize(int d, T val = {}) requires (N == 1 || M == 1) {
        storage.resize(d, val);
        if constexpr (N == 1) { n = 1; m = d; }
        else { n = d; m = 1; }
    } 
    Matrix(const MatrixType auto& other) requires (N == 1 || M == 1) { *this = other; }
    void operator=(const MatrixType auto& other) requires (N == 1 || M == 1) {
        resize(other.rows() * other.cols());
        this->copy(other, rows(), cols());
    }
};
template<typename T, int N = dynamic>
using Vector = Matrix<T,N,1>;

template<typename T, int M = dynamic>
using RowVector = Matrix<T,1,M>;

template<typename M> concept ViewMatrixType = MatrixType<M> && !requires(M A) { A.storage; };

// Decide if to hold a copy of the matrix (if its a cheap view type), or a reference.
template<MatrixType M>
using HoldThisMatrix = conditional_t<
    ViewMatrixType<M>,
    rm_ref<M>,
    M&
>;


// Makes a view of the submatrix defined by corner `(i,j)` and the dimensions `(rows, cols)`.
// `constexpr int dynamic = -1;`.
// members: `M&` or `M matrix;` `int start_i, start_j, r, c;`. 
template<MatrixType M, int R = dynamic, int C = dynamic>
struct SubMatrix : MatrixRules<SubMatrix<M, R, C>, typename M::value_type> {
    HoldThisMatrix<M> matrix;  int start_i, start_j, r, c; 
    auto& operator[](int i, int j) const { return matrix[start_i + i, start_j + j]; }
    constexpr int rows() const { return R == dynamic ? r : R; }
    constexpr int cols() const { return C == dynamic ? c : C; } 
    constexpr SubMatrix& operator=(const SubMatrix& other) { 
        this->copy(other, rows(), cols());
        return *this;
    }
    using MatrixRules<SubMatrix<M, R, C>, typename M::value_type>::operator=; 
};

// Returns a view of the submatrix defined by corner `(i,j)` and the dimensions `(rows, cols)`. 
// Omit `cols` to take every column from `j` to end. Omit `rows` and `cols` to take every row and col from `i, j` to end. 
template<int R = dynamic, int C = dynamic> 
constexpr auto submatrix(MatrixType auto&& matrix, int i, int j, int rows = R, int cols = C) { 
    if (rows == dynamic) rows = matrix.rows() - i;
    if (cols == dynamic) cols = matrix.cols() - j;
    using M = rm_ref<decltype(matrix)>;
    return SubMatrix<M, R, C>{ {},
        matrix, i, j, 
        rows,
        cols
    };
}

constexpr bool operator==(const MatrixType auto& A, const MatrixType auto& B) {
    if (A.rows() != B.rows() || A.cols() != B.cols()) return false;
    for (int i = 0; i < A.rows(); i++) 
        for (int j = 0; j < A.cols(); j++) 
            if (A[i, j] != B[i, j]) return false;
    return true;
}
constexpr bool is_square(const MatrixType auto& A) { return A.rows() == A.cols(); } 

std::ostream& operator<<(std::ostream& out, const MatrixType auto& A) {
    for (int i = 0; i < A.rows(); i++)
        for (int j = 0; j < A.cols(); j++) 
            out << A[i,j] << (j < A.cols()-1 ? ' ' : '\n');
    return out;
}



}