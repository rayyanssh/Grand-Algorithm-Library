#include <cassert>
#include <iostream>
#include <bit>
using namespace std;

void tower_of_hanoi(int n) { assert(n <= 32);
    unsigned int moves = (1ULL<<n) - 1;  
    cout << moves << '\n';
    // Disks 1,2,...,n
    // Poles 0, 1, 2
    for (unsigned int i = 1; i <= moves; i++) {
        int disk = countr_zero(i) + 1;
        int from = (i & (i - 1)) % 3;
        int to = ((i | (i - 1)) + 1) % 3;
        if (n % 2 == 0) {
            from = 3 - from;
            to = 3 - to;
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
