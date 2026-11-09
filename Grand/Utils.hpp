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
#include <utility>

namespace Grand {
namespace rgs = std::ranges;
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
 