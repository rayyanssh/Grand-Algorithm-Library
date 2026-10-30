#include <cassert>
#include <iostream>
#include <bit>
using namespace std;

void tower_of_hanoi(int n) { assert(n <= 32);
    unsigned int moves = (1ULL<<n) - 1;  
    cout << moves << '\n';
    // Disks 1,2,...,n
    // Poles 1, 2, 3
    for (unsigned int i = 1; i <= moves; i++) {
        int disk = countr_zero(i) + 1;
        int from = (i & (i - 1)) % 3 + 1;
        int to = ((i | (i - 1)) + 1) % 3 + 1;
        if (n % 2 == 0) {
            if (from != 1) from = 5 - from;
            if (to != 1) to = 5 - to;
        }
        cout << from << ' ' << to << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr); 
    int n;
    cin >> n;
    tower_of_hanoi(n); 
}
