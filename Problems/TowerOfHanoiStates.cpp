#include <span> 
#include <cassert>
using ull = unsigned long long;
using u128 = unsigned __int128;

// Disks `1,2...n`. Poles `0,1,2`.    
// The state is represented by a ternary number. (Base 3).
// The `kth` digit of the number represents the pole number of the kth disk.  
 

// State represented by a container of ternary digits. 
// Returns distance from given state to the solved state where all disks are on target pole.
// Huge answer, given modulo `MOD`.
template<int MOD>
ull TOH_distance_to_solved(std::span<char> state, int target = 2) {
    int n = state.size(); 
    ull dist = 0;
    for (int k = n - 1; k >= 0; k--) {
        int b = state[k] != target;
        dist = (dist * 2 + b) % MOD;
        if (b) target = 3 - state[k] - target;
    }
    return dist;
}

// Distance from a given state to the target state.
template<int MOD>
ull TOH_distance(std::span<char> state1, std::span<char> state2) {
    int n = state1.size(); 
    ull dist1 = 0, dist2 = 0;
    // If largest disk not on the required pole
    // Option A:
    //    1. Move the smaller disks to the aux space P = 3-target-current pole (subproblem). 
    //    dist_to_end(state1[0..n-1], P)
    //    2. Move the largest disk to target (1 move).
    //    3. Move the other pile of disks from the empty spot P to their required poles in state2
    //    == dist(state{P,P,P..}, state2[0...n-1]) == dist(state2[0..n-1], state{P,P,...})
    //    == dist_to_end(state2[0..n-1], P);
    //  Option B:
    //    1. Move smaller disks to target. 
    //    2. Move largest to aux space.
    //    3. Move smaller disks to current pole that largest was originally on. (2^(n-1) - 1 moves)
    //    4. Move largest to target.
    //    5. Move smaller disks to their required state in state2. == dist_to_end(state2[0..n-1], curr);
    //  Return min(moves(Option A), moves(Option B));
    // else: recurse to the subproblem of dist(state1[0..n-1], state2[0..n-1])

    if (state1[n-1] != state2[n-1]) {
        int curr = state1[n-1]; int target = state2[n-1];
        int aux = 3 - target - curr;
        dist1 += TOH_distance_to_solved<MOD>(state1.subspan(0,n-1), aux);
        dist1++;
        dist1 += TOH_distance_to_solved<MOD>(state2.subspan(0,n-1), aux); 
        dist2 += TOH_distance_to_solved<MOD>(state1.subspan(0,n-1), target);
        dist2++;
        dist2 += (1 << (n-1)) - 1; // Move pile from target to curr.
        dist2++;
        dist2 += TOH_distance_to_solved<MOD>(state2.subspan(0,n-1), curr);
        ull min_dist = std::min(dist1, dist2);
        min_dist %= MOD;
        return min_dist;
    }
    else {
        if (n != 1) return TOH_distance<MOD>(state1.subspan(0,n-1), state2.subspan(0,n-1));
        else return 0;
    }
}


// State represented by an unsigned 128 bit integer, where each 2 bits is a ternary digit.
ull TOH_distance_to_solved(int n, u128 state, int target = 2) {   assert(n <= 64);   // 2 bits per disk, n <= 64
    ull dist = 0;
    for (int k = n - 1; k >= 0; k--) {
        int peg = (state >> (2 * k)) & 3;
        if (peg != target) {
            dist += (ull(1) << k);
            target = 3 - peg - target;
        }
    }
    return dist;
}

// State represented by an unsigned 128 bit integer, where each 2 bits is a ternary digit.
ull TOH_distance(int n, u128 state1, u128 state2) {   assert(n <= 64);   // 2 bits per disk, n <= 64
    int curr   = (state1 >> 2*(n-1)) & 3;
    int target = (state2 >> 2*(n-1)) & 3;
    ull dist1 = 0, dist2 = 0;
    if (curr != target) {
        int aux = 3 - curr - target;
        dist1 += TOH_distance_to_solved(n-1, state1, aux);
        dist1++;
        dist1 += TOH_distance_to_solved(n-1, state2, aux);

        dist2 += TOH_distance_to_solved(n-1, state1, target);
        dist2++;
        dist2 += (ull(1) << (n-1)) - 1; // Move pile from target to curr.
        dist2++;
        dist2 += TOH_distance_to_solved(n-1, state2, curr);
        return std::min(dist1, dist2);
    } 
    else return (n>1) ? TOH_distance(n-1, state1, state2) : 0; 
}
