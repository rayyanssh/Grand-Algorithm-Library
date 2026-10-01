#pragma once
#include "Structures/Matrix.cpp"
#include "Utils.cpp"
#include <cassert>
using std::plus, std::minus, std::multiplies, std::divides, std::modulus, std::integral;
//   - Element-wise operations 
//   - Matrix multiplication and its variants
//   - Inplace operations
//   - Binary operations (returning a new matrix)
//      - Operator overloads for matrix
//   - Scalar operations (Inplace and Binary Ops)
void elem_wise_to(MatType auto&& A, MatType auto&& B, MatType auto& out, auto f) { assert(A.rows() == B.rows() && A.cols() == B.cols() && A.rows() == out.rows() && A.cols() == out.cols());
    for (int i = 0; i < A.rows(); i++) 
        for (int j = 0; j < A.cols(); j++) 
            out[i, j] = f(A[i, j], B[i, j]);
}
void elem_wise(MatType auto& A, MatType auto&& B, auto f) { elem_wise_to(A, B, A, f); }
void matadd(MatType auto&& A, MatType auto&& B, MatType auto& out) { elem_wise_to(A, B, out, plus{}); }
void matsub(MatType auto&& A, MatType auto&& B, MatType auto& out) { elem_wise_to(A, B, out, minus{}); }
void elem_prod(MatType auto&& A, MatType auto&& B, MatType auto& out) { elem_wise_to(A, B, out, multiplies{}); }

// Matrix Multiplication into output buffer. Assumes output buffer is zeroed. 
// If not, then it adds the result of the multiplication to it. 
// Can pass custom addition/multiplication operations
void matmul(MatType auto&& A, MatType auto&& B, MatType auto& out, auto AddOp = plus{}, auto MulOp = multiplies{}) { assert(A.cols() == B.rows() && A.rows() == out.rows() && B.cols() == out.cols());
    int n = A.rows(), m = A.cols(), p = B.cols();  
    for (int i = 0; i < n; i++)
        for (int k = 0; k < m; k++) 
            for (int j = 0; j < p; j++) out[i, j] = AddOp(out[i, j], MulOp(A[i, k], B[k, j]));
} 
// Sparse matrix multiplication into output buffer. Assumes output buffer is zeroed.
// If not, then it adds the result of the multiplication to it. Can pass custom addition/multiplication operations
void sparse_matmul(MatType auto&& A, MatType auto&& B, MatType auto& out, auto AddOp = plus{}, auto MulOp = multiplies{}) { assert(A.cols() == B.rows() && A.rows() == out.rows() && B.cols() == out.cols());
    int n = A.rows(), m = A.cols(), p = B.cols(); 
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < m; k++) {
            if (!A[i, k]) continue;
            for (int j = 0; j < p; j++) {
                out[i, j] = AddOp(out[i, j], MulOp(A[i, k], B[k, j]));
            }
        } 
    }
}
// Modular matrix multiplication into output buffer. Assumes output buffer is zeroed. If not, then it adds the result of the multiplication to it.
// For negative integers the mod result may be negative, so make sure to normalize to [0, mod-1] if needed.
void mod_matmul(MatType auto&& A, MatType auto&& B, MatType auto& out, integral auto mod) { 
    matmul(A, B, out, [mod](auto a, auto b) { return (a + b) % mod; });
}
// Sparse modular matrix multiplication into output buffer. Assumes output buffer is zeroed. If not, then it adds the result of the multiplication to it. Can pass custom addition/multiplication operations
// For negative integers the mod result may be negative, so make sure to normalize to [0, mod-1] if needed.
void sparse_mod_matmul(MatType auto&& A, MatType auto&& B, MatType auto& out, integral auto mod) {
    sparse_matmul(A, B, out, [mod](auto a, auto b) { return (a + b) % mod; });
}

