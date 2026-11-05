#include <span> 
#include <cassert>
#include "../Grand/Math.hpp"
using u64 = unsigned long long;
using u128 = unsigned __int128;

// Disks `1,2...n`. Poles `0,1,2`.    
// The state is represented by a ternary number. (Base 3).
// The `kth` digit of the number represents the pole number of the kth disk.  
 
// State represented by an unsigned 128 bit integer, where each 2 bits is a ternary digit.
// Returns distance from given state to the solved state where all disks are on target pole.
u64 TOH_distance_to_solved(int n, u128 state, int target = 2) { assert(n <= 64);   // 2 bits per disk, n <= 64
    u64 dist = 0;
    for (int k = n - 1; k >= 0; k--) {
        int peg = (state >> (2 * k)) & 3;
        if (peg != target) {
            dist += (u64(1) << k);
            target = 3 - peg - target;
        }
    }
    return dist;
}

// State represented by an unsigned 128 bit integer, where each 2 bits is a ternary digit.
// Returns distance from a given state to the target state.
u64 TOH_distance(int n, u128 state1, u128 state2) { assert(n <= 64);   // 2 bits per disk, n <= 64
    int curr   = (state1 >> 2*(n-1)) & 3;
    int target = (state2 >> 2*(n-1)) & 3;
    u64 dist1 = 0, dist2 = 0;
    if (curr != target) {
        int aux = 3 - curr - target;
        dist1 += TOH_distance_to_solved(n-1, state1, aux);
        dist1++; // Move largest to target
        dist1 += TOH_distance_to_solved(n-1, state2, aux);

        dist2 += TOH_distance_to_solved(n-1, state1, target);
        dist2++; // Move largest to aux
        dist2 += (u64(1) << (n-1)) - 1; // Move pile from target to curr.
        dist2++; // Move largest to target
        dist2 += TOH_distance_to_solved(n-1, state2, curr);
        return std::min(dist1, dist2);
    } 
    else return (n>1) ? TOH_distance(n-1, state1, state2) : 0; 
}


// State represented by a container of ternary digits. 
// Returns distance from given state to the solved state where all disks are on target pole.
// Huge answer, given modulo `MOD`.
template<int MOD>
u64 TOH_distance_to_solved(std::span<const char> state, int target = 2) {
    int n = state.size(); 
    u64 dist = 0;
    for (int k = n - 1; k >= 0; k--) {
        int b = state[k] != target;
        dist = (dist * 2 + b) % MOD;
        if (b) target = 3 - state[k] - target;
    }
    return dist;
}

// Distance from a given state to the target state.
// Huge answer, given modulo `MOD`.
template<int MOD>
u64 TOH_distance(std::span<const char> state1, std::span<const char> state2) {
    int n = state1.size();
    u64 dist = 0;
    if (state1[n-1] != state2[n-1]) {
        int curr = state1[n-1]; int target = state2[n-1];
        int aux = 3 - target - curr;
        auto compare = [=]() -> bool {
            int lead = 2;
            int target1 = aux, target2 = aux, target3 = target, target4 = curr;
            for (int k = n-2; k >= 0; k--) {
                if (state1[k] != target1) { lead--; target1 = 3 - state1[k] - target1; } 
                if (state2[k] != target2) { lead--; target2 = 3 - state2[k] - target2; } 
                if (state1[k] != target3) { lead++; target3 = 3 - state1[k] - target3; } 
                if (state2[k] != target4) { lead++; target4 = 3 - state2[k] - target4; } 
                if (lead >= 2) return 1;
                if (lead <= -2) return 0;
                lead *= 2;
            }
            if (lead >= 0) return 1;
            return 0;
        };
        if (compare() == 1) {
            dist += TOH_distance_to_solved<MOD>(state1.subspan(0,n-1), aux);
            dist++; // Move largest to target
            dist += TOH_distance_to_solved<MOD>(state2.subspan(0,n-1), aux); 
            return dist % MOD;
        }
        else {
            dist += TOH_distance_to_solved<MOD>(state1.subspan(0,n-1), target);
            dist++; // Move largest to aux
            dist += Grand::modpow(2,n-1,MOD) - 1; // Move pile from target to curr.
            dist++; // Move largest to target
            dist += TOH_distance_to_solved<MOD>(state2.subspan(0,n-1), curr);
            return dist % MOD;
        }
    }
    else return (n>1) ? 
                TOH_distance<MOD>(state1.subspan(0,n-1), state2.subspan(0,n-1)) : 
                0;
}


