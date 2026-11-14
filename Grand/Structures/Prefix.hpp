#include <numeric>
#include <algorithm>
#include <vector>
int main() {
    int n = 100;
    std::vector<int> nums(100);
    std::iota(nums.begin(), nums.end(), 100);
    std::vector<int> prefix(100);
    for (int i = 1; i < n; i++) {
        prefix[i] = nums[i-1] + nums[i];
    }
}