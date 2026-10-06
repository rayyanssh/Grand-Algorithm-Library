#pragma once 
#include "../Utils.hpp" 
namespace Grand {

constexpr long long product(auto... dims) { return (1LL * ... * dims); }
template<int Rank> constexpr long long product(array<int, Rank> dims) { 
    long long prod = 1; 
    for (int d : dims) prod *= d; 
    return prod; 
}
template<typename T, int Rank> struct TensorView; // consider this similar as std::mdspan

/// Tensor is a contiguous multi-dimensional array withfixed rank and dynamic dimensions. It supports [i,j,k,...] indexing and slicing with () operator.
template<typename T, int Rank>
struct Tensor {
    vector<T> storage;
    array<int, Rank> dims_;
    /// example: `Tensor<int, 3> t(2,3,4);`
    Tensor(auto... ds) requires (sizeof...(ds) == Rank) : storage(product(ds...)), dims_{(int)ds...} {}
    // example: `Tensor<char, 3> t({2,3,4}, 'A');
    Tensor(array<int, Rank> ds, T val)                  : storage(product(ds), val), dims_{ds} {}
    // example1: `Tensor<float, 2> t({2,3}, {1.0, 1.5, 2.0, 2.5, 3.0, 3.5});`
    // example2: `Tensor<int, 3> t({2,2,2}, {1,0,1})`, a tensor of size 8 with 3 elements provided, 
    // and the rest of the 5 elements are default initialized to 0.
    Tensor(array<int, Rank> ds, init_list<T> init) : storage(product(ds)), dims_{ds} { assert(init.size() <= storage.size());
        int i = 0; for (const auto& val : init) storage[i++] = val; 
    }
    auto view() { return TensorView(storage.data(), dims_); } 

    auto& operator[](this auto&& self, auto... idx) requires (sizeof...(idx) == Rank) {
        size_t offset = 0;
        int indices[] = { (int)idx... };
        for (int i = 0; i < Rank; ++i)
            offset = offset * self.dims_[i] + indices[i];
        return self.storage[offset];
    }
    /// Linear indexing for 1D access to the underlying storage.
    auto& operator[](this auto&& self, int i) { return self.storage[i]; }
    TensorView<T, Rank - 1> operator()(int i) requires (Rank > 1) {
        array<int, Rank - 1> sub_dims;
        for (int j = 0; j < Rank - 1; ++j)
            sub_dims[j] = dims_[j + 1];
        return TensorView<T, Rank - 1>(storage.data() + i * product(sub_dims), sub_dims);
    }
    TensorView<const T, Rank - 1> operator()(int i) const requires (Rank > 1) {
        array<int, Rank - 1> sub_dims;
        for (int j = 0; j < Rank - 1; ++j)
            sub_dims[j] = dims_[j + 1];
        return TensorView<const T, Rank - 1>(storage.data() + i * product(sub_dims), sub_dims);
    }
    constexpr int rank() const { return Rank; }
    constexpr long long size() const { return product(dims_); }
    constexpr int dims(int dim) const { return dims_[dim]; }
    constexpr int rows() const requires (Rank == 2) { return dims_[0]; }
    constexpr int cols() const requires (Rank == 2) { return dims_[1]; }
}; 



template<typename T, int Rank>
struct TensorView {
    T* storage;
    array<int, Rank> dims_;
    TensorView(T* d, auto... ds) requires (sizeof...(ds) == Rank) : storage(d), dims_{(int)ds...} {}
    TensorView(T* d, array<int, Rank> ds) : storage(d), dims_{ds} {}
    T& operator[](auto... idx) requires (sizeof...(idx) == Rank) { 
        size_t offset = 0;
        int indices[] = { (int)idx... };
        for (int i = 0; i < Rank; ++i)
            offset = offset * dims_[i] + indices[i];
        return storage[offset];
    }
    constexpr int dims(int dim) const { return dims_[dim]; }
};
template<typename T, typename... dims>
TensorView(T*, dims...) -> TensorView<T, sizeof...(dims)>;



template<typename T>
using Cube = Tensor<T, 3>;

template<typename T>
using Tesseract = Tensor<T, 4>;

}