#include <array>
#include <vector>
using std::vector, std::array;

// Note: Helpers for n dimensional arrays and views, since Tensor from Grand library has some features that
// aren't supported on CSES due to requiring gcc14's c++23 while CSES has gcc13's c++ currently

template<typename T>
struct Array3D {
    vector<T> storage;
    int n, m, p;
    Array3D(int n, int m, int p, T val = {}) : n(n), m(m), p(p) , storage(n*m*p, val) {}

    T& operator[](int i, int j, int k) { return storage[i*m*p + j*m + k]; }
    T& operator[](int k) { return storage[k]; } 
    
    const T& operator[](int i, int j, int k) const { return storage[i*m*p + j*m + k]; }
    const T& operator[](int k) const { return storage[k]; } 

    void resize(int r, int s, int t, T val = {}) {
        storage.resize(r*s*t, val);
        n = r; m = s; p = t;
    } 
    T* data() { return storage.data(); }
    long long size() { return storage.size(); }

    
};
 