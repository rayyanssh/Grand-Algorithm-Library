#include <vector>
#include <ranges>
#include <algorithm>
#include <iostream>

using namespace std;
namespace rgs=std::ranges;
    
int LIS(vector<int>& nums) {
    int n = nums.size(); 
    vector<int> tails; tails.reserve(n);
    for (int i = 0; i < n; i++) {
        auto it = rgs::lower_bound(tails, nums[i]);
        if (it == tails.end()) tails.push_back(nums[i]); 
        else *it = nums[i]; 
    }
    return tails.size();
}
int main() { 
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int i : views::iota(0,n)) cin >> nums[i];
    int lis = LIS(nums);
    cout << lis;
}