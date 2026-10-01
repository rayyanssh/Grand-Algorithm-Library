#pragma once 
#include <vector>
using std::remove_reference_t, std::remove_cvref_t, std::vector;

// Simplified, minimal version of the Matrix library, for learning

// Matrix view. To reinterpret contiguous data as a 2D matrix.
// For example:
// Suppose vector<int> v(100);
// MatrixView M{v.data(), 10, 10}; gives a 10 by 10 2d view of that vector
template<typename T>
struct MatrixView {
    T* ptr; int r, c;  // ptr to start of data, rows, cols
    T& operator[](int i, int j) const { return ptr[i*c + j]; }     
    int rows() const { return r; }
    int cols() const { return c; } 
}; 

template<typename T>
struct Matrix {
    vector<T> storage;
    int r, c; // rows, cols
    Matrix() : r(0), c(0) {} 
    Matrix(int n, int m, T val = {})  : r(n), c(m),  storage(n * m, val) {}
    void resize(int n, int m, T val = {}) { r = n; c = m; storage.resize(n * m, val); } 
    T&       operator[](int i, int j)       { return storage[i*c + j]; }
    const T& operator[](int i, int j) const { return storage[i*c + j]; } 
    int rows() const { return r; } 
    int cols() const { return c; } 
};
// matrixlike concept
template<typename T>
concept MatType = requires(T t) { t[0,0]; t.rows(); t.cols(); };

// operations

auto& operator+=(const MatType auto& A, const MatType auto& B) { 
    for (int i = 0; i < A.rows(); i++) for (int j = 0; j < A.cols(); j++) A[i, j] += B[i, j];
    return A;
}
auto& operator-=(const MatType auto& A, const MatType auto& B) { 
    for (int i = 0; i < A.rows(); i++) for (int j = 0; j < A.cols(); j++) A[i, j] -= B[i, j];
    return A;
}
template<typename T>
auto operator+(Matrix<T> A, const MatType auto& B) { A += B; return A; }
template<typename T>
auto operator-(Matrix<T> A, const MatType auto& B) { A -= B; return A; }
// Matrix multiplication. out = A * B. make sure out is zeroed out first (otherwise the matmul result is added to it.)
void matmul(MatType auto&& A, MatType auto&& B, MatType auto& out) { 
    int n = A.rows(), m = A.cols(), p = B.cols();    
    for (int i = 0; i < n; i++)
        for (int k = 0; k < m; k++) 
            for (int j = 0; j < p; j++) out[i, j] += A[i, k] * B[k, j];
} 
template<typename T>
auto operator*(const Matrix<T>& A, const MatType auto& B) {
    Matrix<T> C(A.rows(), B.cols(), 0);
    matmul(A, B, C);
    return C;
}
template<typename T>
auto& operator*=(const Matrix<T>& A, const MatType auto& B) {
    Matrix<T> C(A.rows(), B.cols(), 0);
    matmul(A, B, C);
    A = std::move(C);
    return A;
}
// Mod Matrix multiplication. out = A * B. make sure out is zeroed out first (otherwise the matmul result is added to it.)
void modmatmul(MatType auto&& A, MatType auto&& B, MatType auto& out, auto mod) { 
    int n = A.rows(), m = A.cols(), p = B.cols();    
    for (int i = 0; i < n; i++)
        for (int k = 0; k < m; k++) 
            for (int j = 0; j < p; j++) out[i, j] = (out[i, j] + A[i, k] * B[k, j]) % mod;
}
// Sparse Matrix multiplication. out = A * B. make sure out is zeroed out first (otherwise the matmul result is added to it.)
void sparsematmul(MatType auto&& A, MatType auto&& B, MatType auto& out) { 
    int n = A.rows(), m = A.cols(), p = B.cols();    
    for (int i = 0; i < n; i++)
        for (int k = 0; k < m; k++) 
            if (A[i, k] != 0)
                for (int j = 0; j < p; j++) out[i, j] += A[i, k] * B[k, j];
}
// Sparse Mod Matrix multiplication. out = A * B. make sure out is zeroed out first (otherwise the matmul result is added to it.)
void sparsemodmatmul(MatType auto&& A, MatType auto&& B, MatType auto& out, auto mod) { 
    int n = A.rows(), m = A.cols(), p = B.cols();    
    for (int i = 0; i < n; i++)
        for (int k = 0; k < m; k++) 
            if (A[i, k] != 0)
                for (int j = 0; j < p; j++) out[i, j] = (out[i, j] + A[i, k] * B[k, j]) % mod;
}


// Left/CCW rotation
template<MatType M>
struct RotateMatrix {
    M& matrix; 
    auto& operator[](int i, int j) const { return matrix[j, matrix.cols() - i - 1]; }
    int rows() const { return matrix.cols(); }
    int cols() const { return matrix.rows(); }
};
// Left/CCW rotation
auto left_rotation(MatType auto& matrix)   
    { return RotateMatrix{matrix}; }  

template<MatType M>
struct TransposeMatrix {
    M& matrix;
    auto& operator[](int i, int j) const { return matrix[j, i]; }
    int rows() const { return matrix.cols(); }
    int cols() const { return matrix.rows(); }
};
auto transpose(MatType auto& matrix) { return TransposeMatrix{matrix}; }

template<MatType M, bool Horizontal = true >
struct ReflectMatrix {
    M& matrix;
    auto& operator[](int i, int j) const { 
        if constexpr (Horizontal) return matrix[i, matrix.cols() - j - 1]; 
        else                      return matrix[matrix.rows() - i - 1, j]; 
    }
    int rows() const { return matrix.rows(); }
    int cols() const { return matrix.cols(); }
};
auto x_reflection(MatType auto& matrix) { return ReflectMatrix{matrix}; } 
auto y_reflection(MatType auto& matrix) { 
    return ReflectMatrix<remove_reference_t<decltype(matrix)>, false>{matrix}; 
}

// Copy the n by m contents of matrix A to matrix out. 
void matcopy(MatType auto&& A, MatType auto& out, int n, int m) { assert(n <= A.rows() && m <= A.cols() && n <= out.rows() && m <= out.cols());
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < m; j++) 
            out[i, j] = A[i, j];
} 
void matcopy(MatType auto&& A, MatType auto& out) { matcopy(A, out, A.rows(), A.cols()); }
// Fill the n by m contents of matrix A with the value val.
void matfill(MatType auto&& A, auto val, int n, int m) { assert(n <= A.rows() && m <= A.cols());
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < m; j++) 
            A[i, j] = val;
}
void matfill(MatType auto&& A, auto val) { matfill(A, val, A.rows(), A.cols()); }

bool equal(MatType auto&& A, MatType auto&& B) {
    if (A.rows() != B.rows() || A.cols() != B.cols()) return false;
    for (int i = 0; i < A.rows(); i++) 
        for (int j = 0; j < A.cols(); j++) 
            if (A[i, j] != B[i, j]) return false;
    return true;
}
bool is_square(MatType auto&& A) { return A.rows() == A.cols(); }

// return a flattened 1D vector of the matrix
auto flatten(MatType auto&& A) {
    using T = std::remove_cvref_t<decltype(A)>::value_type;
    vector<T> flat;
    flat.reserve(A.rows() * A.cols());
    for (int i = 0; i < A.rows(); i++) 
        for (int j = 0; j < A.cols(); j++) 
            flat.push_back(A[i, j]);
    return flat;
} 


