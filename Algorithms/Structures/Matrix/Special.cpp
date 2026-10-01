#include "Matrix.cpp"
static constexpr bool lower = true;
static constexpr bool upper = false;
// Triangular matrix. Only stores the lower or upper half of the matrix (default lower half), 
// flattened row-by-row into a 1D vector, along with a value denoting the other_half (default 0). 
template<typename T, bool Half = lower>
struct TriangularMatrix {
    std::vector<T> storage;
    int n; T other_half{};
    T& operator[](int i, int j) { 
        if constexpr (Half == lower) return (i < j) ? other_half : storage[i * (i + 1)/2 + j]; 
        else                         return (i > j) ? other_half : storage[i * (2*n - i + 1)/2 + j - i];
    }
    const T& operator[](int i, int j) const { 
        if constexpr (Half == lower) return (i < j) ? other_half : storage[i * (i + 1)/2 + j]; 
        else                         return (i > j) ? other_half : storage[i * (2*n - i + 1)/2 + j - i];
    }
    void copy(const MatrixType auto& B) {
        n = B.rows(); 
        storage.clear(); storage.reserve(n * (n + 1) / 2);
        if constexpr (Half == lower) 
             for (int i = 0; i < n; i++) for (int j = 0; j <= i; j++) storage.push_back(B[i, j]);
        else for (int i = 0; i < n; i++) for (int j = i; j < n ; j++) storage.push_back(B[i, j]);
    }
    TriangularMatrix() : n(0) {}
    TriangularMatrix(int n, T val = {}) : n(n), storage(n * (n + 1) / 2, val) {}
    TriangularMatrix(const MatrixType auto& B) { assert(is_square(B)); copy(B); }
    void operator=(const MatrixType auto& B) { assert(is_square(B)); copy(B); }
    void resize(int s, T val = {}) { n = s; storage.resize(n * (n + 1) / 2, val); }
    int rows() const { return n; }
    int cols() const { return n; }
    using value_type = T;
};

// Symmetric matrix. Space-optimized; stores n(n+1)/2 elements in a 1D vector, representing the lower half.
template<typename T>
struct SymmetricMatrix {
    std::vector<T> storage;
    int n; 
    const T& operator[](int i, int j) const { 
        if (i < j) swap(i, j);
        return storage[i * (i + 1) / 2 + j];
    }
    void copy(const MatrixType auto& B) {
        n = B.rows(); 
        storage.clear(); storage.reserve(n * (n + 1) / 2);
        for (int i = 0; i < n; i++) for (int j = 0; j <= i; j++) storage.push_back(B[i, j]);
    }
    SymmetricMatrix() : n(0) {}
    SymmetricMatrix(int n, T val = {}) : n(n), storage(n * (n + 1) / 2, val) {}
    SymmetricMatrix(const MatrixType auto& B) { assert(is_square(B)); copy(B); }
    void operator=(const MatrixType auto& B) { assert(is_square(B)); copy(B); }
    void resize(int s, T val = {}) { n = s; storage.resize(n * (n + 1) / 2, val); }
    int rows() const { return n; }
    int cols() const { return n; }
    using value_type = T;
};

template<typename T>
struct DiagonalMatrix {
    std::vector<T> storage;
    int n; T zero{};
    T& operator[](int i, int j) { return (i == j) ? storage[i] : zero; }
    const T& operator[](int i, int j) const { return (i == j) ? storage[i] : zero; }
    T& operator[](int i) { return storage[i]; }
    const T& operator[](int i) const { return storage[i]; }
    void copy(const MatrixType auto& B) {
        n = B.rows(); 
        storage.clear(); storage.reserve(n);
        for (int i = 0; i < n; i++) storage.push_back(B[i, i]);
    }
    DiagonalMatrix() : n(0) {}
    DiagonalMatrix(int n, T val = {}) : n(n), storage(n, val) {}
    DiagonalMatrix(const MatrixType auto& B)  { assert(is_square(B)); copy(B); }
    void operator=(const MatrixType auto& B) { assert(is_square(B)); copy(B); }    
    void resize(int s, T val = {}) { n = s; storage.resize(n, val); }
    int rows() const { return n; }
    int cols() const { return n; }
    using value_type = T;
};

template<typename T>
struct SameValuesDiagonalMatrix {
    int n; T val; T zero{};
    T& operator[](int i, int j) { return (i == j) ? val : zero; }
    const T& operator[](int i, int j) const { return (i == j) ? val : zero; }
    void operator=(const MatrixType auto& B) { assert(is_square(B)); val = B[0, 0]; }
    SameValuesDiagonalMatrix() : n(0), val{} {}
    SameValuesDiagonalMatrix(int n, T val = {}) : n(n), val(val) {}
    int rows() const { return n; }
    int cols() const { return n; }
    using value_type = T;
};
auto identity_matrix(int n, auto identity = 1) { return SameValuesDiagonalMatrix(n, identity); }


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