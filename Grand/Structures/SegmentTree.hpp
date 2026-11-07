#include <vector>
#include "../Utils.hpp"
namespace Grand {

template<typename T, typename Op>
struct SegmentTree {
    std::vector<T> data;
    Op op;
    T identity;
    int n;
    SegmentTree(Op f, T id) : op(f), identity(id) {}
    SegmentTree(SizedRange auto&& nums, Op f, T id) : op(f), identity(id) { build(nums); }

    void build(SizedRange auto&& nums) {
        n = nums.size();
        data.resize(2 * n);
        rgs::copy(nums, data.begin() + n);
        for (int i = n-1; i >= 1; i--) 
            data[i] = op(data[2*i], data[2*i + 1]);
    }
    // Inclusive Range `[l, r]`.
    T query(int l, int r) const {
        l+=n; r+=n;
        T left = identity; T right = identity;
        while (l <= r) {
            if (l & 1) left = op(left, data[l++]);
            if (!(r & 1)) right = op(data[r--], right);
            l /= 2; r /= 2;
        }
        return op(left, right);
    }

    void update(int i, T val) {
        i += n;
        data[i] = val;
        for (i /= 2; i > 0; i /= 2)
            data[i] = op(data[2*i], data[2*i + 1]); 
    }

};

}