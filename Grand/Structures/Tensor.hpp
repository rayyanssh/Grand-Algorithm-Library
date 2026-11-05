#pragma once 
#include "../Utils.hpp" 
#include <vector>
namespace Grand {

// TensorView: a multidimensional view over contiguous data. Similar to std::mdspan.
// Tensor : a multidimensional array.

constexpr long long product_dims(auto... dims) { return (1LL * ... * dims); }  

template<typename T, int Rank>
struct TensorView {
    T* ptr;
    array<int, Rank> dims;
    TensorView(T* d, auto... ds) requires (sizeof...(ds) == Rank) : ptr(d), dims{(int)ds...} {}
    TensorView(T* d, array<int, Rank> ds) : ptr(d), dims{ds} {}

    T& operator[](auto... idx) const requires (sizeof...(idx) == Rank) { 
        size_t offset = 0;
        int indices[] = { (int)idx... };
        for (int i = 0; i < Rank; ++i)
            offset = offset * dims[i] + indices[i];
        return ptr[offset];
    }
    T& operator[](int k) const { return ptr[k]; } 

    TensorView<T, Rank - 1> operator()(int s) const {
        T* start = s * apply(product_dims, span<T, 4>(dims + 1));
        return TensorView<T, Rank - 1>(start, span<T, Rank - 1>(dims + 1));
    }

    constexpr int  extent(int d) const { return dims[d]; }
    constexpr auto shape() const { return dims; }
    constexpr long long size() const { return apply(product_dims, dims); }
    T* begin() const { return ptr; }
    T* end() const { return ptr + size(); }
    T* data() const { return ptr; }
};
template<typename T, typename... dims>
TensorView(T*, dims...) -> TensorView<T, sizeof...(dims)>;


/// Tensor is a contiguous multi-dimensional array with fixed rank and dynamic dimensions. It supports [i,j,k,...] indexing and slicing with () operator.
template<typename T, size_t Rank>
struct Tensor {
    std::vector<T> storage;
    array<int, Rank> dims_;
    /// example: `Tensor<int, 3> t(2,3,4);`
    Tensor(auto... ds) requires (sizeof...(ds) == Rank) : storage(product_dims(ds...)), dims_{(int)ds...} {}
    // example: `Tensor<char, 3> t({2,3,4}, 'A');
    Tensor(array<int, Rank> ds, T val = {})             : storage(product_dims(ds), val), dims_{ds} {}
    // example1: `Tensor<float, 2> t({2,3}, {1.0, 1.5, 2.0, 2.5, 3.0, 3.5});`
    // example2: `Tensor<int, 3> t({2,2,2}, {1,0,1})`, a tensor of size 8 with 3 elements provided, 
    // and the rest of the 5 elements are default initialized to 0.
    Tensor(array<int, Rank> ds, init_list<T> init) : storage(product_dims(ds)), dims_{ds} { assert(init.size() <= storage.size());
        assert(init.size() <= product_dims(ds));
        rgs::copy(init, storage.begin());
    }
    auto view(this auto&& self) { return TensorView(self.storage.data(), self.dims_); } 

    auto& operator[](this auto&& self, auto... idx) requires (sizeof...(idx) == Rank) {
        size_t offset = 0;
        int indices[] = { (int)idx... };
        for (int i = 0; i < Rank; ++i)
            offset = offset * self.dims_[i] + indices[i];
        return self.storage[offset];
    }
    /// Linear indexing for 1D access to the underlying storage.
    auto& operator[](this auto&& self, int i) { return self.storage[i]; }

    auto operator()(this auto&& self, int i) requires (Rank > 1) {
        array<int, Rank - 1> sub_dims;
        for (int j = 0; j < Rank - 1; ++j)
            sub_dims[j] = self.dims_[j + 1];
        using T_ = T if not const else const T
        return TensorView<T_, Rank - 1>(self.storage.data() + i * product_dims(sub_dims), sub_dims);
    } 
    constexpr int rank() const { return Rank; }
    constexpr long long size() const { return product_dims(dims_); }
    constexpr int dims(int dim) const { return dims_[dim]; }
    constexpr int rows() const requires (Rank == 2) { return dims_[0]; }
    constexpr int cols() const requires (Rank == 2) { return dims_[1]; }
}; 

}