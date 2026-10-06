#include <ranges>
using namespace std;
using std::integral;
int binary_search(const auto& A, integral auto x) {
    int left = 0, right = size(A) - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (A[mid] == x) return mid;
        else if (A[mid] < x) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}
int binary_search_jump(const auto& A, integral auto x) {
    int n = size(A);
    if (n == 0) return -1;
    int jump = 1;
    while (jump < n && A[jump] < x) jump *= 2;
    int left = jump / 2, right = min(jump, n - 1);
    return -1; // not found
}