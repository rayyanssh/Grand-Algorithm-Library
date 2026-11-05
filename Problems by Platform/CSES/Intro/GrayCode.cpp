#include <iostream>
#include <string>
#include <format>
using namespace std;

      
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr); 
    int n;
    cin >> n; 
    for (int b = 0; b < 1 << n; b++) {
        int code = b^(b>>1);
        string s = format("{:0{}b}", code, n);
        cout << s << '\n';
    }
}
