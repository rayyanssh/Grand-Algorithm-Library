#line 1 "D_TowerOfHanoi.cpp"
#include <bits/stdc++.h>
#line 2 "/home/rayyan/CodeHub/Grand Library/Grand/Utils.hpp"
// Just general utilities that I use often
#include <concepts> 
#include <bit>
#include <initializer_list>
#line 7 "/home/rayyan/CodeHub/Grand Library/Grand/Utils.hpp"
#include <type_traits> 
#line 10 "/home/rayyan/CodeHub/Grand Library/Grand/Utils.hpp"
#include <ranges>
#include <span>
#line 13 "/home/rayyan/CodeHub/Grand Library/Grand/Utils.hpp"

namespace Grand {
namespace rgs = std::ranges;
using std::integral, std::floating_point,  std::swap, std::max, std::min, std::move,
      std::array, std::span, std::pair, std::tuple, std::make_tuple, std::apply, std::tie, std::to_array,
      std::make_index_sequence, std::index_sequence,
      std::countr_zero, std::countl_zero, std::countr_one, std::countl_one, std::popcount, std::has_single_bit, std::bit_ceil, std::bit_floor, std::bit_width,
      std::is_same_v, std::is_const_v, std::is_reference_v, std::conditional_t, std::convertible_to,
      std::remove_cvref_t, std::remove_reference_t, std::remove_const_t, std::remove_pointer_t;
    
#define RANGE_CONCEPT(name, ...) \
    template<class T> concept name = __VA_ARGS__;
RANGE_CONCEPT(InputRange,   rgs::input_range<T>)
RANGE_CONCEPT(InputIt,      std::input_iterator<T>)
RANGE_CONCEPT(ForwardRange, rgs::forward_range<T>)
RANGE_CONCEPT(ForwardIt,    std::forward_iterator<T>)
RANGE_CONCEPT(BidirRange,   rgs::bidirectional_range<T>)
RANGE_CONCEPT(BidirIt,      std::bidirectional_iterator<T>)
RANGE_CONCEPT(RA_Range,     rgs::random_access_range<T>)
RANGE_CONCEPT(RA_It,        std::random_access_iterator<T>)
RANGE_CONCEPT(ContRange,    rgs::contiguous_range<T>)
RANGE_CONCEPT(ContIt,       std::contiguous_iterator<T>)
RANGE_CONCEPT(SizedRange,   rgs::sized_range<T>)
#undef RANGE_CONCEPT

template<typename T> concept Number = integral<T> || floating_point<T>;
struct none{};
constexpr int dynamic = -1; 

template<typename T>
using init_list = std::initializer_list<T>;


} // namespace Grand
 
#line 3 "/home/rayyan/CodeHub/Grand Library/Grand/Structures/SegmentTree.hpp"
namespace Grand {

template<typename T, typename Op>
struct SegmentTree {
    std::vector<T> data;
    Op op;
    T identity;
    int n = 0;
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
            if (l & 1)     left = op(left, data[l++]);
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
#line 3 "D_TowerOfHanoi.cpp"
using namespace std;
using ull = unsigned long long; 
constexpr int MOD = 998244353; 

struct Node {
    array<ull, 3> dist;
    array<char, 3> target;
    int len;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q; cin >> n >> q; 
    vector<int> p(n+1); 
    vector<int> pow2(n+1); pow2[0] = 1;
    for (int i = 1; i <= n; i++) { cin >> p[i]; } 
    for (int i = 1; i <= n; i++) { pow2[i] = pow2[i-1]*2 % MOD; }

    auto combine = [&](Node a, Node b) { 
        Node c;
        for (int i = 0; i < 3; i++) {
            c.dist[i] = (a.dist[b.target[i]] + pow2[a.len] * b.dist[i]) % MOD;
            c.target[i] = a.target[b.target[i]];
        }
        c.len = a.len + b.len;
        return c;
    };
    Node identity = { {0,0,0}, {0,1,2}, 0 };
    vector<Node> nodes(n+1); nodes[0] = identity;
    auto create_leaf = [&](int p){ 
        Node leaf;
        for (int j = 0; j < 3; j++) {
            leaf.dist[j]   = !(p-1 == j);
            leaf.target[j] =   p-1 == j ? j : 3 - (p-1) - j;  
        }
        leaf.len = 1;
        return leaf;
    };
    for (int i = 1; i <= n; i++) nodes[i] = create_leaf(p[i]);
    Grand::SegmentTree S(nodes, combine, identity); 
    while (q--) {
        char op; int x, y;
        cin >> op >> x >> y;
        if (op == 'c') {
            S.update(x, create_leaf(y));
        }
        else {
            int l = x; int r = y;
            Node res = S.query(l, r);
            cout << res.dist[0] << '\n';
        }
    }
}
