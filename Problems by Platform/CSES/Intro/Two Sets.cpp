#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    if (!(n % 4 == 0 || n % 4 == 3)) {
        cout << "NO";
        return 0;
    }
    cout << "YES\n";
    if (n%4 == 0) {
        cout << n/2 << '\n';
        for (int i = 1; i <= n/4; i++) {
            cout << i << ' ' << n - i + 1 << ' ';
        }
        cout << '\n' << n/2 << '\n';
        for (int i = n/4 + 1; i <= n/2; i++) {
            cout << i << ' ' << n - i + 1 << ' ';
        }
    }
    if (n%4 == 3) {
        long long S = 1LL * n*(n+1)/2; long long target = S/2;
        int k = n;
        vector<int> list1, list2; list2.reserve(n/2); list1.reserve(n/2);
        while (target > 0) {
            if (k <= target) {
                list1.push_back(k);
                target -= k;
                k--;
            }
            else {
                list2.push_back(k);
                k--;
            }
        }
        while (k>0) list2.push_back(k--);
        cout << list1.size() << '\n';
        for (int i: list1) cout << i << ' ';
        cout << '\n' << list2.size() << '\n';
        for (int i: list2) cout << i << ' ';
    }
    return 0;
}