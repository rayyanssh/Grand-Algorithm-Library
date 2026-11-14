#include <vector>
#include "../Utils.hpp"
namespace Grand {

// Bottom-up Segment Tree.
// The operation must be associative and have an identity element.
template<typename T, typename Op>
struct SegmentTree {
    std::vector<T> tree;
    Op op;
    T identity;
    int n = 0;
    SegmentTree(Op f, T id) : op(f), identity(id) {}
    SegmentTree(SizedRange auto&& nums, Op f, T id) : op(f), identity(id) { build(nums); }

    void build(SizedRange auto&& nums) {
        n = rgs::size(nums);
        tree.resize(2 * n);
        rgs::copy(nums, tree.begin() + n);
        for (int i = n-1; i >= 1; i--) 
            tree[i] = op(tree[2*i], tree[2*i + 1]);
    }
    // Inclusive Range `[l, r]`.
    T query(int l, int r) const {
        l+=n; r+=n;
        T left = identity; T right = identity;
        while (l <= r) {
            if (l & 1)     left = op(left, tree[l++]);
            if (!(r & 1)) right = op(tree[r--], right);
            l /= 2; r /= 2;
        }
        return op(left, right);
    }

    void update(int i, T val) {
        i += n;
        tree[i] = val;
        for (i /= 2; i > 0; i /= 2)
            tree[i] = op(tree[2*i], tree[2*i + 1]); 
    }
};

}