#pragma once
#include <cassert> 
#include <array>
#include <vector>
#include <span>
#include <iostream>
#include "../Utils.hpp"  // brings constexpr int dynamic = -1
namespace Grand {
template<typename M> concept MatrixType = requires(M A) { A.rows(); A.cols(); A[0, 0]; };   
template<typename M> concept ViewMatrixType = MatrixType<M> && !requires(M A) { A.storage; };
/*
    Matrix. Contiguous storage, 2D indexing. Dynamic or Fixed size.
    MatrixView. To reinterpret contiguous data as a 2D matrix. 
    SubMatrix. A submatrix view into a matrix.
    Helpers. is_equal(A,B), is_square(A)
    Matrify. Convert a nested type (like vector<vector<T>>) with [i][j] access 
            to a 2D matrix interface with [i,j] access and rows(), cols(). 
            I found it useful on leetcode problems with a nested vector input representing a matrix.
    In all matrices, operator=(MatrixType other) copies the elements of the other matrix to this
                     operator=(val) fills the matrix with val.
                     copy(other, n, m); fill(n, m, val); print();
    
*/

struct MatrixRules {
    constexpr void copy(this auto& self, const MatrixType auto& other, int n, int m);
    constexpr void fill(this auto& self, int n, int m, auto val);
    // Assignment operator=(MatrixType other) copies the elements of the other matrix to this.
    constexpr void operator=(this auto& self, const MatrixType auto& other) { assert(other.rows() == self.rows() && other.cols() == self.cols());
        self.copy(other, other.rows(), other.cols()); 
    }
    // Assignment operator=(val) fills the matrix with a value.
    constexpr void operator=(this auto& self, auto val) { 
        self.fill(self.rows(), self.cols(), val); 
    }
    constexpr long long size(this const auto& self) { return 1LL * self.rows() * self.cols(); }
    constexpr bool operator==(this const auto& self, const MatrixType auto& other); 
    auto row(this auto&& self, int i); // Defined in Library/Matrix/Iterator.cpp, or Library/Matrix/Library.hpp
    auto col(this auto&& self, int j); // Defined in Library/Matrix/Iterator.cpp, or Library/Matrix/Library.hpp
    void print(this const auto& self); // Defined in Library/UtilsIO.hpp
};

template<typename T, int N = dynamic, int M = dynamic>
struct Matrix;

// Fixed size matrix. For example: Matrix<int,3,4> M; creates a 3x4 matrix of ints.
template<typename T, int N, int M> 
struct Matrix : MatrixRules {
    array<T, N * M> storage;
    constexpr Matrix() : storage{} {}
    constexpr Matrix(T val) { storage.fill(val); }
    constexpr explicit Matrix(int n, int m, T val = {}) { storage.fill(val); assert(n == N && m == M); }
    constexpr Matrix(const array<T, N * M>& arr) : storage(arr) {}
    constexpr Matrix(const MatrixType auto& other) { *this = other; }

    constexpr auto& operator[](this auto&& self, int k) { return self.storage[k]; }
    constexpr auto& operator[](this auto&& self, int i, int j) { return self.storage[i * M + j]; }
    constexpr auto row(this auto&& self, int i) { return span(&self[i, 0], self.cols()); }

    constexpr int rows() const { return N; }
    constexpr int cols() const { return M; }
    using MatrixRules::operator=;
    using value_type = T;
};

// Dynamic size matrix. 
// Example: Matrix<int> M(3,4) creates a 3x4 matrix of ints.
// Example: Matrix<int, dynamic, 5> M(3,5) creates a 3x5 matrix of ints. 
// .storage is a vector<T> of size n*m. T = bool won't compile.
template<typename T, int N, int M> requires (N == dynamic || M == dynamic)
struct Matrix<T, N, M> : MatrixRules {
    int n, m;
    vector<T> storage;
    Matrix() : n(max(0, N)), m(max(0, M)) {}
    explicit Matrix(int r, int c, T val={}) : n(r), m(c), storage(n*m, val) { if constexpr (N != dynamic) assert(r == N); if constexpr (M != dynamic) assert(c == M); }
    Matrix(const MatrixType auto& other) { *this = other; }
    
    void operator=(const MatrixType auto& other) { if constexpr (N != dynamic) assert(other.rows() == N); if constexpr (M != dynamic) assert(other.cols() == M); 
        n = other.rows(); 
        m = other.cols(); 
        storage.resize(n*m); 
        copy(other, n, m); 
    }

    auto& operator[](this auto&& self, int k) { return self.storage[k]; }
    auto& operator[](this auto&& self, int i, int j) { return self.storage[i * self.cols() + j]; }
    auto row(this auto&& self, int i) { return span(&self[i, 0], self.cols()); }

