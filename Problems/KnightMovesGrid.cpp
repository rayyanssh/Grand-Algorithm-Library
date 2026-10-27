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
    M[0,0] = M[2,1] = 1;
    Deque<pair<int, int>> q; 
    q.push_back({2,1});
    auto valid = [=](int i, int j) { return 0<=i&&i<n&&0<=j&&j<n; };
    while (!q.empty()) {
        auto [i,j] = q.pop_front();
        for (auto [ni, nj] : neighbors(i, j)) {
            if (valid(ni, nj) && !M[ni, nj]) {
                M[ni, nj] = M[i, j] + 1;
                q.push_back({ni, nj});
            }  
        }
    }
    M[0,0] = 0;
    return M;
}