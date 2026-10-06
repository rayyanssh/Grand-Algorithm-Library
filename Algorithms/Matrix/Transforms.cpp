#include "Matrix.cpp"

// Makes a left (CCW) rotated view of the matrix.
template<MatrixType M>
struct RotatedMatrix : MatrixRules<typename M::value_type> {
    HoldThisMatrix<M> matrix; 
    auto& operator[](int i, int j) const { return matrix[j, matrix.cols() - i - 1]; }
    constexpr int rows() const { return matrix.cols(); }
    constexpr int cols() const { return matrix.rows(); } 
};
auto left_rotation(MatrixType auto&& matrix) { 
    using M = remove_reference_t<decltype(matrix)>;
    return RotatedMatrix<M>{matrix}; 
} 
auto rotation_180(MatrixType auto&& matrix) { return left_rotation(left_rotation(matrix)); }  
auto right_rotation(MatrixType auto&& matrix) { return left_rotation(left_rotation(left_rotation(matrix))); }

template<MatrixType M>
struct TransposedMatrix : MatrixRules<typename M::value_type> {
    HoldThisMatrix<M> matrix;
    auto& operator[](int i, int j) const { return matrix[j, i]; }
    constexpr int rows() const { return matrix.cols(); }
    constexpr int cols() const { return matrix.rows(); } 
};
auto transpose(MatrixType auto&& matrix) { 
    using M = remove_reference_t<decltype(matrix)>;
    return TransposedMatrix<M>{matrix}; 
} 

template<MatrixType M, bool Horizontal = true >
struct ReflectedMatrix {
    HoldThisMatrix<M> matrix;
    auto& operator[](int i, int j) const { 
        if constexpr (Horizontal) return matrix[i, matrix.cols() - j - 1]; 
        else                      return matrix[matrix.rows() - i - 1, j]; 
    }
    constexpr int rows() const { return matrix.rows(); }
    constexpr int cols() const { return matrix.cols(); }
    using value_type = M::value_type;
};
auto horizontal_reflection(MatrixType auto&& matrix) { 
    using M = remove_reference_t<decltype(matrix)>;
    return ReflectedMatrix<M>{matrix}; 
}
auto vertical_reflection(MatrixType auto&& matrix) { 
    using M = remove_reference_t<decltype(matrix)>;
    return ReflectedMatrix<M, false>{matrix}; 
}


template<MatrixType M>
struct CyclicShiftedMatrix {};