    void append_row(const auto& row) requires (N == dynamic) { 
        assert(row.size() == cols()); 
        storage.insert(storage.end(), row.begin(), row.end()); 
        n++; 
    }
    void pop_row() requires (N == dynamic) { resize(n-1, m); }
    void resize(int r, int c, T val={}) { if constexpr (N != dynamic) assert(r == N); if constexpr (M != dynamic) assert(c == M);
        n = r; 
        m = c; 
        storage.resize(n*m, val); 
    } 
    void clear() { 
        if constexpr (N == dynamic) n = 0; 
        if constexpr (M == dynamic) m = 0;
        storage.clear(); 
    }

    constexpr int rows() const { return N == dynamic ? n : N; }
    constexpr int cols() const { return M == dynamic ? m : M; } 
    using MatrixRules::operator=;
    using value_type = T;
};

// MatrixView. A view of contiguous data as a 2D matrix. It is similar to std::mdspan of dimension 2.
// For example, `MatrixView<int> MV(vec.data(), 3, 4)` creates a 3x4 view of vec. 
// ctors: MatrixView(), MatrixView(T* p, int r, int c), MatrixView(const MatrixView& other)
// However, assignment `operator=(const MatrixType auto& other)` copies the elements of the other matrix to this.
// If you want to copy only the attributes of another MatrixView, use .rebind(); A.rebind(B)
template<typename T, int N = dynamic, int M = dynamic>
struct MatrixView : MatrixRules {
    T* ptr; int n, m;
    constexpr MatrixView() : n(max(0, N)), m(max(0, M)), ptr(nullptr) {}
    constexpr explicit MatrixView(T* p, int r, int c) : n(r), m(c), ptr(p) { if constexpr (N != dynamic) assert(r == N); if constexpr (M != dynamic) assert(c == M); } 
    constexpr explicit MatrixView(T* p) requires (N != dynamic && M != dynamic) : n(N), m(M), ptr(p) {}
    
    constexpr T& operator[](int k) const { return ptr[k]; }
    constexpr T& operator[](int i, int j) const { return ptr[i * cols() + j]; }
    constexpr auto row(int i) const { return span(&(*this)[i, 0], cols()); }

    void rebind(const MatrixView& other) { 
        ptr = other.ptr; 
        n = other.rows(); 
        m = other.cols(); 
    } 

    void resize(int r, int c) requires (N == dynamic || M == dynamic) { if constexpr (N != dynamic) assert(r == N); if constexpr (M != dynamic) assert(c == M);
         n = r; m = c;
    } 
    
    constexpr int rows() const { return N == dynamic ? n : N; }
    constexpr int cols() const { return M == dynamic ? m : M; }
    using MatrixRules::operator=;
    using value_type = T;
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
struct SubMatrix : MatrixRules {
    HoldThisMatrix<M> matrix;  int start_i, start_j, r, c; 
    auto& operator[](int i, int j) const { return matrix[start_i + i, start_j + j]; }
    constexpr int rows() const { return R == dynamic ? r : R; }
    constexpr int cols() const { return C == dynamic ? c : C; } 
    using MatrixRules::operator=;
    using value_type = M::value_type;
};
template<int R = dynamic, int C = dynamic> 
constexpr auto submatrix(MatrixType auto&& matrix, int i, int j, int rows = 0, int cols = 0) { 
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

constexpr void MatrixRules::copy(this auto& self, const MatrixType auto& other, int n, int m) { assert(n <= self.rows() && m <= self.cols()); assert(n <= other.rows() && m <= other.cols());
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            self[i, j] = other[i, j];
}

constexpr void MatrixRules::fill(this auto& self, int n, int m, auto val) { assert(n <= self.rows() && m <= self.cols());
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            self[i, j] = val;
}
constexpr bool MatrixRules::operator==(this const auto& self, const MatrixType auto& other) {
    return is_equal(self, other);
}
void MatrixRules::print(this const auto& self) { 
    for (int i = 0; i < self.rows(); i++) {
        for (int j = 0; j < self.cols(); j++)
            std::cout << self[i, j] << " ";
        std::cout << "\n";
    }
}

template<typename T> concept NestedType2D = requires(T t) { t[0][0]; size(t); size(t[0]); };
// Convert a 2d array or vector, or similar nested type
// to a 2D interface supporting [i,j] and rows(), cols()
// I found it useful on leetcode problems with a nested vector input representing a matrix, it works with all my functions
template<NestedType2D G>
struct Matrify : MatrixRules {
    G& grid;
    Matrify(G& g) : grid(g) {}
    auto& operator[](int i) const { return grid[i]; }
    auto& operator[](int i, int j) const { return grid[i][j]; }
    constexpr int rows() const { return size(grid); }
    constexpr int cols() const { return size(grid[0]); }
    using value_type = remove_reference_t<decltype(grid[0][0])>;
    using MatrixRules::operator=;
};
}