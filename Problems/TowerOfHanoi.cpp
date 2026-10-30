#include <cassert>
#include <bit>
#include <span>
using u128 = unsigned __int128;
// Disks 1,2,...,n.
// Poles 0, 1, 2.
// function process(disk, src, dest)
void tower_of_hanoi(int n, auto process) { assert(n <= 32);
    unsigned int moves = (1ULL<<n) - 1;   
    for (unsigned int i = 1; i <= moves; i++) {
        int disk = std::countr_zero(i) + 1;
        int src = (i & (i - 1)) % 3; // same as (i >> disk) * dir mod 3. where dir == 2 if i is odd, else 1.( 2 == -1 mod 3)
        int dest = ((i | (i - 1)) + 1) % 3; // same as ((i >> disk) + 1) * dir mod 3.  
        if (n % 2 == 0) {
            if (src != 0) src = 3 - src;
            if (dest != 0) dest = 3 - dest;
        } 
        process(disk, src, dest);
    }
}




