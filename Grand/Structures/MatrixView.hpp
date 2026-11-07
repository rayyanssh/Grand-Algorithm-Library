#include "Matrix.hpp" 
namespace Grand {
// MatrixView. A view of contiguous data as a 2D matrix. It is similar to std::mdspan of dimension 2.
// For example, `MatrixView<int> MV(vec.data(), 3, 4)` creates a 3x4 view of vec.
// Assignment `operator=(const MatrixType auto& B)` peforms deep copy of B's elements.
// To copy only the attributes of another MatrixView, use `.rebind()`. `A.rebind(B)`.
template<typename T, int N = dynamic, int M = dynamic>
struct MatrixView : MatrixRules {
    T* ptr; int n, m;
    constexpr MatrixView() : n(max(0, N)), m(max(0, M)), ptr(nullptr) {}
    constexpr explicit MatrixView(T* p, int r, int c) : n(r), m(c), ptr(p) { assert((N == dynamic || r == N) && (M == dynamic || c == M)); } 
    constexpr explicit MatrixView(T* p) requires (N != dynamic && M != dynamic) : n(N), m(M), ptr(p) {} 

    constexpr T& operator[](int k) const { return ptr[k]; }
    constexpr T& operator[](int i, int j) const { return ptr[i * cols() + j]; }
    constexpr span<T>   row(int i) const { return span(&(*this)[i, 0], cols()); }

    void rebind(MatrixView other) { 
        ptr = other.ptr;
        n   = other.rows(); 
        m   = other.cols();
    } 
    
    constexpr int rows() const { return N == dynamic ? n : N; }
    constexpr int cols() const { return M == dynamic ? m : M; }
    constexpr T* begin() const { return ptr; }
    constexpr T*   end() const { return ptr + n * m; }
    constexpr T*  data() const { return ptr; }
    // Assignment to another MatrixView is a DEEP COPY of its elements.
    constexpr MatrixView& operator=(const MatrixView& other) {
        assert(other.rows() == n && other.cols() == m);
        copy(other, rows(), cols());
        return *this;
    }
    using MatrixRules::operator=;
    using value_type = T;
};  
// Returns a `MatrixView` of the `Matrix`'s data.
template<typename _, int N, int M>
constexpr auto Matrix<_, N, M>::view(this auto&& self) {
    using T = remove_pointer_t<decltype(self.data())>;
    return MatrixView<T, N, M>(self.data(), self.rows(), self.cols());
}

}