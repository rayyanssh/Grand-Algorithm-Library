#pragma once
#include <vector>
#include <array>
using std::vector, std::array;

constexpr int product_dims(auto... dims) { return (1 * ... * dims); }
template<size_t Rank> constexpr int product_dims(array<int, Rank> dims) { 
    size_t prod = 1; 
    for (int d : dims) prod *= d; 
    return prod; 
}

template<typename T, size_t Rank>
struct Tensor {
    vector<T> storage;
    array<int, Rank> dims;
    Tensor(auto... ds) requires (sizeof...(ds) == Rank) : storage(product_dims(ds...)), dims{(int)ds...} {}
    Tensor(array<int, Rank> ds, T val)                  : storage(product_dims(ds), val), dims{ds} {}
    Tensor(array<int, Rank> ds, auto&& init) : storage(product_dims(ds)), dims{ds} { assert(init.size() == product_dims(ds));
        int i = 0; for (auto& val : init) storage[i++] = val; 
    }
    auto view() { return TensorView(storage.data(), dims, dims); } 
    T& operator[](auto... idx)  requires (sizeof...(idx) == Rank) {
        size_t offset = 0;
        int indices[] = { (int)idx... };
        for (int i = 0; i < Rank; ++i)
            offset = offset * dims[i] + indices[i];
        return storage[offset];
    }
    int extents(int dim) const { return dims[dim]; }
}; 


template<typename T, size_t Rank>
struct TensorView {
    T* storage;
    array<int, Rank> dims, strides;
    TensorView(T* d, auto... ds) requires (sizeof...(ds) == Rank) : storage(d), dims{(int)ds...}, strides{dims} {}
    TensorView(T* d, array<int, Rank> ds, array<int, Rank> s) : storage(d), dims(ds), strides(s) {}
    T& operator[](auto... idx)   requires (sizeof...(idx) == Rank) { 
        size_t offset = 0;
        int indices[] = { (int)idx... };
        for (int i = 0; i < Rank; ++i)
            offset = offset * dims[i] + indices[i];
        return storage[offset];
    }
    int extents(int dim) const { return dims[dim]; }
};
template<typename T, typename... Dims>
TensorView(T*, Dims...) -> TensorView<T, sizeof...(Dims)>;

template<typename T>
using Cube = Tensor<T, 3>;

template<typename T>
using Tesseract = Tensor<T, 4>;

template<typename T>
using SpaceStone = Tesseract<T>;

template<typename T>
using Pentaract = Tensor<T, 5>;

template<typename T>
using Hexeract = Tensor<T, 6>;