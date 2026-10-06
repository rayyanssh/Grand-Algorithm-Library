#include "Matrix.hpp" // To bring the concept and is_equal() function
namespace Grand {
static constexpr bool Lower = true;
static constexpr bool Upper = false;
// Triangular matrix. Stores the Lower or Upper half of the matrix (default Lower half), 
// in a row major layout, in a vector of size n(n+1)/2,
// along with a member `other_half` denoting the other half (0 by default).
template<typename T, bool Half = Lower>
struct TriangularMatrix {
    std::vector<T> storage;
    int n; T other_half{};
    // The entire other half can be modified with `A[i,j] = val if (i,j) ∈ other half.`
    T& operator[](int i, int j) { 
        if constexpr (Half == Lower) return (i < j) ? other_half : storage[i * (i + 1)/2 + j]; 
        else                         return (i > j) ? other_half : storage[i * (2*n - i + 1)/2 + j - i];
    }
    const T& operator[](int i, int j) const { 
        if constexpr (Half == Lower) return (i < j) ? other_half : storage[i * (i + 1)/2 + j]; 
        else                         return (i > j) ? other_half : storage[i * (2*n - i + 1)/2 + j - i];
    }
    T& operator[](int k) { return storage[k]; } const T& operator[](int k) const { return storage[k]; }

    TriangularMatrix() : n(0) {}
    TriangularMatrix(int n, T val = {}, T other = {}) : storage(n * (n + 1) / 2, val), n(n), other_half(other) {}
    TriangularMatrix(const MatrixType auto& B) { *this = B; }
    void operator=(const MatrixType auto& B) { assert(is_square(B)); 
        resize(B.rows());
        copy(B, B.rows()); 
    }

    // Copy the required half of the s by s portion of matrix B to this matrix.
    void copy(const MatrixType auto& B, int s) { assert(s <= B.rows() && s <= B.cols()); assert(s <= n);
        int k = 0;
        if constexpr (Half == Lower) 
             for (int i = 0; i < s; i++) for (int j = 0; j <= i; j++) storage[k++] = B[i, j];
        else for (int i = 0; i < s; i++) for (int j = i; j < s ; j++) storage[k++] = B[i, j];
    } 
    void fill(int s, T val) { assert(s <= n);
        std::fill(storage.begin(), storage.begin() + s * (s + 1) / 2, val);
    }
    void fill(T val) { fill(n, val); } 

    void resize(int s, T val = {}) { n = s; storage.resize(n * (n + 1) / 2, val); }
    int rows() const { return n; }
    int cols() const { return n; }
    long long size() const { return 1LL * n * (n + 1) / 2; }
    bool operator==(const MatrixType auto& B) const { return is_equal(*this, B); }
    using value_type = T;
};

// Symmetric matrix. O(n*(n+1)/2) space.
// Space-optimized to store the lower half only, representing both halves.
// The symmetric invariant, `A[i,j] == A[j,i]`, is always maintained.
template<typename T>
struct SymmetricMatrix {
    std::vector<T> storage;
    int n; 
    // The entry A[i,j] and A[j,i] are same.
    T& operator[](int i, int j) { 
        if (i < j) swap(i, j);
        return storage[i * (i + 1) / 2 + j];
    }
    // The entry A[i,j] and A[j,i] are same.
    const T& operator[](int i, int j) const { 
        if (i < j) swap(i, j);
        return storage[i * (i + 1) / 2 + j];
    }
    T& operator[](int k) { return storage[k]; } const T& operator[](int k) const { return storage[k]; }
    SymmetricMatrix() : n(0) {}
    SymmetricMatrix(int n, T val = {}) : n(n), storage(n * (n + 1) / 2, val) {}
    SymmetricMatrix(const MatrixType auto& B) { *this = B; }
    void operator=(const MatrixType auto& B) { assert(is_square(B)); resize(B.rows()); copy(B); }
    void operator=(T val) { fill(n, val); }

    // Copy the lower half of the s by s portion of matrix B to this matrix.
    void copy(const MatrixType auto& B, int s) { assert(s <= B.rows() && s <= B.cols() && s <= n);
        int k = 0;
        for (int i = 0; i < s; i++) 
            for (int j = 0; j <= i; j++) {
                storage[k++] = B[i, j];}
    }
    void fill(int s, T val) { assert(s <= n);
        std::fill(storage.begin(), storage.begin() + s * (s + 1) / 2, val);
    }
    
    void resize(int s, T val = {}) { n = s; storage.resize(n * (n + 1) / 2, val); }
    int rows() const { return n; }
    int cols() const { return n; }
    long long size() const { return 1LL * n * (n + 1) / 2; }
    bool operator==(const MatrixType auto& B) const { return is_equal(*this, B); }
    using value_type = T;
};

template<typename T>
struct DiagonalMatrix {
    std::vector<T> storage;
    int n; T zero{};
    // A write to a non diagonal entry changes all the non diagonal entries, by modifying the `zero` member. 
    T& operator[](int i, int j) { return (i == j) ? storage[i] : zero; }
    const T& operator[](int i, int j) const { return (i == j) ? storage[i] : zero; }
    T& operator[](int i) { return storage[i]; }
    const T& operator[](int i) const { return storage[i]; }

    DiagonalMatrix() : n(0) {}
    DiagonalMatrix(int n, T val = {}, T zero = {}) : n(n), storage(n, val), zero(zero) {}
    DiagonalMatrix(const MatrixType auto& B) { *this = B; }
    void operator=(const MatrixType auto& B) { resize(B.rows()); assert(is_square(B)); copy(B); }    

    void copy(const MatrixType auto& B, int s) { assert(s <= B.rows() && s <= B.cols() && s <= n);
        for (int i = 0; i < s; i++) storage[i] = B[i, i];
    }
    void fill(int s, T val) { assert(s <= n);
        std::fill(storage.begin(), storage.begin() + s, val);
    }
    void resize(int s, T val = {}) { n = s; storage.resize(n, val); }
    int rows() const { return n; }
    int cols() const { return n; }
    int size() const { return n; }
    bool operator==(const MatrixType auto& B) const { return is_equal(*this, B); }
    using value_type = T;
};

// A scalar matrix is a square diagonal matrix with the same entries. It is also a multiple of the identity Matrix.
// The value of the diagonal entries is stored in "value", and the other entries are represented by member `zero` (0) and `zero` is modifiable.
// `const int n; T value; T zero{};`
// The size of the matrix is immutable.
template<typename T>
struct ScalarMatrix {
    const int n; 
    T value; T zero{}; 
    constexpr const T& operator[](int k) const { return value; }
    constexpr const T& operator[](int i, int j) const { return (i == j) ? value : zero; }

    constexpr ScalarMatrix() : n(0), value{} {}
    constexpr ScalarMatrix(int n, T val = {}, T zero = {}) : n(n), value(val), zero(zero) {}

    constexpr int rows() const { return n; }
    constexpr int cols() const { return n; } 
    bool operator==(const MatrixType auto& B) const { return is_equal(*this, B); }
    using value_type = T;
};
// Returns an identity matrix view of size n x n
template<typename T = int>
constexpr auto identity_matrix(int n, T identity = 1) { 
    return ScalarMatrix<const T>(n, identity); 
}
}