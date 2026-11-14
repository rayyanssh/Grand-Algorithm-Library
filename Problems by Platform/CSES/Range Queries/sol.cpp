#line 2 "/home/rayyan/CodeHub/Grand Library/Grand/Structures/Matrix.hpp"
#include <algorithm>
#include <cassert>
#include <vector>
#include <iostream>
#line 2 "/home/rayyan/CodeHub/Grand Library/Grand/Utils.hpp"
// Just general utilities that I use often
#include <concepts> 
#include <bit>
#include <initializer_list>
#include <iterator>
#include <type_traits> 
#line 9 "/home/rayyan/CodeHub/Grand Library/Grand/Utils.hpp"
#include <tuple> 
#include <ranges>
#include <span>
#include <utility>

namespace Grand {
namespace rgs = std::ranges;
namespace views = std::views;
using std::integral, std::floating_point,  std::swap, std::max, std::min, std::move,
      std::array, std::span, std::pair, std::tuple, std::make_tuple, std::apply, std::tie, std::to_array,
      std::make_index_sequence, std::index_sequence,
      std::countr_zero, std::countl_zero, std::countr_one, std::countl_one, std::popcount, std::has_single_bit, std::bit_ceil, std::bit_floor, std::bit_width,
      std::is_same_v, std::is_const_v, std::is_reference_v, std::conditional_t, std::convertible_to;
template<typename T> using rm_ref   = std::remove_reference_t<T>;
template<typename T> using rm_const = std::remove_const_t<T>;
template<typename T> using rm_ptr   = std::remove_pointer_t<T>;
template<typename T> using rm_cvref = std::remove_cvref_t<T>;
#define RANGE_CONCEPT(name, ...) \
    template<class T> concept name = __VA_ARGS__;
RANGE_CONCEPT(InputRange,   rgs::input_range<T>)
RANGE_CONCEPT(InputIt,      std::input_iterator<T>)
RANGE_CONCEPT(ForwardRange, rgs::forward_range<T>)
RANGE_CONCEPT(ForwardIt,    std::forward_iterator<T>)
RANGE_CONCEPT(BidirRange,   rgs::bidirectional_range<T>)
RANGE_CONCEPT(BidirIt,      std::bidirectional_iterator<T>)
RANGE_CONCEPT(RA_Range,     rgs::random_access_range<T>)
RANGE_CONCEPT(RA_It,        std::random_access_iterator<T>)
RANGE_CONCEPT(ContRange,    rgs::contiguous_range<T>)
RANGE_CONCEPT(ContIt,       std::contiguous_iterator<T>)
RANGE_CONCEPT(SizedRange,   rgs::sized_range<T>)
#undef RANGE_CONCEPT
 

template<typename T> concept Number = integral<T> || floating_point<T>;
struct none{};
constexpr int dynamic = -1; 

template<typename T>
using init_list = std::initializer_list<T>;


} // namespace Grand
 
#line 7 "/home/rayyan/CodeHub/Grand Library/Grand/Structures/Matrix.hpp"
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

struct MatrixRules {
    constexpr void copy(this auto& self, const MatrixType auto& other, int n, int m) { assert(n <= self.rows() && m <= self.cols()); assert(n <= other.rows() && m <= other.cols());
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                self[i, j] = other[i, j]; 
    } 
    constexpr void fill(this auto& self, int n, int m, auto val) {                     assert(n <= self.rows() && m <= self.cols());
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                self[i, j] = val;
    }
    // Assignment operator=(MatrixType other) copies the elements of the other matrix to this.
    constexpr auto& operator=(this auto& self, const MatrixType auto& other) { assert(other.rows() == self.rows() && other.cols() == self.cols());
        self.copy(other, other.rows(), other.cols()); 
        return self;
    }
    // Assignment operator=(val) fills the matrix with a value.
    constexpr auto& operator=(this auto& self, auto val) { 
        self.fill(self.rows(), self.cols(), val); 
        return self;
    }
    constexpr long long size(this const auto& self) { return 1LL * self.rows() * self.cols(); }  
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
    constexpr Matrix(init_list<T> init) : storage{} { rgs::copy(init, storage.begin()); } 
    constexpr Matrix(const MatrixType auto& other) { *this = other; } // Deep copy

