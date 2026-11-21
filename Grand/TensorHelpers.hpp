#pragma once
#include <ostream> 
#include "Utils.hpp"
namespace Grand {
#ifndef __GRAND_TENSOR 
#define __GRAND_TENSOR 
inline constexpr auto product_dims = [](auto... dims) { return (1LL * ... * dims); };
template<typename T>
concept TensorType = requires { T::rank(); };
#endif

template<TensorType T1, TensorType T2> requires (Range<T1> && Range<T2>)   && (T1::rank() == T2::rank())
constexpr void tensor_copy(const T1& A, T2& B) { rgs::copy(A, B.begin()); }

template<TensorType T> requires Range<T>
constexpr void tensor_fill(const T& A, auto val) { rgs::fill(A, val); }

template<TensorType T1, TensorType T2> requires (Range<T1> && Range<T2>) 
constexpr bool operator==(const T1& A, const T2& B) { 
    return T1::rank() == T2::rank && A.shape() == B.shape() && rgs::equal(A, B); 
}

template<TensorType T1, TensorType T2> requires (!Range<T1> || !Range<T2>) && (T1::rank() == rm_ref<T2>::rank())
constexpr void tensor_copy(const T1& A, T2&& B) {
    assert(A.shape() == B.shape());
    if constexpr (T1::rank() == 1) 
        for (int j = 0; j < A.shape()[0]; j++) A[j] = B[j];
    else
        for (int i = 0; i < A.shape()[0]; i++) 
            tensor_copy(A(i), B(i));
}

template<TensorType T> requires (!Range<T>)
constexpr void tensor_fill(const T& A, auto val) {
    if constexpr (T::rank() == 1) 
        for (int j = 0; j < A.shape()[0]; j++) A[j] = val;
    else
        for (int i = 0; i < A.shape()[0]; i++) 
            tensor_fill(A(i), val);
}
template<TensorType T1, TensorType T2>  
constexpr bool tensor_equal(const T1& A, const T2& B) {
    if constexpr (T1::rank() == 1) {
        for (int j = 0; j < A.shape()[0]; j++) if (A[j] != B[j]) return false;
    }
    else 
        for (int i = 0; i < A.shape()[0]; i++) 
            if (!tensor_equal(A(i), B(i))) return false;
    return true;
}
template<TensorType T1, TensorType T2>  
constexpr bool operator==(const T1& A, const T2& B) { 
    return T1::rank() == T2::rank() && A.shape() == B.shape() && tensor_equal(A,B);
}


void tensor_print(const TensorType auto& t, std::ostream& out, auto... dims) {
    constexpr int R = rm_ref<decltype(t)>::rank(); 
    if constexpr (R > 2) {
        for (int i = 0; i < t.shape()[0]; i++)
            tensor_print(t(i), out, dims..., i);
        if constexpr (sizeof...(dims) > 0) out << '\n';
    }
    if constexpr (R == 2) {
        if constexpr (sizeof...(dims) > 0) {
            out << '[';
            const char* sep = "";
            ((out << sep << dims, sep = ", "), ...);
            out << "]:\n";
        }
        for (int i = 0; i < t.shape()[0]; i++)
            for (int j = 0; j < t.shape()[1]; j++)
                out << t[i, j] << (j + 1 < t.shape()[1] ? ' ' : '\n'); 
    }
    if constexpr (R == 1) 
        for (int j = 0; j < t.size(); j++) out << t[j] << (j + 1 < t.size() ? ' ' : '\n');
}

std::ostream& operator<<(std::ostream& out, const TensorType auto& t) {
    tensor_print(t, out);
    return out;
}



}