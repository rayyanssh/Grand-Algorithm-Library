#include <vector>
#include <stack>
template<typename T>
struct Stack : std::vector<T> {
    using std::vector<T>::vector;
    void push(const T& val) { this->push_back(val); }
    void push(std::initializer_list<T> list) { this->insert(this->end(), list); }
    T pop() { T t = std::move(this->back()); this->pop_back(); return t; }
    T& top() { return this->back();}
    const T& top() const { return this->back(); }
};