#include "../Utils.hpp"
namespace Grand {
 
#ifndef __GRAND_TENSOR 
#define __GRAND_TENSOR 
inline constexpr auto product_dims = [](auto... dims) { return (1LL * ... * dims); };
template<typename T>
concept TensorType = requires { T::rank(); };
#endif


template<typename T, int... Dims>
struct FixedTensorView {
    static constexpr size_t Rank = sizeof...(Dims);
    static constexpr array<int, Rank> dims = {Dims...}; 
    T* ptr;
    constexpr FixedTensorView(T* d = nullptr) : ptr(d) {}
    constexpr T& operator[](auto... idx) const requires (sizeof...(idx) == Rank) { 
        int indices[Rank] = { (int)idx... };
        size_t offset = indices[0];
        for (int i = 1; i < Rank; ++i)
            offset = offset * dims[i] + indices[i];
        return ptr[offset];
    }
    // Linear index. 
    constexpr T& operator[](int k) const { return ptr[k]; } 
    // `A(i)`: i-th slab (subtensor) along leading axis, rank drops by one. `{4,3,5} -> {3,5}`.
    // `A(i)[j,k]` is the same element as `A[i,j,k]`. 
    // Return type: `FixedTensorView<T, Dims[1...]>`.
    constexpr auto operator()(int i) const requires (Rank > 1) { 
        return [&]<int First, int... Rest>() {
            T* start = ptr + i * product_dims(Rest...);
            return FixedTensorView<T, Rest...>(start);
        }.template operator()<Dims...>();
    }
    static consteval int rank() { return Rank; }
    static consteval int extent(int d) { return dims[d]; }
    static consteval auto shape() { return dims; }
    static consteval long long size() { return product_dims(Dims...); }
    constexpr T* begin() const { return ptr; }
    constexpr T* end()   const { return ptr + size(); }
    constexpr T* data()  const { return ptr; }
};

template<typename T, int... Dims>
struct FixedTensor {
    static constexpr size_t Rank = sizeof...(Dims);
    static constexpr array<int, Rank> dims = {Dims...}; 
    array<T, product_dims(Dims...)> storage;
    constexpr FixedTensor(T val = {}) { storage.fill(val); }
    constexpr FixedTensor(init_list<T> init) : storage{} { rgs::copy(init, storage.begin()); }

    constexpr auto& operator[](this auto&& self, auto... idx) requires (sizeof...(idx) == Rank) { 
        int indices[Rank] = { (int)idx... };
        size_t offset = indices[0];
        for (int i = 1; i < Rank; ++i)
            offset = offset * self.dims[i] + indices[i];
        return self.storage[offset];
    }
    // Linear index. 
    constexpr auto& operator[](this auto&& self, int k) { return self.storage[k]; }
    constexpr auto view(this auto&& self) { 
        using t = remove_reference_t<decltype(self[0])>;
        return FixedTensorView<t, Dims...>(self.data()); 
    }
    // `A(i)`: i-th slab (subtensor) along leading axis, rank drops by one. `{4,3,5} -> {3,5}`.
    // `A(i)[j,k]` is the same element as `A[i,j,k]`. 
    // Return type: `FixedTensorView<T, Dims[1...]>`.
    constexpr auto operator()(this auto&& self, int i) requires (Rank > 1) { 
        return self.view()(i);
    }
    static consteval int rank() { return Rank; }
    static consteval int extent(int d) { return dims[d]; }
    static consteval auto shape() { return dims; }
    static consteval long long size() { return product_dims(Dims...); }
    constexpr auto* begin(this auto&& self) { return self.storage.begin(); }
    constexpr auto* end(this auto&& self)   { return self.storage.end(); }
    constexpr auto* data(this auto&& self)  { return self.storage.data(); }
};

template<TensorType T>  
using HoldThisTensor = conditional_t<
    !requires(T t) { t.storage; },
    remove_reference_t<T>,
    T&
>;

template<TensorType T, int... Dims>
struct FixedSubTensor {
    static constexpr size_t Rank = sizeof...(Dims);
    static constexpr array<int, Rank> dims = {Dims...}; 
    HoldThisTensor<T> tensor;
    array<int, Rank> start;
    constexpr auto& operator[](auto... idx) const requires (sizeof...(idx) == Rank) { 
        return [&]<size_t... I>(index_sequence<I...>) -> auto& {
            return tensor[(start[I] + (int)idx)...];
        }(make_index_sequence<Rank>());
    }
    static consteval int rank() { return Rank; }
    static consteval int extent(int d) { return dims[d]; }
    static consteval auto shape() { return dims; }
    static consteval long long size() { return product_dims(Dims...); } 
};
template<int... Dims>
auto subtensor(auto&& tensor, const int (&start)[sizeof...(Dims)]) {
    using T = remove_reference_t<decltype(tensor)>; 
    return FixedSubTensor<T, Dims...>{ tensor, std::to_array(start) }; 
}
template<int... Dims>
auto subtensor(auto&& tensor, array<int, sizeof...(Dims)> start) {
    using T = remove_reference_t<decltype(tensor)>; 
    return FixedSubTensor<T, Dims...>{ tensor, start }; 
}


}