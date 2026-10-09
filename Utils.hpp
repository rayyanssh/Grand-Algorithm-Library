#pragma once
// Just general utilities that I use often
#include <concepts> 
#include <bit>
#include <initializer_list>
#include <type_traits> 
#include <algorithm>
#include <tuple>
#include <vector>
#include <ranges>
#include <span>

namespace Grand {
using std::integral, std::floating_point,  std::swap, std::max, std::min, std::array, std::vector, std::span,
      std::pair, std::tuple, std::make_tuple, std::get, std::apply, std::tie,
      std::countr_zero, std::countl_zero, std::countr_one, std::countl_one, std::popcount, std::has_single_bit, std::bit_ceil, std::bit_floor, std::bit_width,
      std::is_same_v, std::remove_cvref_t, std::remove_reference_t, std::remove_const_t,
      std::conditional_t;

template<typename T> using init_list = std::initializer_list<T>;
template<typename T> concept Number = integral<T> || floating_point<T>;
struct none{};
constexpr int dynamic = -1; 


template<int D, int N>
void ndcopy(const auto& src, auto& dest, const array<int, N>& dims, array<int, N>& idx) {
    if constexpr (D == N) std::apply([&](auto... i) { dest[i...] = src[i...]; }, idx);
    else for (idx[D] = 0; idx[D] < dims[D]; ++idx[D])
            ndcopy<D + 1>(src, dest, dims, idx);
}
template<int N>
void ndcopy(const auto& src, auto& dest, array<int, N> dims) {
    array<int, N> idx{};
    ndcopy<0>(src, dest, dims, idx);
}
void ndcopy(const auto& src, auto& dest, auto... dims) {
    ndcopy(src, dest, array{dims...});
}




} // namespace Grand

/*
#include <iostream>
#include <print>
int main() {
    using std::vector;
    vector<vector<int>> vv = {{1,2,3},{4,5,6},{7,8,9}};
    Gridify g{vv};   // CTAD from the ctor, deduces G = vector<vector<int>>
    std::cout << g[1,1] << std::endl;  // Output: 5
    std::println("{}", vv);
}
*/