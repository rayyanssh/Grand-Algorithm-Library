#pragma once 
#include <string>
#include <iostream>
void matprint(auto&& A, std::string name = "") { 
    std::cout << name << (!name.empty() ? ":\n" : "");
    for (int i = 0; i < A.rows(); i++) {
        for (int j = 0; j < A.cols(); j++) 
            std::cout << A[i, j] << ' ';
        std::cout << '\n';
    }
}
#define MATPRINT(A) matprint(A, #A)