#include "../Utils.hpp"
namespace Grand {
template<typename Deque> struct DequeIterator;
// Circular Deque.
// Resizing invalidates iterators, references/pointers to elements.
// Resize/declare size upfront to avoid invalidation.
// max capacity is 2^31-1
template<typename T>
struct Deque {
    using value_type = T;
    vector<T> data;
    unsigned int start = 0, _end;
    // Rounds up internal vector's size to nearest power of 2, to allow fast bit modulo operations
    Deque(unsigned int n = 0, T val = {}) : data(bit_ceil(n), val), _end(n) {}
    unsigned mod_by_size(unsigned int i) const { return i & (data.size() - 1); }

    T& operator[](int i) { return data[mod_by_size(start+i)]; }     const T& operator[](int i) const { return data[mod_by_size(start+i)]; }

    void push_back(T val) {
        if (full()) grow();
        data[mod_by_size(_end++)] = move(val);
    }
    T pop_back()     { return move(data[mod_by_size(--_end)]); }
    T& back() { return data[mod_by_size(_end-1)]; }                 const T& back() const { return data[mod_by_size(_end-1)]; }

    void push_front(T val) {
        if (full()) grow();
        data[mod_by_size(--start)] = move(val);
    }
    T pop_front()     { return move(data[mod_by_size(start++)]); }
    T& front() { return data[mod_by_size(start)]; }                 const T& front() const { return data[mod_by_size(start)]; }

    void clear() { start = _end = 0; }

    void resize(unsigned int n, T val = {}) {
        while (size() > n) pop_back();
        if (n > data.size()) {
            unsigned old = size();
            rgs::rotate(data, data.begin() + mod_by_size(start));
            start = 0;
            _end = n;
            data.resize(bit_ceil(n));
            std::fill(data.begin() + old, data.begin() + n, val);
        }
        else while (size() < n) push_back(val);
    }
    
    unsigned size() const { return _end - start; } 
    bool    empty() const { return _end == start; } 

    auto begin() { return DequeIterator{data.data(), unsigned(data.size()-1), start}; }  auto begin() const { return DequeIterator{data.data(), data.size()-1, start}; }
    auto end()   { return DequeIterator{data.data(), unsigned(data.size()-1), _end}; }   auto end()   const { return DequeIterator{data.data(), data.size()-1, _end}; }
    
private:
    bool full() const { return size() == data.size(); }
    void grow() {
        rgs::rotate(data, data.begin() + mod_by_size(start));
        start = 0;
        _end = data.size();
        data.resize(2 * data.size());
    } 
};


template<typename T>
struct DequeIterator {
    using value_type = std::remove_const_t<T>;
    T* base = nullptr; unsigned mod_mask = 0, pos = 0;  
    T& operator*() const { return base[pos & mod_mask]; }
    T& operator[](int i) const { return *(*this + i); }
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
static_assert(std::random_access_iterator<DequeIterator<Deque<int>>>); 

}