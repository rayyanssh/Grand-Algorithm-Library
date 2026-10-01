#pragma once
// Just general utilities that I use often
#include <concepts> 
#include <bit>
#include <initializer_list>
#include <type_traits> 
using std::integral, std::floating_point,  std::swap, 
      std::countr_zero, std::countl_zero, std::countr_one, std::countl_one, std::popcount, std::has_single_bit, std::bit_ceil, std::bit_floor, std::bit_width,
      std::is_same_v, std::remove_cvref_t, std::remove_reference_t, std::remove_const_t,
      std::conditional_t;

template<typename T> using init_list = std::initializer_list<T>;
template<typename T> concept Number = integral<T> || floating_point<T>;
struct none{};
constexpr int dynamic = -1; 

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