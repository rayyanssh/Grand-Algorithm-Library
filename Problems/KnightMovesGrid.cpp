#include "../Grand/Matrix/SpecialMatrices.hpp"
#include "../Grand/Structures/Deque.hpp"
using namespace Grand;

array<pair<int,int>, 8> neighbors(int i, int j) {
    return {
        pair{i-1,j-2}, {i-2,j-1}, {i-2,j+1}, {i-1,j+2}, 
            {i+1,j+2}, {i+2,j+1}, {i+2,j-1}, {i+1,j-2}
    };
}

SymmetricMatrix<int> knight_moves_grid(int n) { assert(n>=4);
    SymmetricMatrix<int> M(n);
    M[2,1] = 1;
    Deque<pair<int, int>> q; 
    q.push_back({2,1});
    while (!q.empty()) {
        for (auto [i,j] : neighbors(q.pop())) {

        }
    }
}