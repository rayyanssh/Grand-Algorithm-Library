#pragma once 
#include "../Utils.hpp"   
#include <vector>
namespace Grand { 
// TensorView: a multidimensional view over contiguous data. Similar to std::mdspan.
// Tensor : a multidimensional array.
#ifndef __GRAND_TENSOR 
#define __GRAND_TENSOR 
inline constexpr auto product_dims = [](auto... dims) { return (1LL * ... * dims); };
template<typename T>
concept TensorType = requires { T::rank(); };
#endif

template<typename T, size_t Rank>
struct TensorView {
    T* ptr;
    array<int, Rank> dims;
    TensorView(T* d, auto... ds) requires (sizeof...(ds) == Rank) : ptr(d), dims{(int)ds...} {}
    TensorView(T* d, array<int, Rank> ds) : ptr(d), dims{ds} {}

    T& operator[](auto... idx) const requires (sizeof...(idx) == Rank) { 
        int indices[Rank] = { (int)idx... };
        size_t offset = indices[0];
        for (int i = 1; i < Rank; ++i)
            offset = offset * dims[i] + indices[i];
        return ptr[offset];
    }
    // Linear index. 
    T& operator[](int k) const { return ptr[k]; }
    // `A(i)`: i-th slab (subtensor) along leading axis, rank drops by one. `{4,3,5} -> {3,5}`.
    // Fixes the first index to i: `A(i)[j,k]` is the same element as `A[i,j,k]`.
    // Like `A[i]` on a built-in array such as `int A[4][3][5]` (not `A[i]` here, which is flat).
    TensorView<T, Rank - 1> operator()(int i) const requires (Rank > 1) { 
        array<int, Rank - 1> rem_dims; 
        for (int j = 0; j < Rank - 1; j++) rem_dims[j] = dims[j+1];
        T* start = ptr + i * apply(product_dims, rem_dims);
        return TensorView<T, Rank - 1>(start, rem_dims);
    }
    // `A.subview(s, count)`: `count` slabs starting at `s`, rank kept. `{4,3,5} -> {count,3,5}`.
    // Omit `count` to take everything from `s` to the end.
    TensorView<T, Rank> subview(int s, int count = -1) const {
        if (count == -1) count = dims[0] - s;
        assert(s + count <= dims[0]);
        T* start = ptr + s * rgs::fold_left(dims.begin() + 1, dims.end(), 1LL, std::multiplies());
        auto sub_dims = dims;
        sub_dims[0] = count;
        return TensorView<T, Rank>(start, sub_dims);
    }
    static consteval int rank() { return Rank; }
    constexpr int extent(int d) const { return dims[d]; }
    constexpr auto shape() const { return dims; }
    constexpr long long size() const { return apply(product_dims, dims); }
    T* begin() const { return ptr; }
    T* end()   const { return ptr + size(); }
    T* data()  const { return ptr; }
};
template<typename T, typename... dims>
TensorView(T*, dims...) -> TensorView<T, sizeof...(dims)>;

/// Tensor is a contiguous multi-dimensional array with fixed rank and dynamic dimensions. It supports [i,j,k,...] indexing and slicing with () operator.
template<typename T, size_t Rank> requires (!is_same_v<T, bool>)
struct Tensor {
    std::vector<T> storage;
    array<int, Rank> dims;
    /// example: `Tensor<int, 3> t(2,3,4);`
    Tensor(auto... ds) requires (sizeof...(ds) == Rank) : storage(product_dims(ds...)), dims{(int)ds...} {}
    // example: `Tensor<char, 3> t({2,3,4}, 'A');
    Tensor(array<int, Rank> ds, T val = {}) : storage(apply(product_dims, ds), val), dims{ds} {}
    // example1: `Tensor<float, 2> t({2,3}, {1.0, 1.5, 2.0, 2.5, 3.0, 3.5});`
    // example2: `Tensor<int, 3> t({2,2,2}, {1,0,1})`, a tensor of size 8 with 3 elements provided, 
    // and the rest of the 5 elements are default initialized to 0.
    Tensor(array<int, Rank> ds, init_list<T> init) : storage(apply(product_dims, ds)), dims{ds} { 
        assert(init.size() <= product_dims(ds));
        rgs::copy(init, storage.begin());
    }
    auto view(this auto&& self) { return TensorView(self.storage.data(), self.dims); } 

    auto& operator[](this auto&& self, auto... idx) requires (sizeof...(idx) == Rank) {
        int indices[Rank] = { (int)idx... };
        size_t offset = indices[0];
        for (int i = 1; i < Rank; ++i)
            offset = offset * self.dims[i] + indices[i];
        return self.storage[offset];
    }
    /// Linear index.
    auto& operator[](this auto&& self, int i) { return self.storage[i]; }
    // `A(i)`: returns a view of the i-th slab (subtensor) along leading axis, rank drops by one. 
    // `{4,3,5} -> {3,5}`.
    // `A(i)[j,k]` is the same element as `A[i,j,k]`.
    auto operator()(this auto&& self, int i) requires (Rank > 1) {
        return self.view()(i);
    }
    // `A.subview(s, count)`: `count` slabs starting at `s`, rank kept. `{4,3,5} -> {count,3,5}`.
    // Omit `count` to take everything from `s` to the end.
    auto subview(this auto&& self, int s, int count = -1) {
        return self.view().subview(s, count);
    }
    void resize(array<int, Rank> ds, T val = {}) {
        storage.resize(apply(product_dims, ds), val);
        dims = ds;
    }
    void resize(auto... ds) requires (sizeof...(ds) == Rank) { resize(array{(int)ds...}); } 
    void reserve(int c) { storage.reserve(c); }

    static consteval int rank() { return Rank; } 
    int extent(int i) const { return dims[i]; }
    long long size() const { return apply(product_dims, dims); }
    auto shape() const { return dims; }
    auto* data(this auto&& self) { return self.storage.data(); }
    auto begin(this auto&& self) { return self.storage.begin(); }
    auto   end(this auto&& self) { return self.storage.end(); }
}; 



template<TensorType T>  
using HoldThisTensor = conditional_t<
    !requires(T t) { t.storage; },
    rm_ref<T>,
    T&
>;
  

template<TensorType T, size_t Rank>
struct SubTensor {
    HoldThisTensor<T> tensor;
    array<int, Rank> start;
    array<int, Rank> dims;
    auto& operator[](auto... idx) const requires (sizeof...(idx) == Rank) { 
        return [&]<size_t... I>(index_sequence<I...>) -> auto& {
            return tensor[(start[I] + (int)idx)...];
        }(make_index_sequence<Rank>());
    }
    static consteval int rank() { return Rank; }
    int extent(int i) const { return dims[i]; }
    auto shape() const { return dims; }
    long long size() const { return apply(product_dims, dims);}
};
template<size_t Rank>
auto subtensor(auto&& tensor, const int (&start)[Rank], const int (&window)[Rank]) {
    using T = rm_ref<decltype(tensor)>; 
    return SubTensor<T, Rank>{tensor, std::to_array(start), std::to_array(window)}; 
}
template<size_t Rank>
auto subtensor(auto&& tensor, array<int, Rank> start, array<int, Rank> window) {
    using T = rm_ref<decltype(tensor)>; 
    return SubTensor<T, Rank>{tensor, start, window}; 
}

}