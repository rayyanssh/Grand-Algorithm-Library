#include <functional>
#include <vector>
#include "../Utils.hpp"
namespace Grand {

// `Fenwick Tree (Binary Indexed Tree)`. Supports `O(log n)` range queries and point updates.
// By default, the operation is SUM.
// You can specify the operation and its inverse. The requirements are:
// Commutativity, associativity, having an inverse function and an identity element.
template<typename T, typename Op = std::plus<T>, typename Inv = std::minus<T>>
struct FenwickTree {
    std::vector<T> tree;
    Op op{};
    Inv inv{}; 
    T identity{}; 
    int n = 0;
    // Default ctors for sum fenwick tree
    FenwickTree() = default;
    FenwickTree(SizedRange auto&& nums) { build(nums); }

    FenwickTree(Op op, Inv inverse, T id) : op(op), inv(inverse), identity(id) {}
    FenwickTree(SizedRange auto&& nums, Op op, Inv inverse, T id) 
        : op(op), inv(inverse), identity(id) { build(nums); }
    void build(SizedRange auto&& nums) {
        n = rgs::size(nums);
        tree.resize(n + 1);
        rgs::copy(nums, tree.begin() + 1);
        for (int i = 1; i <= n; i++) {
            int p = i + (i & -i);
            if (p <= n) tree[p] = op(tree[p], tree[i]);
        }  
    }
    T query(int a, int b) const { return op(prefix(b), inv(prefix(a - 1))); }
    T prefix(int k) const { 
        T sum = identity;
        for (int i = k + 1; i != 0; i &= (i - 1))
            sum = op(sum, tree[i]); 
        return sum;
    }
    void update(int i, T x) { 
        T delta = inv(x, query(i,i));
        add(i, delta); 
    }
    void add(int i, T delta) {
        for (i++; i <= n; i += i & -i) tree[i] = op(tree[i], delta); 
    } 
}; 
template<SizedRange R> 
FenwickTree(R&&) -> FenwickTree<rgs::range_value_t<R>>;   

}



