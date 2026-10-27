#include <vector> 
#include "../Utils.hpp"

namespace Grand {
template<typename T>
struct Stack : std::vector<T> {
    using std::vector<T>::vector;
    void push(auto&& val) { this->push_back(std::forward<decltype(val)>(val)); } 
    void extend(InputRange auto&& list) { this->insert(this->end(), rgs::begin(list), rgs::end(list)); }
    void emplace(auto&&... args) { this->emplace_back(std::forward<decltype(args)>(args)...); }
    T pop() { T t = std::move(this->back()); this->pop_back(); return t; }
    T& top() { return this->back();}
    const T& top() const { return this->back(); }
};
}