// Inplace operations
void operator+=(MatType auto& A, MatType auto&& B) { elem_wise(A, B, plus{}); }
void operator-=(MatType auto& A, MatType auto&& B) { elem_wise(A, B, minus{}); }
void inplace_elem_prod(MatType auto& A, MatType auto&& B) { elem_wise(A, B, multiplies{}); }
void operator*=(MatType auto& A, MatType auto&& B) { assert(A.cols() == A.rows());
    using T = remove_cvref_t<decltype(A)>::value_type;
    Matrix<T> C(A.rows(), A.cols());
    matmul(A, B, C);
    matcopy(C, A);
} 
// Binary operations. The left operand must be a Owning Matrix to deduce the returning Matrix type
auto operator+(OwningMatType auto&& A, MatType auto&& B) {
    auto C = A;
    C += B;
    return C;
}
auto operator-(OwningMatType auto&& A, MatType auto&& B) {
    auto C = A;
    C -= B;
    return C;
}
auto elem_prod(OwningMatType auto&& A, MatType auto&& B) {
    auto C = A;
    inplace_elem_prod(C, B);
    return C;
}
// Dynamic matrix multiplication if one of the matrices is dynamic
template<typename T, int N, int M>
auto operator*(const Matrix<T,N,M>& A, const MatType auto& B) {  // made B as const auto& to not overload the 2nd * for static matrix. otherwise same as auto&&
    Matrix<T> C(A.rows(), B.cols());
    matmul(A, B, C);
    return C;
}
// Static matrix multipliation specialization for when both operands have a fixed dimension.
template<typename T, int N, int M, int P> requires (N != dynamic && P != dynamic)
Matrix<T, N, P> operator*(const Matrix<T, N, M>& A, const Matrix<T, M, P>& B) {
    Matrix<T, N, P> C = {}; // zeroed  
    matmul(A, B, C); 
    return C;
} 

template<typename T, int N>
void operator*=(Matrix<T,N,N>& A, MatType auto&& B) { assert(A.cols() == A.rows());
    Matrix<T,N,N> C(A.rows(), A.rows()); // if A is static, this will be static Matrix<T, N, N>, zeroed.
    matmul(A, B, C);
    A = std::move(C); // saves a copy when A is dynamic 
}

// Scalar operations 
void elem_wise(MatType auto& A, Number auto b, auto f) { int n = A.rows(), m = A.cols();
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) A[i, j] = f(A[i, j], b);
}
void elem_wise(Number auto b, MatType auto& A, auto f) { int n = A.rows(), m = A.cols();
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) A[i, j] = f(b, A[i, j]);
}
// Inplace operations with scalar
void operator+=(MatType auto& A, Number auto b) { elem_wise(A, b, plus{}); }
void operator-=(MatType auto& A, Number auto b) { elem_wise(A, b, minus{}); }
void operator*=(MatType auto& A, Number auto b) { elem_wise(A, b, multiplies{}); }
void operator/=(MatType auto& A, Number auto b) { elem_wise(A, b, divides{}); }
void operator%=(MatType auto& A, integral auto b) { elem_wise(A, b, modulus{}); }
// Binary operations with scalar (returning a new matrix)
auto operator+(OwningMatType auto&& A, Number auto b) { auto C = A; C += b; return C; }
auto operator+(Number auto b, OwningMatType auto&& A) { auto C = A; C += b; return C; }
auto operator-(OwningMatType auto&& A, Number auto b) { auto C = A; C -= b; return C; }
auto operator-(Number auto b, OwningMatType auto&& A) { auto C = A; elem_wise(b, C, minus{}); return C; }
auto operator-(OwningMatType auto&& A)                { auto C = A; elem_wise(0, C, minus{}); return C; }
auto operator*(OwningMatType auto&& A, Number auto b) { auto C = A; C *= b; return C; }
auto operator*(Number auto b, OwningMatType auto&& A) { auto C = A; C *= b; return C; }
auto operator/(OwningMatType auto&& A, Number auto b) { auto C = A; C /= b; return C; }
auto operator%(OwningMatType auto&& A, integral auto b) { auto C = A; C %= b; return C; }
/*
int main() {
    Matrix<int, 3,3> Identity = {{1,0,0},{0,1,0},{0,0,1}};
    Matrix<int, 3,3> A = {{1,2,3},{4,5,6},{7,8,9}};
    matprint(A, "A");
    matprint(Identity, "Identity");
    matprint(A * Identity, "A * Identity");
    Matrix<int, 4, dynamic> B = {{1,2,3},{5,6,7},{9,10,11},{13,14,15}};
    Matrix<int, dynamic, 2> C = {{1,2},{3,4},{5,6}};
    Matrix<int> D = {{1,2,3},{4,5,6},{7,8,9}}; // shorthand for Matrix<int, dynamic, dynamic>
    matprint(B, "B");
    matprint(C, "C");
    auto res = B * C; // deduces Matrix<int, 4, 2>
    matprint(res, "B * C");
    auto res2 = B * D; // deduces Matrix<int, dynamic, dynamic> 
    matprint(res2, "B * D");
    auto res3 = D * C; // deduces Matrix<int, dynamic, dynamic>
    matprint(res3, "D * C");
}
*/
