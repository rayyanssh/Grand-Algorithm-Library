#include <bits/stdc++.h>
using namespace std;

int main() { 
    int n, k; 
    int x, a, b, c;
    cin >> n >> k;
    cin >> x >> a >> b >> c;
    int y = x;
    int running_xor = 0;
    int window = 0;
    while (n--) {
        deque<int> q; 
        
        while (!q.empty() && q.back().first > y) q.pop_back();
        q.push_back(y);
        window++;
        if (window >= k) {
            
        }
        
        int y = (a*x + b) % c;
        running_xor ^= minimum
    }
}