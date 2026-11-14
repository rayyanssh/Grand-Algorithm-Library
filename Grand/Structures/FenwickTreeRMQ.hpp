#include "../Utils.hpp"
#include <vector>
namespace Grand {

// Variant of the Fenwick Tree for Range Minimum Queries and Updates.   
// Updates modify the input data that the tree was built from.
// Allows any type T that has total ordering (`operator<`) defined.
// Specify the max element in the ctor.
// Faster queries than segment tree, but slower updates.
// Reference: Efficient Range Minimum Queries using Binary Indexed Trees.
template<typename T>
struct FenwickTreeRMQ {
    std::vector<T> BIT1, BIT2;  
    span<T> data; 
    T inf;
    int n = 0;   
    FenwickTreeRMQ(T inf) : inf(inf) {}
    FenwickTreeRMQ(span<T> nums, T inf) : inf(inf) { build(nums); } 
    void build(span<T> nums) { 
        n = nums.size();
        data = nums;
        BIT1.resize(n + 1); BIT2.resize(n + 1);
        rgs::copy(nums, BIT1.begin() + 1); rgs::copy(nums, BIT2.begin() + 1); 
        for (int i = 1; i <= n; i++) {
            int p = i + (i & -i);
            if (p <= n) BIT1[p] = min(BIT1[i], BIT1[p]); 
        }
        for (int i = n; i >= 1; i--) {
            int p = i & (i - 1);
            if (p >= 1) BIT2[p] = min(BIT2[i], BIT2[p]);
        }
    }
    T query(int l, int r) const { l++; r++;
        int i = l;
        T res = inf;
        while (i + (i & -i) <= r) {
            res = min(res, BIT2[i]);
            i += (i & -i);
        }
        i = r;
        while ((i & (i - 1)) >= l) {
            res = min(res, BIT1[i]);
            i &= i - 1;
        }
        res = min(res, data[i-1]); 
        return res;
    } 
    void update(int k, T v) { k++;
        T old = data[k-1];
        data[k-1] = v;
        T L = inf, R = inf;
        for (int i = k, l = k - 1, r = k + 1; i <= n; i += i & -i) {
            if (BIT1[i] != old) BIT1[i] = min(v, BIT1[i]);
            else {
                for (; r < i; r += r & -r) 
                    R = min(R, BIT2[r]); 
                R = min(R, data[i-1]); 
                
                for (; l > (i & (i - 1)); l &= l - 1) 
                    L = min(L, BIT1[l]);  
                BIT1[i] = min({v, L, R});
            }
        }
        L = R = inf;
        for (int i = k, l = k - 1, r = k + 1; i >= 1; i &= (i-1)) {
            if (BIT2[i] != old) BIT2[i] = min(v, BIT2[i]);
            else {
                for (; l > i; l &= l - 1) 
                    L = min(L, BIT1[l]);
                L = min(L, data[i-1]);
                for (; r < i + (i & -i) && r <= n; r += r & -r) 
                    R = min(R, BIT2[r]);
                BIT2[i] = min({v, L, R});
            }
        }
    } 
};

}