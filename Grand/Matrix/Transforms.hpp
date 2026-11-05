#include "../Structures/Matrix.hpp"
namespace Grand {
// Lightweight Transforms. Rotation, Transpose, Reflection, Cyclic Shift.
// Functions only return a transformed view of the underlying matrix. 
// Inplace transformations are also defined. 
// e.g. `rotate_left(m)` is inplace rotation of a square matrix, 
// `rotated_left(m)` is a rotated view of the matrix. 

// Makes a left (CCW) rotated view of the matrix.
template<MatrixType M>
struct RotatedMatrix : MatrixRules {
    HoldThisMatrix<M> matrix;
    auto& operator[](int i, int j) const { return matrix[j, matrix.cols() - i - 1]; }
    constexpr int rows() const { return matrix.cols(); }
    constexpr int cols() const { return matrix.rows(); } 
    using MatrixRules::operator=;
    using value_type = M::value_type; 
};
auto rotated_left(MatrixType auto&& matrix) { 
    using M = remove_reference_t<decltype(matrix)>;
    return RotatedMatrix<M>{{}, matrix}; 
} 
auto rotated_180(MatrixType auto&& matrix) { return rotated_left(rotated_left(matrix)); }  
auto rotated_right(MatrixType auto&& matrix) { return rotated_left(rotated_left(rotated_left(matrix))); }

template<MatrixType M>
struct TransposedMatrix : MatrixRules {
    HoldThisMatrix<M> matrix;
    auto& operator[](int i, int j) const { return matrix[j, i]; }
    constexpr int rows() const { return matrix.cols(); }
    constexpr int cols() const { return matrix.rows(); } 
    using MatrixRules::operator=;
    using value_type = M::value_type; 
};
auto transposed(MatrixType auto&& matrix) { 
    using M = remove_reference_t<decltype(matrix)>;
    return TransposedMatrix<M>{{}, matrix}; 
} 

template<MatrixType M, bool Horizontal = true >
struct ReflectedMatrix : MatrixRules {
    HoldThisMatrix<M> matrix;
    auto& operator[](int i, int j) const { 
        if constexpr (Horizontal) return matrix[i, matrix.cols() - j - 1]; 
        else                      return matrix[matrix.rows() - i - 1, j]; 
    }
    constexpr int rows() const { return matrix.rows(); }
    constexpr int cols() const { return matrix.cols(); }
    using MatrixRules::operator=;
    using value_type = M::value_type;
};
auto reflected_horizontal(MatrixType auto&& matrix) { 
    using M = remove_reference_t<decltype(matrix)>;
    return ReflectedMatrix<M>{{}, matrix}; 
}
auto reflected_vertical(MatrixType auto&& matrix) { 
    using M = remove_reference_t<decltype(matrix)>;
    return ReflectedMatrix<M, false>{{}, matrix}; 
}


template<MatrixType M>
struct CyclicShiftedMatrix {};


void rotate_left(MatrixType auto& A) {
    assert(is_square(A));
    int n = A.rows();
    for (int i = 0; i < n/2; i++) {
        for (int j = i; j < n - 1 - i; j++) {
            auto temp = move(A[i, j]);
            A[i, j]         = move(A[j, n-i-1]);
            A[j, n-i-1]     = move(A[n-i-1, n-j-1]);
            A[n-i-1, n-j-1] = move(A[n-j-1, i]);
            A[n-j-1, i]     = move(temp);
        }
    }
}
void rotate_right(MatrixType auto& A) {
    assert(is_square(A));
    int n = A.rows();
    for (int i = 0; i < n/2; i++) {
        for (int j = i; j < n - 1 - i; j++) {
            auto temp = move(A[i, j]);
            A[i, j]         = move(A[n-j-1, i]);
            A[n-j-1, i]     = move(A[n-i-1, n-j-1]);
            A[n-i-1, n-j-1] = move(A[j, n-i-1]);
            A[j, n-i-1]     = move(temp);
        }
    }   
}
void rotate_180(MatrixType auto& A) { 
    int n = A.rows(), m = A.cols();
    for (int i = 0; i < n / 2; i++)
        for (int j = 0; j < m; j++)
            swap(A[i, j], A[n-i-1, m-j-1]);
    if (n % 2 == 1) 
        for (int j = 0; j < m / 2; j++) 
            swap(A[n/2, j], A[n/2, m-j-1]);
}
void transpose(MatrixType auto& A) {
    assert(is_square(A));
    int n = A.rows();
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) 
            swap(A[i, j], A[j, i]); 
}
void reflect_horizontal(MatrixType auto& A) {
    for (int i = 0; i < A.rows(); i++)
        for (int j = 0; j < A.cols() / 2; j++) 
            swap(A[i, j], A[i, A.cols() - j - 1]);
}
void reflect_vertical(MatrixType auto& A) {
    for (int i = 0; i < A.rows() / 2; i++)
        for (int j = 0; j < A.cols(); j++) 
            swap(A[i, j], A[A.rows() - i - 1, j]);
}

}
