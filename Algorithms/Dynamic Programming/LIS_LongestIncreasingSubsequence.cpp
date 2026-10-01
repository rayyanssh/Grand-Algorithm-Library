#pragma once
#include <span>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ranges>
namespace ranges = std::ranges;
using std::vector, ranges::range_value_t;
/*
1. the simple approach O(n^2)
2. the efficient approach O(n log n)
    a. the length
    b. the actual sequence
*/
// O(n^2) time and O(n) space
vector<int> LIS_simple(std::span<int> numbers) {
    int n = numbers.size();
    vector<int> l(n, 1), prev(n, -1);
    int max_l = 1, max_i = 0;
    for (int k = 1; k < n; k++) {
        for (int i = 0; i < k; i++) {
            if (numbers[i] < numbers[k] && l[i] >= l[k]) {
                l[k] = l[i] + 1;
                prev[k] = i;
            }
        }
        if (l[k] > max_l) {
            max_l = l[k];
            max_i = k;
        }
    }
    vector<int> result;
    for (int i = max_i; i != -1; i = prev[i]) result.push_back(numbers[i]);
    ranges::reverse(result);
    return result;
}

// O(n log n) time and O(n) space
int LIS_length(const auto& nums) {
    int n = nums.size(); 
    using T = range_value_t<decltype(nums)>;
    vector<T> piles; 
    int expected_piles = int(2 * sqrt(n)); piles.reserve(expected_piles);
    piles.push_back(nums[0]);
    for (int i = 1; i < n; i++) {
        int l = ranges::lower_bound(piles, nums[i]) - piles.begin();
        if (l == piles.size()) 
            piles.push_back(nums[i]);
        else piles[l] = nums[i];
    }
    return piles.size();
}

// O(n log n) time and O(n) space. Use patience sorting (piles) to find the longest increasing subsequence.
auto LIS(const auto& nums) {
    int n = nums.size(); 
    int expected_piles = int(2.2 * sqrt(n));
    using T = decltype(nums)::value_type;
    vector< T > piles;                piles.reserve(expected_piles);
    vector<int> piles_i;              piles_i.reserve(expected_piles);
    vector<int> prev(n, -1); 
    for (int i = 0; i < n; i++) {
        int p = ranges::lower_bound(piles, nums[i]) - piles.begin();
        if (p > 0) prev[i] = piles_i[p - 1]; 
        if (p == piles.size()) {
            piles.push_back(nums[i]);
            piles_i.push_back(i); 
        } else {
            piles[p] = nums[i];
            piles_i[p] = i;
        }
    }
    for (int p = piles.size() - 1, i = piles_i.back();
             p >= 0; p--, i = prev[i]) 
        piles[p] = nums[i];
    return piles;
}