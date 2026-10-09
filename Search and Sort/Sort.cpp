#include <span>
#include <vector>
#include <ranges>
#include <algorithm>
using namespace std; using ranges::contiguous_range;

// MergeSort implementation, takes std::span as arguments
template<typename T>
void mergesort(span<T> A, span<T> temp) {
    int n = A.size();
    if (n <= 1) return;
    int mid = n / 2;  
    span<T> left = A.subspan(0, mid); span<T> left_temp = temp.subspan(0, mid);
    span<T> right = A.subspan(mid);   span<T> right_temp = temp.subspan(mid);
    mergesort(left, left_temp);
    mergesort(right, right_temp);
    ranges::merge(left, right, temp.begin());
    ranges::copy(temp, A.begin());
}

// Generic MergeSort, works on any contiguous container
void mergesort(auto& A) {
    using T = ranges::range_value_t<decltype(A)>;
    vector<T> temp(size(A));
    mergesort(span(A), span(temp));
}
#include <iostream>
int main() {
    vector<int> A = {5, 2, 9, 1, 5, 6};
    mergesort(A);
    for (int x : A) {
        cout << x << " ";
    }
    cout << endl;
}