    constexpr auto& operator[](this auto&& self, int k) { return self.storage[k]; }
    constexpr auto& operator[](this auto&& self, int i, int j) { return self.storage[i * M + j]; }
    constexpr auto row(this auto&& self, int i) { return span(&self[i, 0], self.cols()); }
    // Returns a view of the matrix. `MatrixView`.
    constexpr auto view(this auto&& self); 

    consteval int rows() const { return N; }
    consteval int cols() const { return M; }
    constexpr auto begin(this auto&& self) { return self.storage.begin(); }
    constexpr auto   end(this auto&& self) { return self.storage.end(); }
    constexpr auto* data(this auto&& self) { return self.storage.data(); } 
    using MatrixRules::operator=;
    using value_type = T;
};

// Dynamic size matrix. 
// Example: Matrix<int> M(3,4) creates a 3x4 matrix of ints.
// Example: Matrix<int, dynamic, 5> M(3,5) creates a 3x5 matrix of ints. 
// .storage is a vector<T> of size n*m. T = bool won't compile.
// A 1 by m or n by 1 Matrix is a Vector, defined by an alias,
template<typename T, int N, int M> requires (N == dynamic || M == dynamic)
struct Matrix<T, N, M> : MatrixRules {
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
        copy(other, n, m); 
        return *this;
    }

    auto& operator[](this auto&& self, int k) { return self.storage[k]; }
    auto& operator[](this auto&& self, int i, int j) { return self.storage[i * self.cols() + j]; }
    auto row(this auto&& self, int i) { return span(&self[i, 0], self.cols()); }
    // Returns a view of the matrix. `MatrixView`.
    constexpr auto view(this auto&& self);

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
    auto begin(this auto&& self) { return self.storage.begin(); }
    auto   end(this auto&& self) { return self.storage.end(); }
    auto* data(this auto&& self) { return self.storage.data(); }
    using MatrixRules::operator=;
    using value_type = T;

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
        copy(other, rows(), cols());
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
struct SubMatrix : MatrixRules {
    HoldThisMatrix<M> matrix;  int start_i, start_j, r, c; 
    auto& operator[](int i, int j) const { return matrix[start_i + i, start_j + j]; }
    constexpr int rows() const { return R == dynamic ? r : R; }
    constexpr int cols() const { return C == dynamic ? c : C; } 
    constexpr SubMatrix& operator=(const SubMatrix& other) { 
        copy(other, rows(), cols());
        return *this;
    }
    using MatrixRules::operator=;
    using value_type = M::value_type;
};

// Returns a view of the submatrix defined by corner `(i,j)` and the dimensions `(rows, cols)`. 
// Omit `rows` to take every row from `i` to end. Omit `cols` to take every column from `j` to end.
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

void print(const MatrixType auto& A) {
    for (int i = 0; i < A.rows(); i++) {
        for (int j = 0; j < A.cols(); j++) 
            std::cout << A[i,j] << " ";
        std::cout << "\n";
    }
}

}
#line 3 "/home/rayyan/CodeHub/Grand Library/Grand/Structures/SparseTable.hpp"
namespace Grand {
// Sparse Table answers static range queries in O(1).
// Requires an associative, idempotent operation.
template<typename T, typename Op>
struct SparseTable {
    Matrix<T> table;
    Op op;
    unsigned int n;
    SparseTable(Op op) : op(op) {}
    SparseTable(SizedRange auto&& nums, Op op) : op(op) { build(nums); }
    void build(SizedRange auto&& nums) {
        n = rgs::size(nums);
        int log_n = bit_width(n)-1;
        table.resize(log_n + 1, n);
        rgs::copy(nums, table.row(0).begin());
        for (int i = 1; i <= log_n; i++)
            for (int j = 0; j + (1<<i) <= n; j++)
                table[i, j] = op(table[i-1, j], table[i-1, j + (1<<(i-1))]); 
    }
    T query(int a, int b) const {
        int k = bit_width(b - a + 1u) - 1;
        return op(table[k, a], table[k, b - (1<<k) + 1]);
    }
}; 
template<SizedRange R, typename Op>
SparseTable(R&&, Op) -> SparseTable<rgs::range_value_t<R>, Op>;

}
#line 2 "StaticRangeMinimumQueries.cpp"
using namespace Grand; 
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    cin >> n >> q;
    vector<int> nums(n+1);
    for (int i = 1; i <= n; i++) cin >> nums[i]; 
    SparseTable S(nums, [&](int a, int b) { return min(a,b); });
    while (q--) {
        int l, r; cin >> l >> r;
        cout << S.query(l, r) << '\n';
    }
}
