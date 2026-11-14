#include <vector>
#include "../Utils.hpp"
namespace Grand {
using u64 = unsigned long long;
struct BitSpan;

struct BitMatrix {
    std::vector<u64> storage;
    int n, m;
    BitMatrix(int n, int m, bool val = 0) : storage(n * (m+63)/64, val ? u64(-1) : 0), n(n), m(m) {}
    bool operator[](int i, int j) {
        return storage[i * (m+63)/64 + (j+63)/64] & (1 << j%64); 
    }
    BitSpan row(int i);
    int rows() const { return n; }
    int cols() const { return m; } 
};

struct BitSpan {
    u64* start;
    int m;
    BitSpan(u64* p, int m) : start(p), m(m) {}
    span<u64> words() { return span<u64>(start, (m+63/64)); }
    bool operator[](int i) { return start[(i+63)/64] & (1 << i%64); }
};

BitSpan BitMatrix::row(int i) { return BitSpan(&storage[i * (m+63)/64], m); }

}