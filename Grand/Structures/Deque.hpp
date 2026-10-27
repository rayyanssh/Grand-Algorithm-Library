#include "../Utils.hpp"

namespace Grand {
template<typename Deque> struct DequeIterator;
// Fast Circular Deque, keeps internal capacity at a power of 2 to allow fast bit modulo operations.
// Resizing upon pushing elements can invalidate iterators.
// Reserve upfront to avoid invalidation. 
// Max capacity is 2^30 ≈ 1 billion
template<typename T>
struct Deque {
    using value_type = T;
    vector<T> data;
    unsigned int start = 0, _end;
    Deque() : data(1), _end(0) {}
    // Initialize deque with n elements. Rounds up capacity to nearest power of 2.
    Deque(unsigned n, T val = {}) : data(bit_ceil(n), val), _end(n) {}

    unsigned mod_capacity(unsigned k) const { return k & (capacity() - 1); }

    T& operator[](int i) { return data[mod_capacity(start+i)]; }     const T& operator[](int i) const { return data[mod_capacity(start+i)]; }

    void push_back(const T& val) {
        if (size() == capacity()) { T tmp = val; reserve(2 * size()); push_back(std::move(tmp)); }
        else data[mod_capacity(_end++)] = val;
    }
    void push_back(T&& val) {
        if (size() == capacity()) reserve(2 * size());
        data[mod_capacity(_end++)] = std::move(val);
    }
    T pop_back() { return std::move(data[mod_capacity(--_end)]); }
    T& back() { return data[mod_capacity(_end-1)]; }                 const T& back() const { return data[mod_capacity(_end-1)]; }

    void push_front(const T& val) {
        if (size() == capacity()) { T tmp = val; reserve(2 * size()); push_front(std::move(tmp)); }
        else data[mod_capacity(--start)] = val;
    }
    void push_front(T&& val) {
        if (size() == capacity()) reserve(2 * size());
        data[mod_capacity(--start)] = std::move(val);
    }
    T pop_front() { return std::move(data[mod_capacity(start++)]); }
    T& front() { return data[mod_capacity(start)]; }                 const T& front() const { return data[mod_capacity(start)]; }
   
    unsigned size()     const { return _end - start; } 
    unsigned capacity() const { return data.size(); }
    bool     empty()    const { return _end == start; } 
    
    // Rounds up capacity to nearest power of 2
    void reserve(unsigned int n) { 
        if (capacity() < bit_ceil(n)) {
            rgs::rotate(data, data.begin() + mod_capacity(start));
            _end = _end - start;
            start = 0;
            data.resize(bit_ceil(n)); 
        }
    }

    void clear() { start = _end = 0; }

    void resize(unsigned int n, T val = {}) {
        while (size() > n) pop_back();
        if (n > capacity()) reserve(n);
        while (size() < n) push_back(val);
    }

    auto begin() { return DequeIterator{data.data(), unsigned(capacity()-1), start}; }  auto begin() const { return DequeIterator{data.data(), unsigned(capacity()-1), start}; }
    auto end()   { return DequeIterator{data.data(), unsigned(capacity()-1), _end}; }   auto end()   const { return DequeIterator{data.data(), unsigned(capacity()-1), _end}; }
};


template<typename T>
struct DequeIterator {
    using value_type = std::remove_const_t<T>;
    T* base = nullptr; unsigned mod_mask = 0, pos = 0;  
    T& operator*() const { return base[pos & mod_mask]; }
    T& operator[](int i) const { return base[(pos+i) & mod_mask]; }
    auto& operator++() { pos++; return *this; } auto operator++(int) { auto copy = *this; pos++; return copy; }
    auto& operator--() { pos--; return *this; } auto operator--(int) { auto copy = *this; pos--; return copy; }
    auto& operator+=(int i) { pos += i; return *this; }
    auto& operator-=(int i) { pos -= i; return *this; }
    friend auto operator+(DequeIterator it, int i) { return it += i; } friend auto operator+(int i, DequeIterator it) { return it += i; }
    friend auto operator-(DequeIterator it, int i) { return it -= i; }
    int operator-(DequeIterator b) const { return (int)(pos - b.pos); }
    auto operator==(DequeIterator o) const { return pos == o.pos; }
    auto operator<=>(DequeIterator o) const { return int(pos - o.pos) <=> 0; }
};
static_assert(std::random_access_iterator<DequeIterator<int>>); 

}