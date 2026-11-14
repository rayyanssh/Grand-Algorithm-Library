#include "../Utils.hpp"
#include <vector>
namespace Grand {

// Variant of Fenwick Tree for Monoids.
// Just needs an associative operation and an identity element.
// Updates modify the input data that the tree was built from. 
// Faster queries than segment tree for cheap ops, but slower updates.
// Adapted from the RMQ Fenwick Tree.
// Reference: Efficient Range Minimum Queries using Binary Indexed Trees.
template<typename T, typename Op>
struct FenwickTreeMonoid {
    std::vector<T> BIT1, BIT2;  
    span<T> data; 
    Op op;
    T identity;
    int n = 0;  
    FenwickTreeMonoid(Op op, T identity) : op(op), identity(identity) {} 
    FenwickTreeMonoid(span<T> nums, Op op, T identity) : op(op), identity(identity) { build(nums); } 
    void build(span<T> nums) { 
        int n = nums.size();
        data = nums;
        BIT1.assign(n + 1, identity); 
        BIT2.assign(n + 1, identity);
        for (int i = 1; i <= n; i++) {
            BIT1[i] = op(BIT1[i], data[i-1]);
            int p = i + (i & -i);
            if (p <= n) BIT1[p] = op(BIT1[p], BIT1[i]); 
        }
        for (int i = n; i >= 1; i--) {
            BIT2[i] = op(data[i-1], BIT2[i]);
            int p = i & (i - 1);
            if (p >= 1) BIT2[p] = op(BIT2[i], BIT2[p]);
        }
    }

    T query(int l, int r) const { l++; r++;
        int i = l;
        T L = identity;
        while (i + (i & -i) <= r) {
            L = op(L, BIT2[i]);
            i += (i & -i);
        }
        T R = identity;
        i = r;
        while ((i & (i - 1)) >= l) {
            R = op(BIT1[i], R);
            i &= i - 1;
        } 
        return op(L, op(data[i-1], R));
    }

    void update(int k, T v) { k++; 
        T L = identity, R = identity;
        for (int i = k, l = k - 1, r = k + 1; i <= n; i += i & -i) { 
            for (; r < i; r += r & -r) 
                R = op(R, BIT2[r]);  
            for (; l > (i & (i - 1)); l &= l - 1) 
                L = op(BIT1[l], L);  
            BIT1[i] = (i == k) ? op(L, v)
                               : op(L, op(v, op(R, data[i-1])));
        }
        L = R = identity;
        for (int i = k, l = k - 1, r = k + 1; i >= 1; i &= i - 1) {  
            for (; l > i; l &= l - 1) 
                L = op(BIT1[l], L); 
            for (; r < i + (i & -i) && r <= n; r += r & -r) 
                R = op(R, BIT2[r]);
            BIT2[i] = (i == k) ? op(v, R) 
                               : op(data[i-1], op(L, op(v, R))); 
        }
        data[k-1] = v;
    }
};

}