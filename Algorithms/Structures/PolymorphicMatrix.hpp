#include "Matrix.hpp"
// Polymorphic wrapper that can hold a pointer to any matrix type
template<typename T>
struct AnyMatrix : MatrixRules<T> {
    void* matrix;
    int (*rows_func)(void*);
    int (*cols_func)(void*);
    T&  (*access)(void*, int, int);
    template<MatrixType M>
    AnyMatrix(M& A) : matrix(&A) {
        rows_func = [](void* m) { return static_cast<M*>(m)->rows(); };
        cols_func = [](void* m) { return static_cast<M*>(m)->cols(); };
        access    = [](void* m, int i, int j) -> T& { return (*static_cast<M*>(m))[i, j]; };
    }
    int rows() const { return rows_func(matrix); }
    int cols() const { return cols_func(matrix); }
    T& operator[](int i, int j) const { return access(matrix, i, j); }
};
template<MatrixType M> AnyMatrix(M& A) -> AnyMatrix<typename M::value_type>;