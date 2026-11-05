#pragma once
// Just general utilities that I use often
#include <concepts> 
#include <bit>
#include <initializer_list>
#include <iterator>
#include <type_traits> 
#include <algorithm>
#include <tuple> 
#include <ranges>
#include <span>

namespace Grand {
namespace rgs = std::ranges;
using std::integral, std::floating_point,  std::swap, std::max, std::min, std::move,
      std::array, std::span, std::pair, std::tuple, std::make_tuple, std::apply, std::tie,
      std::countr_zero, std::countl_zero, std::countr_one, std::countl_one, std::popcount, std::has_single_bit, std::bit_ceil, std::bit_floor, std::bit_width,
      std::is_same_v, std::is_const_v, std::is_reference_v,
      std::remove_cvref_t, std::remove_reference_t, std::remove_const_t, std::conditional_t;
    
template<class T> concept InputRange   = rgs::input_range<T>; 
template<class T> concept InputIt      = std::input_iterator<T>;

template<class T> concept ForwardRange = rgs::forward_range<T>; 
template<class T> concept ForwardIt    = std::forward_iterator<T>;

template<class T> concept BidirRange   = rgs::bidirectional_range<T>; 
template<class T> concept BidirIt      = std::bidirectional_iterator<T>;

template<class T> concept RA_Range     = rgs::random_access_range<T>; 
template<class T> concept RA_It        = std::random_access_iterator<T>;

template<class T> concept ContRange    = rgs::contiguous_range<T>; 
template<class T> concept ContIt       = std::contiguous_iterator<T>;


template<typename T> concept Number = integral<T> || floating_point<T>;
struct none{};
constexpr int dynamic = -1; 

template<typename T>
using init_list = std::initializer_list<T>;


} // namespace Grand
 