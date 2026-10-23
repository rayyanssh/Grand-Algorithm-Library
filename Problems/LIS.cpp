#pragma once 
#include <vector>
#include <algorithm> 
#include <ranges>
namespace rgs = std::ranges;
using std::vector, rgs::range_value_t;
/*
  a. the length
  b. the actual sequence
*/


// O(n log n) time and O(n) space
int LIS_length(const auto& nums) {
    int n = nums.size(); 
    using T = decltype(nums)::value_type;
    vector<T> piles;  piles.reserve(n);
    piles.push_back(nums[0]);
    for (int i = 1; i < n; i++) {
        int l = rgs::lower_bound(piles, nums[i]) - piles.begin();
        if (l == piles.size()) piles.push_back(nums[i]);
        else piles[l] = nums[i];
    }
    return piles.size();
}

// O(n log n) time and O(n) space. Use patience sorting to find the longest increasing subsequence.
// returns vector<T> 
auto LIS(const auto& nums) {
    int n = nums.size();  
    using T = decltype(nums)::value_type;
    vector< T > piles;                piles.reserve(n);
    vector<int> piles_i;              piles_i.reserve(n);
    vector<int> prev(n, -1); 
    for (int i = 0; i < n; i++) {
        int p = rgs::lower_bound(piles, nums[i]) - piles.begin();
        if (p > 0) prev[i] = piles_i[p - 1]; 
        if (p == piles.size()) {
            piles.push_back(nums[i]);
            piles_i.push_back(i); 
        } else {
            piles[p] = nums[i];
            piles_i[p] = i;
        }
    }
    auto& lis = piles;
    for (int p = piles.size() - 1, i = piles_i.back();
             p >= 0; p--, i = prev[i]) 
        lis[p] = nums[i];
    return lis;
}